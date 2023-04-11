#include "ScriptPCH.h"
#include "Spell.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "WorldPacket.h"
#include "CreatureTextMgr.h"
#include "Config.h"
#include "ArenaTeam.h"
#include "ArenaTeamMgr.h"
#include "Player.h"

#define GTS session->GetTrinityString

enum enus
{
    ARENA_MAX_RESULTS = 10,
    ARENA_2V2_LADDER = 815 + 1,
    ARENA_GOODBYE = 820 + 4,
    ARENA_START_TEAM_LOOKUP = 8600 + 5,
};

const uint8  GamesOnPage    = 15;

const int MAX_PLAYER_SPAWN_POINTS = 20;
static const Position playerSpawnPoint[MAX_PLAYER_SPAWN_POINTS] =
{
	{-1172.90f, -5331.40f, 4.85f, 4.64f},
	{-1319.34f, -5385.09f, 3.34f, 5.88f},
	{-1135.17f, -5587.93f, 10.5f, 1.53f},
	{-1235.21f, -5592.60f, 8.49f, 1.39f},
	{-1056.33f, -5639.83f, 5.49f, 1.74f},
	{-1031.71f, -5530.50f, 4.75f, 3.14f},
	{-989.878f, -5418.93f, 3.90f, 3.26f},
	{-1077.64f, -5347.45f, 3.93f, 4.18f},
	{-1136.49f, -5420.34f, 11.4f, 3.23f},
	{-1235.88f, -5462.10f, 5.76f, 1.29f},
	{-1162.98f, -5585.53f, 12.3f, 2.29f},
	{-840.228f, -5616.32f, 2.97f, 6.28f},
	{-719.558f, -5685.03f, 23.5f, 1.78f},
	{-638.830f, -5622.10f, 6.42f, 2.90f},
	{-662.432f, -5509.45f, 4.08f, 3.95f},
	{-772.277f, -5353.24f, 2.03f, 2.83f},
	{-949.171f, -5179.04f, 2.11f, 3.98f},
	{-1174.97f, -5116.36f, 3.65f, 6.14f},
	{-1372.61f, -5169.68f, 1.81f, 0.45f},
	{-1602.78f, -5250.65f, 6.50f, 5.05f},
};

//Evento Laberinto (id de game_event = 89)
const int MAX_PLAYER_SPAWN_POINTS_EVENTO_LABERINTO = 8;
static const Position playerSpawnPointEventoLaberinto[MAX_PLAYER_SPAWN_POINTS_EVENTO_LABERINTO] =
{
	{-7364.641113f, 1105.026978f, 131.407379f, 1.551686f},
	{-7403.822754f, 1041.762939f, 131.408051f, 0.037827f},
	{-7441.120605f, 1101.512695f, 131.407288f, 4.742365f},
	{-7494.867676f, 1116.905029f, 131.406952f, 4.716446f},
	{-7508.709473f, 1028.469971f, 131.407089f, 6.271536f},
	{-7476.597656f, 851.968018f, 131.407394f, 1.533622f},
	{-7422.970703f, 877.438223f, 131.407532f, 3.188865f},
	{-7447.919434f, 1048.742310f, 153.471115f, 0.000129f},
};

void aprender_habilidad_clase(Player* player);

uint32 GetGuidByDb_item(const char* name);

uint32 GetTeamByDb_item(uint64 GUID, uint32 type);

uint32 GetArenaTeamId_item(uint64 Guid, uint32 type);

std::string GetTypeAsString_item(uint32 type);

std::string getPlayerStatus_item(uint32 guid);

std::string getWinPercent_item(uint32 wins, uint32 losses);

std::string raceToString_item(uint8 race);

std::string classToString_item(uint8 Class);

void SendTeamInfo(Player* player, Item* item, std::string code);

void SendSoloInfo(Player* player, Item* item, uint32 teamId, uint64 target, uint32 type);

void SendOptions(Player* player, Item* item, const char* code);

uint32 Jugadores_item();

bool PlayerAlreadyHasTwoProfessions(const Player *player);

bool LearnAllRecipesInProfession(Player *player, SkillType skill);

void LearnSkillRecipesHelper(Player *player, uint32 skill_id);

bool IsSecondarySkill(SkillType skill);

void CompleteLearnProfession(Player* player, Item* item, SkillType skill);

bool isEquiped(Player* player, uint8 slot);

bool validItem(uint8 type, Item* item);

void Enchant(Player* player, Item* item, uint8 slot, uint32 enchant, uint8 type = 0);

std::string GetClassNameById(uint8 id);

std::string GetGamesStringData(Battleground *arena, uint16 mmr);

uint64 GetFirstPlayerGuid(Battleground *arena);

void ShowPage(Player *player, uint16 page, bool isTop);

void CreatePet(Player *player, Item * item, uint32 entry);

uint32 optionToTeamType(uint32 option);

uint32 teamTypeToOption(uint32 teamType);
