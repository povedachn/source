#include "ScriptMgr.h"
#include "Common.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "ArenaTeamMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include "ArenaTeam.h"
#include "BattlegroundMgr.h"
#include "BattlegroundWS.h"
#include "Battleground.h"
#include "Chat.h"
#include "Language.h"
#include "Log.h"
#include "Player.h"
#include "Object.h"
#include "Opcodes.h"
#include "DisableMgr.h"
#include "Group.h"
#include "OutdoorPvPWG.h"

#include "OutdoorPvPMgr.h"

int Habilitada3()
{
	QueryResult valor5 = WorldDatabase.PQuery("SELECT Deshabilitadas FROM arenas_habilitadas WHERE ID = 1");

	if (!valor5)
		return 0;
	else
	{
		Field *fields = valor5->Fetch();
			return fields[0].GetUInt32();
	}
}

class arenaqueue : public CommandScript
{
public:
  arenaqueue() : CommandScript("arenaqueue") { }
 
  ChatCommand* GetCommands() const
  {
		static ChatCommand arenaCommandTable[] =
 
		{
			//{ "1v1",	SEC_PLAYER,		false,	&Handle1v1Command,		"", NULL },
			{ "2v2",	SEC_PLAYER,		false,	&Handle2v2Command,		"", NULL },
			{ "3v3",	SEC_PLAYER,		false,	&Handle3v3Command,		"", NULL },
			//{ "5v5",	SEC_PLAYER,		false,	&Handle5v5Command,		"", NULL },
			{  NULL,			 0,		false,				 NULL,					   "", NULL }
		};
 
		static ChatCommand commandTable[] =
		{
			{ "arena",	SEC_PLAYER,		true,				 NULL,					   "", arenaCommandTable},
			{    NULL,			 0,	   false,				 NULL,					   "",			   NULL }
		};
		return commandTable;
 
	}
	
	/*static bool Handle1v1Command(ChatHandler* handler, const char* args)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (_player->isInCombat())
		{
			ChatHandler(_player).PSendSysMessage(LANG_YOU_IN_COMBAT);
			return true;
		}

		if(_player->GetMap()->IsBattlegroundOrArena() || _player->HasStealthAura() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || _player->isDead())
		{
			ChatHandler(_player).PSendSysMessage("No puedes hacer eso estando en BG, Arena o estando en sigilo.");
			return true;
		}

		if (_player->GetZoneId() == 14)
		{
			ChatHandler(_player).PSendSysMessage("No puedes hacer eso estando en la Zona PvP.");
			return true;
		}

		bool isRated = true;
		GroupQueueInfo* ginfo            = new GroupQueueInfo;
		ginfo->IsRated                   = isRated;

		uint8 arenatype = ARENA_TEAM_1v1;
		uint32 arenaRating = 0;
		uint32 matchmakerRating = 0;

		Battleground* pBG = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
		if(!pBG)
		{
			sLog->outError("Battleground: template bg (all arenas) not found");
			return true;
		}

		if(sDisableMgr->IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
		{
			ChatHandler(_player).PSendSysMessage(LANG_ARENA_DISABLED);
			return true;
		}

		BattlegroundTypeId bgTypeId = pBG->GetTypeID();
		BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, ARENA_TYPE_1v1);
		PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(pBG->GetMapId(), _player->getLevel());
		if(!bracketEntry)
			return false;

		GroupJoinBattlegroundResult err = ERR_GROUP_JOIN_BATTLEGROUND_FAIL;

		uint8 arenaslot = ARENA_TEAM_1v1; // 1c1
			
		if (Habilitada3() == 0) //Arenas Habilitadas, se modifica en SmartAI.cpp, la hora se pone por game_event (evento 83)
		{
			uint32 ateamId = 0;

			if(isRated)
			{
				ateamId = _player->GetArenaTeamId(arenaslot);

				ArenaTeam* pAT = sArenaTeamMgr->GetArenaTeamById(ateamId);
				if(!pAT)
				{
					_player->GetSession()->SendNotInArenaTeamPacket(ARENA_TYPE_1v1);
					return true;
				}

				arenaRating         = pAT->GetRating();
				matchmakerRating    = arenaRating;

				if(arenaRating <= 0)
				arenaRating = 1;
			}

			BattlegroundQueue &bgQueue = sBattlegroundMgr->m_BattlegroundQueues[bgQueueTypeId];
				uint32 avgTime = 0;

				if(err > 0)
				{
					sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: arena join as group start");
					if(isRated)
					{
						sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: arena team id %u, leader %s queued with matchmaker rating %u for type %u", _player->GetArenaTeamId(arenaslot), _player->GetName(), matchmakerRating, ARENA_TYPE_2v2);
						pBG->SetRated(true);
					}
					else
						pBG->SetRated(false);

					GroupQueueInfo * ginfo = bgQueue.AddGroup(_player, NULL, bgTypeId, bracketEntry, ARENA_TYPE_1v1, true, false, arenaRating, matchmakerRating, ateamId);
					avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
				}

				WorldPacket data;

				if(err <= 0)
				{
					sBattlegroundMgr->BuildGroupJoinedBattlegroundPacket(&data, err);
					_player->GetSession()->SendPacket(&data);
				}

					uint32 queueSlot = _player->AddBattlegroundQueueId(bgQueueTypeId);

					sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, pBG, queueSlot, STATUS_WAIT_QUEUE, avgTime, 0, ARENA_TYPE_1v1);
					_player->GetSession()->SendPacket(&data);
					sBattlegroundMgr->BuildGroupJoinedBattlegroundPacket(&data, err);
					_player->GetSession()->SendPacket(&data);
					sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for arena as group bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, _player->GetGUIDLow(), _player->GetName());
				}
			
			sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, ARENA_TYPE_1v1, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
		
		return true;
	}*/
 
	static bool Handle2v2Command(ChatHandler* handler, const char* args)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (_player->isInCombat())
		{
			ChatHandler(_player).PSendSysMessage(LANG_YOU_IN_COMBAT);
			return true;
		}

		if(_player->GetMap()->IsBattlegroundOrArena() || _player->HasStealthAura() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || _player->isDead())
		{
			ChatHandler(_player).PSendSysMessage("No puedes hacer eso estando en BG, Arena o estando en sigilo.");
			return true;
		}

		if (_player->GetZoneId() == 14)
		{
			ChatHandler(_player).PSendSysMessage("No puedes hacer eso estando en la Zona PvP.");
			return true;
		}

		bool isRated = true;
		GroupQueueInfo* ginfo            = new GroupQueueInfo;
		ginfo->IsRated                   = isRated;

		uint8 arenatype = 0;
		uint32 arenaRating = 0;
		uint32 matchmakerRating = 0;

		Battleground* pBG = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
		if(!pBG)
		{
			sLog->outError("Battleground: template bg (all arenas) not found");
			return true;
		}

		if(sDisableMgr->IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
		{
			ChatHandler(_player).PSendSysMessage(LANG_ARENA_DISABLED);
			return true;
		}

		BattlegroundTypeId bgTypeId = pBG->GetTypeID();
		BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, ARENA_TYPE_2v2);
		PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(pBG->GetMapId(), _player->getLevel());
		if(!bracketEntry)
			return false;

		GroupJoinBattlegroundResult err = ERR_GROUP_JOIN_BATTLEGROUND_FAIL;

		Group * grp = NULL;
		uint8 arenaslot = 0; // 2c2
			
		if (Habilitada3() == 0) //Arenas Habilitadas, se modifica en SmartAI.cpp, la hora se pone por game_event (evento 83)
		{
			if (grp = _player->GetGroup())
			{
				grp = _player->GetGroup();
				if(!grp)
				{
					handler->PSendSysMessage("Debes estar en grupo.");
					return true;
				}
				
				if(grp->GetLeaderGUID() != _player->GetGUID())
				{
					handler->PSendSysMessage("No eres el lider del grupo.");
					return true;
				}

				err = grp->CanJoinBattlegroundQueue(pBG, bgQueueTypeId, ARENA_TYPE_2v2, ARENA_TYPE_2v2, true, arenaslot);
			}
			else
			{
				handler->PSendSysMessage("Debes estar en grupo.");
					return true;
			}

			uint32 ateamId = 0;

			if(isRated)
			{
				ateamId = _player->GetArenaTeamId(arenaslot);

				ArenaTeam* pAT = sArenaTeamMgr->GetArenaTeamById(ateamId);
				if(!pAT)
				{
					_player->GetSession()->SendNotInArenaTeamPacket(ARENA_TYPE_2v2);
					return true;
				}

				arenaRating         = pAT->GetRating();
				matchmakerRating    = pAT->GetAverageMMR(grp);

				if(arenaRating <= 0)
				arenaRating = 1;
			}

			BattlegroundQueue &bgQueue = sBattlegroundMgr->m_BattlegroundQueues[bgQueueTypeId];
			if(grp = _player->GetGroup())
			{
				uint32 avgTime = 0;

				if(err > 0)
				{
					sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: arena join as group start");
					if(isRated)
					{
						sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: arena team id %u, leader %s queued with matchmaker rating %u for type %u", _player->GetArenaTeamId(arenaslot), _player->GetName(), matchmakerRating, ARENA_TYPE_2v2);
						pBG->SetRated(true);
					}
					else
						pBG->SetRated(false);

					GroupQueueInfo * ginfo = bgQueue.AddGroup(_player, grp, bgTypeId, bracketEntry, ARENA_TYPE_2v2, true, false, arenaRating, matchmakerRating, ateamId);
					avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
				}

				for(GroupReference *itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
				{
					Player* member = itr->getSource();
					if(!member)
						continue;

					WorldPacket data;

					if(err <= 0)
					{
						sBattlegroundMgr->BuildGroupJoinedBattlegroundPacket(&data, err);
						member->GetSession()->SendPacket(&data);
						continue;
					}

					uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

					sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, pBG, queueSlot, STATUS_WAIT_QUEUE, avgTime, 0, ARENA_TYPE_2v2);
					member->GetSession()->SendPacket(&data);
					sBattlegroundMgr->BuildGroupJoinedBattlegroundPacket(&data, err);
					member->GetSession()->SendPacket(&data);
					sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for arena as group bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, member->GetGUIDLow(), member->GetName());
				}
			}
			else
			{
				handler->PSendSysMessage("Debes estar en grupo.");
					return true;
			}
			sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, ARENA_TYPE_2v2, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
		}
		else if (Habilitada3() == 1) //Arenas deshabilitadas, se modifica en SmartAI.cpp, la hora se pone por game_event (evento 83)
		{
			_player->GetSession()->SendNotification("Las arenas est""\xC3\xA1""n deshabilitadas en este momento, a las 10:00 Hora del Servidor, volver""\xC3\xA1""n a estar activas.");
			return true;
		}
		
		return true;
	}
 
	static bool Handle3v3Command(ChatHandler* handler, const char* args)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (_player->isInCombat())
		{
			ChatHandler(_player).PSendSysMessage(LANG_YOU_IN_COMBAT);
			return true;
		}

		if(_player->GetMap()->IsBattlegroundOrArena() || _player->HasStealthAura() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || _player->isDead())
		{
			ChatHandler(_player).PSendSysMessage("No puedes hacer eso estando en BG, Arena o estando en sigilo.");
			return true;
		}

		if (_player->GetZoneId() == 14)
		{
			ChatHandler(_player).PSendSysMessage("No puedes hacer eso estando en la Zona PvP.");
			return true;
		}

		bool isRated = true;
		GroupQueueInfo* ginfo            = new GroupQueueInfo;
		ginfo->IsRated                   = isRated;

		uint8 arenatype = 1;
		uint32 arenaRating = 0;
		uint32 matchmakerRating = 0;

		Battleground* pBG = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
		if(!pBG)
		{
			sLog->outError("Battleground: template bg (all arenas) not found");
			return true;
		}

		if(sDisableMgr->IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
		{
			ChatHandler(_player).PSendSysMessage(LANG_ARENA_DISABLED);
			return true;
		}

		BattlegroundTypeId bgTypeId = pBG->GetTypeID();
		BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, ARENA_TYPE_3v3);
		PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(pBG->GetMapId(), _player->getLevel());
		if(!bracketEntry)
			return false;

		GroupJoinBattlegroundResult err = ERR_GROUP_JOIN_BATTLEGROUND_FAIL;

		Group * grp = NULL;
		uint8 arenaslot = 1; // 3c3
		
		if (Habilitada3() == 0) //Arenas Habilitadas, se modifica en SmartAI.cpp, la hora se pone por game_event (evento 83)
		{
			if (grp = _player->GetGroup())
			{
				grp = _player->GetGroup();
				if(!grp)
				{
					handler->PSendSysMessage("Debes estar en grupo.");
					return true;
				}
				
				if(grp->GetLeaderGUID() != _player->GetGUID())
				{
					handler->PSendSysMessage("No eres el lider del grupo.");
					return true;
				}

				err = grp->CanJoinBattlegroundQueue(pBG, bgQueueTypeId, ARENA_TYPE_3v3, ARENA_TYPE_3v3, true, arenaslot);
			}
			else
			{
				handler->PSendSysMessage("Debes estar en grupo.");
					return true;
			}

			uint32 ateamId = 0;

			if(isRated)
			{
				ateamId = _player->GetArenaTeamId(arenaslot);

				ArenaTeam* pAT = sArenaTeamMgr->GetArenaTeamById(ateamId);
				if(!pAT)
				{
					_player->GetSession()->SendNotInArenaTeamPacket(ARENA_TYPE_3v3);
					return true;
				}

				arenaRating         = pAT->GetRating();
				matchmakerRating    = pAT->GetAverageMMR(grp);

				if(arenaRating <= 0)
				arenaRating = 1;
			}

			BattlegroundQueue &bgQueue = sBattlegroundMgr->m_BattlegroundQueues[bgQueueTypeId];
			if(grp = _player->GetGroup())
			{
				uint32 avgTime = 0;

				if(err > 0)
				{
					sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: arena join as group start");
					if(isRated)
					{
						sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: arena team id %u, leader %s queued with matchmaker rating %u for type %u", _player->GetArenaTeamId(arenaslot), _player->GetName(), matchmakerRating, ARENA_TYPE_3v3);
						pBG->SetRated(true);
					}
					else
						pBG->SetRated(false);

					GroupQueueInfo * ginfo = bgQueue.AddGroup(_player, grp, bgTypeId, bracketEntry, ARENA_TYPE_3v3, true, false, arenaRating, matchmakerRating, ateamId);
					avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
				}

				for(GroupReference *itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
				{
					Player* member = itr->getSource();
					if(!member)
						continue;

					WorldPacket data;

					if(err <= 0)
					{
						sBattlegroundMgr->BuildGroupJoinedBattlegroundPacket(&data, err);
						member->GetSession()->SendPacket(&data);
						continue;
					}

					uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

					sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, pBG, queueSlot, STATUS_WAIT_QUEUE, avgTime, 0, ARENA_TYPE_3v3);
					member->GetSession()->SendPacket(&data);
					sBattlegroundMgr->BuildGroupJoinedBattlegroundPacket(&data, err);
					member->GetSession()->SendPacket(&data);
					sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for arena as group bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, member->GetGUIDLow(), member->GetName());
				}
			}
			else
			{
				handler->PSendSysMessage("Debes estar en grupo.");
					return true;
			}
			sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, ARENA_TYPE_3v3, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
		}
		else if (Habilitada3() == 1) //Arenas deshabilitadas, se modifica en SmartAI.cpp, la hora se pone por game_event (evento 83)
		{
			_player->GetSession()->SendNotification("Las arenas est""\xC3\xA1""n deshabilitadas en este momento, a las 10:00 Hora del Servidor, volver""\xC3\xA1""n a estar activas.");
			return true;
		}
		
		return true;
	}
 
	static bool Handle5v5Command(ChatHandler* handler, const char* args)
	{
		Player* _player = handler->GetSession()->GetPlayer();

		if (_player->isInCombat())
		{
			ChatHandler(_player).PSendSysMessage(LANG_YOU_IN_COMBAT);
			return true;
		}

		if(_player->GetMap()->IsBattlegroundOrArena() || _player->HasStealthAura() || _player->HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH) || _player->isDead())
		{
			ChatHandler(_player).PSendSysMessage("No puedes hacer eso estando en BG, Arena o estando en sigilo.");
			return true;
		}

		if (_player->GetZoneId() == 14)
		{
			ChatHandler(_player).PSendSysMessage("No puedes hacer eso estando en la Zona PvP.");
			return true;
		}

		bool isRated = true;
		GroupQueueInfo* ginfo            = new GroupQueueInfo;
		ginfo->IsRated                   = isRated;

		uint8 arenatype = 2;
		uint32 arenaRating = 0;
		uint32 matchmakerRating = 0;

		Battleground* pBG = sBattlegroundMgr->GetBattlegroundTemplate(BATTLEGROUND_AA);
		if(!pBG)
		{
			sLog->outError("Battleground: template bg (all arenas) not found");
			return true;
		}

		if(sDisableMgr->IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, BATTLEGROUND_AA, NULL))
		{
			ChatHandler(_player).PSendSysMessage(LANG_ARENA_DISABLED);
			return true;
		}

		BattlegroundTypeId bgTypeId = pBG->GetTypeID();
		BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(bgTypeId, ARENA_TYPE_5v5);
		PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(pBG->GetMapId(), _player->getLevel());
		if(!bracketEntry)
			return false;

		GroupJoinBattlegroundResult err = ERR_GROUP_JOIN_BATTLEGROUND_FAIL;

		Group * grp = NULL;
		uint8 arenaslot = 2; // 5c5
			
		if (Habilitada3() == 0) //Arenas Habilitadas, se modifica en SmartAI.cpp, la hora se pone por game_event (evento 83)
		{
			if (grp = _player->GetGroup())
			{
				grp = _player->GetGroup();
				if(!grp)
				{
					handler->PSendSysMessage("Debes estar en grupo.");
					return true;
				}
				
				if(grp->GetLeaderGUID() != _player->GetGUID())
				{
					handler->PSendSysMessage("No eres el lider del grupo.");
					return true;
				}

				err = grp->CanJoinBattlegroundQueue(pBG, bgQueueTypeId, ARENA_TYPE_5v5, ARENA_TYPE_5v5, true, arenaslot);
			}
			else
			{
				handler->PSendSysMessage("Debes estar en grupo.");
					return true;
			}

			uint32 ateamId = 0;

			if(isRated)
			{
				ateamId = _player->GetArenaTeamId(arenaslot);

				ArenaTeam* pAT = sArenaTeamMgr->GetArenaTeamById(ateamId);
				if(!pAT)
				{
					_player->GetSession()->SendNotInArenaTeamPacket(ARENA_TYPE_5v5);
					return true;
				}

				arenaRating         = pAT->GetRating();
				matchmakerRating    = pAT->GetAverageMMR(grp);

				if(arenaRating <= 0)
				arenaRating = 1;
			}

			BattlegroundQueue &bgQueue = sBattlegroundMgr->m_BattlegroundQueues[bgQueueTypeId];
			if(grp = _player->GetGroup())
			{
				uint32 avgTime = 0;

				if(err > 0)
				{
					sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: arena join as group start");
					if(isRated)
					{
						sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: arena team id %u, leader %s queued with matchmaker rating %u for type %u", _player->GetArenaTeamId(arenaslot), _player->GetName(), matchmakerRating, ARENA_TYPE_5v5);
						pBG->SetRated(true);
					}
					else
						pBG->SetRated(false);

					GroupQueueInfo * ginfo = bgQueue.AddGroup(_player, grp, bgTypeId, bracketEntry, ARENA_TYPE_5v5, true, false, arenaRating, matchmakerRating, ateamId);
					avgTime = bgQueue.GetAverageQueueWaitTime(ginfo, bracketEntry->GetBracketId());
				}

				for(GroupReference *itr = grp->GetFirstMember(); itr != NULL; itr = itr->next())
				{
					Player* member = itr->getSource();
					if(!member)
						continue;

					WorldPacket data;

					if(err <= 0)
					{
						sBattlegroundMgr->BuildGroupJoinedBattlegroundPacket(&data, err);
						member->GetSession()->SendPacket(&data);
						continue;
					}

					uint32 queueSlot = member->AddBattlegroundQueueId(bgQueueTypeId);

					sBattlegroundMgr->BuildBattlegroundStatusPacket(&data, pBG, queueSlot, STATUS_WAIT_QUEUE, avgTime, 0, ARENA_TYPE_5v5);
					member->GetSession()->SendPacket(&data);
					sBattlegroundMgr->BuildGroupJoinedBattlegroundPacket(&data, err);
					member->GetSession()->SendPacket(&data);
					sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: player joined queue for arena as group bg queue type %u bg type %u: GUID %u, NAME %s", bgQueueTypeId, bgTypeId, member->GetGUIDLow(), member->GetName());
				}
			}
			else
			{
				handler->PSendSysMessage("Debes estar en grupo.");
					return true;
			}
			sBattlegroundMgr->ScheduleQueueUpdate(matchmakerRating, ARENA_TYPE_5v5, bgQueueTypeId, bgTypeId, bracketEntry->GetBracketId());
		}
		else if (Habilitada3() == 1) //Arenas deshabilitadas, se modifica en SmartAI.cpp, la hora se pone por game_event (evento 83)
		{
			_player->GetSession()->SendNotification("Las arenas est""\xC3\xA1""n deshabilitadas en este momento, a las 10:00 Hora del Servidor, volver""\xC3\xA1""n a estar activas.");
			return true;
		}
		
		return true;
	}
 };
 
void AddSC_arenaqueue()
{
    new arenaqueue();
}