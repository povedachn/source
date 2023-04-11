#include "ScriptPCH.h"
#include "BattlegroundMgr.h"
#include "DisableMgr.h"
#include "Group.h"

class challenge_commands : public CommandScript
{
public:
    challenge_commands() : CommandScript("challenge_commands") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand challengeCreateCommandTable[] =
        {
            { "1c1",            SEC_MODERATOR,      false, &HandleChallengeCreate1x1Command,    "", NULL },
            { "2c2",            SEC_MODERATOR,      false, &HandleChallengeCreate2x2Command,    "", NULL },
            { "3c3",            SEC_MODERATOR,      false, &HandleChallengeCreate3x3Command,    "", NULL },
            //{ "5c5",            SEC_MODERATOR,      false, &HandleChallengeCreate5x5Command,    "", NULL },
            { NULL,             0,                  false, NULL,                                "", NULL }
        };

        static ChatCommand challengeCommandTable[] =
        {
            { "crear",         SEC_MODERATOR,      false, NULL,                                "", challengeCreateCommandTable },
            { "aceptar",        SEC_PLAYER,         false, &HandleChallengeAcceptCommand,       "", NULL },
			//{ "rechazar",       SEC_PLAYER,         false, &HandleChallengeRechazarCommand,     "", NULL },
            { "modo",           SEC_PLAYER,         false, &HandleChallengeModeCommand,         "", NULL },
            { "1c1",            SEC_PLAYER,         false, &HandleChallenge1x1Command,          "", NULL },
            { "2c2",            SEC_PLAYER,         false, &HandleChallenge2x2Command,          "", NULL },
            { "3c3",            SEC_PLAYER,         false, &HandleChallenge3x3Command,          "", NULL },
            //{ "5c5",            SEC_PLAYER,         false, &HandleChallenge5x5Command,          "", NULL },
            { "on",             SEC_PLAYER,         false, &HandleChallengeOnCommand,           "", NULL },
            { "off",            SEC_PLAYER,         false, &HandleChallengeOffCommand,          "", NULL },
            { NULL,             0,                  false, NULL,                                "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "challenge",      SEC_PLAYER,         false, NULL,                                "", challengeCommandTable },
            { NULL,             0,                  false, NULL,                                "", NULL }
        };
        return commandTable;
    }

    static bool HandleChallengeModeCommand(ChatHandler* handler, const char* args)
    {
        if (!args)
        {
            handler->PSendSysMessage("Cambia el modo de aceptar los retos, una breve descripción:\n .challenge modo 0: Todo reto que se te haga te preguntará antes si quieres aceptarlo mediante el comando .challenge aceptar\n .challenge modo 1: Todo reto que se te haga será automáticamente aceptado y saldrá el aviso de unirse a Arenas.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        ChallengeOption *option = handler->GetSession()->GetPlayer()->challengeData->options;

        switch (args[0])
        {
            case '1':
                if (option->mode != 1)
                {
                    option->mode   = 1;
                    option->changed = true;
                }
                break;
            case '0':
                if (option->mode != 0)
                {
                    option->mode   = 0;
                    option->changed = true;
                }
                break;
            default:
                handler->PSendSysMessage("Valor incorrecto, solo puedes poner 0 ó 1. \nExplicación: \nCambia el modo de aceptar los retos, una breve descripción:\n .challenge modo 0: Todo reto que se te haga te preguntará antes si quieres aceptarlo mediante el comando .challenge aceptar\n .challenge modo 1: Todo reto que se te haga será automáticamente aceptado y saldrá el aviso de unirse a Arenas.");
                handler->SetSentErrorMessage(true);
                return false;
        }

        handler->PSendSysMessage("Modo cambiado.");
        return true;
    }

    static bool HandleChallengeOnCommand(ChatHandler* handler, const char* /*args*/)
    {
        ChallengeOption *option = handler->GetSession()->GetPlayer()->challengeData->options;
        if (!option->enable)
        {
            option->enable = true;
            option->changed = true;
        }

        handler->PSendSysMessage("Retos habilitado.");
        return true;
    }

    static bool HandleChallengeOffCommand(ChatHandler* handler, const char* /*args*/)
    {
        ChallengeOption *option = handler->GetSession()->GetPlayer()->challengeData->options;
        if (option->enable)
        {
            option->enable = false;
            option->changed = true;
        }

        handler->PSendSysMessage("Retos deshabilitado.");
        return true;
    }

    static bool HandleChallengeAcceptCommand(ChatHandler* handler, const char* /*args*/)
    {
		Player* chr = handler->GetSession()->GetPlayer();
		if(!chr)
			return false;
		
        uint64 challengerGuid = chr->challengeData->challenger;
        if (!challengerGuid)
        {
            handler->PSendSysMessage("No tienes un contrincante.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player *challenger = ObjectAccessor::FindPlayer(challengerGuid);
        if (!challenger)
        {
            handler->PSendSysMessage("No se puede encontrar un contrincante, quizás esté Offline.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if(chr->isInCombat() || chr->isInFlight() || chr->GetMap()->IsBattlegroundOrArena() || chr->HasStealthAura() || chr->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || chr->isDead())
		{
			handler->PSendSysMessage("No puedes hacer eso en este momento (estás en combate, volando, en BG o Arena, sigilo o muerto).");
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		if(chr->GetZoneId() == 14)
		{
			handler->PSendSysMessage("No puedes aceptar retos estando en la Zona PvP.");
			handler->SetSentErrorMessage(true);
			return false;
		}

        if (!sChallengeMgr->InviteGroupsToArena(chr, challenger, (ArenaChallengeType)chr->challengeData->challengeType))
        {
            handler->PSendSysMessage("No puedes unirte a la cola.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        return true;
    }
	
	/*static bool HandleChallengeRechazarCommand(ChatHandler* handler, const char* args)
    {
		Player *player = handler->GetSession()->GetPlayer();
		uint64 challengerGuid = handler->GetSession()->GetPlayer()->challengeData->challenger;
		Player *challenger = ObjectAccessor::FindPlayer(challengerGuid);

		if (!challengerGuid || !challenger || !player)
        {
            handler->PSendSysMessage("No hay ningún contrincante.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if(player->challengeData)
		{
			player->CleanChallengeData();
			ChatHandler(player).PSendSysMessage("La oferta para el reto ha sido rechazada.");
		}

		if(challenger->challengeData)
		{
			challenger->CleanChallengeData();
			ChatHandler(challenger).PSendSysMessage("La oferta para el reto ha sido rechazada.");
		}

		return true;
    }*/

    static bool HandleChallenge1x1Command(ChatHandler* handler, const char* args)
    {
        Player *target = NULL;
        Player *player = handler->GetSession()->GetPlayer();

        if (!handler->extractPlayerTarget((char*)args, &target))
        {
            handler->PSendSysMessage("No se puede encontrar al jugador.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if(player->isInCombat() || player->isInFlight() || player->GetMap()->IsBattlegroundOrArena() || player->HasStealthAura() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
		{
			handler->PSendSysMessage("No puedes hacer eso en este momento.");
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		if(player->GetZoneId() == 14)
		{
			handler->PSendSysMessage("No puedes retar a nadie estando en la Zona PvP.");
			handler->SetSentErrorMessage(true);
			return false;
		}

        if (!target->challengeData->options->enable)
        {
            handler->PSendSysMessage("El jugador no tiene habilitados los retos.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if ((!player->GetGroup() || player->GetGroup()->GetMembersCount() != ARENA_CHALLENGE_TYPE_1v1 ||
            !target->GetGroup() || target->GetGroup()->GetMembersCount() != ARENA_CHALLENGE_TYPE_1v1) && ARENA_CHALLENGE_TYPE_1v1 != ARENA_CHALLENGE_TYPE_1v1)
        {
            handler->PSendSysMessage("Cantidad de jugadores incorrecta o no están en el mismo grupo.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->challengeData->options->mode == 0)
        {
            if (!sChallengeMgr->MakeChallengeOffer(player, target, (ArenaChallengeType)ARENA_CHALLENGE_TYPE_1v1))
            {
                handler->PSendSysMessage("No puedes unirte a la cola.");
                handler->SetSentErrorMessage(true);
                return false;
            }

			//Creamos los colores custom
			//Colores retado
			char* color_clase_retado;
			switch (target->getClass())
			{
				case CLASS_WARRIOR:
					color_clase_retado = "|CFFC79C6E";
				break;
				case CLASS_DEATH_KNIGHT:
					color_clase_retado = "|CFFC41F3B";
				break;
				case CLASS_DRUID:
					color_clase_retado = "|CFFFF7D0A";
				break;
				case CLASS_HUNTER:
					color_clase_retado = "|CFFABD473";
				break;
				case CLASS_MAGE:
					color_clase_retado = "|CFF69CCF0";
				break;
				case CLASS_PALADIN:
					color_clase_retado = "|CFFF58CBA";
				break;
				case CLASS_PRIEST:
					color_clase_retado = "|CFFFFFFFF";
				break;
				case CLASS_ROGUE:
					color_clase_retado = "|CFFFFF569";
				break;
				case CLASS_SHAMAN:
					color_clase_retado = "|CFF0070DE";
				break;
				case CLASS_WARLOCK:
					color_clase_retado = "|CFF9482C9";
				break;
			}

            handler->PSendSysMessage("Has retado a %s%s|r. Espera 20 segundos mientras toma una decisión.", color_clase_retado, target->GetName());
        }
        else
        {
            if (!sChallengeMgr->InviteGroupsToArena(player, target, (ArenaChallengeType)ARENA_CHALLENGE_TYPE_1v1))
            {
                handler->PSendSysMessage("No puedes unirte a la cola.");
                handler->SetSentErrorMessage(true);
                return false;
            }
        }
		
        return true;
    }

    static bool HandleChallenge2x2Command(ChatHandler* handler, const char* args)
    {
        Player *target = NULL;
        Player *player = handler->GetSession()->GetPlayer();

        if (!handler->extractPlayerTarget((char*)args, &target))
        {
            handler->PSendSysMessage("No se puede encontrar al jugador.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if(player->isInCombat() || player->isInFlight() || player->GetMap()->IsBattlegroundOrArena() || player->HasStealthAura() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
		{
			handler->PSendSysMessage("No puedes hacer eso en este momento.");
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		if(player->GetZoneId() == 14)
		{
			handler->PSendSysMessage("No puedes retar a nadie estando en la Zona PvP.");
			handler->SetSentErrorMessage(true);
			return false;
		}

        if (!target->challengeData->options->enable)
        {
            handler->PSendSysMessage("El jugador no tiene habilitados los retos.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if ((!player->GetGroup() || player->GetGroup()->GetMembersCount() != ARENA_CHALLENGE_TYPE_2v2 ||
            !target->GetGroup() || target->GetGroup()->GetMembersCount() != ARENA_CHALLENGE_TYPE_2v2) && ARENA_CHALLENGE_TYPE_2v2 != ARENA_CHALLENGE_TYPE_2v2)
        {
            handler->PSendSysMessage("Cantidad de jugadores incorrecta o no están en el mismo grupo.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->challengeData->options->mode == 0)
        {
            if (!sChallengeMgr->MakeChallengeOffer(player, target, (ArenaChallengeType)ARENA_CHALLENGE_TYPE_2v2))
            {
                handler->PSendSysMessage("No puedes unirte a la cola.");
                handler->SetSentErrorMessage(true);
                return false;
            }

			//Creamos los colores custom
			//Colores retado
			char* color_clase_retado;
			switch (target->getClass())
			{
				case CLASS_WARRIOR:
					color_clase_retado = "|CFFC79C6E";
				break;
				case CLASS_DEATH_KNIGHT:
					color_clase_retado = "|CFFC41F3B";
				break;
				case CLASS_DRUID:
					color_clase_retado = "|CFFFF7D0A";
				break;
				case CLASS_HUNTER:
					color_clase_retado = "|CFFABD473";
				break;
				case CLASS_MAGE:
					color_clase_retado = "|CFF69CCF0";
				break;
				case CLASS_PALADIN:
					color_clase_retado = "|CFFF58CBA";
				break;
				case CLASS_PRIEST:
					color_clase_retado = "|CFFFFFFFF";
				break;
				case CLASS_ROGUE:
					color_clase_retado = "|CFFFFF569";
				break;
				case CLASS_SHAMAN:
					color_clase_retado = "|CFF0070DE";
				break;
				case CLASS_WARLOCK:
					color_clase_retado = "|CFF9482C9";
				break;
			}

            handler->PSendSysMessage("Has retado a %s%s|r. Espera 20 segundos mientras toma una decisión.", color_clase_retado, target->GetName());
        }
        else
        {
            if (!sChallengeMgr->InviteGroupsToArena(player, target, (ArenaChallengeType)ARENA_CHALLENGE_TYPE_2v2))
            {
                handler->PSendSysMessage("No puedes unirte a la cola.");
                handler->SetSentErrorMessage(true);
                return false;
            }
        }
		
        return true;
    }

    static bool HandleChallenge3x3Command(ChatHandler* handler, const char* args)
    {
        Player *target = NULL;
        Player *player = handler->GetSession()->GetPlayer();

        if (!handler->extractPlayerTarget((char*)args, &target))
        {
            handler->PSendSysMessage("No se puede encontrar al jugador.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if(player->isInCombat() || player->isInFlight() || player->GetMap()->IsBattlegroundOrArena() || player->HasStealthAura() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
		{
			handler->PSendSysMessage("No puedes hacer eso en este momento.");
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		if(player->GetZoneId() == 14)
		{
			handler->PSendSysMessage("No puedes retar a nadie estando en la Zona PvP.");
			handler->SetSentErrorMessage(true);
			return false;
		}

        if (!target->challengeData->options->enable)
        {
            handler->PSendSysMessage("El jugador no tiene habilitados los retos.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if ((!player->GetGroup() || player->GetGroup()->GetMembersCount() != ARENA_CHALLENGE_TYPE_3v3 ||
            !target->GetGroup() || target->GetGroup()->GetMembersCount() != ARENA_CHALLENGE_TYPE_3v3) && ARENA_CHALLENGE_TYPE_3v3 != ARENA_CHALLENGE_TYPE_3v3)
        {
            handler->PSendSysMessage("Cantidad de jugadores incorrecta o no están en el mismo grupo.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->challengeData->options->mode == 0)
        {
            if (!sChallengeMgr->MakeChallengeOffer(player, target, (ArenaChallengeType)ARENA_CHALLENGE_TYPE_3v3))
            {
                handler->PSendSysMessage("No puedes unirte a la cola.");
                handler->SetSentErrorMessage(true);
                return false;
            }

			//Creamos los colores custom
			//Colores retado
			char* color_clase_retado;
			switch (target->getClass())
			{
				case CLASS_WARRIOR:
					color_clase_retado = "|CFFC79C6E";
				break;
				case CLASS_DEATH_KNIGHT:
					color_clase_retado = "|CFFC41F3B";
				break;
				case CLASS_DRUID:
					color_clase_retado = "|CFFFF7D0A";
				break;
				case CLASS_HUNTER:
					color_clase_retado = "|CFFABD473";
				break;
				case CLASS_MAGE:
					color_clase_retado = "|CFF69CCF0";
				break;
				case CLASS_PALADIN:
					color_clase_retado = "|CFFF58CBA";
				break;
				case CLASS_PRIEST:
					color_clase_retado = "|CFFFFFFFF";
				break;
				case CLASS_ROGUE:
					color_clase_retado = "|CFFFFF569";
				break;
				case CLASS_SHAMAN:
					color_clase_retado = "|CFF0070DE";
				break;
				case CLASS_WARLOCK:
					color_clase_retado = "|CFF9482C9";
				break;
			}

            handler->PSendSysMessage("Has retado a %s%s|r. Espera 20 segundos mientras toma una decisión.", color_clase_retado, target->GetName());
        }
        else
        {
            if (!sChallengeMgr->InviteGroupsToArena(player, target, (ArenaChallengeType)ARENA_CHALLENGE_TYPE_3v3))
            {
                handler->PSendSysMessage("No puedes unirte a la cola.");
                handler->SetSentErrorMessage(true);
                return false;
            }
        }
		
        return true;
    }

    /*static bool HandleChallenge5x5Command(ChatHandler* handler, const char* args)
    {
        return ChallengeForBracket(handler, args, ARENA_CHALLENGE_TYPE_5v5);
    }*/

    /*static bool ChallengeForBracket(ChatHandler* handler, const char* args, uint8 type)
    {
        Player *target = NULL;
        Player *player = handler->GetSession()->GetPlayer();

        if (!handler->extractPlayerTarget((char*)args, &target))
        {
            handler->PSendSysMessage("No se puede encontrar al jugador.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if(player->isInCombat() || player->isInFlight() || player->GetMap()->IsBattlegroundOrArena() || player->HasStealthAura() || player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || player->isDead())
		{
			handler->PSendSysMessage("No puedes hacer eso en este momento.");
			handler->SetSentErrorMessage(true);
			return false;
		}
		
		if(player->GetZoneId() == 14)
		{
			handler->PSendSysMessage("No puedes retar a nadie estando en la Zona PvP.");
			handler->SetSentErrorMessage(true);
			return false;
		}

        if (!target->challengeData->options->enable)
        {
            handler->PSendSysMessage("El jugador no tiene habilitados los retos.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if ((!player->GetGroup() || player->GetGroup()->GetMembersCount() != type ||
            !target->GetGroup() || target->GetGroup()->GetMembersCount() != type) && type != ARENA_CHALLENGE_TYPE_1v1)
        {
            handler->PSendSysMessage("Cantidad de jugadores incorrecta o no están en el mismo grupo.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (target->challengeData->options->mode == 0)
        {
            if (!sChallengeMgr->MakeChallengeOffer(handler->GetSession()->GetPlayer(), target, (ArenaChallengeType)type))
            {
                handler->PSendSysMessage("No puedes unirte a la cola.");
                handler->SetSentErrorMessage(true);
                return false;
            }

			//Creamos los colores custom
			//Colores retado
			char* color_clase_retado;
			switch (target->getClass())
			{
				case CLASS_WARRIOR:
					color_clase_retado = "|CFFC79C6E";
				break;
				case CLASS_DEATH_KNIGHT:
					color_clase_retado = "|CFFC41F3B";
				break;
				case CLASS_DRUID:
					color_clase_retado = "|CFFFF7D0A";
				break;
				case CLASS_HUNTER:
					color_clase_retado = "|CFFABD473";
				break;
				case CLASS_MAGE:
					color_clase_retado = "|CFF69CCF0";
				break;
				case CLASS_PALADIN:
					color_clase_retado = "|CFFF58CBA";
				break;
				case CLASS_PRIEST:
					color_clase_retado = "|CFFFFFFFF";
				break;
				case CLASS_ROGUE:
					color_clase_retado = "|CFFFFF569";
				break;
				case CLASS_SHAMAN:
					color_clase_retado = "|CFF0070DE";
				break;
				case CLASS_WARLOCK:
					color_clase_retado = "|CFF9482C9";
				break;
			}

            handler->PSendSysMessage("Has retado a %s%s|r. Espera 20 segundos mientras toma una decisión.", color_clase_retado, target->GetName());
        }
        else
        {
            if (!sChallengeMgr->InviteGroupsToArena(handler->GetSession()->GetPlayer(), target, (ArenaChallengeType)type))
            {
                handler->PSendSysMessage("No puedes unirte a la cola.");
                handler->SetSentErrorMessage(true);
                return false;
            }
        }
		
        return true;
    }*/

    static bool HandleChallengeCreate1x1Command(ChatHandler* handler, const char* args)
    {
        return ChallengeCreateForBracket(handler, args, ARENA_CHALLENGE_TYPE_1v1);
    }

    static bool HandleChallengeCreate2x2Command(ChatHandler* handler, const char* args)
    {
		std::string sargs = args;
        Tokens playersTokens(sargs, ' ');
        if (playersTokens.size() != 4)
        {
            handler->PSendSysMessage("Se necesitan como mínimo 4 nombres de jugadores.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player *player1 = sObjectMgr->GetPlayer(playersTokens[0]);
        Player *player2 = sObjectMgr->GetPlayer(playersTokens[1]);
		Player *player3 = sObjectMgr->GetPlayer(playersTokens[2]);
		Player *player4 = sObjectMgr->GetPlayer(playersTokens[3]);

        if (!player1)
        {
            handler->PSendSysMessage("No se puede encontrar al jugador 1.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!player2)
        {
            handler->PSendSysMessage("No se puede encontrar al jugador 2.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!player3)
        {
            handler->PSendSysMessage("No se puede encontrar al jugador 3.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!player4)
        {
            handler->PSendSysMessage("No se puede encontrar al jugador 4.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if ((!player1->GetGroup() || player1->GetGroup()->GetMembersCount() != 2 ||
            !player2->GetGroup() || player2->GetGroup()->GetMembersCount() != 2 ||
            !player3->GetGroup() || player3->GetGroup()->GetMembersCount() != 2 ||
            !player4->GetGroup() || player4->GetGroup()->GetMembersCount() != 2) && !ARENA_CHALLENGE_TYPE_2v2)
        {
            handler->PSendSysMessage("Algún jugador (o todos) no están en grupo o su grupo es mayor a 2 jugadores.");
            handler->SetSentErrorMessage(true);
            return false;
        }

		if(!player1->IsInSameGroupWith(player2))
		{
            handler->PSendSysMessage("Los jugadores 1 y 2 no están en el mismo grupo.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if(!player3->IsInSameGroupWith(player4))
		{
            handler->PSendSysMessage("Los jugadores 3 y 4 no están en el mismo grupo.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sChallengeMgr->InviteGroupsToArena(player1, player3, (ArenaChallengeType)ARENA_CHALLENGE_TYPE_2v2))
        {
            handler->PSendSysMessage("Alguno de los 4 participantes está en cola de Arenas o BG, diles que salgan de cola.");
            handler->SetSentErrorMessage(true);
            return false;
        }

		char* color_clase_player1;
		switch (player1->getClass())
		{
			case CLASS_WARRIOR:
				color_clase_player1 = "|CFFC79C6E";
			break;
			case CLASS_DEATH_KNIGHT:
				color_clase_player1 = "|CFFC41F3B";
			break;
			case CLASS_DRUID:
				color_clase_player1 = "|CFFFF7D0A";
			break;
			case CLASS_HUNTER:
				color_clase_player1 = "|CFFABD473";
			break;
			case CLASS_MAGE:
				color_clase_player1 = "|CFF69CCF0";
			break;
			case CLASS_PALADIN:
				color_clase_player1 = "|CFFF58CBA";
			break;
			case CLASS_PRIEST:
				color_clase_player1 = "|CFFFFFFFF";
			break;
			case CLASS_ROGUE:
				color_clase_player1 = "|CFFFFF569";
			break;
			case CLASS_SHAMAN:
				color_clase_player1 = "|CFF0070DE";
			break;
			case CLASS_WARLOCK:
				color_clase_player1 = "|CFF9482C9";
			break;
		}
		
		char* color_clase_player2;
		switch (player2->getClass())
		{
			case CLASS_WARRIOR:
				color_clase_player2 = "|CFFC79C6E";
			break;
			case CLASS_DEATH_KNIGHT:
				color_clase_player2 = "|CFFC41F3B";
			break;
			case CLASS_DRUID:
				color_clase_player2 = "|CFFFF7D0A";
			break;
			case CLASS_HUNTER:
				color_clase_player2 = "|CFFABD473";
			break;
			case CLASS_MAGE:
				color_clase_player2 = "|CFF69CCF0";
			break;
			case CLASS_PALADIN:
				color_clase_player2 = "|CFFF58CBA";
			break;
			case CLASS_PRIEST:
				color_clase_player2 = "|CFFFFFFFF";
			break;
			case CLASS_ROGUE:
				color_clase_player2 = "|CFFFFF569";
			break;
			case CLASS_SHAMAN:
				color_clase_player2 = "|CFF0070DE";
			break;
			case CLASS_WARLOCK:
				color_clase_player2 = "|CFF9482C9";
			break;
		}
		
		char* color_clase_player3;
		switch (player3->getClass())
		{
			case CLASS_WARRIOR:
				color_clase_player3 = "|CFFC79C6E";
			break;
			case CLASS_DEATH_KNIGHT:
				color_clase_player3 = "|CFFC41F3B";
			break;
			case CLASS_DRUID:
				color_clase_player3 = "|CFFFF7D0A";
			break;
			case CLASS_HUNTER:
				color_clase_player3 = "|CFFABD473";
			break;
			case CLASS_MAGE:
				color_clase_player3 = "|CFF69CCF0";
			break;
			case CLASS_PALADIN:
				color_clase_player3 = "|CFFF58CBA";
			break;
			case CLASS_PRIEST:
				color_clase_player3 = "|CFFFFFFFF";
			break;
			case CLASS_ROGUE:
				color_clase_player3 = "|CFFFFF569";
			break;
			case CLASS_SHAMAN:
				color_clase_player3 = "|CFF0070DE";
			break;
			case CLASS_WARLOCK:
				color_clase_player3 = "|CFF9482C9";
			break;
		}
		
		char* color_clase_player4;
		switch (player4->getClass())
		{
			case CLASS_WARRIOR:
				color_clase_player4 = "|CFFC79C6E";
			break;
			case CLASS_DEATH_KNIGHT:
				color_clase_player4 = "|CFFC41F3B";
			break;
			case CLASS_DRUID:
				color_clase_player4 = "|CFFFF7D0A";
			break;
			case CLASS_HUNTER:
				color_clase_player4 = "|CFFABD473";
			break;
			case CLASS_MAGE:
				color_clase_player4 = "|CFF69CCF0";
			break;
			case CLASS_PALADIN:
				color_clase_player4 = "|CFFF58CBA";
			break;
			case CLASS_PRIEST:
				color_clase_player4 = "|CFFFFFFFF";
			break;
			case CLASS_ROGUE:
				color_clase_player4 = "|CFFFFF569";
			break;
			case CLASS_SHAMAN:
				color_clase_player4 = "|CFF0070DE";
			break;
			case CLASS_WARLOCK:
				color_clase_player4 = "|CFF9482C9";
			break;
		}

		sWorld->SendAllWorldText("Se ha creado un Challenge de 2c2 entre: %s%s|r - %s%s|r VS %s%s|r - %s%s|r", color_clase_player1, player1->GetName(), color_clase_player2, player2->GetName(), color_clase_player3, player3->GetName(), color_clase_player4, player4->GetName());
        return true;
    }

    static bool HandleChallengeCreate3x3Command(ChatHandler* handler, const char* args)
    {
        std::string sargs = args;
        Tokens playersTokens(sargs, ' ');
        if (playersTokens.size() != 6)
        {
            handler->PSendSysMessage("Se necesitan como mínimo 6 nombres de jugadores.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player *player1 = sObjectMgr->GetPlayer(playersTokens[0]);
        Player *player2 = sObjectMgr->GetPlayer(playersTokens[1]);
		Player *player3 = sObjectMgr->GetPlayer(playersTokens[2]);
		Player *player4 = sObjectMgr->GetPlayer(playersTokens[3]);
		Player *player5 = sObjectMgr->GetPlayer(playersTokens[4]);
		Player *player6 = sObjectMgr->GetPlayer(playersTokens[5]);

        if (!player1)
        {
            handler->PSendSysMessage("No se puede encontrar al jugador 1.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!player2)
        {
            handler->PSendSysMessage("No se puede encontrar al jugador 2.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!player3)
        {
            handler->PSendSysMessage("No se puede encontrar al jugador 3.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!player4)
        {
            handler->PSendSysMessage("No se puede encontrar al jugador 4.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!player5)
        {
            handler->PSendSysMessage("No se puede encontrar al jugador 5.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if (!player6)
        {
            handler->PSendSysMessage("No se puede encontrar al jugador 6.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if ((!player1->GetGroup() || player1->GetGroup()->GetMembersCount() != 3 ||
            !player2->GetGroup() || player2->GetGroup()->GetMembersCount() != 3 ||
            !player3->GetGroup() || player3->GetGroup()->GetMembersCount() != 3 ||
            !player4->GetGroup() || player4->GetGroup()->GetMembersCount() != 3 ||
            !player5->GetGroup() || player5->GetGroup()->GetMembersCount() != 3 ||
            !player6->GetGroup() || player6->GetGroup()->GetMembersCount() != 3) && !ARENA_CHALLENGE_TYPE_3v3)
        {
            handler->PSendSysMessage("Algún jugador (o todos) no están en grupo o su grupo es mayor o menor a 3 jugadores.");
            handler->SetSentErrorMessage(true);
            return false;
        }

		if(!player1->IsInSameGroupWith(player2) || !player1->IsInSameGroupWith(player3) || !player2->IsInSameGroupWith(player3))
		{
            handler->PSendSysMessage("Algunos de los jugadores 1, 2 o 3 no está en grupo con los demás.");
            handler->SetSentErrorMessage(true);
            return false;
        }
		
		if(!player4->IsInSameGroupWith(player5) || !player4->IsInSameGroupWith(player6) || !player5->IsInSameGroupWith(player6))
		{
            handler->PSendSysMessage("Algunos de los jugadores 4, 5 o 6 no está en grupo con los demás.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sChallengeMgr->InviteGroupsToArena(player1, player4, (ArenaChallengeType)ARENA_CHALLENGE_TYPE_3v3))
        {
            handler->PSendSysMessage("Alguno de los 6 participantes está en cola de Arenas o BG, diles que salgan de cola.");
            handler->SetSentErrorMessage(true);
            return false;
        }

		char* color_clase_player1;
		switch (player1->getClass())
		{
			case CLASS_WARRIOR:
				color_clase_player1 = "|CFFC79C6E";
			break;
			case CLASS_DEATH_KNIGHT:
				color_clase_player1 = "|CFFC41F3B";
			break;
			case CLASS_DRUID:
				color_clase_player1 = "|CFFFF7D0A";
			break;
			case CLASS_HUNTER:
				color_clase_player1 = "|CFFABD473";
			break;
			case CLASS_MAGE:
				color_clase_player1 = "|CFF69CCF0";
			break;
			case CLASS_PALADIN:
				color_clase_player1 = "|CFFF58CBA";
			break;
			case CLASS_PRIEST:
				color_clase_player1 = "|CFFFFFFFF";
			break;
			case CLASS_ROGUE:
				color_clase_player1 = "|CFFFFF569";
			break;
			case CLASS_SHAMAN:
				color_clase_player1 = "|CFF0070DE";
			break;
			case CLASS_WARLOCK:
				color_clase_player1 = "|CFF9482C9";
			break;
		}
		
		char* color_clase_player2;
		switch (player2->getClass())
		{
			case CLASS_WARRIOR:
				color_clase_player2 = "|CFFC79C6E";
			break;
			case CLASS_DEATH_KNIGHT:
				color_clase_player2 = "|CFFC41F3B";
			break;
			case CLASS_DRUID:
				color_clase_player2 = "|CFFFF7D0A";
			break;
			case CLASS_HUNTER:
				color_clase_player2 = "|CFFABD473";
			break;
			case CLASS_MAGE:
				color_clase_player2 = "|CFF69CCF0";
			break;
			case CLASS_PALADIN:
				color_clase_player2 = "|CFFF58CBA";
			break;
			case CLASS_PRIEST:
				color_clase_player2 = "|CFFFFFFFF";
			break;
			case CLASS_ROGUE:
				color_clase_player2 = "|CFFFFF569";
			break;
			case CLASS_SHAMAN:
				color_clase_player2 = "|CFF0070DE";
			break;
			case CLASS_WARLOCK:
				color_clase_player2 = "|CFF9482C9";
			break;
		}
		
		char* color_clase_player3;
		switch (player3->getClass())
		{
			case CLASS_WARRIOR:
				color_clase_player3 = "|CFFC79C6E";
			break;
			case CLASS_DEATH_KNIGHT:
				color_clase_player3 = "|CFFC41F3B";
			break;
			case CLASS_DRUID:
				color_clase_player3 = "|CFFFF7D0A";
			break;
			case CLASS_HUNTER:
				color_clase_player3 = "|CFFABD473";
			break;
			case CLASS_MAGE:
				color_clase_player3 = "|CFF69CCF0";
			break;
			case CLASS_PALADIN:
				color_clase_player3 = "|CFFF58CBA";
			break;
			case CLASS_PRIEST:
				color_clase_player3 = "|CFFFFFFFF";
			break;
			case CLASS_ROGUE:
				color_clase_player3 = "|CFFFFF569";
			break;
			case CLASS_SHAMAN:
				color_clase_player3 = "|CFF0070DE";
			break;
			case CLASS_WARLOCK:
				color_clase_player3 = "|CFF9482C9";
			break;
		}
		
		char* color_clase_player4;
		switch (player4->getClass())
		{
			case CLASS_WARRIOR:
				color_clase_player4 = "|CFFC79C6E";
			break;
			case CLASS_DEATH_KNIGHT:
				color_clase_player4 = "|CFFC41F3B";
			break;
			case CLASS_DRUID:
				color_clase_player4 = "|CFFFF7D0A";
			break;
			case CLASS_HUNTER:
				color_clase_player4 = "|CFFABD473";
			break;
			case CLASS_MAGE:
				color_clase_player4 = "|CFF69CCF0";
			break;
			case CLASS_PALADIN:
				color_clase_player4 = "|CFFF58CBA";
			break;
			case CLASS_PRIEST:
				color_clase_player4 = "|CFFFFFFFF";
			break;
			case CLASS_ROGUE:
				color_clase_player4 = "|CFFFFF569";
			break;
			case CLASS_SHAMAN:
				color_clase_player4 = "|CFF0070DE";
			break;
			case CLASS_WARLOCK:
				color_clase_player4 = "|CFF9482C9";
			break;
		}
		
		char* color_clase_player5;
		switch (player5->getClass())
		{
			case CLASS_WARRIOR:
				color_clase_player5 = "|CFFC79C6E";
			break;
			case CLASS_DEATH_KNIGHT:
				color_clase_player5 = "|CFFC41F3B";
			break;
			case CLASS_DRUID:
				color_clase_player5 = "|CFFFF7D0A";
			break;
			case CLASS_HUNTER:
				color_clase_player5 = "|CFFABD473";
			break;
			case CLASS_MAGE:
				color_clase_player5 = "|CFF69CCF0";
			break;
			case CLASS_PALADIN:
				color_clase_player5 = "|CFFF58CBA";
			break;
			case CLASS_PRIEST:
				color_clase_player5 = "|CFFFFFFFF";
			break;
			case CLASS_ROGUE:
				color_clase_player5 = "|CFFFFF569";
			break;
			case CLASS_SHAMAN:
				color_clase_player5 = "|CFF0070DE";
			break;
			case CLASS_WARLOCK:
				color_clase_player5 = "|CFF9482C9";
			break;
		}
		
		char* color_clase_player6;
		switch (player6->getClass())
		{
			case CLASS_WARRIOR:
				color_clase_player6 = "|CFFC79C6E";
			break;
			case CLASS_DEATH_KNIGHT:
				color_clase_player6 = "|CFFC41F3B";
			break;
			case CLASS_DRUID:
				color_clase_player6 = "|CFFFF7D0A";
			break;
			case CLASS_HUNTER:
				color_clase_player6 = "|CFFABD473";
			break;
			case CLASS_MAGE:
				color_clase_player6 = "|CFF69CCF0";
			break;
			case CLASS_PALADIN:
				color_clase_player6 = "|CFFF58CBA";
			break;
			case CLASS_PRIEST:
				color_clase_player6 = "|CFFFFFFFF";
			break;
			case CLASS_ROGUE:
				color_clase_player6 = "|CFFFFF569";
			break;
			case CLASS_SHAMAN:
				color_clase_player6 = "|CFF0070DE";
			break;
			case CLASS_WARLOCK:
				color_clase_player6 = "|CFF9482C9";
			break;
		}

		sWorld->SendAllWorldText("Se ha creado un Challenge de 3c3 entre: %s%s|r - %s%s|r - %s%s|r VS %s%s|r - %s%s|r - %s%s|r", color_clase_player1, player1->GetName(), color_clase_player2, player2->GetName(), color_clase_player3, player3->GetName(), color_clase_player4, player4->GetName(), color_clase_player5, player5->GetName(), color_clase_player6, player6->GetName());
        return true;
    }

    /*static bool HandleChallengeCreate5x5Command(ChatHandler* handler, const char* args)
    {
        return ChallengeCreateForBracket(handler, args, ARENA_CHALLENGE_TYPE_5v5);
    }*/

    static bool ChallengeCreateForBracket(ChatHandler* handler, const char* args, uint8 type)
    {
        std::string sargs = args;
        Tokens playersTokens(sargs, ' ');
        if (playersTokens.size() != 2)
        {
            handler->PSendSysMessage("Se necesitan como mínimo 2 argumentos.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player *player1 = sObjectMgr->GetPlayer(playersTokens[0]);
        Player *player2 = sObjectMgr->GetPlayer(playersTokens[1]);

        if (!player1 || !player2)
        {
            handler->PSendSysMessage("No se puede encontrar a algún jugador.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if ((!player1->GetGroup() || player1->GetGroup()->GetMembersCount() != type ||
            !player2->GetGroup() || player2->GetGroup()->GetMembersCount() != type) && type != ARENA_CHALLENGE_TYPE_1v1)
        {
            handler->PSendSysMessage("Cantidad de jugadores incorrecta.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!sChallengeMgr->InviteGroupsToArena(player1, player2, (ArenaChallengeType)type))
        {
            handler->PSendSysMessage("No puedes unirte a la cola.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        return true;
    }
};

void AddSC_challenge_script()
{
    new challenge_commands();
}