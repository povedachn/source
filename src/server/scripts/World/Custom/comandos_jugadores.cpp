#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "Chat.h"
#include "Common.h"
#include "ArenaTeamMgr.h"
#include "ArenaTeam.h"
	
class comandos_jugadores : public CommandScript
{
public:
    comandos_jugadores() : CommandScript("comandos_jugadores") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand utilityCommandTable[] =
        {
		
			{ "race",    SEC_PLAYER,  false, &HandleChangeRaceCommand,		"", NULL },
			{ "faction",	SEC_PLAYER,  false, &HandleChangeFactionCommand,		"", NULL },
			{ "customize",	SEC_PLAYER,  false, &HandleCustomizeCommand,		"", NULL },
			{ "mmr",	SEC_PLAYER,  false, &HandleMMRCommand,		"", NULL },
 
            { NULL,             0,                     false, NULL,                                           "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "utility",	    SEC_PLAYER,   true, NULL,      "",  utilityCommandTable},
	       { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

	static bool HandleutilityCommand(ChatHandler* handler, const char* args)
    {
        Player* me = handler->GetSession()->GetPlayer();

            me->Say("comando utilidad?", LANG_UNIVERSAL);
            return true;
    }

	static bool HandleChangeRaceCommand(ChatHandler* handler, const char* args)
    {
        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
		handler->PSendSysMessage("Reloguea para cambiar la raza de tu personaje.");
        return true;
    }

	static bool HandleChangeFactionCommand(ChatHandler* handler, const char* args)
    {
        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
		handler->PSendSysMessage("Reloguea para cambiar la faccion de tu personaje.");
        return true;
    }

	static bool HandleCustomizeCommand(ChatHandler* handler, const char* args)
    {
        Player* me = handler->GetSession()->GetPlayer();
		me->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
		handler->PSendSysMessage("Reloguea para customizar personaje.");
        return true;
    }

	static bool HandleMMRCommand(ChatHandler* handler, const char* args)
    {
        Player* me = handler->GetSession()->GetPlayer();

        // Revision de MMR de 2c2
        uint16 mmr1;
        {
			/*if(ArenaTeam* getmmr = sArenaTeamMgr->GetArenaTeamById(me->GetArenaTeamId(0)))
			    mmr1 = getmmr->GetMember(me->GetGUID())->MatchMakerRating;*/
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_LOAD_MATCH_MAKER_RATING);
			stmt->setUInt32(0, GUID_LOPART(me->GetGUID()));
			stmt->setUInt8(1, 0);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if(result)
				mmr1 = (*result)[0].GetUInt32();
		    else if (!mmr1)
			    mmr1 = sWorld->getIntConfig(CONFIG_ARENA_START_MATCHMAKER_RATING);
		}
		
		uint16 mmr2;
		{
			/*if(ArenaTeam* getmmr = sArenaTeamMgr->GetArenaTeamById(me->GetArenaTeamId(1)))
			    mmr2 = getmmr->GetMember(me->GetGUID())->MatchMakerRating;*/
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_LOAD_MATCH_MAKER_RATING);
			stmt->setUInt32(0, GUID_LOPART(me->GetGUID()));
			stmt->setUInt8(1, 1);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if(result)
				mmr2 = (*result)[0].GetUInt32();
		    else if (!mmr2)
			    mmr2 = sWorld->getIntConfig(CONFIG_ARENA_START_MATCHMAKER_RATING);
		}
		
		/*uint16 mmr3;
		{
			PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_LOAD_MATCH_MAKER_RATING);
			stmt->setUInt32(0, GUID_LOPART(me->GetGUID()));
			stmt->setUInt8(1, 2);
			PreparedQueryResult result = CharacterDatabase.Query(stmt);

			if(result)
				mmr3 = (*result)[0].GetUInt32();				
		    else if (!mmr3)
			    mmr3 = sWorld->getIntConfig(CONFIG_ARENA_START_MATCHMAKER_RATING);
		}*/

		handler->PSendSysMessage("|CFFFFD700Tu MMR en 2c2 es de: |CFFFF0000%u", mmr1);
		handler->PSendSysMessage("|CFFFFD700Tu MMR en 3c3 es de: |CFFFF0000%u", mmr2);
		//handler->PSendSysMessage("|CFFFFD700Tu MMR en 5c5 es de: |CFFFF0000%u", mmr3);
	    return true;
    }
	/*static bool HandleMMRCommand(ChatHandler* handler, const char* args)
    {
 
        Player* player = handler->GetSession()->GetPlayer();
                uint64 playerGuid = player->GetGUID();
 
                uint16 *MMR = new uint16[2];
               
                //PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_MATCH_MAKER_RATING); //Trinity
				PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_LOAD_MATCH_MAKER_RATING); //Mythcore
                stmt->setUInt32(0, GUID_LOPART(playerGuid));
 
                for (int i = 0; i < 2; i++)
                {
                        stmt->setUInt8(1, i);
                        PreparedQueryResult result = CharacterDatabase.Query(stmt);
 
                        if (result)
                                MMR[i] = (*result)[0].GetUInt32();
                        else
                                MMR[i] = sWorld->getIntConfig(CONFIG_ARENA_START_MATCHMAKER_RATING);
                }
 
                if (MMR[0] != 0)
                        handler->PSendSysMessage("|CFFFFD700Tu MMR en 2c2 es de: |CFFFF0000%u", MMR[0]);
                else
                        handler->PSendSysMessage("Error interno.");
 
                if (MMR[1] != 0)
                        handler->PSendSysMessage("|CFFFFD700Tu MMR en 3c3 es de: |CFFFF0000%u", MMR[1]);
                else
                        handler->PSendSysMessage("Error interno.");
 
        return true;
    }*/
};

void AddSC_comandos_jugadores()
{
    new comandos_jugadores();
}