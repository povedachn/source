#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "Chat.h"
#include "Common.h"

class fin_season : public CommandScript
{
	public:
		fin_season() : CommandScript("fin_season") { }

		ChatCommand* GetCommands() const
		{
			static ChatCommand SeasonCommandTable[] =
			{
				{ "fin_season",   SEC_PLAYER,  false, &HandleSeasonFinSeasonCommand,       "", NULL },

				{ NULL,             0,                     false, NULL,                                           "", NULL }
			};
			static ChatCommand commandTable[] =
			{
				{ "season",	    SEC_PLAYER,   true, NULL,      "",  SeasonCommandTable},
				{ NULL,             0,                  false, NULL,                               "", NULL }
			};
			return commandTable;
		}
		
	static bool HandleSeasonFinSeasonCommand(ChatHandler* handler, const char* args)
	{
		Player* chr = handler->GetSession()->GetPlayer();
		
		int calificacion=1;
		int8 season=1;
			
		QueryResult resultseason = CharacterDatabase.PQuery("SELECT MAX(`season`) FROM `arena_season_char`");
		if(resultseason)
		{
			Field *ss = resultseason->Fetch();
			season = (ss[0].GetUInt32() +1);
		}
		
		QueryResult result_teams = CharacterDatabase.PQuery("SELECT `arenaTeamId`, `name`, `rating`, `seasonGames`, `seasonWins`, `type` FROM `arena_team` where `type`='2'  order by `rating` desc limit 3"); //Equipos de 2vs2
		if(result_teams)
		{
			do
			{
				Field *team = result_teams->Fetch();
				int32 arenaTeamId=team[0].GetUInt32();
				//std::string name = team[1].GetString();
				int32 rating_e=team[2].GetUInt32();
				int32 games_e=team[3].GetUInt32();
				int32 wins_e=team[4].GetUInt32();
				int32 loss_e=games_e - wins_e;
				int8  type=team[5].GetUInt32();
				
				QueryResult resultChars = CharacterDatabase.PQuery("SELECT `guid`, `personalRating`, `seasonGames`, `seasonWins` FROM `arena_team_member` WHERE `arenaTeamID`='%u' order by `seasonGames` desc limit 2", arenaTeamId); //Equipos de 2vs2
				if(resultChars)
				{
					do
					{
						Field *charac = resultChars->Fetch();
						int32 guid=charac[0].GetUInt32();
						int32 rating_p=charac[1].GetUInt32();
						int32 games_p=charac[2].GetUInt32();
						int32 wins_p=charac[3].GetUInt32();
						int32 loss_p=games_p - wins_p;

						Player* plr = sObjectMgr->GetPlayer(guid);	

						//Entrega de premios
						/*switch(calificacion)
						{
							case 1: //Primer puesto
							{
								SQLTransaction trans = CharacterDatabase.BeginTransaction();
								MailDraft draft("Premio de Season: TOP 1", "Enhorabuena por tu TOP 1 en la Season de Array, a continuación dispones de los premios que se te han otorgado por esa clasificación.");
								draft.AddItem(Item::CreateItem(17031, 1, plr ? plr : 0));
								draft.SendMailTo(trans, MailReceiver(plr, GUID_LOPART(guid)), MailSender(chr, MAIL_STATIONERY_GM), MAIL_CHECK_MASK_COPIED);
								CharacterDatabase.CommitTransaction(trans);
							}
							break;
									
							/*case 2: //Segundo puesto
								plr->AddItem(12064, 1);
							break;
									
							case 3: //Tercer puesto
								plr->AddItem(12064, 1);
							break;
						}*/
																																												   //season - calificacion - guid player - /*nombre equipo*/ - rating equipo - rating personal - jugadas equipo - ganadas equipo - derrotas equipo - jugadas personal - ganadas personal - derrotas personal - type
						QueryResult ganadores = CharacterDatabase.PQuery("REPLACE INTO `arena_season_char` VALUES('%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u')", season, calificacion, guid, /*name,*/rating_e, rating_p, games_e, wins_e, loss_e, games_p, wins_p, loss_p, type); //Equipos de 2vs2
					}while(resultChars->NextRow());
				}
				calificacion++;
			}while(result_teams->NextRow());
		}
		handler->PSendSysMessage("Tabla arena_season_char actualizada y premios entregados.");
		return true;
	}
};

void AddSC_fin_season()
{
    new fin_season();
}