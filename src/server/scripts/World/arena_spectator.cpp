/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
Name: Arena Spectator
%Complete: 100
Comment: Script allow spectate arena games
Category: Custom Script
EndScriptData */

#include "ScriptPCH.h"
#include "Chat.h"
#include "ArenaTeamMgr.h"
#include "BattlegroundMgr.h"
#include "WorldPacket.h"
#include "Battleground.h"
#include "CreatureTextMgr.h"
#include "Config.h"
#include "ArenaTeam.h"
#include "Player.h"

//int8 UsingGossip;

class arena_spectator_commands : public CommandScript
{
    public:
        arena_spectator_commands() : CommandScript("arena_spectator_commands") { }

        static bool HandleSpectateCommand(ChatHandler* handler, const char *args)
        {
            Player* target;
            uint64 target_guid;
            std::string target_name;
            if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
                return false;

            Player* player = handler->GetSession()->GetPlayer();
            if (target == player || target_guid == player->GetGUID())
            {
                handler->SendSysMessage(LANG_CANT_TELEPORT_SELF);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->isInCombat())
            {
                handler->SendSysMessage(LANG_YOU_IN_COMBAT);
                handler->SetSentErrorMessage(true);
                return false;
            }
			
			if (player->isSpectator())
			{
				handler->SendSysMessage(LANG_SPEC_ALREADY_SPECTATOR);
				handler->SetSentErrorMessage(true);
				return false;
			}
			
			if (player->GetMap()->IsBattlegroundOrArena())
			{
				handler->SendSysMessage(LANG_SPEC_ON_ARENA_OR_BG);
				handler->SetSentErrorMessage(true);
				return false;
			}

            if (!target)
            {
                handler->SendSysMessage(LANG_PLAYER_NOT_EXIST_OR_OFFLINE);
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->GetPet())
            {
                handler->PSendSysMessage("Debes guardar tu mascota.");
                handler->SetSentErrorMessage(true);
                return false;
            }

			if (player->IsMounted())
			{
                handler->PSendSysMessage("Debes bajarte de tu montura.");
                handler->SetSentErrorMessage(true);
                return false;
            }
			
			if (player->InBattlegroundQueue() || !player->CanJoinToBattleground() || player->isUsingLfg())
			{
                handler->PSendSysMessage("Debes abandonar la cola de Arenas o BG.");
                handler->SetSentErrorMessage(true);
                return false;
            }
			
            if (player->GetMap()->IsBattlegroundOrArena() && !player->isSpectator())
            {
                handler->PSendSysMessage("Ya estas en un campo de batalla.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            Map* cMap = target->GetMap();
            if (!cMap->IsBattleArena())
            {
                handler->PSendSysMessage("Jugador no encontrado en la arena");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->GetMap()->IsBattleground())
            {
                handler->PSendSysMessage("No puedes hacer eso mientras estas en un campo de batalla.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            // all's well, set bg id
            // when porting out from the bg, it will be reset to 0
            player->SetBattlegroundId(target->GetBattlegroundId(), target->GetBattlegroundTypeId());
            // remember current position as entry point for return at bg end teleportation
            if (!player->GetMap()->IsBattlegroundOrArena())
                player->SetBattlegroundEntryPoint();

            if (target->isSpectator())
            {
                handler->PSendSysMessage("No puedes hacer eso, el jugador es un espectador.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            // stop flight if need
            if (player->isInFlight())
            {
                player->GetMotionMaster()->MovementExpired();
                player->CleanupAfterTaxiFlight();
            }
            // save only in non-flight case
            else
                player->SaveRecallPosition();

            // search for two teams
            Battleground *bGround = target->GetBattleground();

            if (bGround->isRated())
            {				
                uint32 slot = bGround->GetArenaType() - 2;
                if (bGround->GetArenaType() > 3)
                    slot = 2;
                uint32 firstTeamID = target->GetArenaTeamId(slot);
                uint32 secondTeamID = 0;
                Player *firstTeamMember  = target;
                Player *secondTeamMember = NULL;
                for (Battleground::BattlegroundPlayerMap::const_iterator itr = bGround->GetPlayers().begin(); itr != bGround->GetPlayers().end(); ++itr)
                    if (Player* tmpPlayer = ObjectAccessor::FindPlayer(itr->first))
                    {							
                        if (tmpPlayer->isSpectator())
                            continue;

                        uint32 tmpID = tmpPlayer->GetArenaTeamId(slot);
                        if (tmpID != firstTeamID && tmpID > 0)
                        {
                            secondTeamID = tmpID;
                            secondTeamMember = tmpPlayer;
                            break;
                        }
                    }

                if (firstTeamID > 0 && secondTeamID > 0 && secondTeamMember)
                {
                    ArenaTeam *firstTeam  = sArenaTeamMgr->GetArenaTeamById(firstTeamID);
                    ArenaTeam *secondTeam = sArenaTeamMgr->GetArenaTeamById(secondTeamID);
                    if (firstTeam && secondTeam)
                    {
						handler->PSendSysMessage("Has entrado en una arena puntuada.");
                        handler->PSendSysMessage("Equipos:");
                        handler->PSendSysMessage("%s - %s", firstTeam->GetName().c_str(), secondTeam->GetName().c_str());
                        handler->PSendSysMessage("%u(%u) - %u(%u)", firstTeam->GetRating(), firstTeam->GetAverageMMR(firstTeamMember->GetGroup()),
                                                                    secondTeam->GetRating(), secondTeam->GetAverageMMR(secondTeamMember->GetGroup()));
                    }
                }
            }
		
			//Custom
			handler->PSendSysMessage("|CFFFF0000Recuerda que dispones de los comandos:|R");
			handler->PSendSysMessage("|CFF87CEFA.spectate player |CFF800080#nombre_jugador|R : Sirve para espectar a un jugador en concreto");
			handler->PSendSysMessage("|CFF87CEFA.spectate view |CFF800080#nombre_jugador|R : Pasas a ver la misma vista que el jugador #nombre_jugador");
			handler->PSendSysMessage("|CFF87CEFA.spectate reset|R : Reinicia la cámara por si has puesto el comando .spectate view");
			handler->PSendSysMessage("|CFF87CEFA.spectate leave|R : Dejas de espectar la Arena y vuelves a tu posición original.");
			
			ItemPosCountVec off_dest;
			ItemPosCountVec off_dest_2;
			//Item* RangedItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
			Item* FirstItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
			Item* OffItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

			/*uint8 off_msg = player->CanStoreItem(NULL_BAG, NULL_SLOT, off_dest, RangedItem, false);
			if(off_msg == EQUIP_ERR_OK)
			{
				player->RemoveItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED, true);
				player->StoreItem(off_dest, RangedItem, true);
			}*/
			
			InventoryResult off_msg_2 = player->CanStoreItem(NULL_BAG, NULL_SLOT, off_dest, FirstItem, false);
			if(off_msg_2 == EQUIP_ERR_OK)
			{
				player->RemoveItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND, true);
				player->StoreItem(off_dest, FirstItem, true);
			}
			else
                player->SendEquipError(off_msg_2, FirstItem, NULL);
			
			InventoryResult off_msg_3 = player->CanStoreItem(NULL_BAG, NULL_SLOT, off_dest_2, OffItem, false);
			if(off_msg_3 == EQUIP_ERR_OK)
			{
				player->RemoveItem(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND, true);
				player->StoreItem(off_dest_2, OffItem, true);
			}
			else
                player->SendEquipError(off_msg_3, OffItem, NULL);

            // to point to see at target with same orientation
            float x, y, z;
            target->GetContactPoint(player, x, y, z);

            player->TeleportTo(target->GetMapId(), x, y, z, player->GetAngle(target), TELE_TO_GM_MODE);
			//std::string chrNameLink = handler->playerLink(target_name);
			//handler->PSendSysMessage("Has aparecido ante %u", chrNameLink.c_str());
            player->SetPhaseMask(target->GetPhaseMask(), true);
            player->SetSpectate(true);
            target->GetBattleground()->AddSpectator(player->GetGUID());

            return true;
        }

        static bool HandleSpectateCancelCommand(ChatHandler* handler, const char* /*args*/)
        {
            Player* player =  handler->GetSession()->GetPlayer();

            if (!player->isSpectator())
            {
                handler->PSendSysMessage("Tu no eres un espectador.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            player->GetBattleground()->RemoveSpectator(player->GetGUID());
            player->CancelSpectate();
            player->TeleportToBGEntryPoint();

            return true;
        }

        static bool HandleSpectateFromCommand(ChatHandler* handler, const char *args)
        {
            Player* target;
            uint64 target_guid;
            std::string target_name;
            if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
                return false;

            Player* player = handler->GetSession()->GetPlayer();

            if (!target)
            {
                handler->PSendSysMessage("No se puede encontrar al jugador.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (!player->isSpectator())
            {
                handler->PSendSysMessage("Tu no eres un espectador, mira a alguien antes.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (target->isSpectator() && target != player)
            {
                handler->PSendSysMessage("No puedes hacer eso. El jugador es un espectador.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (player->GetMap() != target->GetMap())
            {
                handler->PSendSysMessage("No puedes hacer eso. Arenas diferentes?");
                handler->SetSentErrorMessage(true);
                return false;
            }

            // check for arena preperation
            // if exists than battle didn`t begin
            /*if (target->HasAura(32728) || target->HasAura(32727) && UsingGossip == 0)
            {
                handler->PSendSysMessage("No puedes hacer eso. La arena aun no ha empezado.");
                handler->SetSentErrorMessage(true);
                return false;
            }*/

            (target == player && player->getSpectateFrom()) ? player->SetViewpoint(player->getSpectateFrom(), false) :
                                                                player->SetViewpoint(target, true);
            return true;
        }

        static bool HandleSpectateResetCommand(ChatHandler* handler, const char *args)
        {
            Player* player = handler->GetSession()->GetPlayer();

            if (!player)
            {
                handler->PSendSysMessage("No se puede encontrar el jugador.");
                handler->SetSentErrorMessage(true);
                return false;
            }

            if (!player->isSpectator())
            {
                handler->PSendSysMessage("No eres un espectador!");
                handler->SetSentErrorMessage(true);
                return false;
            }

            Battleground *bGround = player->GetBattleground();
            if (!bGround)
                return false;

            if (bGround->GetStatus() != STATUS_IN_PROGRESS)
                return true;

            for (Battleground::BattlegroundPlayerMap::const_iterator itr = bGround->GetPlayers().begin(); itr != bGround->GetPlayers().end(); ++itr)
                if (Player* tmpPlayer = ObjectAccessor::FindPlayer(itr->first))
                {
                    if (tmpPlayer->isSpectator())
                        continue;

                    uint32 tmpID = bGround->GetPlayerTeam(tmpPlayer->GetGUID());

                    // generate addon massage
                    std::string pName = tmpPlayer->GetName();
                    std::string tName = "";

                    if (Player *target = tmpPlayer->GetSelectedPlayer())
                        tName = target->GetName();

                    SpectatorAddonMsg msg;
                    msg.SetPlayer(pName);
                    if (tName != "")
                        msg.SetTarget(tName);
                    msg.SetStatus(tmpPlayer->isAlive());
                    msg.SetClass(tmpPlayer->getClass());
                    msg.SetCurrentHP(tmpPlayer->GetHealth());
                    msg.SetMaxHP(tmpPlayer->GetMaxHealth());
                    Powers powerType = tmpPlayer->getPowerType();
                    msg.SetMaxPower(tmpPlayer->GetMaxPower(powerType));
                    msg.SetCurrentPower(tmpPlayer->GetPower(powerType));
                    msg.SetPowerType(powerType);
                    msg.SetTeam(tmpID);
                    msg.SendPacket(player->GetGUID());
                }

            return true;
        }

        ChatCommand* GetCommands() const
        {
            static ChatCommand spectateCommandTable[] =
            {
                { "player",         SEC_PLAYER,      true,  &HandleSpectateCommand,        "", NULL },
                { "view",           SEC_PLAYER,      true,  &HandleSpectateFromCommand,    "", NULL },
                { "reset",          SEC_PLAYER,      true,  &HandleSpectateResetCommand,   "", NULL },
                { "leave",          SEC_PLAYER,      true,  &HandleSpectateCancelCommand,  "", NULL },
                { NULL,             0,               false, NULL,                          "", NULL }
            };

            static ChatCommand commandTable[] =
            {
                { "spectate",       SEC_PLAYER, false,  NULL, "", spectateCommandTable },
                { NULL,             0,          false,  NULL, "", NULL }
            };
            return commandTable;
        }
};


enum NpcSpectatorAtions {
    // will be used for scrolling
    NPC_SPECTATOR_ACTION_LIST_GAMES         = 1000,
    NPC_SPECTATOR_ACTION_LIST_TOP_GAMES     = 2000,

    // NPC_SPECTATOR_ACTION_SELECTED_PLAYER + player.Guid()
    NPC_SPECTATOR_ACTION_SELECTED_PLAYER    = 3000
};

//const uint16 TopGamesRating = 1800;
//const uint8  GamesOnPage    = 20;
const uint8  GamesOnPage    = 15;

class npc_arena_spectator : public CreatureScript
{
    public:
        npc_arena_spectator() : CreatureScript("npc_arena_spectator") { }

        bool OnGossipHello(Player* pPlayer, Creature* pCreature)
        {
			pPlayer->ADD_GOSSIP_ITEM(9, "Partidas: 2c2", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES);
            pPlayer->ADD_GOSSIP_ITEM(9, "Partidas: 3c3", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_TOP_GAMES);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            if (action >= NPC_SPECTATOR_ACTION_LIST_GAMES && action < NPC_SPECTATOR_ACTION_LIST_TOP_GAMES)
            {
                ShowPage(player, action - NPC_SPECTATOR_ACTION_LIST_GAMES, false);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }
            else if (action >= NPC_SPECTATOR_ACTION_LIST_TOP_GAMES && action < NPC_SPECTATOR_ACTION_SELECTED_PLAYER)
            {
                ShowPage(player, action - NPC_SPECTATOR_ACTION_LIST_TOP_GAMES, true);
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
            }
            else
            {
                uint64 guid = action - NPC_SPECTATOR_ACTION_SELECTED_PLAYER;
                if (Player* target = ObjectAccessor::FindPlayer(guid))
                {
                    ChatHandler handler(player);
					char const* pTarget = target->GetName();
					Battleground* bgroundd = target->GetBattleground();
          			//UsingGossip = 1;
                    arena_spectator_commands::HandleSpectateCommand(&handler, pTarget);
                }
            }			
            return true;
        }

        std::string GetClassNameById(uint8 id)
        {
            std::string sClass = "";
            switch (id)
            {
                case CLASS_WARRIOR:         sClass = "Guerrero ";        break;
                case CLASS_PALADIN:         sClass = "Paladin ";           break;
                case CLASS_HUNTER:          sClass = "Cazador ";           break;
                case CLASS_ROGUE:           sClass = "Picaro ";          break;
                case CLASS_PRIEST:          sClass = "Sacerdote ";         break;
                case CLASS_DEATH_KNIGHT:    sClass = "DK ";             break;
                case CLASS_SHAMAN:          sClass = "Chaman ";          break;
                case CLASS_MAGE:            sClass = "Mago ";           break;
                case CLASS_WARLOCK:         sClass = "Brujo ";        break;
                case CLASS_DRUID:           sClass = "Druida ";          break;
            }
            return sClass;
        }

        std::string GetGamesStringData(Battleground *arena, uint16 mmr)
        {
            std::string teamsMember[BG_TEAMS_COUNT];
            uint32 firstTeamId = 0;
            for (Battleground::BattlegroundPlayerMap::const_iterator itr = arena->GetPlayers().begin(); itr != arena->GetPlayers().end(); ++itr)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                {
                    if (player->isSpectator())
                        continue;

                    uint32 team = itr->second.Team;
                    if (!firstTeamId)
                        firstTeamId = team;

                    teamsMember[firstTeamId == team] += GetClassNameById(player->getClass());
                }


			/*uint16 a;
			a = arena->GetArenaMatchmakerRating(0);
            std::stringstream ss;
			ss << a;
			std::string data = teamsMember[0] +" ("+ss.str();
			uint16 b;
			b = arena->GetArenaMatchmakerRating(1);
            std::stringstream sss;
			sss << b;
            data += ") - " + teamsMember[1] +" ("+sss.str()+")";
            return data;*/
			
			/*std::string data = teamsMember[0] + "(";
            std::stringstream ss;
            ss << mmr;
            data += ss.str();
            data += ") - " + teamsMember[1];		
            return data;*/

			//Caida aqui:
			/*Player* jugador;
			Battleground *bGround = jugador->GetBattleground();
			uint32 slot = bGround->GetArenaType() - 2;
            if (bGround->GetArenaType() > 3)
                slot = 2;
			uint32 firstTeamID = jugador->GetArenaTeamId(slot);
			uint32 secondTeamID = 0;*/
			//Fin caida
			/*for (Battleground::BattlegroundPlayerMap::const_iterator itr = bGround->GetPlayers().begin(); itr != bGround->GetPlayers().end(); ++itr)
                    if (Player* tmpPlayer = ObjectAccessor::FindPlayer(itr->first))
                    {
                        uint32 tmpID = tmpPlayer->GetArenaTeamId(slot);
                        if (tmpID != firstTeamID && tmpID > 0)
                        {
                            secondTeamID = tmpID;
                            break;
                        }
                    }
			//uint32 a;
			ArenaTeam *PrimerTeam = sArenaTeamMgr->GetArenaTeamById(firstTeamID);
			uint32 PrimerTeamRating = PrimerTeam->GetRating();
            std::stringstream ss;
			ss << PrimerTeamRating;
			//uint32 b;
			ArenaTeam *SegundoTeam = sArenaTeamMgr->GetArenaTeamById(secondTeamID);
			uint32 SegundoTeamRating = PrimerTeam->GetRating();
            std::stringstream sss;
			sss << SegundoTeamRating;
			//Mensaje:
			std::string data = teamsMember[0] +" ("+ss.str();
            data += ") - " + teamsMember[1] +" ("+sss.str()+")";
            return data;*/
			std::string data = teamsMember[0] +" - ";
            data += teamsMember[1];		
            return data;
			/*std::string data = teamsMember[0] + " - ";
			std::stringstream ss;
			ss << mmr;
			data += ss.str();
			data += " - " + teamsMember[1];
			return data;*/
        }

        uint64 GetFirstPlayerGuid(Battleground *arena)
        {
            for (Battleground::BattlegroundPlayerMap::const_iterator itr = arena->GetPlayers().begin(); itr != arena->GetPlayers().end(); ++itr)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                    return itr->first;
            return 0;
        }

        void ShowPage(Player *player, uint16 page, bool isTop)
        {
            /*uint16 highGames  = 0;
            uint16 lowGames   = 0;*/
			uint16 TypeTwo = 0;
            uint16 TypeThree = 0;
            bool haveNextPage = false;
            for (uint8 i = 0; i <= MAX_BATTLEGROUND_TYPE_ID; ++i)
            {
                if (!sBattlegroundMgr->IsArenaType(BattlegroundTypeId(i)))
                    continue;

                BattlegroundSet bgs = sBattlegroundMgr->GetBattlegroundsByType(BattlegroundTypeId(i));
                //for (BattlegroundSet::iterator itr = bgs.begin(); itr != bgs.end(); ++itr)

				for(BattlegroundSet::iterator itr = bgs.begin(); itr != bgs.end(); ++itr)
				{
                    Battleground* arena = itr->second;

					/*if (Player* target = ObjectAccessor::FindPlayer(GetFirstPlayerGuid(arena)))
						if (target && (target->HasAura(32728) || target->HasAura(32727)))
							continue;*/

                    if (!arena->GetPlayersSize())
                        continue;

                    /*uint16 mmr = arena->GetArenaMatchmakerRating(0) + arena->GetArenaMatchmakerRating(1);
                    mmr /= 2;*/
					uint16 mmrTwo = arena->GetArenaMatchmakerRating(0);
					uint16 mmrThree = arena->GetArenaMatchmakerRating(1);

                    if (isTop && /*mmr >= TopGamesRating*/arena->GetArenaType() == ARENA_TYPE_3v3)
                    {
                        /*highGames++;
                        if(highGames > (page + 1) * GamesOnPage)*/
						TypeThree++;
						if(TypeThree > (page + 1) * GamesOnPage)
                        {
                            haveNextPage = true;
                            break;
                        }

                        /*if(highGames >= page * GamesOnPage)
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGamesStringData(arena, mmr), GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER + GetFirstPlayerGuid(arena));*/
						player->ADD_GOSSIP_ITEM(0, "Arenas 3c3 disput""\xC3\xA1""ndose actualmente:", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_TOP_GAMES);
						if(TypeThree >= page * GamesOnPage)
                           player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGamesStringData(arena, mmrThree), GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER + GetFirstPlayerGuid(arena));
                    }
                    else if (!isTop && /*mmr < TopGamesRating*/arena->GetArenaType() == ARENA_TYPE_2v2)
                    {
                        /*lowGames++;
                        if(lowGames > (page + 1) * GamesOnPage)*/
						TypeTwo++;
                        if(TypeTwo > (page + 1) * GamesOnPage)
                        {
                            haveNextPage = true;
                            break;
                        }

                        /*if(lowGames >= page * GamesOnPage)
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGamesStringData(arena, mmr), GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER + GetFirstPlayerGuid(arena));*/
						player->ADD_GOSSIP_ITEM(0, "Arenas 2c2 disput""\xC3\xA1""ndose actualmente:", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES);
						if(TypeTwo >= page * GamesOnPage)
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGamesStringData(arena, mmrTwo), GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_SELECTED_PLAYER + GetFirstPlayerGuid(arena));
					}
                }
            }

            if (page > 0)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cff9b0000<- Anterior", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES + page - 1);

            if (haveNextPage)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cff2e2efeSiguiente ->", GOSSIP_SENDER_MAIN, NPC_SPECTATOR_ACTION_LIST_GAMES + page + 1);
        }
};


void AddSC_arena_spectator_script()
{
    new arena_spectator_commands();
    new npc_arena_spectator();
}