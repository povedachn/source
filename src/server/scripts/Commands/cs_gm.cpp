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
#include "Chat.h"

class gm_commandscript : public CommandScript
{
public:
    gm_commandscript() : CommandScript("gm_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand gmCommandTable[] =
        {
            { "chat",           SEC_MODERATOR,      false, &HandleGMChatCommand,              "", NULL },
            { "fly",            SEC_ADMINISTRATOR,  false, &HandleGMFlyCommand,               "", NULL },
            { "ingame",         SEC_PLAYER,         true,  &HandleGMListIngameCommand,        "", NULL },
            { "list",           SEC_ADMINISTRATOR,  true,  &HandleGMListFullCommand,          "", NULL },
            { "visible",        SEC_MODERATOR,      false, &HandleGMVisibleCommand,           "", NULL },
            { "",               SEC_MODERATOR,      false, &HandleGMCommand,                  "", NULL },
            { NULL,             0,                  false, NULL,                              "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "gm",             SEC_MODERATOR,      false, NULL,                     "", gmCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    // Enables or disables hiding of the staff badge
    static bool HandleGMChatCommand(ChatHandler* handler, const char* args)
    {
        if(!*args)
        {
            if(handler->GetSession()->GetPlayer()->isGMChat())
                handler->GetSession()->SendNotification(LANG_GM_CHAT_ON);
            else
                handler->GetSession()->SendNotification(LANG_GM_CHAT_OFF);
            return true;
        }

        std::string argstr = (char*)args;

        if(argstr == "on")
        {
            handler->GetSession()->GetPlayer()->SetGMChat(true);
            handler->GetSession()->SendNotification(LANG_GM_CHAT_ON);
            return true;
        }

        if(argstr == "off")
        {
            handler->GetSession()->GetPlayer()->SetGMChat(false);
            handler->GetSession()->SendNotification(LANG_GM_CHAT_OFF);
            return true;
        }

        handler->SendSysMessage(LANG_USE_BOL);
        handler->SetSentErrorMessage(true);
        return false;
    }

    static bool HandleGMFlyCommand(ChatHandler* handler, const char* args)
    {
        if(!*args)
            return false;

        Player* target =  handler->getSelectedPlayer();
        if(!target)
            target = handler->GetSession()->GetPlayer();

        WorldPacket data(12);
        if(strncmp(args, "on", 3) == 0)
            data.SetOpcode(SMSG_MOVE_SET_CAN_FLY);
        else if(strncmp(args, "off", 4) == 0)
            data.SetOpcode(SMSG_MOVE_UNSET_CAN_FLY);
        else
        {
            handler->SendSysMessage(LANG_USE_BOL);
            return false;
        }
        data.append(target->GetPackGUID());
        data << uint32(0);                                      // unknown
        target->SendMessageToSet(&data, true);
        handler->PSendSysMessage(LANG_COMMAND_FLYMODE_STATUS, handler->GetNameLink(target).c_str(), args);
        return true;
    }

    static bool HandleGMListIngameCommand(ChatHandler* handler, const char* /*args*/)
    {
        bool first = true;
        bool footer = false;

        ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, *HashMapHolder<Player>::GetLock(), true);
        HashMapHolder<Player>::MapType &m = sObjectAccessor->GetPlayers();
        for(HashMapHolder<Player>::MapType::const_iterator itr = m.begin(); itr != m.end(); ++itr)
        {
            AccountTypes itr_sec = itr->second->GetSession()->GetSecurity();
            if((itr->second->isGameMaster() || (itr_sec >= SEC_MODERATOR && itr_sec <= AccountTypes(sWorld->getIntConfig(CONFIG_GM_LEVEL_IN_GM_LIST)))) &&
                (!handler->GetSession() || itr->second->IsVisibleGloballyFor(handler->GetSession()->GetPlayer())))
            {
                if(first)
                {
                    first = false;
                    footer = true;
                    handler->SendSysMessage(LANG_GMS_ON_SRV);
                    handler->SendSysMessage("========================");
                }
                const char* name = itr->second->GetName();
                uint8 security = itr_sec;
				if (security == 0)
                    continue;
                uint8 max = ((16 - strlen(name)) / 2);
                uint8 max2 = max;
                if(((max)+(max2)+(strlen(name))) == 16)
                    max2 = ((max)-1);
                if(handler->GetSession())
                    handler->PSendSysMessage("|    %s Nivel de MJ %u", name, security);
                else
                    handler->PSendSysMessage("|%*s%s%*s|   %u  |", max, " ", name, max2, " ", security);
            }
        }
        if(footer)
            handler->SendSysMessage("========================");
        if(first)
            handler->SendSysMessage(LANG_GMS_NOT_LOGGED);
        return true;
    }

    /// Display the list of GMs
    static bool HandleGMListFullCommand(ChatHandler* handler, const char* /*args*/)
    {
        ///- Get the accounts with GM Level >0
        QueryResult result = LoginDatabase.Query("SELECT a.username, aa.gmlevel FROM account a, account_access aa WHERE a.id=aa.id AND aa.gmlevel > 0");
        if(result)
        {
            handler->SendSysMessage(LANG_GMLIST);
            handler->SendSysMessage("========================");
            ///- Cycle through them. Display username and GM level
            do
            {
                Field* fields = result->Fetch();
                const char* name = fields[0].GetCString();
                uint8 security = fields[1].GetUInt8();
                uint8 max = ((16 - strlen(name)) / 2);
                uint8 max2 = max;
                if(((max)+(max2)+(strlen(name))) == 16)
                    max2 = ((max)-1);
                if(handler->GetSession())
                    handler->PSendSysMessage("|    %s Nivel de MJ %u", name, security);
                else
                    handler->PSendSysMessage("|%*s%s%*s|   %u  |", max, " ", name, max2, " ", security);
            }
            while(result->NextRow());

            handler->SendSysMessage("========================");
        }
        else
            handler->PSendSysMessage(LANG_GMLIST_EMPTY);
        return true;
    }

    //Enable\Disable Invisible mode
    static bool HandleGMVisibleCommand(ChatHandler* handler, const char* args)
    {
        if(!*args)
        {
            handler->PSendSysMessage(LANG_YOU_ARE, handler->GetSession()->GetPlayer()->isGMVisible() ?  handler->GetTrinityString(LANG_VISIBLE) : handler->GetTrinityString(LANG_INVISIBLE));
            return true;
        }

        std::string argstr = (char*)args;
        Player* pPlayer = handler->GetSession()->GetPlayer();
        if(argstr == "on")
        {
            if(pPlayer->HasAura(37800, 0))
                pPlayer->RemoveAurasDueToSpell(37800);
            handler->GetSession()->GetPlayer()->SetGMVisible(true);
            handler->GetSession()->SendNotification(LANG_INVISIBLE_VISIBLE);
            return true;
        } else if(argstr == "off") {
            handler->GetSession()->GetPlayer()->SetGMVisible(false);
            handler->GetSession()->SendNotification(LANG_INVISIBLE_INVISIBLE);
            pPlayer->AddAura(37800, pPlayer);
            return true;
        }

        handler->SendSysMessage(LANG_USE_BOL);
        handler->SetSentErrorMessage(true);
        return false;
    }

    //Enable\Disable GM Mode
    static bool HandleGMCommand(ChatHandler* handler, const char* args)
    {
        if(!*args)
        {
            if(handler->GetSession()->GetPlayer()->isGameMaster())
                handler->GetSession()->SendNotification(LANG_GM_ON);
            else
                handler->GetSession()->SendNotification(LANG_GM_OFF);
            return true;
        }

        std::string argstr = (char*)args;

        if(argstr == "on")
        {
            handler->GetSession()->GetPlayer()->SetGameMaster(true);
            handler->GetSession()->SendNotification(LANG_GM_ON);
            handler->GetSession()->GetPlayer()->UpdateTriggerVisibility();
#ifdef _DEBUG_VMAPS
            VMAP::IVMapManager *vMapManager = VMAP::VMapFactory::createOrGetVMapManager();
            vMapManager->processCommand("stoplog");
#endif
            return true;
        }

        if(argstr == "off")
        {
            handler->GetSession()->GetPlayer()->SetGameMaster(false);
            handler->GetSession()->SendNotification(LANG_GM_OFF);
            handler->GetSession()->GetPlayer()->UpdateTriggerVisibility();
#ifdef _DEBUG_VMAPS
            VMAP::IVMapManager *vMapManager = VMAP::VMapFactory::createOrGetVMapManager();
            vMapManager->processCommand("startlog");
#endif
            return true;
        }

        handler->SendSysMessage(LANG_USE_BOL);
        handler->SetSentErrorMessage(true);
        return false;
    }
};

void AddSC_gm_commandscript()
{
    new gm_commandscript;
}
