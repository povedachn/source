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

#ifndef TRINITYCORE_CHAT_H
#define TRINITYCORE_CHAT_H

#define MSG_COLOR_LIGHTRED     "|cffff6060"
#define MSG_COLOR_LIGHTBLUE    "|cff00ccff"
#define MSG_COLOR_ANN_GREEN    "|c1f40af20"
#define MSG_COLOR_RED          "|cffff0000"
#define MSG_COLOR_GOLD         "|cffffcc00"
#define MSG_COLOR_SUBWHITE     "|cffbbbbbb"
#define MSG_COLOR_MAGENTA      "|cffff00ff"
#define MSG_COLOR_YELLOW       "|cffffff00"
#define MSG_COLOR_CYAN         "|cff00ffff"
#define MSG_COLOR_DARKBLUE     "|cff0000ff"

#define MSG_COLOR_GREY         "|cff9d9d9d"
#define MSG_COLOR_WHITE        "|cffffffff"
#define MSG_COLOR_GREEN        "|cff1eff00"
#define MSG_COLOR_BLUE         "|cff0080ff"
#define MSG_COLOR_PURPLE       "|cffb048f8"
#define MSG_COLOR_ORANGE       "|cffff8000"

#define MSG_COLOR_DRUID        "|cffff7d0a"
#define MSG_COLOR_HUNTER       "|cffabd473"
#define MSG_COLOR_MAGE         "|cff69ccf0"
#define MSG_COLOR_PALADIN      "|cfff58cba"
#define MSG_COLOR_PRIEST       "|cffffffff"
#define MSG_COLOR_ROGUE        "|cfffff569"
#define MSG_COLOR_SHAMAN       "|cff0070de"
#define MSG_COLOR_WARLOCK      "|cff9482c9"
#define MSG_COLOR_WARRIOR      "|cffc79c6e"
#define MSG_COLOR_DEATH_KNIGHT "|cffc41f3b"
#define MSG_COLOR_MONK         "|cff00ff96"

#include "SharedDefines.h"
#include "Player.h"

#include <vector>

class ChatHandler;
class WorldSession;
class WorldObject;
class Creature;
class Player;
class Unit;
struct GameTele;

class ChatCommand
{
    public:
        const char *       Name;
        uint32             SecurityLevel;                   // function pointer required correct align (use uint32)
        bool               AllowConsole;
        bool (*Handler)(ChatHandler*, const char* args);
        std::string        Help;
        ChatCommand *      ChildCommands;
};

class ChatHandler
{
    public:
        WorldSession* GetSession() { return m_session; }
        explicit ChatHandler(WorldSession* session) : m_session(session) { }
        explicit ChatHandler(Player* player) : m_session(player->GetSession()) { }
        virtual ~ChatHandler() { }

        static void FillMessageData(WorldPacket *data, WorldSession* session, uint8 type, uint32 language, const char *channelName, uint64 target_guid, const char *message, Unit* speaker);

        void FillMessageData(WorldPacket *data, uint8 type, uint32 language, uint64 target_guid, const char* message)
        {
            FillMessageData(data, m_session, type, language, NULL, target_guid, message, NULL);
        }

        void FillSystemMessageData(WorldPacket *data, const char* message)
        {
            FillMessageData(data, CHAT_MSG_SYSTEM, LANG_UNIVERSAL, 0, message);
        }

        static char* LineFromMessage(char*& pos) { char* start = strtok(pos, "\n"); pos = NULL; return start; }

        // function with different implementation for chat/console
        virtual const char *GetTrinityString(int32 entry) const;
        virtual void SendSysMessage(const char *str);

        void SendSysMessage(int32     entry);
        void PSendSysMessage(const char *format, ...) ATTR_PRINTF(2, 3);
        void PSendSysMessage(int32     entry, ...);
        std::string PGetParseString(int32 entry, ...) const;

        int ParseCommands(const char* text);

        static ChatCommand* getCommandTable();

        bool isValidChatMessage(const char* msg);
        void SendGlobalSysMessage(const char *str);

        bool hasStringAbbr(const char* name, const char* part);

        // function with different implementation for chat/console
        virtual bool isAvailable(ChatCommand const& cmd) const;
        virtual std::string GetNameLink() const { return GetNameLink(m_session->GetPlayer()); }
        virtual bool needReportToTarget(Player* chr) const;
        virtual LocaleConstant GetSessionDbcLocale() const;
        virtual int GetSessionDbLocaleIndex() const;

        bool HasLowerSecurity(Player* target, uint64 guid, bool strong = false);
        bool HasLowerSecurityAccount(WorldSession* target, uint32 account, bool strong = false);

        void SendGlobalGMSysMessage(const char *str);
        Player* getSelectedPlayer();
        Creature* getSelectedCreature();
        Unit* getSelectedUnit();
        WorldObject* getSelectedObject();

        char*     extractKeyFromLink(char* text, char const* linkType, char** something1 = NULL);
        char*     extractKeyFromLink(char* text, char const* const* linkTypes, int* found_idx, char** something1 = NULL);

        // if args have single value then it return in arg2 and arg1 == NULL
        void      extractOptFirstArg(char* args, char** arg1, char** arg2);
        char*     extractQuotedArg(char* args);

        uint32    extractSpellIdFromLink(char* text);
        uint64    extractGuidFromLink(char* text);
        GameTele const* extractGameTeleFromLink(char* text);
        bool GetPlayerGroupAndGUIDByName(const char* cname, Player* &plr, Group* &group, uint64 &guid, bool offline = false);
        std::string extractPlayerNameFromLink(char* text);
        // select by arg (name/link) or in-game selection online/offline player
        bool extractPlayerTarget(char* args, Player** player, uint64* player_guid = NULL, std::string* player_name = NULL);

        std::string playerLink(std::string const& name) const { return m_session ? "|cffffffff|Hplayer:"+name+"|h["+name+"]|h|r" : name; }
        std::string GetNameLink(Player* chr) const { return playerLink(chr->GetName()); }

        GameObject* GetNearbyGameObject();
        GameObject* GetObjectGlobalyWithGuidOrNearWithDbGuid(uint32 lowguid, uint32 entry);
        bool HasSentErrorMessage() { return sentErrorMessage;}
        void SetSentErrorMessage(bool val){ sentErrorMessage = val;};
        static bool LoadCommandTable() { return load_command_table;}
        static void SetLoadCommandTable(bool val){ load_command_table = val;};

    protected:
        explicit ChatHandler() : m_session(NULL) { }      // for CLI subclass
        static bool SetDataForCommandInTable(ChatCommand *table, const char* text, uint32 security, std::string const& help, std::string const& fullcommand);
        bool ExecuteCommandInTable(ChatCommand *table, const char* text, const std::string& fullcmd);
        bool ShowHelpForCommand(ChatCommand *table, const char* cmd);
        bool ShowHelpForSubCommands(ChatCommand *table, char const* cmd, char const* subcmd);

        bool HandleNameAnnounceCommand(const char* args);
        bool HandleGMNameAnnounceCommand(const char* args);
        bool HandleGMAnnounceCommand(const char* args);
        bool HandleGMNotifyCommand(const char* args);

        bool HandleBanAccountCommand(const char* args);
        bool HandleBanAccountByCharCommand(const char* args);
        bool HandleBanCharacterCommand(const char* args);
        bool HandleBanIPCommand(const char* args);
        bool HandleBanInfoAccountCommand(const char* args);
        bool HandleBanInfoCharacterCommand(const char* args);
        bool HandleBanInfoIPCommand(const char* args);
        bool HandleBanListAccountCommand(const char* args);
        bool HandleBanListCharacterCommand(const char* args);
        bool HandleBanListIPCommand(const char* args);

        bool HandleCastCommand(const char *args);
        bool HandleCastBackCommand(const char *args);
        bool HandleCastDistCommand(const char *args);
        bool HandleCastSelfCommand(const char *args);
        bool HandleCastTargetCommand(const char *args);

        bool HandleCharacterCustomizeCommand(const char * args);
        bool HandleCharacterChangeFactionCommand(const char * args);
        bool HandleCharacterChangeRaceCommand(const char * args);
        bool HandleCharacterDeletedDeleteCommand(const char* args);
        bool HandleCharacterDeletedListCommand(const char* args);
        bool HandleCharacterDeletedRestoreCommand(const char* args);
        bool HandleCharacterDeletedOldCommand(const char* args);
        bool HandleCharacterEraseCommand(const char* args);
        bool HandleCharacterLevelCommand(const char* args);
        bool HandleCharacterRenameCommand(const char * args);
        bool HandleCharacterReputationCommand(const char* args);
        bool HandleCharacterTitlesCommand(const char* args);

        bool HandleMmapPathCommand(const char* args);
        bool HandleMmapLocCommand(const char* args);
        bool HandleMmapLoadedTilesCommand(const char* args);
        bool HandleMmapStatsCommand(const char* args);
        bool HandleMmapTestAreaCommand(const char* args);

        bool HandleChannelSetOwnership(const char *args);

        bool HandlePossessCommand(const char* args);
        bool HandleUnPossessCommand(const char* args);
        bool HandleBindSightCommand(const char* args);
        bool HandleUnbindSightCommand(const char* args);

        bool HandleGuildCreateCommand(const char* args);
        bool HandleGuildInviteCommand(const char* args);
        bool HandleGuildUninviteCommand(const char* args);
        bool HandleGuildRankCommand(const char* args);
        bool HandleGuildDeleteCommand(const char* args);

        bool HandleInstanceListBindsCommand(const char* args);
        bool HandleInstanceUnbindCommand(const char* args);
        bool HandleInstanceStatsCommand(const char* args);
        bool HandleInstanceSaveDataCommand(const char * args);

        bool HandleListAurasCommand(const char * args);
        bool HandleListCreatureCommand(const char* args);
        bool HandleListItemCommand(const char* args);
        bool HandleListObjectCommand(const char* args);

        bool HandleLookupAreaCommand(const char* args);
        bool HandleLookupCreatureCommand(const char* args);
        bool HandleLookupEventCommand(const char* args);
        bool HandleLookupFactionCommand(const char * args);
        bool HandleLookupItemCommand(const char * args);
        bool HandleLookupItemSetCommand(const char * args);
        bool HandleLookupObjectCommand(const char* args);
        bool HandleLookupPlayerIpCommand(const char* args);
        bool HandleLookupPlayerAccountCommand(const char* args);
        bool HandleLookupPlayerEmailCommand(const char* args);
        bool HandleLookupQuestCommand(const char* args);
        bool HandleLookupSkillCommand(const char* args);
        bool HandleLookupSpellCommand(const char* args);
        bool HandleLookupTaxiNodeCommand(const char * args);
        bool HandleLookupTeleCommand(const char * args);
        bool HandleLookupTitleCommand(const char * args);

        bool HandlePDumpLoadCommand(const char *args);
        bool HandlePDumpWriteCommand(const char *args);

        bool HandleResetAchievementsCommand(const char * args);
        bool HandleResetAllCommand(const char * args);
        bool HandleResetHonorCommand(const char * args);
        bool HandleResetLevelCommand(const char * args);
        bool HandleResetSpellsCommand(const char * args);
        bool HandleResetStatsCommand(const char * args);
        bool HandleResetTalentsCommand(const char * args);

        bool HandleSendItemsCommand(const char* args);
        bool HandleSendMailCommand(const char* args);
        bool HandleSendMessageCommand(const char * args);
        bool HandleSendMoneyCommand(const char* args);

        bool HandleServerExitCommand(const char* args);
        bool HandleServerIdleRestartCommand(const char* args);
        bool HandleServerIdleShutDownCommand(const char* args);
        bool HandleServerInfoCommand(const char* args);
        bool HandleServerMotdCommand(const char* args);
        bool HandleServerPLimitCommand(const char* args);
        bool HandleServerRestartCommand(const char* args);
        bool HandleServerShutDownCommand(const char* args);
        bool HandleServerShutDownCancelCommand(const char* args);
        bool HandleServerSetClosedCommand(const char* args);
        bool HandleServerSetLogLevelCommand(const char* args);
        bool HandleServerSetMotdCommand(const char* args);
        bool HandleServerToggleQueryLogging(const char* args);

        bool HandleServerSetLogFileLevelCommand(const char* args);
        bool HandleServerSetDiffTimeCommand(const char* args);

        bool HandleUnBanAccountCommand(const char* args);
        bool HandleUnBanAccountByCharCommand(const char* args);
        bool HandleUnBanCharacterCommand(const char* args);
        bool HandleUnBanIPCommand(const char* args);

        bool HandleHelpCommand(const char* args);
        bool HandleCommandsCommand(const char* args);
        bool HandleStartCommand(const char* args);
        bool HandleDismountCommand(const char* args);
        bool HandleSaveCommand(const char* args);

        bool HandleSummonCommand(const char* args);
        bool HandleAppearCommand(const char* args);
        bool HandleGroupSummonCommand(const char* args);
        bool HandleRecallCommand(const char* args);
        bool HandleAnnounceCommand(const char* args);
        bool HandleNotifyCommand(const char* args);
        bool HandleTaxiCheatCommand(const char* args);
        bool HandleWhispersCommand(const char* args);

        bool HandleGUIDCommand(const char* args);
        bool HandleItemMoveCommand(const char* args);
        bool HandleDeMorphCommand(const char* args);
        bool HandlePInfoCommand(const char* args);
        bool HandleMuteCommand(const char* args);
        bool HandleUnmuteCommand(const char* args);
        bool HandleMovegensCommand(const char* args);
        bool HandleFreezeCommand(const char *args);
        bool HandleUnFreezeCommand(const char *args);
        bool HandleListFreezeCommand(const char* args);

        bool HandleCooldownCommand(const char* args);
        bool HandleUnLearnCommand(const char* args);
        bool HandleGetDistanceCommand(const char* args);
        bool HandleDieCommand(const char* args);
        bool HandleDamageCommand(const char *args);
        bool HandleReviveCommand(const char* args);
        bool HandleAuraCommand(const char* args);
        bool HandleUnAuraCommand(const char* args);
        bool HandleLinkGraveCommand(const char* args);
        bool HandleNearGraveCommand(const char* args);
        bool HandleActivateObjectCommand(const char* args);
        bool HandleSpawnTransportCommand(const char* args);
        bool HandleExploreCheatCommand(const char* args);
        bool HandleHoverCommand(const char* args);
        bool HandleWaterwalkCommand(const char* args);
        bool HandleLevelUpCommand(const char* args);
        bool HandleShowAreaCommand(const char* args);
        bool HandleHideAreaCommand(const char* args);
        bool HandleAddItemCommand(const char* args);
        bool HandleAddItemSetCommand(const char* args);
        bool HandlePetUnlearnCommand(const char* args);
        bool HandlePetLearnCommand(const char* args);
        bool HandleCreatePetCommand(const char* args);

        bool HandleGroupLeaderCommand(const char* args);
        bool HandleGroupDisbandCommand(const char* args);
        bool HandleGroupRemoveCommand(const char* args);

        bool HandleBankCommand(const char* args);
        bool HandleKickPlayerCommand(const char * args);

        // GM ticket command handlers
        bool HandleGMTicketListCommand(const char* args);
        bool HandleGMTicketListOnlineCommand(const char* args);
        bool HandleGMTicketListClosedCommand(const char* args);
        bool HandleGMTicketListEscalatedCommand(const char* args);
        bool HandleGMTicketGetByIdCommand(const char* args);
        bool HandleGMTicketGetByNameCommand(const char* args);
        bool HandleGMTicketCloseByIdCommand(const char* args);
        bool HandleGMTicketAssignToCommand(const char* args);
        bool HandleGMTicketUnAssignCommand(const char* args);
        bool HandleGMTicketCommentCommand(const char* args);
        bool HandleGMTicketDeleteByIdCommand(const char* args);
        bool HandleGMTicketReloadCommand(const char*);
        bool HandleToggleGMTicketSystem(const char* args);
        bool HandleGMTicketEscalateCommand(const char* args);
        bool HandleGMTicketCompleteCommand(const char* args);
        bool HandleGMTicketResponseAppendCommand(const char* args);
        bool HandleGMTicketResponseAppendLnCommand(const char* args);

        bool HandleMaxSkillCommand(const char* args);
        bool HandleSetSkillCommand(const char* args);
        bool HandleRespawnCommand(const char* args);
        bool HandleComeToMeCommand(const char *args);
        bool HandleCombatStopCommand(const char *args);

        /*bool HandleCharDeleteCommand(const char *args);
        bool HandleSendMessageCommand(const char * args);*/

        bool HandleFlushArenaPointsCommand(const char *args);
        bool HandlePlayAllCommand(const char* args);
        bool HandleRepairitemsCommand(const char* args);

        bool HandleTempGameObjectCommand(const char* args);

        //! Development Commands
        bool HandleSaveAllCommand(const char* args);

		// Usar con mucha precaucion, es muy inestable
        bool HandleReloadGameobjectTemplateCommand(const char* args);
        bool HandleReloadItemTemplateCommand(const char* args);
        bool HandleReloadCreatureTemplateCommand(const char* args);
		bool HandleReloadBattlegroundTemplateCommand(const char* args);
		
		bool HandleBGStartCommand(const char*);
		
        // Utility methods for commands
        bool LookupPlayerSearchCommand(QueryResult result, int32 limit);
        bool HandleBanListHelper(QueryResult result);
        bool HandleBanHelper(BanMode mode, char const* args);
        bool HandleBanInfoHelper(uint32 accountid, char const* accountname);
        bool HandleUnBanHelper(BanMode mode, char const* args);
        void HandleCharacterLevel(Player* player, uint64 player_guid, uint32 oldlevel, uint32 newlevel);
        void HandleLearnSkillRecipesHelper(Player* player, uint32 skill_id);

        //Wintergrasp
        bool HandleWintergraspStatusCommand(const char *args);
        bool HandleWintergraspStartCommand(const char *args);
        bool HandleWintergraspStopCommand(const char *args);
        bool HandleWintergraspSwitchTeamCommand(const char *args);
        bool HandleWintergraspTimerCommand(const char *args);

        // Stores informations about a deleted character
        struct DeletedInfo
        {
            uint32      lowguid;                            ///< the low GUID from the character
            std::string name;                               ///< the character name
            uint32      accountId;                          ///< the account id
            std::string accountName;                        ///< the account name
            time_t      deleteDate;                         ///< the date at which the character has been deleted
        };

        typedef std::list<DeletedInfo> DeletedInfoList;
        bool GetDeletedCharacterInfoList(DeletedInfoList& foundList, std::string searchString = "");
        std::string GenerateDeletedCharacterGUIDsWhereStr(DeletedInfoList::const_iterator& itr, DeletedInfoList::const_iterator const& itr_end);
        void HandleCharacterDeletedListHelper(DeletedInfoList const& foundList);
        void HandleCharacterDeletedRestoreHelper(DeletedInfo const& delInfo);

    private:
        bool _HandleGMTicketResponseAppendCommand(const char* args, bool newLine);

        WorldSession* m_session;                           // != NULL for chat command call and NULL for CLI command

        // common global flag
        static bool load_command_table;
        bool sentErrorMessage;
};

class CliHandler : public ChatHandler
{
    public:
        typedef void Print(void*, char const*);
        explicit CliHandler(void* callbackArg, Print* zprint) : m_callbackArg(callbackArg), m_print(zprint) { }

        // overwrite functions
        const char *GetTrinityString(int32 entry) const;
        bool isAvailable(ChatCommand const& cmd) const;
        void SendSysMessage(const char *str);
        std::string GetNameLink() const;
        bool needReportToTarget(Player* chr) const;
        LocaleConstant GetSessionDbcLocale() const;
        int GetSessionDbLocaleIndex() const;

    private:
        void* m_callbackArg;
        Print* m_print;
};

#endif
