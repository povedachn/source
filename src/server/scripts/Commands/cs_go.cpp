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

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "TicketMgr.h"
#include "Chat.h"

class go_commandscript : public CommandScript
{
public:
    go_commandscript() : CommandScript("go_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand goCommandTable[] =
        {
            { "creature",       SEC_MODERATOR,      false, &HandleGoCreatureCommand,          "", NULL },
            { "graveyard",      SEC_MODERATOR,      false, &HandleGoGraveyardCommand,         "", NULL },
            { "grid",           SEC_MODERATOR,      false, &HandleGoGridCommand,              "", NULL },
            { "object",         SEC_MODERATOR,      false, &HandleGoObjectCommand,            "", NULL },
            { "taxinode",       SEC_MODERATOR,      false, &HandleGoTaxinodeCommand,          "", NULL },
            { "trigger",        SEC_MODERATOR,      false, &HandleGoTriggerCommand,           "", NULL },
            { "xyz",            SEC_MODERATOR,      false, &HandleGoXYZCommand,               "", NULL },
            { "ticket",         SEC_MODERATOR,      false, &HandleGoTicketCommand,            "", NULL },
            { "",               SEC_MODERATOR,      false, &HandleGoXYZCommand,               "", NULL },
            { NULL,             0,                  false, NULL,                              "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "go",             SEC_MODERATOR,      false, NULL,                     "", goCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }
    /** \brief Teleport the GM to the specified creature
    *
    * .gocreature <GUID>      --> TP using creature.guid
    * .gocreature azuregos    --> TP player to the mob with this name
    *                             Warning: If there is more than one mob with this name
    *                                      you will be teleported to the first one that is found.
    * .gocreature id 6109     --> TP player to the mob, that has this creature_template.entry
    *                             Warning: If there is more than one mob with this "id"
    *                                      you will be teleported to the first one that is found.
    */
    //teleport to creature
    static bool HandleGoCreatureCommand(ChatHandler* handler, const char* args)
    {
        if(!*args)
            return false;
        Player* _player = handler->GetSession()->GetPlayer();

        // "id" or number or [name] Shift-click form |color|Hcreature_entry:creature_id|h[name]|h|r
        char* pParam1 = handler->extractKeyFromLink((char*)args, "Hcreature");
        if(!pParam1)
            return false;

        std::ostringstream whereClause;

        // User wants to teleport to the NPC's template entry
        if(strcmp(pParam1, "id") == 0)
        {
            //sLog->outError("DEBUG: ID found");

            // Get the "creature_template.entry"
            // number or [name] Shift-click form |color|Hcreature_entry:creature_id|h[name]|h|r
            char* tail = strtok(NULL, "");
            if(!tail)
                return false;
            char* cId = handler->extractKeyFromLink(tail, "Hcreature_entry");
            if(!cId)
                return false;

            int32 tEntry = atoi(cId);
            //sLog->outError("DEBUG: ID value: %d", tEntry);
            if(!tEntry)
                return false;

            whereClause << "WHERE id = '" << tEntry << "'";
        }
        else
        {
            //sLog->outError("DEBUG: ID *not found*");

            int32 guid = atoi(pParam1);

            // Number is invalid - maybe the user specified the mob's name
            if(!guid)
            {
                std::string name = pParam1;
                WorldDatabase.EscapeString(name);
                whereClause << ", creature_template WHERE creature.id = creature_template.entry AND creature_template.name " _LIKE_" '" << name << "'";
            }
            else
            {
                whereClause <<  "WHERE `guid` = '" << guid << "'";
            }
        }
        //sLog->outError("DEBUG: %s", whereClause.c_str());

        QueryResult result = WorldDatabase.PQuery("SELECT position_x, position_y, position_z, orientation, map, guid, id FROM creature %s", whereClause.str().c_str());
        if(!result)
        {
            handler->SendSysMessage(LANG_COMMAND_GOCREATNOTFOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }
        if(result->GetRowCount() > 1)
            handler->SendSysMessage(LANG_COMMAND_GOCREATMULTIPLE);

        Field* fields = result->Fetch();
        float x = fields[0].GetFloat();
        float y = fields[1].GetFloat();
        float z = fields[2].GetFloat();
        float ort = fields[3].GetFloat();
        int mapid = fields[4].GetUInt16();
        uint32 guid = fields[5].GetUInt32();
        uint32 id = fields[6].GetUInt32();

        // if creature is in same map with caster go at its current location
        if(Creature* creature = sObjectAccessor->GetCreature(*_player, MAKE_NEW_GUID(guid, id, HIGHGUID_UNIT)))
        {
            x = creature->GetPositionX();
            y = creature->GetPositionY();
            z = creature->GetPositionZ();
            ort = creature->GetOrientation();
        }

        if(!MapManager::IsValidMapCoord(mapid, x, y, z, ort))
        {
            handler->PSendSysMessage(LANG_INVALID_TARGET_COORD, x, y, mapid);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if(_player->isInFlight())
        {
            _player->GetMotionMaster()->MovementExpired();
            _player->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            _player->SaveRecallPosition();

        _player->TeleportTo(mapid, x, y, z, ort);
        return true;
    }
    static bool HandleGoGraveyardCommand(ChatHandler* handler, const char* args)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if(!*args)
            return false;

        char *gyId = strtok((char*)args, " ");
        if(!gyId)
            return false;

        int32 i_gyId = atoi(gyId);

        if(!i_gyId)
            return false;

        WorldSafeLocsEntry const* gy = sWorldSafeLocsStore.LookupEntry(i_gyId);
        if(!gy)
        {
            handler->PSendSysMessage(LANG_COMMAND_GRAVEYARDNOEXIST, i_gyId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if(!MapManager::IsValidMapCoord(gy->map_id, gy->x, gy->y, gy->z))
        {
            handler->PSendSysMessage(LANG_INVALID_TARGET_COORD, gy->x, gy->y, gy->map_id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if(_player->isInFlight())
        {
            _player->GetMotionMaster()->MovementExpired();
            _player->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            _player->SaveRecallPosition();

        _player->TeleportTo(gy->map_id, gy->x, gy->y, gy->z, _player->GetOrientation());
        return true;
    }
    //teleport to grid
    static bool HandleGoGridCommand(ChatHandler* handler, const char* args)
    {
        if(!*args)    return false;
        Player* _player = handler->GetSession()->GetPlayer();

        char* px = strtok((char*)args, " ");
        char* py = strtok(NULL, " ");
        char* pmapid = strtok(NULL, " ");

        if(!px || !py)
            return false;

        float grid_x = (float)atof(px);
        float grid_y = (float)atof(py);
        uint32 mapid;
        if(pmapid)
            mapid = (uint32)atoi(pmapid);
        else mapid = _player->GetMapId();

        // center of grid
        float x = (grid_x-CENTER_GRID_ID+0.5f)*SIZE_OF_GRIDS;
        float y = (grid_y-CENTER_GRID_ID+0.5f)*SIZE_OF_GRIDS;

        if(!MapManager::IsValidMapCoord(mapid, x, y))
        {
            handler->PSendSysMessage(LANG_INVALID_TARGET_COORD, x, y, mapid);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if(_player->isInFlight())
        {
            _player->GetMotionMaster()->MovementExpired();
            _player->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            _player->SaveRecallPosition();

        Map const *map = sMapMgr->CreateBaseMap(mapid);
        float z = std::max(map->GetHeight(x, y, MAX_HEIGHT), map->GetWaterLevel(x, y));
        _player->TeleportTo(mapid, x, y, z, _player->GetOrientation());

        return true;
    }
    //teleport to gameobject
    static bool HandleGoObjectCommand(ChatHandler* handler, const char* args)
    {
        if(!*args)
            return false;

        Player* _player = handler->GetSession()->GetPlayer();

        // number or [name] Shift-click form |color|Hgameobject:go_guid|h[name]|h|r
        char* cId = handler->extractKeyFromLink((char*)args, "Hgameobject");
        if(!cId)
            return false;

        int32 guid = atoi(cId);
        if(!guid)
            return false;

        float x, y, z, ort;
        int mapid;

        // by DB guid
        if(GameObjectData const* go_data = sObjectMgr->GetGOData(guid))
        {
            x = go_data->posX;
            y = go_data->posY;
            z = go_data->posZ;
            ort = go_data->orientation;
            mapid = go_data->mapid;
        }
        else
        {
            handler->SendSysMessage(LANG_COMMAND_GOOBJNOTFOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if(!MapManager::IsValidMapCoord(mapid, x, y, z, ort))
        {
            handler->PSendSysMessage(LANG_INVALID_TARGET_COORD, x, y, mapid);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if(_player->isInFlight())
        {
            _player->GetMotionMaster()->MovementExpired();
            _player->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            _player->SaveRecallPosition();

        _player->TeleportTo(mapid, x, y, z, ort);
        return true;
    }
    static bool HandleGoTaxinodeCommand(ChatHandler* handler, const char* args)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if(!*args)
            return false;

        char* cNodeId = handler->extractKeyFromLink((char*)args, "Htaxinode");
        if(!cNodeId)
            return false;

        int32 i_nodeId = atoi(cNodeId);
        if(!i_nodeId)
            return false;

        TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(i_nodeId);
        if(!node)
        {
            handler->PSendSysMessage(LANG_COMMAND_GOTAXINODENOTFOUND, i_nodeId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if((node->x == 0.0f && node->y == 0.0f && node->z == 0.0f) ||
            !MapManager::IsValidMapCoord(node->map_id, node->x, node->y, node->z))
        {
            handler->PSendSysMessage(LANG_INVALID_TARGET_COORD, node->x, node->y, node->map_id);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if(_player->isInFlight())
        {
            _player->GetMotionMaster()->MovementExpired();
            _player->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            _player->SaveRecallPosition();

        _player->TeleportTo(node->map_id, node->x, node->y, node->z, _player->GetOrientation());
        return true;
    }
    static bool HandleGoTriggerCommand(ChatHandler* handler, const char* args)
    {
        Player* _player = handler->GetSession()->GetPlayer();

        if(!*args)
            return false;

        char *atId = strtok((char*)args, " ");
        if(!atId)
            return false;

        int32 i_atId = atoi(atId);

        if(!i_atId)
            return false;

        AreaTriggerEntry const* at = sAreaTriggerStore.LookupEntry(i_atId);
        if(!at)
        {
            handler->PSendSysMessage(LANG_COMMAND_GOAREATRNOTFOUND, i_atId);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if(!MapManager::IsValidMapCoord(at->mapid, at->x, at->y, at->z))
        {
            handler->PSendSysMessage(LANG_INVALID_TARGET_COORD, at->x, at->y, at->mapid);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if(_player->isInFlight())
        {
            _player->GetMotionMaster()->MovementExpired();
            _player->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            _player->SaveRecallPosition();

        _player->TeleportTo(at->mapid, at->x, at->y, at->z, _player->GetOrientation());
        return true;
    }

    //teleport at coordinates, including Z
    static bool HandleGoXYZCommand(ChatHandler* handler, const char* args)
    {
        if(!*args)
            return false;

        Player* _player = handler->GetSession()->GetPlayer();

        char* px = strtok((char*)args, " ");
        char* py = strtok(NULL, " ");
        char* pz = strtok(NULL, " ");
        char* pmapid = strtok(NULL, " ");

        if(!px || !py || !pz)
            return false;

        float x = (float)atof(px);
        float y = (float)atof(py);
        float z = (float)atof(pz);
        uint32 mapid;
        if(pmapid)
            mapid = (uint32)atoi(pmapid);
        else
            mapid = _player->GetMapId();

        if(!MapManager::IsValidMapCoord(mapid, x, y, z))
        {
            handler->PSendSysMessage(LANG_INVALID_TARGET_COORD, x, y, mapid);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // stop flight if need
        if(_player->isInFlight())
        {
            _player->GetMotionMaster()->MovementExpired();
            _player->CleanupAfterTaxiFlight();
        }
        // save only in non-flight case
        else
            _player->SaveRecallPosition();

        _player->TeleportTo(mapid, x, y, z, _player->GetOrientation());

        return true;
    }
    static bool HandleGoTicketCommand(ChatHandler* handler, const char* args)
    {
        if(!*args)
            return false;

        char *sTicketId = strtok((char*)args, " ");
        if(!sTicketId)
            return false;

        uint32 ticketId = atoi(sTicketId);
        if(!ticketId)
            return false;

        GmTicket* ticket = sTicketMgr->GetTicket(ticketId);
        if(!ticket)
        {
            handler->SendSysMessage(LANG_COMMAND_TICKETNOTEXIST);
            return true;
        }

        Player* _player = handler->GetSession()->GetPlayer();
        if(_player->isInFlight())
        {
            _player->GetMotionMaster()->MovementExpired();
            _player->CleanupAfterTaxiFlight();
        }
        else
            _player->SaveRecallPosition();

        ticket->TeleportTo(_player);
        return true;
    }
};

void AddSC_go_commandscript()
{
    new go_commandscript;
}
