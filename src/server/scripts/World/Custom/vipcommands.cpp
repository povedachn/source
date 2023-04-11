#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "Chat.h"
#include "Common.h"
#include "Guild.h"
#include "GuildMgr.h"

class vipcommands : public CommandScript
{
	public:
		vipcommands() : CommandScript("vipcommands") { }

		ChatCommand* GetCommands() const
		{
			static ChatCommand vipCommandTable[] =
			{
				{ "shopvip",	    SEC_VIP,     true, &HandleVipMallCommand,         "", NULL },
				//{ "cambioderaza",    SEC_VIP,  false, &HandleChangeRaceCommand,             "", NULL },
				//{ "cambiodefaccion",	SEC_VIP,  false, &HandleChangeFactionCommand,		"", NULL },
				//{ "customizarpj",	SEC_VIP,  false, &HandleCustomizeCommand,		"", NULL },
				//{ "maxskills",	SEC_VIP,  false, &HandleMaxSkillsCommand,		"", NULL },
				{ "reiniciartalentos",   SEC_VIP,  false, &HandleVipResetTalentsCommand,       "", NULL },
				{ "visor",   SEC_VIP,  false, &HandleVipVisorCommand,       "", NULL },
				{ "guildcreate",   SEC_VIP,  false, &HandleVipGuildCreateCommand,       "", NULL },
				{ "titlesadd",   SEC_VIP,  false, &HandleVipTitlesAddCommand,       "", NULL },
				//{ "bank",   SEC_VIP,  false, &HandleVipBankCommand,       "", NULL },
				
				//{ "tele",           SEC_VIP,  false, &HandleTeleCommand,		"", NULL },
			
				{ NULL,             0,                     false, NULL,                                           "", NULL }
			};
			static ChatCommand commandTable[] =
			{
				{ "vip",	    SEC_VIP,   true, NULL,      "",  vipCommandTable},
				{ NULL,             0,                  false, NULL,                               "", NULL }
			};
			return commandTable;
		}

	static bool HandleVipGuildCreateCommand(ChatHandler* handler, const char* args)
	{
		if(!*args)
			return false;
		
		Player* target = handler->GetSession()->GetPlayer();

		char* tailStr = *args != '"' ? strtok(NULL, "") : (char*)args;
		if(!tailStr)
			return false;

		char* guildStr = handler->extractQuotedArg(tailStr);
		if(!guildStr)
			return false;

		std::string guildname = guildStr;
		
		if(guildname.size() > 20)
		{
			handler->SendSysMessage ("Nombre de Hermandad demasiado largo, máximo 20 carácteres.");
			return false;
		}
		
		if(target->GetGuildId())
		{
			handler->SendSysMessage (LANG_PLAYER_IN_GUILD);
			return true;
		}

		Guild* guild = new Guild;
		if(!guild->Create (target, guildname))
		{
			delete guild;
			handler->SendSysMessage (LANG_GUILD_NOT_CREATED);
			handler->SetSentErrorMessage (true);
			return false;
		}

		sGuildMgr->AddGuild(guild);
		return true;
	}
	
	static bool HandleVipTitlesAddCommand(ChatHandler* handler, const char* args)
    {
		Player* target = handler->GetSession()->GetPlayer();

        // number or [name] Shift-click form |color|Htitle:title_id|h[name]|h|r
        char* id_p = handler->extractKeyFromLink((char*)args, "Htitle");
        if(!id_p)
            return false;

        int32 id = atoi(id_p);
        if(id <= 0)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if(id == 128 || 
		id == 163 || 	//Vencedor
		id == 177 || 	//Gladiador colérico
		id == 169 || 	//Gladiador incansable
		id == 167 || 	//Gladiador furioso
		id == 157 || 	//Gladiador mortal
		id == 80 ||  	//Gladiador Brutal
		id == 71 || 	//Gladiador vengativo
		id == 62 ||		//Gladiador despiadado
		id == 82 ||		//Maestro Arena
		id == 45 ||		//Challenger
		id == 44 ||		//Rival
		id == 43 ||		//Duelista
		id == 42		//Gladiador
		)
        {
            handler->PSendSysMessage("Título no permitido.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if(handler->HasLowerSecurity(target, 0))
            return false;

        CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(id);
        if(!titleInfo)
        {
            handler->PSendSysMessage(LANG_INVALID_TITLE_ID, id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        std::string tNameLink = handler->GetNameLink(target);

        char const* targetName = target->GetName();
        char titleNameStr[80];
        snprintf(titleNameStr, 80, titleInfo->name[handler->GetSessionDbcLocale()], targetName);

        target->SetTitle(titleInfo);
        handler->PSendSysMessage(LANG_TITLE_ADD_RES, id, titleNameStr, tNameLink.c_str());

        return true;
    }

	/*static bool HandleTeleCommand(ChatHandler* handler, const char* args)
		{
			if (!*args)
				return false;

			Player* chr = handler->GetSession()->GetPlayer();

			// id, or string, or [name] Shift-click form |color|Htele:id|h[name]|h|r
			GameTele const* tele = handler->extractGameTeleFromLink((char*)args);

			if (!tele)
			{
				handler->SendSysMessage(LANG_COMMAND_TELE_NOTFOUND);
				handler->SetSentErrorMessage(true);
				return false;
			}

			if (chr->isInCombat())
			{
				handler->SendSysMessage(LANG_YOU_IN_COMBAT);
				handler->SetSentErrorMessage(true);
				return false;
			}

			MapEntry const* map = sMapStore.LookupEntry(tele->mapId);
			if (!map || map->IsBattlegroundOrArena())
			{
				handler->SendSysMessage(LANG_CANNOT_TELE_TO_BG);
				handler->SetSentErrorMessage(true);
				return false;
			}

			// Si esta volando se para
			if (chr->isInFlight())
			{
				chr->GetMotionMaster()->MovementExpired();
				chr->CleanupAfterTaxiFlight();
			}
			// Solo se guarda si no esta volando
			else
				chr->SaveRecallPosition();

			chr->TeleportTo(tele->mapId, tele->position_x, tele->position_y, tele->position_z, tele->orientation);
			return true;
			}
	*/
	
	static bool HandlevipCommand(ChatHandler* handler, const char* args)
	{
		Player* chr = handler->GetSession()->GetPlayer();
		chr->Say("Comando VIP?", LANG_UNIVERSAL);
		return true;
	}
	
	static bool HandleChangeRaceCommand(ChatHandler* handler, const char* args)
	{
		Player* chr = handler->GetSession()->GetPlayer();
		chr->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
		handler->PSendSysMessage("Reloguea para cambiar la raza de tu personaje.");
		return true;
	}

	static bool HandleChangeFactionCommand(ChatHandler* handler, const char* args)
	{
		Player* chr = handler->GetSession()->GetPlayer();
		chr->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
		handler->PSendSysMessage("Reloguea para cambiar la faccion de tu personaje.");
		return true;
	}

	static bool HandleMaxSkillsCommand(ChatHandler* handler, const char* args)
	{
		Player* chr = handler->GetSession()->GetPlayer();
		chr->UpdateSkillsForLevel();
		handler->PSendSysMessage("Tus habilidades con armas estan ahora al maximo.");
		return true;
	}

	static bool HandleCustomizeCommand(ChatHandler* handler, const char* args)
	{
		Player* chr = handler->GetSession()->GetPlayer();
		chr->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
		handler->PSendSysMessage("Reloguea para customizar tu personaje.");
		return true;
	}

	/*static bool HandleVipTaxiCommand(ChatHandler* handler, const char* /*args*//* )
	   {
			Player *chr = handler->GetSession()->GetPlayer();
			if(chr->isInCombat() || chr->isInFlight() || chr->GetMap()->IsBattlegroundOrArena() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
			{
				handler->SendSysMessage("No puedes hacer eso");
				handler->SetSentErrorMessage(true);
				return false;
			}

			chr->SetTaxiCheater(true);
			handler->PSendSysMessage(LANG_YOU_GIVE_TAXIS, handler->GetNameLink(chr).c_str());
			if (handler->needReportToTarget(chr))
				ChatHandler(chr).PSendSysMessage(LANG_YOURS_TAXIS_ADDED, handler->GetNameLink().c_str());
			return true;
		}
	*/

	static bool HandleVipResetTalentsCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *chr = handler->GetSession()->GetPlayer();
		if(chr->isInCombat() || chr->isInFlight() || chr->GetMap()->IsBattlegroundOrArena() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
		{
			handler->SendSysMessage("No puedes hacer eso en este momento.");
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		handler->GetSession()->GetPlayer()->resetTalents(true);
		handler->GetSession()->GetPlayer()->SendTalentsInfoData(false);
		handler->PSendSysMessage(LANG_RESET_TALENTS_ONLINE, handler->GetNameLink(handler->GetSession()->GetPlayer()).c_str());
		return true;
	}
		
	static bool HandleVipBankCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *chr = handler->GetSession()->GetPlayer();
		if(chr->GetZoneId() == 14 || chr->isInCombat() || chr->isInFlight() || chr->GetMap()->IsBattlegroundOrArena() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
		{
			handler->SendSysMessage("No puedes hacer eso en este momento.");
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		handler->GetSession()->SendShowBank(handler->GetSession()->GetPlayer()->GetGUID());
		return true;
	}
	
	static bool HandleVipVisorCommand(ChatHandler* handler, const char* /*args*/)
	{
		Player *chr = handler->GetSession()->GetPlayer();
		if (chr->HasItemCount(1404, 1))
		{
			handler->SendSysMessage("Ya dispones del Visor VIP.");
			handler->SetSentErrorMessage(true);
			return true;
		}
		else if (!chr->HasItemCount(1404, 1))
		{
			chr->AddItem(1404, 1);
			return true;
		}
	}

	static bool HandleVipMallCommand(ChatHandler* handler, const char* args)
	{
		Player* chr = handler->GetSession()->GetPlayer();
		if(chr->GetZoneId() == 14 || chr->isInCombat() || chr->GetMap()->IsBattlegroundOrArena() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
		{
			handler->SendSysMessage("No puedes hacer eso en este momento.");
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		if (!chr->HasItemCount(1404, 1))
		{
			handler->SendSysMessage("Necesitas el Visor VIP para poder usar este comando, si no dispones del el, puedes a""\xC3\xB1""adirtelo con el comando .vip visor.");
			handler->SetSentErrorMessage(true);
			return false;
		}

		// Si esta volando se para
		if (chr->isInFlight())
		{
			chr->GetMotionMaster()->MovementExpired();
			chr->CleanupAfterTaxiFlight();
		}
		// Solo se guarda si no esta volando
		else
			chr->SaveRecallPosition();

		chr->TeleportTo(530, 10386.635742f, -6403.786621f, 161.023819f, 3.483490f); // MapId, X, Y, Z, O
		return true;
	}
	
	/*static bool HandleVipVisibilityCommand(ChatHandler* handler, const char* args)
	{
		Player* chr = handler->GetSession()->GetPlayer();
		
		if (!*args)
			return false;
		
		uint8 valor = atoi((char*)args);

		if(valor == 0)
		{
			chr->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_PLAYER);
			chr->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GM, SEC_PLAYER);
			//chr->SetPhaseMask(1, true);
		}
		else if(valor == 1)
		{
			chr->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GM, 1); // Zona de duelos, por defecto
			chr->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, 1);
			chr->UpdateObjectVisibility();
			//chr->SetPhaseMask(3, true);
		}

		return true;
	}*/
};

void AddSC_vipcommands()
{
    new vipcommands();
}