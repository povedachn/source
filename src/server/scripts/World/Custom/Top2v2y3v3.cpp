#include "ScriptPCH.h"
using namespace std;
 
class npc_arena_teamTop : public CreatureScript
{
   public:
           npc_arena_teamTop() : CreatureScript("npc_arena_teamTop") { }
 
           bool OnGossipHello(Player * player, Creature * creature)
           {
                   player->ADD_GOSSIP_ITEM(9, "|cff00ff00|TInterface\\icons\\Achievement_arena_2v2_7:26|t|r Ver el Top de Arenas de 2vs2", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                   player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Olvidalo", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
                   player->SEND_GOSSIP_MENU(1, creature->GetGUID());
                   return true;
           }
 
           bool OnGossipSelect(Player * player, Creature * creature, uint32 sender, uint32 actions)
           {
                   if(sender == GOSSIP_SENDER_MAIN)
                   {
                           switch(actions)
                           {
                               case GOSSIP_ACTION_INFO_DEF+1:
                                   {
                                           QueryResult result = CharacterDatabase.Query("SELECT name,rating FROM arena_team WHERE type='2' ORDER BY rating DESC LIMIT 10");
                                           if(!result)
                                                   return false;
 
                                           Field * fields = NULL;
                                           player->MonsterWhisper("|cff4169E1Aqui esta el Top 10 de equipos de 2vs2:|r", player->GetGUID());
                                           do
                                           {
                                                   fields = result->Fetch();
                                                   string arena_name = fields[0].GetString();
                                                   uint32 rating = fields[1].GetUInt32();
                                                   char msg[250];
                                                   snprintf(msg, 250, "Equipo: |cffFFFF00%s|r, Rating: |cffFFFF00%u \n", arena_name.c_str(), rating);
                                                   player->MonsterWhisper(msg, player->GetGUID());
                                           }while(result->NextRow());
                                   }break;
 
                               case GOSSIP_ACTION_INFO_DEF+4:
                                           {
                                                   player->PlayerTalkClass->SendCloseGossip();
                                           }break;
                           }
                   }
 
                   return true;
           }
};
 
void AddSC_npc_arena_teamTop()
{
        new npc_arena_teamTop;
}