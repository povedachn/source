/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#include "Common.h"
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "MapManager.h"

void WorldSession::SendNameQueryOpcode(uint64 guid)
{
    Player* player = ObjectAccessor::FindPlayer(guid);
    CharacterNameData const* nameData = sWorld->GetCharacterNameData(GUID_LOPART(guid));

                                                            // guess size
    WorldPacket data(SMSG_NAME_QUERY_RESPONSE, (8+1+1+1+1+1+10));
    data.appendPackGUID(guid);                              // player guid
    if(!nameData)
    {
        data << uint8(1);                                   // name unknown
        SendPacket(&data);
        return;
    }

    data << uint8(0);                                       // name known
    data << nameData->m_name;                               // played name
    data << uint8(0);                                       // realm name for cross realm BG usage
    data << uint8(player ? player->GetFakeRaceOrRace() : nameData->m_race);
    data << uint8(nameData->m_gender);
    data << uint8(nameData->m_class);

    if(DeclinedName const* names = (player ? player->GetDeclinedNames() : NULL))
    {
        data << uint8(1);                                   // name is declined
        for(uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
            data << names->name[i];
    }
    else
        data << uint8(0);                                   // name is not declined

    SendPacket(&data);
}

void WorldSession::HandleNameQueryOpcode(WorldPacket &recv_data)
{
    uint64 guid;

    recv_data >> guid;

    SendNameQueryOpcode(guid);
}

void WorldSession::HandleQueryTimeOpcode(WorldPacket & /*recv_data*/)
{
    SendQueryTimeResponse();
}

void WorldSession::SendQueryTimeResponse()
{
    WorldPacket data(SMSG_QUERY_TIME_RESPONSE, 4+4);
    data << uint32(time(NULL));
    data << uint32(sWorld->GetNextDailyQuestsResetTime() - time(NULL));
    SendPacket(&data);
}

/// Only _static_ data send in this packet !!!
void WorldSession::HandleCreatureQueryOpcode(WorldPacket &recv_data)
{
    uint32 entry;
    recv_data >> entry;
    uint64 guid;
    recv_data >> guid;

    CreatureTemplate const *ci = sObjectMgr->GetCreatureTemplate(entry);
    if(ci)
    {
        std::string Name, SubName;
        Name = ci->Name;
        SubName = ci->SubName;

        int loc_idx = GetSessionDbLocaleIndex();
        if(loc_idx >= 0)
        {
            if(CreatureLocale const *cl = sObjectMgr->GetCreatureLocale(entry))
            {
                sObjectMgr->GetLocaleString(cl->Name, loc_idx, Name);
                sObjectMgr->GetLocaleString(cl->SubName, loc_idx, SubName);
            }
        }
        sLog->outDetail("WORLD: CMSG_CREATURE_QUERY '%s' - Entry: %u.", ci->Name.c_str(), entry);
                                                            // guess size
        WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 100);
        data << uint32(entry);                              // creature entry
        data << Name;
        data << uint8(0) << uint8(0) << uint8(0);           // name2, name3, name4, always empty
        data << SubName;
        data << ci->IconName;                               // "Directions" for guard, string for Icons 2.3.0
        data << uint32(ci->type_flags);                     // flags
        data << uint32(ci->type);                           // CreatureType.dbc
        data << uint32(ci->family);                         // CreatureFamily.dbc
        data << uint32(ci->rank);                           // Creature Rank (elite, boss, etc)
        data << uint32(ci->KillCredit[0]);                  // new in 3.1, kill credit
        data << uint32(ci->KillCredit[1]);                  // new in 3.1, kill credit
        data << uint32(ci->Modelid1);                       // Modelid1
        data << uint32(ci->Modelid2);                       // Modelid2
        data << uint32(ci->Modelid3);                       // Modelid3
        data << uint32(ci->Modelid4);                       // Modelid4
        data << float(ci->ModHealth);                       // dmg/hp modifier
        data << float(ci->ModMana);                         // dmg/mana modifier
        data << uint8(ci->RacialLeader);
        for(uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            data << uint32(ci->questItems[i]);              // itemId[6], quest drop
        data << uint32(ci->movementId);                     // CreatureMovementInfo.dbc
        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_CREATURE_QUERY - NO CREATURE INFO! (GUID: %u, ENTRY: %u)",
            GUID_LOPART(guid), entry);
        WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 4);
        data << uint32(entry | 0x80000000);
        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
}

/// Only _static_ data send in this packet !!!
void WorldSession::HandleGameObjectQueryOpcode(WorldPacket &recv_data)
{
    uint32 entry;
    recv_data >> entry;
    uint64 guid;
    recv_data >> guid;

    const GameObjectTemplate *info = sObjectMgr->GetGameObjectTemplate(entry);
    if(info)
    {
        std::string Name;
        std::string IconName;
        std::string CastBarCaption;

        Name = info->name;
        IconName = info->IconName;
        CastBarCaption = info->castBarCaption;

        int loc_idx = GetSessionDbLocaleIndex();
        if(loc_idx >= 0)
        {
            if(GameObjectLocale const *gl = sObjectMgr->GetGameObjectLocale(entry))
            {
                sObjectMgr->GetLocaleString(gl->Name, loc_idx, Name);
                sObjectMgr->GetLocaleString(gl->CastBarCaption, loc_idx, CastBarCaption);
            }
        }
        sLog->outDetail("WORLD: CMSG_GAMEOBJECT_QUERY '%s' - Entry: %u. ", info->name.c_str(), entry);
        WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE, 150);
        data << uint32(entry);
        data << uint32(info->type);
        data << uint32(info->displayId);
        data << Name;
        data << uint8(0) << uint8(0) << uint8(0);           // name2, name3, name4
        data << IconName;                                   // 2.0.3, string. Icon name to use instead of default icon for go's (ex: "Attack" makes sword)
        data << CastBarCaption;                             // 2.0.3, string. Text will appear in Cast Bar when using GO (ex: "Collecting")
        data << info->unk1;                                 // 2.0.3, string
        data.append(info->raw.data, 24);
        data << float(info->size);                          // go size
        for(uint32 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
            data << uint32(info->questItems[i]);              // itemId[6], quest drop
        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_GAMEOBJECT_QUERY - Missing gameobject info for(GUID: %u, ENTRY: %u)",
            GUID_LOPART(guid), entry);
        WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE, 4);
        data << uint32(entry | 0x80000000);
        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseQueryOpcode(WorldPacket & /*recv_data*/)
{
    sLog->outDetail("WORLD: Received MSG_CORPSE_QUERY");

    Corpse *corpse = GetPlayer()->GetCorpse();

    if(!corpse)
    {
        WorldPacket data(MSG_CORPSE_QUERY, 1);
        data << uint8(0);                                   // corpse not found
        SendPacket(&data);
        return;
    }

    uint32 mapid = corpse->GetMapId();
    float x = corpse->GetPositionX();
    float y = corpse->GetPositionY();
    float z = corpse->GetPositionZ();
    uint32 corpsemapid = mapid;

    // if corpse at different map
    if(mapid != GetPlayer()->GetMapId())
    {
        // search entrance map for proper show entrance
        if(MapEntry const* corpseMapEntry = sMapStore.LookupEntry(mapid))
        {
            if(corpseMapEntry->IsDungeon() && corpseMapEntry->entrance_map >= 0)
            {
                // if corpse map have entrance
                if(Map const* entranceMap = sMapMgr->CreateBaseMap(corpseMapEntry->entrance_map))
                {
                    mapid = corpseMapEntry->entrance_map;
                    x = corpseMapEntry->entrance_x;
                    y = corpseMapEntry->entrance_y;
                    z = entranceMap->GetHeight(GetPlayer()->GetPhaseMask(), x, y, MAX_HEIGHT);
                }
            }
        }
    }

    WorldPacket data(MSG_CORPSE_QUERY, 1+(6*4));
    data << uint8(1);                                       // corpse found
    data << int32(mapid);
    data << float(x);
    data << float(y);
    data << float(z);
    data << int32(corpsemapid);
    data << uint32(0);                                      // unknown
    SendPacket(&data);
}

void WorldSession::HandleNpcTextQueryOpcode(WorldPacket &recv_data)
{
    uint32 textID;
    uint64 guid;

    recv_data >> textID;
    sLog->outDetail("WORLD: CMSG_NPC_TEXT_QUERY ID '%u'", textID);

    recv_data >> guid;
    GetPlayer()->SetSelection(guid);

    GossipText const* pGossip = sObjectMgr->GetGossipText(textID);

    WorldPacket data(SMSG_NPC_TEXT_UPDATE, 100);          // guess size
    data << textID;

    if(!pGossip)
    {
        for(uint32 i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << float(0);
            data << "Saludos $N. ""\xc2\xbf""En qu""\xC3\xA9"" te puedo ayudar?";
            data << "Saludos $N. ""\xc2\xbf""En qu""\xC3\xA9"" te puedo ayudar?";
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
        }
    }
    else
    {
        std::string Text_0[MAX_LOCALES], Text_1[MAX_LOCALES];
        for(int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            Text_0[i]=pGossip->Options[i].Text_0;
            Text_1[i]=pGossip->Options[i].Text_1;
        }

        int loc_idx = GetSessionDbLocaleIndex();
        if(loc_idx >= 0)
        {
            if(NpcTextLocale const *nl = sObjectMgr->GetNpcTextLocale(textID))
            {
                for(int i = 0; i < MAX_LOCALES; ++i)
                {
                    sObjectMgr->GetLocaleString(nl->Text_0[i], loc_idx, Text_0[i]);
                    sObjectMgr->GetLocaleString(nl->Text_1[i], loc_idx, Text_1[i]);
                }
            }
        }

        for(int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << pGossip->Options[i].Probability;

            if(Text_0[i].empty())
                data << Text_1[i];
            else
                data << Text_0[i];

            if(Text_1[i].empty())
                data << Text_0[i];
            else
                data << Text_1[i];

            data << pGossip->Options[i].Language;

            for(int j = 0; j < MAX_GOSSIP_TEXT_EMOTES; ++j)
            {
                data << pGossip->Options[i].Emotes[j]._Delay;
                data << pGossip->Options[i].Emotes[j]._Emote;
            }
        }
    }

    SendPacket(&data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_NPC_TEXT_UPDATE");
}

void WorldSession::HandlePageTextQueryOpcode(WorldPacket &recv_data)
{
    sLog->outDetail("WORLD: Received CMSG_PAGE_TEXT_QUERY");
    recv_data.hexlike();

    uint32 pageID;
    recv_data >> pageID;
    recv_data.read_skip<uint64>();                          // guid

    while(pageID)
    {
        PageText const* pageText = sObjectMgr->GetPageText(pageID);
                                                            // guess size
        WorldPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 50);
        data << pageID;

        if(!pageText)
        {
            data << "Falta la pagina.";
            data << uint32(0);
            pageID = 0;
        }
        else
        {
            std::string Text = pageText->Text;

            int loc_idx = GetSessionDbLocaleIndex();
            if(loc_idx >= 0)
                if(PageTextLocale const *pl = sObjectMgr->GetPageTextLocale(pageID))
                    sObjectMgr->GetLocaleString(pl->Text, loc_idx, Text);

            data << Text;
            data << uint32(pageText->NextPage);
            pageID = pageText->NextPage;
        }
        SendPacket(&data);

        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_PAGE_TEXT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseMapPositionQuery(WorldPacket &recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recv CMSG_CORPSE_MAP_POSITION_QUERY");

    uint32 unk;
    recv_data >> unk;

    WorldPacket data(SMSG_CORPSE_MAP_POSITION_QUERY_RESPONSE, 4+4+4+4);
    data << float(0);
    data << float(0);
    data << float(0);
    data << float(0);
    SendPacket(&data);
}

void WorldSession::HandleQuestPOIQuery(WorldPacket &recv_data)
{
    uint32 count;
    recv_data >> count; // quest count, max=25

    if(count >= MAX_QUEST_LOG_SIZE)
        return;

    WorldPacket data(SMSG_QUEST_POI_QUERY_RESPONSE, 4+(4+4)*count);
    data << uint32(count); // count

    for(uint32 i = 0; i < count; ++i)
    {
        uint32 questId;
        recv_data >> questId; // quest id

        bool questOk = false;

        uint16 questSlot = _player->FindQuestSlot(questId);

        if(questSlot != MAX_QUEST_LOG_SIZE)
            questOk =_player->GetQuestSlotQuestId(questSlot) == questId;

        if(questOk)
        {
            QuestPOIVector const *POI = sObjectMgr->GetQuestPOIVector(questId);

            if(POI)
            {
                data << uint32(questId); // quest ID
                data << uint32(POI->size()); // POI count

                for(QuestPOIVector::const_iterator itr = POI->begin(); itr != POI->end(); ++itr)
                {
                    data << uint32(itr->Id);                // POI index
                    data << int32(itr->ObjectiveIndex);     // objective index
                    data << uint32(itr->MapId);             // mapid
                    data << uint32(itr->AreaId);            // areaid
                    data << uint32(itr->Unk2);              // unknown
                    data << uint32(itr->Unk3);              // unknown
                    data << uint32(itr->Unk4);              // unknown
                    data << uint32(itr->points.size());     // POI points count

                    for(std::vector<QuestPOIPoint>::const_iterator itr2 = itr->points.begin(); itr2 != itr->points.end(); ++itr2)
                    {
                        data << int32(itr2->x); // POI point x
                        data << int32(itr2->y); // POI point y
                    }
                }
            }
            else
            {
                data << uint32(questId); // quest ID
                data << uint32(0); // POI count
            }
        }
        else
        {
            data << uint32(questId); // quest ID
            data << uint32(0); // POI count
        }
    }

    SendPacket(&data);
}
