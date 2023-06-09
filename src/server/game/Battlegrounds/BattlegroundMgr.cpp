/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Common.h"
#include "ObjectMgr.h"
#include "ArenaTeamMgr.h"
#include "World.h"
#include "WorldPacket.h"

#include "ArenaTeam.h"
#include "BattlegroundMgr.h"
#include "BattlegroundAV.h"
#include "BattlegroundAB.h"
#include "BattlegroundEY.h"
#include "BattlegroundWS.h"
#include "BattlegroundNA.h"
#include "BattlegroundBE.h"
#include "BattlegroundAA.h"
#include "BattlegroundRL.h"
#include "BattlegroundSA.h"
#include "BattlegroundDS.h"
#include "BattlegroundRV.h"
#include "BattlegroundIC.h"
#include "BattlegroundRB.h"
#include "Chat.h"
#include "Map.h"
#include "MapInstanced.h"
#include "MapManager.h"
#include "Player.h"
#include "GameEventMgr.h"
#include "SharedDefines.h"
#include "Formulas.h"
#include "DisableMgr.h"

/*********************************************************/
/***            BATTLEGROUND MANAGER                   ***/
/*********************************************************/

BattlegroundMgr::BattlegroundMgr() : m_AutoDistributionTimeChecker(0), m_ArenaTesting(false)
{
    for(uint32 i = BATTLEGROUND_TYPE_NONE; i < MAX_BATTLEGROUND_TYPE_ID; i++)
        m_Battlegrounds[i].clear();
    //m_NextRatingDiscardUpdate = sWorld->getIntConfig(CONFIG_ARENA_RATING_DISCARD_TIMER);
    m_NextRatedArenaUpdate = sWorld->getIntConfig(CONFIG_ARENA_RATED_UPDATE_TIMER);
	m_Testing=false;
}

BattlegroundMgr::~BattlegroundMgr()
{
    DeleteAllBattlegrounds();
}

void BattlegroundMgr::DeleteAllBattlegrounds()
{
    for(uint32 i = BATTLEGROUND_TYPE_NONE; i < MAX_BATTLEGROUND_TYPE_ID; ++i)
    {
        for(BattlegroundSet::iterator itr = m_Battlegrounds[i].begin(); itr != m_Battlegrounds[i].end();)
        {
            if(Battleground* pBG = itr->second)
            {
                m_Battlegrounds[i].erase(itr++);
                if(!m_ClientBattlegroundIds[i][pBG->GetBracketId()].empty())
                    m_ClientBattlegroundIds[i][pBG->GetBracketId()].erase(pBG->GetClientInstanceID());
                delete pBG;
            }
        }
    }

    // destroy template battlegrounds that listed only in queues (other already terminated)
    for(uint32 bgTypeId = 0; bgTypeId < MAX_BATTLEGROUND_TYPE_ID; ++bgTypeId)
    {
        // ~Battleground call unregistring BG from queue
        while(!BGFreeSlotQueue[bgTypeId].empty())
            delete BGFreeSlotQueue[bgTypeId].front();
    }
}

// used to update running battlegrounds, and delete finished ones
void BattlegroundMgr::Update(uint32 diff)
{
    BattlegroundSet::iterator itr, next;
    for(uint32 i = BATTLEGROUND_TYPE_NONE; i < MAX_BATTLEGROUND_TYPE_ID; ++i)
    {
        itr = m_Battlegrounds[i].begin();
        // skip updating battleground template
        if(itr != m_Battlegrounds[i].end())
            ++itr;
        for(; itr != m_Battlegrounds[i].end(); itr = next)
        {
            next = itr;
            ++next;
            itr->second->Update(diff);
            // use the SetDeleteThis variable
            // direct deletion caused crashes
            if(itr->second->m_SetDeleteThis)
            {
                if(Battleground* pBG = itr->second)
                {
                    m_Battlegrounds[i].erase(itr);
                    if(!m_ClientBattlegroundIds[i][pBG->GetBracketId()].empty())
                        m_ClientBattlegroundIds[i][pBG->GetBracketId()].erase(pBG->GetClientInstanceID());
                    delete pBG;
                }
            }
        }
    }

    // update scheduled queues
    if(!m_QueueUpdateScheduler.empty())
    {
        std::vector<uint64> scheduled;
        {
            //copy vector and clear the other
            scheduled = std::vector<uint64>(m_QueueUpdateScheduler);
            m_QueueUpdateScheduler.clear();
            //release lock
        }

        for(uint8 i = 0; i < scheduled.size(); i++)
        {
            uint32 arenaMMRating = scheduled[i] >> 32;
            uint8 arenaType = scheduled[i] >> 24 & 255;
            BattlegroundQueueTypeId bgQueueTypeId = BattlegroundQueueTypeId(scheduled[i] >> 16 & 255);
            BattlegroundTypeId bgTypeId = BattlegroundTypeId((scheduled[i] >> 8) & 255);
            BattlegroundBracketId bracket_id = BattlegroundBracketId(scheduled[i] & 255);
            m_BattlegroundQueues[bgQueueTypeId].Update(bgTypeId, bracket_id, arenaType, arenaMMRating > 0, arenaMMRating);
        }
    }

    // if rating difference counts, maybe force-update queues
    //if(sWorld->getIntConfig(CONFIG_ARENA_MAX_RATING_DIFFERENCE) && sWorld->getIntConfig(CONFIG_ARENA_RATING_DISCARD_TIMER))
    if(sWorld->getIntConfig(CONFIG_ARENA_MAX_RATING_DIFFERENCE) && sWorld->getIntConfig(CONFIG_ARENA_RATED_UPDATE_TIMER))
	{
        // it's time to force update
        //if(m_NextRatingDiscardUpdate < diff)
        if(m_NextRatedArenaUpdate < diff)
		{
            // forced update for rated arenas (scan all, but skipped non rated)
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundMgr: UPDATING ARENA QUEUES");
            for(int qtype = BATTLEGROUND_QUEUE_2v2; qtype <= BATTLEGROUND_QUEUE_5v5; ++qtype)
                for(int bracket = BG_BRACKET_ID_FIRST; bracket < MAX_BATTLEGROUND_BRACKETS; ++bracket)
                    m_BattlegroundQueues[qtype].Update(
                        BATTLEGROUND_AA, BattlegroundBracketId(bracket),
                        BattlegroundMgr::BGArenaType(BattlegroundQueueTypeId(qtype)), true, 0);

            //m_NextRatingDiscardUpdate = sWorld->getIntConfig(CONFIG_ARENA_RATING_DISCARD_TIMER);
			m_NextRatedArenaUpdate = sWorld->getIntConfig(CONFIG_ARENA_RATED_UPDATE_TIMER);
        }
        else
            //m_NextRatingDiscardUpdate -= diff;
			m_NextRatedArenaUpdate -= diff;
    }
    if(sWorld->getBoolConfig(CONFIG_ARENA_AUTO_DISTRIBUTE_POINTS))
    {
        if(m_AutoDistributionTimeChecker < diff)
        {
            if(time(NULL) > m_NextAutoDistributionTime)
            {
                sArenaTeamMgr->DistributeArenaPoints();
                m_NextAutoDistributionTime = m_NextAutoDistributionTime + BATTLEGROUND_ARENA_POINT_DISTRIBUTION_DAY * sWorld->getIntConfig(CONFIG_ARENA_AUTO_DISTRIBUTE_INTERVAL_DAYS);
                sWorld->setWorldState(WS_ARENA_DISTRIBUTION_TIME, uint64(m_NextAutoDistributionTime));
            }
            m_AutoDistributionTimeChecker = 600000; // check 10 minutes
        }
        else
            m_AutoDistributionTimeChecker -= diff;
    }
}

void BattlegroundMgr::BuildBattlegroundStatusPacket(WorldPacket *data, Battleground* pBG, uint8 QueueSlot, uint8 StatusID, uint32 Time1, uint32 Time2, uint8 arenatype, uint8 uiFrame)
{
    // we can be in 2 queues in same time...

    if(StatusID == 0 || !pBG)
    {
        data->Initialize(SMSG_BATTLEFIELD_STATUS, 4+8);
        *data << uint32(QueueSlot);                         // queue id (0...1)
        *data << uint64(0);
        return;
    }

    data->Initialize(SMSG_BATTLEFIELD_STATUS, (4+8+1+1+4+1+4+4+4));
    *data << uint32(QueueSlot);                             // queue id (0...1) - player can be in 2 queues in time
    // The following segment is read as uint64 in client but can be appended as their original type.
    *data << uint8(arenatype);
    sLog->outDebug(LOG_FILTER_NETWORKIO, "BattlegroundMgr::BuildBattlegroundStatusPacket: arenatype = %u for bg instanceID %u, TypeID %u.", arenatype, pBG->GetClientInstanceID(), pBG->GetTypeID());
    *data << uint8(pBG->isArena() ? 0xC : 0x2);
    *data << uint32(pBG->GetTypeID());
    *data << uint16(0x1F90);
    // End of uint64 segment, decomposed this way for simplicity
    *data << uint8(0);                                      // 3.3.0, some level, only saw 80...
    *data << uint8(0);                                      // 3.3.0, some level, only saw 80...
    *data << uint32(pBG->GetClientInstanceID());
    // alliance/horde for BG and skirmish/rated for Arenas
    // following displays the minimap-icon 0 = faction icon 1 = arenaicon
    *data << uint8(pBG->isRated());                          // 1 for rated match, 0 for bg or non rated match

    *data << uint32(StatusID);                              // status
    switch(StatusID)
    {
        case STATUS_WAIT_QUEUE:                             // status_in_queue
            *data << uint32(Time1);                         // average wait time, milliseconds
            *data << uint32(Time2);                         // time in queue, updated every minute!, milliseconds
            break;
        case STATUS_WAIT_JOIN:                              // status_invite
            *data << uint32(pBG->GetMapId());               // map id
            *data << uint64(0);                             // 3.3.5, unknown
            *data << uint32(Time1);                         // time to remove from queue, milliseconds
            break;
        case STATUS_IN_PROGRESS:                            // status_in_progress
            *data << uint32(pBG->GetMapId());                // map id
            *data << uint64(0);                             // 3.3.5, unknown
            *data << uint32(Time1);                         // time to bg auto leave, 0 at bg start, 120000 after bg end, milliseconds
            *data << uint32(Time2);                         // time from bg start, milliseconds
            *data << uint8(uiFrame);
            break;
        default:
            sLog->outError("Unknown BG status!");
            break;
    }
}

void BattlegroundMgr::BuildPvpLogDataPacket(WorldPacket *data, Battleground* pBG)
{
    uint8 type = (pBG->isArena() ? 1 : 0);
                                                            // last check on 3.0.3
    data->Initialize(MSG_PVP_LOG_DATA, (1+1+4+40*pBG->GetPlayerScoresSize()));
    *data << uint8(type);                                   // type (battleground=0/arena=1)

    if(type)                                                // arena
    {
        // it seems this must be according to BG_WINNER_A/H and _NOT_ BG_TEAM_A/H
        for(int8 i = 1; i >= 0; --i)
        {
            uint32 pointsLost = pBG->m_ArenaTeamRatingChanges[i] < 0 ? abs(pBG->m_ArenaTeamRatingChanges[i]) : 0;
            uint32 pointsGained = pBG->m_ArenaTeamRatingChanges[i] > 0 ? pBG->m_ArenaTeamRatingChanges[i] : 0;
            uint32 MatchmakerRating = pBG->m_ArenaTeamMMR[i];

            *data << uint32(pointsLost);                    // Rating Lost
            *data << uint32(pointsGained);                  // Rating gained
            *data << uint32(MatchmakerRating);              // Matchmaking Value
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "rating change: %d", pBG->m_ArenaTeamRatingChanges[i]);
        }
        for(int8 i = 1; i >= 0; --i)
        {
            uint32 at_id = pBG->m_ArenaTeamIds[i];
            ArenaTeam* at = sArenaTeamMgr->GetArenaTeamById(at_id);
            if(at)
                *data << at->GetName();
            else
                *data << uint8(0);
        }
    }

    if(pBG->GetStatus() != STATUS_WAIT_LEAVE)
        *data << uint8(0);                                  // bg not ended
    else
    {
        *data << uint8(1);                                  // bg ended
        *data << uint8(pBG->GetWinner());                    // who win
    }

    size_t wpos = data->wpos();
    uint32 scoreCount = 0;
    *data << uint32(scoreCount);                            // placeholder

    Battleground::BattlegroundScoreMap::const_iterator itr2 = pBG->GetPlayerScoresBegin();
    for(Battleground::BattlegroundScoreMap::const_iterator itr = itr2; itr != pBG->GetPlayerScoresEnd();)
    {
        itr2 = itr++;
        if(!pBG->IsPlayerInBattleground(itr2->first))
        {
            sLog->outError("Player " UI64FMTD " has scoreboard entry for battleground %u but is not in battleground!", itr->first, pBG->GetTypeID(true));
            continue;
        }

        *data << uint64(itr2->first);
        *data << uint32(itr2->second->KillingBlows);
        if(type == 0)
        {
            *data << uint32(itr2->second->HonorableKills);
            *data << uint32(itr2->second->Deaths);
            *data << uint32(itr2->second->BonusHonor);
        }
        else
        {
            Player* plr = sObjectMgr->GetPlayer(itr2->first);
            uint32 team = pBG->GetPlayerTeam(itr2->first);
            if(!team && plr)
                team = plr->GetBGTeam();
            *data << uint8(team == ALLIANCE ? 1 : 0); // green or yellow

        }
        *data << uint32(itr2->second->DamageDone);              // damage done
        *data << uint32(itr2->second->HealingDone);             // healing done
        switch(pBG->GetTypeID(true))                             // battleground specific things
        {
            case BATTLEGROUND_RB:
                switch(pBG->GetMapId())
                {
                    case 489:
                        *data << uint32(0x00000002);            // count of next fields
                        *data << uint32(((BattlegroundWGScore*)itr2->second)->FlagCaptures);        // flag captures
                        *data << uint32(((BattlegroundWGScore*)itr2->second)->FlagReturns);         // flag returns
                        break;
                    case 566:
                        *data << uint32(0x00000001);            // count of next fields
                        *data << uint32(((BattlegroundEYScore*)itr2->second)->FlagCaptures);        // flag captures
                        break;
                    case 529:
                        *data << uint32(0x00000002);            // count of next fields
                        *data << uint32(((BattlegroundABScore*)itr2->second)->BasesAssaulted);      // bases asssulted
                        *data << uint32(((BattlegroundABScore*)itr2->second)->BasesDefended);       // bases defended
                        break;
                    case 30:
                        *data << uint32(0x00000005);            // count of next fields
                        *data << uint32(((BattlegroundAVScore*)itr2->second)->GraveyardsAssaulted); // GraveyardsAssaulted
                        *data << uint32(((BattlegroundAVScore*)itr2->second)->GraveyardsDefended);  // GraveyardsDefended
                        *data << uint32(((BattlegroundAVScore*)itr2->second)->TowersAssaulted);     // TowersAssaulted
                        *data << uint32(((BattlegroundAVScore*)itr2->second)->TowersDefended);      // TowersDefended
                        *data << uint32(((BattlegroundAVScore*)itr2->second)->MinesCaptured);       // MinesCaptured
                        break;
                    case 607:
                        *data << uint32(0x00000002);            // count of next fields
                        *data << uint32(((BattlegroundSAScore*)itr2->second)->demolishers_destroyed);
                        *data << uint32(((BattlegroundSAScore*)itr2->second)->gates_destroyed);
                        break;
                    case 628:                                   // IC
                        *data << uint32(0x00000002);            // count of next fields
                        *data << uint32(((BattlegroundICScore*)itr2->second)->BasesAssaulted);       // bases asssulted
                        *data << uint32(((BattlegroundICScore*)itr2->second)->BasesDefended);        // bases defended
                    default:
                        *data << uint32(0);
                        break;
                }
            case BATTLEGROUND_AV:
                *data << uint32(0x00000005);                    // count of next fields
                *data << uint32(((BattlegroundAVScore*)itr2->second)->GraveyardsAssaulted); // GraveyardsAssaulted
                *data << uint32(((BattlegroundAVScore*)itr2->second)->GraveyardsDefended);  // GraveyardsDefended
                *data << uint32(((BattlegroundAVScore*)itr2->second)->TowersAssaulted);     // TowersAssaulted
                *data << uint32(((BattlegroundAVScore*)itr2->second)->TowersDefended);      // TowersDefended
                *data << uint32(((BattlegroundAVScore*)itr2->second)->MinesCaptured);       // MinesCaptured
                break;
            case BATTLEGROUND_WS:
                *data << uint32(0x00000002);                    // count of next fields
                *data << uint32(((BattlegroundWGScore*)itr2->second)->FlagCaptures);        // flag captures
                *data << uint32(((BattlegroundWGScore*)itr2->second)->FlagReturns);         // flag returns
                break;
            case BATTLEGROUND_AB:
                *data << uint32(0x00000002);                    // count of next fields
                *data << uint32(((BattlegroundABScore*)itr2->second)->BasesAssaulted);      // bases asssulted
                *data << uint32(((BattlegroundABScore*)itr2->second)->BasesDefended);       // bases defended
                break;
            case BATTLEGROUND_EY:
                *data << uint32(0x00000001);                    // count of next fields
                *data << uint32(((BattlegroundEYScore*)itr2->second)->FlagCaptures);        // flag captures
                break;
            case BATTLEGROUND_SA:
                *data << uint32(0x00000002);                    // count of next fields
                *data << uint32(((BattlegroundSAScore*)itr2->second)->demolishers_destroyed);
                *data << uint32(((BattlegroundSAScore*)itr2->second)->gates_destroyed);
                break;
            case BATTLEGROUND_IC:                           // wotlk
                *data << uint32(0x00000002);                // count of next fields
                *data << uint32(((BattlegroundICScore*)itr2->second)->BasesAssaulted);       // bases asssulted
                *data << uint32(((BattlegroundICScore*)itr2->second)->BasesDefended);        // bases defended
                break;
            case BATTLEGROUND_NA:
            case BATTLEGROUND_BE:
            case BATTLEGROUND_AA:
            case BATTLEGROUND_RL:
            case BATTLEGROUND_DS:                               // wotlk
            case BATTLEGROUND_RV:                               // wotlk
                *data << uint32(0);
                break;
            default:
                sLog->outDebug(LOG_FILTER_NETWORKIO, "Unhandled MSG_PVP_LOG_DATA for BG id %u", pBG->GetTypeID());
                *data << uint32(0);
                break;
        }
        // should never happen
        if(++scoreCount >= pBG->GetMaxPlayers() && itr != pBG->GetPlayerScoresEnd())
        {
            sLog->outError("Battleground %u scoreboard has more entries (%u) than allowed players in this bg (%u)", pBG->GetTypeID(true), pBG->GetPlayerScoresSize(), pBG->GetMaxPlayers());
            break;
        }
    }

    data->put(wpos, scoreCount);
}

void BattlegroundMgr::BuildGroupJoinedBattlegroundPacket(WorldPacket *data, GroupJoinBattlegroundResult result)
{
    data->Initialize(SMSG_GROUP_JOINED_BATTLEGROUND, 4);
    *data << int32(result);
    if(result == ERR_BATTLEGROUND_JOIN_TIMED_OUT || result == ERR_BATTLEGROUND_JOIN_FAILED)
        *data << uint64(0);                                 // player guid
}

void BattlegroundMgr::BuildUpdateWorldStatePacket(WorldPacket *data, uint32 field, uint32 value)
{
    data->Initialize(SMSG_UPDATE_WORLD_STATE, 4+4);
    *data << uint32(field);
    *data << uint32(value);
}

void BattlegroundMgr::BuildPlaySoundPacket(WorldPacket *data, uint32 soundid)
{
    data->Initialize(SMSG_PLAY_SOUND, 4);
    *data << uint32(soundid);
}

void BattlegroundMgr::BuildPlayerLeftBattlegroundPacket(WorldPacket *data, const uint64& guid)
{
    data->Initialize(SMSG_BATTLEGROUND_PLAYER_LEFT, 8);
    *data << uint64(guid);
}

void BattlegroundMgr::BuildPlayerJoinedBattlegroundPacket(WorldPacket *data, Player* plr)
{
    data->Initialize(SMSG_BATTLEGROUND_PLAYER_JOINED, 8);
    *data << uint64(plr->GetGUID());
}

Battleground* BattlegroundMgr::GetBattlegroundThroughClientInstance(uint32 instanceId, BattlegroundTypeId bgTypeId)
{
    //cause at HandleBattlegroundJoinOpcode the clients sends the instanceid he gets from
    //SMSG_BATTLEFIELD_LIST we need to find the battleground with this clientinstance-id
    Battleground* pBG = GetBattlegroundTemplate(bgTypeId);
    if(!pBG)
        return NULL;

    if(pBG->isArena())
        return GetBattleground(instanceId, bgTypeId);

    for(BattlegroundSet::iterator itr = m_Battlegrounds[bgTypeId].begin(); itr != m_Battlegrounds[bgTypeId].end(); ++itr)
    {
        if(itr->second->GetClientInstanceID() == instanceId)
            return itr->second;
    }
    return NULL;
}

Battleground* BattlegroundMgr::GetBattleground(uint32 InstanceID, BattlegroundTypeId bgTypeId)
{
    if(!InstanceID)
        return NULL;
    //search if needed
    BattlegroundSet::iterator itr;
    if(bgTypeId == BATTLEGROUND_TYPE_NONE)
    {
        for(uint32 i = BATTLEGROUND_AV; i < MAX_BATTLEGROUND_TYPE_ID; i++)
        {
            itr = m_Battlegrounds[i].find(InstanceID);
            if(itr != m_Battlegrounds[i].end())
                return itr->second;
        }
        return NULL;
    }
    itr = m_Battlegrounds[bgTypeId].find(InstanceID);
    return ((itr != m_Battlegrounds[bgTypeId].end()) ? itr->second : NULL);
}

Battleground* BattlegroundMgr::GetBattlegroundTemplate(BattlegroundTypeId bgTypeId)
{
    //map is sorted and we can be sure that lowest instance id has only BG template
    return m_Battlegrounds[bgTypeId].empty() ? NULL : m_Battlegrounds[bgTypeId].begin()->second;
}

uint32 BattlegroundMgr::CreateClientVisibleInstanceId(BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id)
{
    if(IsArenaType(bgTypeId))
        return 0;                                           //arenas don't have client-instanceids

    // we create here an instanceid, which is just for
    // displaying this to the client and without any other use..
    // the client-instanceIds are unique for each battleground-type
    // the instance-id just needs to be as low as possible, beginning with 1
    // the following works, because std::set is default ordered with "<"
    // the optimalization would be to use as bitmask std::vector<uint32> - but that would only make code unreadable
    uint32 lastId = 0;
    for(std::set<uint32>::iterator itr = m_ClientBattlegroundIds[bgTypeId][bracket_id].begin(); itr != m_ClientBattlegroundIds[bgTypeId][bracket_id].end();)
    {
        if((++lastId) != *itr)                             //if there is a gap between the ids, we will break..
            break;
        lastId = *itr;
    }
    m_ClientBattlegroundIds[bgTypeId][bracket_id].insert(lastId + 1);
    return lastId + 1;
}

// create a new battleground that will really be used to play
Battleground* BattlegroundMgr::CreateNewBattleground(BattlegroundTypeId bgTypeId, PvPDifficultyEntry const* bracketEntry, uint8 arenaType, bool isRated)
{
    // get the template BG
    Battleground* pBG_TEMPLATE = GetBattlegroundTemplate(bgTypeId);
    BattlegroundSelectionWeightMap *selectionWeights = NULL;

    if(!pBG_TEMPLATE)
    {
        sLog->outError("Battleground: CreateNewBattleground - bg template not found for %u", bgTypeId);
        return NULL;
    }
    bool isRandom = false;

    if(pBG_TEMPLATE->isArena())
        selectionWeights = &m_ArenaSelectionWeights;
    else if(bgTypeId == BATTLEGROUND_RB)
    {
        selectionWeights = &m_BGSelectionWeights;
        isRandom = true;
    }

    if(selectionWeights)
    {
        if(!selectionWeights->size())
           return NULL;
        uint32 Weight = 0;
        uint32 selectedWeight = 0;
        bgTypeId = BATTLEGROUND_TYPE_NONE;
        // Get sum of all weights
        for(BattlegroundSelectionWeightMap::const_iterator it = selectionWeights->begin(); it != selectionWeights->end(); ++it)
            Weight += it->second;
        if(!Weight)
            return NULL;
        // Select a random value
        selectedWeight = urand(0, Weight-1);

        // Select the correct bg (if we have in DB A(10), B(20), C(10), D(15) --> [0---A---9|10---B---29|30---C---39|40---D---54])
        Weight = 0;
        for(BattlegroundSelectionWeightMap::const_iterator it = selectionWeights->begin(); it != selectionWeights->end(); ++it)
        {
            Weight += it->second;
            if(selectedWeight < Weight)
            {
                bgTypeId = it->first;
                break;
            }
        }
        pBG_TEMPLATE = GetBattlegroundTemplate(bgTypeId);
        if(!pBG_TEMPLATE)
        {
            sLog->outError("Battleground: CreateNewBattleground - bg template not found for %u", bgTypeId);
            return NULL;
        }
    }

    Battleground* pBG = NULL;
    // create a copy of the BG template
    switch(bgTypeId)
    {
        case BATTLEGROUND_AV:
            pBG = new BattlegroundAV(*(BattlegroundAV*)pBG_TEMPLATE);
            break;
        case BATTLEGROUND_WS:
            pBG = new BattlegroundWS(*(BattlegroundWS*)pBG_TEMPLATE);
            break;
        case BATTLEGROUND_AB:
            pBG = new BattlegroundAB(*(BattlegroundAB*)pBG_TEMPLATE);
            break;
        case BATTLEGROUND_NA:
            pBG = new BattlegroundNA(*(BattlegroundNA*)pBG_TEMPLATE);
            break;
        case BATTLEGROUND_BE:
            pBG = new BattlegroundBE(*(BattlegroundBE*)pBG_TEMPLATE);
            break;
        case BATTLEGROUND_AA:
            pBG = new BattlegroundAA(*(BattlegroundAA*)pBG_TEMPLATE);
            break;
        case BATTLEGROUND_EY:
            pBG = new BattlegroundEY(*(BattlegroundEY*)pBG_TEMPLATE);
            break;
        case BATTLEGROUND_RL:
            pBG = new BattlegroundRL(*(BattlegroundRL*)pBG_TEMPLATE);
            break;
        case BATTLEGROUND_SA:
            pBG = new BattlegroundSA(*(BattlegroundSA*)pBG_TEMPLATE);
            break;
        case BATTLEGROUND_DS:
            pBG = new BattlegroundDS(*(BattlegroundDS*)pBG_TEMPLATE);
            break;
        case BATTLEGROUND_RV:
            pBG = new BattlegroundRV(*(BattlegroundRV*)pBG_TEMPLATE);
            break;
        case BATTLEGROUND_IC:
            pBG = new BattlegroundIC(*(BattlegroundIC*)pBG_TEMPLATE);
            break;
        case BATTLEGROUND_RB:
            pBG = new BattlegroundRB(*(BattlegroundRB*)pBG_TEMPLATE);
            break;
        default:
            //error, but it is handled few lines above
            return 0;
    }

    // set battelground difficulty before initialization
    pBG->SetBracket(bracketEntry);

    // generate a new instance id
    uint32 instanceId = sMapMgr->GenerateInstanceId();
    // set instance id
    pBG->SetInstanceID(instanceId);

    pBG->SetClientInstanceID(CreateClientVisibleInstanceId(isRandom ? BATTLEGROUND_RB : bgTypeId, bracketEntry->GetBracketId()));

    // reset the new bg (set status to status_wait_queue from status_none)
    pBG->Reset();

    // start the joining of the bg
    pBG->SetStatus(STATUS_WAIT_JOIN);
    pBG->SetArenaType(arenaType);
    pBG->SetRated(isRated);
    pBG->SetRandom(isRandom);
    pBG->SetTypeID(isRandom ? BATTLEGROUND_RB : bgTypeId);
    pBG->SetRandomTypeID(bgTypeId);

	/*if (pBG->isArena())
    {
        uint32 maxPlayersPerTeam = 0;
        switch (arenaType)
        {
            case ARENA_TYPE_2v2:
                maxPlayersPerTeam = 2;
                break;
            case ARENA_TYPE_3v3:
                maxPlayersPerTeam = 3;
                break;
            case ARENA_TYPE_5v5:
                maxPlayersPerTeam = 5;
                break;
			case ARENA_TYPE_1v1:
                maxPlayersPerTeam = 1;
                break;
        }

        pBG->SetMaxPlayersPerTeam(maxPlayersPerTeam);
        pBG->SetMaxPlayers(maxPlayersPerTeam * 2);
    }*/
	
    return pBG;
}

// used to create the BG templates
uint32 BattlegroundMgr::CreateBattleground(BattlegroundTypeId bgTypeId, bool IsArena, uint32 MinPlayersPerTeam, uint32 MaxPlayersPerTeam, uint32 LevelMin, uint32 LevelMax, char* BattlegroundName, uint32 MapID, float Team1StartLocX, float Team1StartLocY, float Team1StartLocZ, float Team1StartLocO, float Team2StartLocX, float Team2StartLocY, float Team2StartLocZ, float Team2StartLocO, uint32 scriptId)
{
    // Create the BG
    Battleground* pBG = NULL;
    switch(bgTypeId)
    {
        case BATTLEGROUND_AV: pBG = new BattlegroundAV; break;
        case BATTLEGROUND_WS: pBG = new BattlegroundWS; break;
        case BATTLEGROUND_AB: pBG = new BattlegroundAB; break;
        case BATTLEGROUND_NA: pBG = new BattlegroundNA; break;
        case BATTLEGROUND_BE: pBG = new BattlegroundBE; break;
        case BATTLEGROUND_AA: pBG = new BattlegroundAA; break;
        case BATTLEGROUND_EY: pBG = new BattlegroundEY; break;
        case BATTLEGROUND_RL: pBG = new BattlegroundRL; break;
        case BATTLEGROUND_SA: pBG = new BattlegroundSA; break;
        case BATTLEGROUND_DS: pBG = new BattlegroundDS; break;
        case BATTLEGROUND_RV: pBG = new BattlegroundRV; break;
        case BATTLEGROUND_IC: pBG = new BattlegroundIC; break;
        case BATTLEGROUND_RB: pBG = new BattlegroundRB; break;
        default:
            pBG = new Battleground;
            break;
    }

    pBG->SetMapId(MapID);
    pBG->SetTypeID(bgTypeId);
    pBG->SetInstanceID(0);
    pBG->SetArenaorBGType(IsArena);
    pBG->SetMinPlayersPerTeam(MinPlayersPerTeam);
    pBG->SetMaxPlayersPerTeam(MaxPlayersPerTeam);
    pBG->SetMinPlayers(MinPlayersPerTeam * 2);
    pBG->SetMaxPlayers(MaxPlayersPerTeam * 2);
    pBG->SetName(BattlegroundName);
    pBG->SetTeamStartLoc(ALLIANCE, Team1StartLocX, Team1StartLocY, Team1StartLocZ, Team1StartLocO);
    pBG->SetTeamStartLoc(HORDE,    Team2StartLocX, Team2StartLocY, Team2StartLocZ, Team2StartLocO);
    pBG->SetLevelRange(LevelMin, LevelMax);
    pBG->SetScriptId(scriptId);

    // add bg to update list
    AddBattleground(pBG->GetInstanceID(), pBG->GetTypeID(), pBG);

    // return some not-null value, bgTypeId is good enough for me
    return bgTypeId;
}

void BattlegroundMgr::CreateInitialBattlegrounds()
{
    uint32 oldMSTime = getMSTime();

    float AStartLoc[4];
    float HStartLoc[4];
    uint32 MaxPlayersPerTeam, MinPlayersPerTeam, MinLvl, MaxLvl, start1, start2;
    uint8 selectionWeight;
    BattlemasterListEntry const *bl;
    WorldSafeLocsEntry const *start;
    bool IsArena;
    uint32 scriptId = 0;

    //                                                       0   1                 2                 3      4      5                6              7             8           9      10
    QueryResult result = WorldDatabase.Query("SELECT id, MinPlayersPerTeam, MaxPlayersPerTeam, MinLvl, MaxLvl, AllianceStartLoc, AllianceStartO, HordeStartLoc, HordeStartO, Weight, ScriptName FROM battleground_template");

    if(!result)
    {
        sLog->outErrorDb(">> Loaded 0 battlegrounds. DB table `battleground_template` is empty.");
        sLog->outString();
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 bgTypeID_ = fields[0].GetUInt32();
        if(sDisableMgr->IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, bgTypeID_, NULL))
            continue;

        // can be overwrite by values from DB
        bl = sBattlemasterListStore.LookupEntry(bgTypeID_);
        if(!bl)
        {
            sLog->outError("Battleground ID %u not found in BattlemasterList.dbc. Battleground not created.", bgTypeID_);
            continue;
        }

        BattlegroundTypeId bgTypeID = BattlegroundTypeId(bgTypeID_);

        IsArena = (bl->type == TYPE_ARENA);
        MinPlayersPerTeam = fields[1].GetUInt32();
        MaxPlayersPerTeam = fields[2].GetUInt32();
        MinLvl = fields[3].GetUInt32();
        MaxLvl = fields[4].GetUInt32();
        //check values from DB
        if(MaxPlayersPerTeam == 0 || MinPlayersPerTeam == 0 || MinPlayersPerTeam > MaxPlayersPerTeam)
        {
            MinPlayersPerTeam = 0;                          // by default now expected strong full bg requirement
            MaxPlayersPerTeam = 40;
        }
        if(MinLvl == 0 || MaxLvl == 0 || MinLvl > MaxLvl)
        {
            //TO-DO: FIX ME
            MinLvl = 0;//bl->minlvl;
            MaxLvl = 80;//bl->maxlvl;
        }

        start1 = fields[5].GetUInt32();

        start = sWorldSafeLocsStore.LookupEntry(start1);
        if(start)
        {
            AStartLoc[0] = start->x;
            AStartLoc[1] = start->y;
            AStartLoc[2] = start->z;
            AStartLoc[3] = fields[6].GetFloat();
        }
        else if(bgTypeID == BATTLEGROUND_AA || bgTypeID == BATTLEGROUND_RB)
        {
            AStartLoc[0] = 0;
            AStartLoc[1] = 0;
            AStartLoc[2] = 0;
            AStartLoc[3] = fields[6].GetFloat();
        }
        else
        {
            sLog->outErrorDb("Table `battleground_template` for id %u have non-existed WorldSafeLocs.dbc id %u in field `AllianceStartLoc`. BG not created.", bgTypeID, start1);
            continue;
        }

        start2 = fields[7].GetUInt32();

        start = sWorldSafeLocsStore.LookupEntry(start2);
        if(start)
        {
            HStartLoc[0] = start->x;
            HStartLoc[1] = start->y;
            HStartLoc[2] = start->z;
            HStartLoc[3] = fields[8].GetFloat();
        }
        else if(bgTypeID == BATTLEGROUND_AA || bgTypeID == BATTLEGROUND_RB)
        {
            HStartLoc[0] = 0;
            HStartLoc[1] = 0;
            HStartLoc[2] = 0;
            HStartLoc[3] = fields[8].GetFloat();
        }
        else
        {
            sLog->outErrorDb("Table `battleground_template` for id %u have non-existed WorldSafeLocs.dbc id %u in field `HordeStartLoc`. BG not created.", bgTypeID, start2);
            continue;
        }

        selectionWeight = fields[9].GetUInt8();
        scriptId = sObjectMgr->GetScriptId(fields[10].GetCString());
        //sLog->outDetail("Creating battleground %s, %u-%u", bl->name[sWorld->GetDBClang()], MinLvl, MaxLvl);
        if(!CreateBattleground(bgTypeID, IsArena, MinPlayersPerTeam, MaxPlayersPerTeam, MinLvl, MaxLvl, bl->name[sWorld->GetDefaultDbcLocale()], bl->mapid[0], AStartLoc[0], AStartLoc[1], AStartLoc[2], AStartLoc[3], HStartLoc[0], HStartLoc[1], HStartLoc[2], HStartLoc[3], scriptId))
            continue;

        if(IsArena)
        {
            if(bgTypeID != BATTLEGROUND_AA)
                m_ArenaSelectionWeights[bgTypeID] = selectionWeight;
        }
        else if(bgTypeID != BATTLEGROUND_RB)
            m_BGSelectionWeights[bgTypeID] = selectionWeight;
        ++count;
    }
    while(result->NextRow());

    sLog->outString(">> Loaded %u battlegrounds in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

void BattlegroundMgr::InitAutomaticArenaPointDistribution()
{
    if(!sWorld->getBoolConfig(CONFIG_ARENA_AUTO_DISTRIBUTE_POINTS))
        return;

    time_t wstime = time_t(sWorld->getWorldState(WS_ARENA_DISTRIBUTION_TIME));
    time_t curtime = time(NULL);
    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Initializing Automatic Arena Point Distribution");
    if(wstime < curtime)
    {
        m_NextAutoDistributionTime = curtime;           // reset will be called in the next update
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: Next arena point distribution time in the past, reseting it now.");
    }
    else
        m_NextAutoDistributionTime = wstime;
    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Automatic Arena Point Distribution initialized.");
}

void BattlegroundMgr::BuildBattlegroundListPacket(WorldPacket *data, const uint64& guid, Player* plr, BattlegroundTypeId bgTypeId, uint8 fromWhere)
{
    if(!plr)
        return;

    uint32 winner_kills = plr->GetRandomWinner() ? BG_REWARD_WINNER_HONOR_LAST : BG_REWARD_WINNER_HONOR_FIRST;
    uint32 winner_arena = plr->GetRandomWinner() ? BG_REWARD_WINNER_ARENA_LAST : BG_REWARD_WINNER_ARENA_FIRST;
    uint32 loser_kills = plr->GetRandomWinner() ? BG_REWARD_LOSER_HONOR_LAST : BG_REWARD_LOSER_HONOR_FIRST;

    winner_kills = Trinity::Honor::hk_honor_at_level(plr->getLevel(), float(winner_kills));
    loser_kills = Trinity::Honor::hk_honor_at_level(plr->getLevel(), float(loser_kills));

    data->Initialize(SMSG_BATTLEFIELD_LIST);
    *data << uint64(guid);                                  // battlemaster guid
    *data << uint8(fromWhere);                              // from where you joined
    *data << uint32(bgTypeId);                              // battleground id
    *data << uint8(0);                                      // unk
    *data << uint8(0);                                      // unk

    // Rewards
    *data << uint8(plr->GetRandomWinner());                 // 3.3.3 hasWin
    *data << uint32(winner_kills);                          // 3.3.3 winHonor
    *data << uint32(winner_arena);                          // 3.3.3 winArena
    *data << uint32(loser_kills);                           // 3.3.3 lossHonor

    uint8 isRandom = bgTypeId == BATTLEGROUND_RB;

    *data << uint8(isRandom);                               // 3.3.3 isRandom
    if(isRandom)
    {
        // Rewards (random)
        *data << uint8(plr->GetRandomWinner());             // 3.3.3 hasWin_Random
        *data << uint32(winner_kills);                      // 3.3.3 winHonor_Random
        *data << uint32(winner_arena);                      // 3.3.3 winArena_Random
        *data << uint32(loser_kills);                       // 3.3.3 lossHonor_Random
    }

    if(bgTypeId == BATTLEGROUND_AA)                         // arena
    {
        *data << uint32(0);                                 // unk (count?)
    }
    else                                                    // battleground
    {
        size_t count_pos = data->wpos();
        *data << uint32(0);                                 // number of bg instances

        if(Battleground* pBG_Template = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId))
        {
            // expected bracket entry
            if(PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(pBG_Template->GetMapId(), plr->getLevel()))
            {
                uint32 count = 0;
                BattlegroundBracketId bracketId = bracketEntry->GetBracketId();
                for(std::set<uint32>::iterator itr = m_ClientBattlegroundIds[bgTypeId][bracketId].begin(); itr != m_ClientBattlegroundIds[bgTypeId][bracketId].end();++itr)
                {
                    *data << uint32(*itr);
                    ++count;
                }
                data->put<uint32>(count_pos, count);
            }
        }
    }
}

void BattlegroundMgr::SendToBattleground(Player* pPlayer, uint32 instanceId, BattlegroundTypeId bgTypeId)
{
    if(Battleground* pBG = GetBattleground(instanceId, bgTypeId))
    {
        uint32 mapid    = pBG->GetMapId();
        float x, y, z, O;
        uint32 team     = pPlayer->GetBGTeam();
        if(team == 0)
            team = pPlayer->GetBGTeam();
        pBG->GetTeamStartLoc(team, x, y, z, O);

        sLog->outDetail("BATTLEGROUND: Sending %s to map %u, X %f, Y %f, Z %f, O %f", pPlayer->GetName(), mapid, x, y, z, O);
        pPlayer->TeleportTo(mapid, x, y, z, O);
    } else sLog->outError("player %u is trying to port to non-existent bg instance %u", pPlayer->GetGUIDLow(), instanceId);
}

void BattlegroundMgr::SendToBattleground(Player *player, Battleground *bg)
{
    if (bg)
    {
        uint32 mapid = bg->GetMapId();
        float x, y, z, O;
        uint32 team = player->GetBGTeam();
        bg->GetTeamStartLoc(team, x, y, z, O);

        sLog->outDetail("BATTLEGROUND: Sending %s to map %u, X %f, Y %f, Z %f, O %f", player->GetName(), mapid, x, y, z, O);
        player->TeleportTo(mapid, x, y, z, O);
    }
}

void BattlegroundMgr::SendAreaSpiritHealerQueryOpcode(Player* pl, Battleground* pBG, const uint64& guid)
{
    WorldPacket data(SMSG_AREA_SPIRIT_HEALER_TIME, 12);
    uint32 time_ = 30000 - pBG->GetLastResurrectTime();      // resurrect every 30 seconds
    if(time_ == uint32(-1))
        time_ = 0;
    data << guid << time_;
    pl->GetSession()->SendPacket(&data);
}

bool BattlegroundMgr::IsArenaType(BattlegroundTypeId bgTypeId)
{
    return (bgTypeId == BATTLEGROUND_AA ||
        bgTypeId == BATTLEGROUND_BE ||
        bgTypeId == BATTLEGROUND_NA ||
        bgTypeId == BATTLEGROUND_DS ||
        bgTypeId == BATTLEGROUND_RV ||
        bgTypeId == BATTLEGROUND_RL ||
        bgTypeId == BATTLEGROUND_DS);
}

BattlegroundQueueTypeId BattlegroundMgr::BGQueueTypeId(BattlegroundTypeId bgTypeId, uint8 arenaType)
{
    switch(bgTypeId)
    {
        case BATTLEGROUND_WS:
            return BATTLEGROUND_QUEUE_WS;
        case BATTLEGROUND_AB:
            return BATTLEGROUND_QUEUE_AB;
        case BATTLEGROUND_AV:
            return BATTLEGROUND_QUEUE_AV;
        case BATTLEGROUND_EY:
            return BATTLEGROUND_QUEUE_EY;
        case BATTLEGROUND_SA:
            return BATTLEGROUND_QUEUE_SA;
        case BATTLEGROUND_IC:
            return BATTLEGROUND_QUEUE_IC;
        case BATTLEGROUND_RB:
            return BATTLEGROUND_QUEUE_RB;
        case BATTLEGROUND_AA:
        case BATTLEGROUND_NA:
        case BATTLEGROUND_RL:
        case BATTLEGROUND_BE:
        case BATTLEGROUND_DS:
        case BATTLEGROUND_RV:
            switch(arenaType)
            {
                case ARENA_TYPE_2v2:
                    return BATTLEGROUND_QUEUE_2v2;
                case ARENA_TYPE_3v3:
                    return BATTLEGROUND_QUEUE_3v3;
                case ARENA_TYPE_5v5:
                    return BATTLEGROUND_QUEUE_5v5;
                default:
                    return BATTLEGROUND_QUEUE_NONE;
            }
        default:
            return BATTLEGROUND_QUEUE_NONE;
    }
}

BattlegroundTypeId BattlegroundMgr::BGTemplateId(BattlegroundQueueTypeId bgQueueTypeId)
{
    switch(bgQueueTypeId)
    {
        case BATTLEGROUND_QUEUE_WS:
            return BATTLEGROUND_WS;
        case BATTLEGROUND_QUEUE_AB:
            return BATTLEGROUND_AB;
        case BATTLEGROUND_QUEUE_AV:
            return BATTLEGROUND_AV;
        case BATTLEGROUND_QUEUE_EY:
            return BATTLEGROUND_EY;
        case BATTLEGROUND_QUEUE_SA:
            return BATTLEGROUND_SA;
        case BATTLEGROUND_QUEUE_IC:
            return BATTLEGROUND_IC;
        case BATTLEGROUND_QUEUE_RB:
            return BATTLEGROUND_RB;
        case BATTLEGROUND_QUEUE_2v2:
        case BATTLEGROUND_QUEUE_3v3:
        case BATTLEGROUND_QUEUE_5v5:
            return BATTLEGROUND_AA;
        default:
            return BattlegroundTypeId(0);                   // used for unknown template (it existed and do nothing)
    }
}

uint8 BattlegroundMgr::BGArenaType(BattlegroundQueueTypeId bgQueueTypeId)
{
    switch(bgQueueTypeId)
    {
        case BATTLEGROUND_QUEUE_2v2:
            return ARENA_TYPE_2v2;
        case BATTLEGROUND_QUEUE_3v3:
            return ARENA_TYPE_3v3;
        case BATTLEGROUND_QUEUE_5v5:
            return ARENA_TYPE_5v5;
        default:
            return 0;
    }
}

void BattlegroundMgr::ToggleTesting()
{
    m_Testing = !m_Testing;
    if(m_Testing)
        sWorld->SendWorldText(LANG_DEBUG_BG_ON);
    else
        sWorld->SendWorldText(LANG_DEBUG_BG_OFF);
}

void BattlegroundMgr::ToggleArenaTesting()
{
    m_ArenaTesting = !m_ArenaTesting;
    if(m_ArenaTesting)
        sWorld->SendWorldText(LANG_DEBUG_ARENA_ON);
    else
        sWorld->SendWorldText(LANG_DEBUG_ARENA_OFF);
}

void BattlegroundMgr::SetHolidayWeekends(uint32 mask)
{
    for(uint32 bgtype = 1; bgtype < MAX_BATTLEGROUND_TYPE_ID; ++bgtype)
    {
        if(Battleground* pBG = GetBattlegroundTemplate(BattlegroundTypeId(bgtype)))
        {
            pBG->SetHoliday(mask & (1 << bgtype));
        }
    }
}

void BattlegroundMgr::ScheduleQueueUpdate(uint32 arenaMatchmakerRating, uint8 arenaType, BattlegroundQueueTypeId bgQueueTypeId, BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id)
{
    //This method must be atomic, TODO add mutex
    //we will use only 1 number created of bgTypeId and bracket_id
    uint64 schedule_id = ((uint64)arenaMatchmakerRating << 32) | (arenaType << 24) | (bgQueueTypeId << 16) | (bgTypeId << 8) | bracket_id;
    bool found = false;
    for(uint8 i = 0; i < m_QueueUpdateScheduler.size(); i++)
    {
        if(m_QueueUpdateScheduler[i] == schedule_id)
        {
            found = true;
            break;
        }
    }
    if(!found)
        m_QueueUpdateScheduler.push_back(schedule_id);
}

uint32 BattlegroundMgr::GetMaxRatingDifference() const
{
    // this is for stupid people who can't use brain and set max rating difference to 0
    uint32 diff = sWorld->getIntConfig(CONFIG_ARENA_MAX_RATING_DIFFERENCE);
    if(diff == 0)
        diff = 5000;
    return diff;
}

uint32 BattlegroundMgr::GetRatingDiscardTimer() const
{
    return sWorld->getIntConfig(CONFIG_ARENA_RATING_DISCARD_TIMER);
}

uint32 BattlegroundMgr::GetPrematureFinishTime() const
{
    return sWorld->getIntConfig(CONFIG_BATTLEGROUND_PREMATURE_FINISH_TIMER);
}

void BattlegroundMgr::LoadBattleMastersEntry()
{
    uint32 oldMSTime = getMSTime();

    mBattleMastersMap.clear();                                  // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, bg_template FROM battlemaster_entry");

    if(!result)
    {
        sLog->outString(">> Loaded 0 battlemaster entries. DB table `battlemaster_entry` is empty!");
        sLog->outString();
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        uint32 bgTypeId  = fields[1].GetUInt32();
        if(!sBattlemasterListStore.LookupEntry(bgTypeId))
        {
            sLog->outErrorDb("Table `battlemaster_entry` contain entry %u for not existed battleground type %u, ignored.", entry, bgTypeId);
            continue;
        }

        mBattleMastersMap[entry] = BattlegroundTypeId(bgTypeId);

    }
    while(result->NextRow());

    sLog->outString(">> Loaded %u battlemaster entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    sLog->outString();
}

HolidayIds BattlegroundMgr::BGTypeToWeekendHolidayId(BattlegroundTypeId bgTypeId)
{
    switch(bgTypeId)
    {
        case BATTLEGROUND_AV: return HOLIDAY_CALL_TO_ARMS_AV;
        case BATTLEGROUND_EY: return HOLIDAY_CALL_TO_ARMS_EY;
        case BATTLEGROUND_WS: return HOLIDAY_CALL_TO_ARMS_WS;
        case BATTLEGROUND_SA: return HOLIDAY_CALL_TO_ARMS_SA;
        case BATTLEGROUND_AB: return HOLIDAY_CALL_TO_ARMS_AB;
        case BATTLEGROUND_IC: return HOLIDAY_CALL_TO_ARMS_IC;
        default: return HOLIDAY_NONE;
    }
}

BattlegroundTypeId BattlegroundMgr::WeekendHolidayIdToBGType(HolidayIds holiday)
{
    switch(holiday)
    {
        case HOLIDAY_CALL_TO_ARMS_AV: return BATTLEGROUND_AV;
        case HOLIDAY_CALL_TO_ARMS_EY: return BATTLEGROUND_EY;
        case HOLIDAY_CALL_TO_ARMS_WS: return BATTLEGROUND_WS;
        case HOLIDAY_CALL_TO_ARMS_SA: return BATTLEGROUND_SA;
        case HOLIDAY_CALL_TO_ARMS_AB: return BATTLEGROUND_AB;
        case HOLIDAY_CALL_TO_ARMS_IC: return BATTLEGROUND_IC;
        default: return BATTLEGROUND_TYPE_NONE;
    }
}

bool BattlegroundMgr::IsBGWeekend(BattlegroundTypeId bgTypeId)
{
    return IsHolidayActive(BGTypeToWeekendHolidayId(bgTypeId));
}

bool BattlegroundMgr::HasBattleground(Battleground *_bg)
{
    BattlegroundSet::iterator itr, next;
    for (uint32 i = BATTLEGROUND_TYPE_NONE; i < MAX_BATTLEGROUND_TYPE_ID; ++i)
    {
        itr = m_Battlegrounds[i].begin();
        // skip updating battleground template
        if (itr != m_Battlegrounds[i].end())
            ++itr;
        for (; itr != m_Battlegrounds[i].end(); itr = next)
        {
            next = itr;
            ++next;
            Battleground* bg = itr->second;
            if (bg == _bg)
                return true;
        }
    }

    return false;
}