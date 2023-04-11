#include "ScriptPCH.h"
 
class skill_npc : public CreatureScript
 
        {
public:
 
skill_npc() : CreatureScript("skill_npc") {}
 
struct skill_npcAI : public ScriptedAI
        {
                skill_npcAI(Creature *c) : ScriptedAI(c)
                {
                }

               
        };
 
        CreatureAI* GetAI(Creature* _creature) const
    {
                return new skill_npcAI(_creature);
    }
 
        void CreatureWhisperBasedOnBool(const char *text, Creature *_creature, Player *pPlayer, bool value)
        {
                if (value)
                        _creature->MonsterWhisper(text, pPlayer->GetGUID());
        }
 
        uint32 PlayerMaxLevel() const
        {
                return sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);
        }
 
 
void MainMenu(Player *pPlayer, Creature* _creature)
{
    pPlayer->ADD_GOSSIP_ITEM(9, "[Profesiones Primarias] ->", GOSSIP_SENDER_MAIN, 196);
	pPlayer->ADD_GOSSIP_ITEM(9, "[Profesiones Secundarias] ->", GOSSIP_SENDER_MAIN, 197);
	pPlayer->ADD_GOSSIP_ITEM(2, "Olvidalo", GOSSIP_SENDER_MAIN, 2050);
    pPlayer->PlayerTalkClass->SendGossipMenu(907, _creature->GetGUID());
}
 
bool PlayerHasItemOrSpell(const Player *plr, uint32 itemId, uint32 spellId) const
{
    return plr->HasItemCount(itemId, 1, true) || plr->HasSpell(spellId);
}
 
bool OnGossipHello(Player* pPlayer, Creature* pCreature)
{
    MainMenu(pPlayer, pCreature);
    return true;
}
 
bool PlayerAlreadyHasTwoProfessions(const Player *pPlayer) const
        {
                uint32 skillCount = 0;
 
                if (pPlayer->HasSkill(SKILL_MINING))
                        skillCount++;
                if (pPlayer->HasSkill(SKILL_SKINNING))
                        skillCount++;
                if (pPlayer->HasSkill(SKILL_HERBALISM))
                        skillCount++;
 
                if (skillCount >= sWorld->getIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL))
                        return true;
 
                for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
                {
                        SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
                        if (!SkillInfo)
                                continue;
 
                        if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
                                continue;
 
                        if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
                                continue;
 
                        const uint32 skillID = SkillInfo->id;
                        if (pPlayer->HasSkill(skillID))
                                skillCount++;
 
                        if (skillCount >= sWorld->getIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL))
                                return true;
                }
 
                return false;
        }
 
        bool LearnAllRecipesInProfession(Player *pPlayer, SkillType skill)
        {
                ChatHandler handler(pPlayer);
        char* skill_name;
 
        SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
                skill_name = SkillInfo->name[handler.GetSessionDbcLocale()];
 
        if (!SkillInfo)
                {
						//TRINITY
                        //sLog->outError(LOG_FILTER_GENERAL,"Teleport NPC: received non-valid skill ID (LearnAllRecipesInProfession)");
						//MYTH
						sLog->outError("Teleport NPC: received non-valid skill ID (LearnAllRecipesInProfession)");
			
			return false;
                }
 
        LearnSkillRecipesHelper(pPlayer, SkillInfo->id);
 
        uint16 maxLevel = pPlayer->GetPureMaxSkillValue(SkillInfo->id);
        pPlayer->SetSkill(SkillInfo->id, pPlayer->GetSkillStep(SkillInfo->id), maxLevel, maxLevel);
        handler.PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, skill_name);
                
                return true;
        }
        
        void LearnSkillRecipesHelper(Player *player, uint32 skill_id)
        {
                uint32 classmask = player->getClassMask();
 
        for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
        {
            SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
            if (!skillLine)
                continue;
 
            // wrong skill
            if (skillLine->skillId != skill_id)
                continue;
 
            // not high rank
            if (skillLine->forward_spellid)
                continue;
 
            // skip racial skills
            if (skillLine->racemask != 0)
                continue;
 
            // skip wrong class skills
            if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
                continue;
				
				
				//MYTH
				SpellEntry const* spellInfo = sSpellStore.LookupEntry(skillLine->spellId);
				//TRINITY
				//SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(skillLine->spellId);
            if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
                continue;
 
            player->learnSpell(skillLine->spellId, false);
        }
        }
 
        bool IsSecondarySkill(SkillType skill) const
        {
                return skill == SKILL_COOKING || skill == SKILL_FIRST_AID || skill == SKILL_FISHING;
        }
 
        void CompleteLearnProfession(Player *pPlayer, Creature *pCreature, SkillType skill)
        {
                if (PlayerAlreadyHasTwoProfessions(pPlayer) && !IsSecondarySkill(skill))
                        pCreature->MonsterWhisper("Ya sabes dos profesiones!", pPlayer->GetGUID());
                else
               {
                        if (!LearnAllRecipesInProfession(pPlayer, skill))
                                pCreature->MonsterWhisper("Error interno!", pPlayer->GetGUID());
                }
       }
        
     bool OnGossipSelect(Player* pPlayer, Creature* _creature, uint32 uiSender, uint32 uiAction)
{ 
        pPlayer->PlayerTalkClass->ClearMenus();
        
        if (uiSender == GOSSIP_SENDER_MAIN)
        {
                
                switch (uiAction)
                {
                                case 196:
                                pPlayer->ADD_GOSSIP_ITEM(3, "Alquimia", GOSSIP_SENDER_MAIN, 1);
                                pPlayer->ADD_GOSSIP_ITEM(3, "Herreria", GOSSIP_SENDER_MAIN, 2);
                                pPlayer->ADD_GOSSIP_ITEM(3, "Peleteria", GOSSIP_SENDER_MAIN, 3);
                                pPlayer->ADD_GOSSIP_ITEM(3, "Sastreria", GOSSIP_SENDER_MAIN, 4);
                                pPlayer->ADD_GOSSIP_ITEM(3, "Ingenieria", GOSSIP_SENDER_MAIN, 5);
                                pPlayer->ADD_GOSSIP_ITEM(3, "Encantador", GOSSIP_SENDER_MAIN, 6);
                                pPlayer->ADD_GOSSIP_ITEM(3, "Joyeria", GOSSIP_SENDER_MAIN, 7);
                                pPlayer->ADD_GOSSIP_ITEM(3, "Inscripcion", GOSSIP_SENDER_MAIN, 8);
                                pPlayer->ADD_GOSSIP_ITEM(3, "Mineria", GOSSIP_SENDER_MAIN, 11);
                                pPlayer->ADD_GOSSIP_ITEM(3, "Herboristeria", GOSSIP_SENDER_MAIN, 12);
                                pPlayer->ADD_GOSSIP_ITEM(3, "Desuello", GOSSIP_SENDER_MAIN, 13);	
								pPlayer->ADD_GOSSIP_ITEM( 2, "<- Volver al Menu Principal.", GOSSIP_SENDER_MAIN, 2051);
								pPlayer->ADD_GOSSIP_ITEM( 2, "Olvidalo", GOSSIP_SENDER_MAIN, 2050);
 
                                pPlayer->PlayerTalkClass->SendGossipMenu(907, _creature->GetGUID());
                                break;
								
								case 197:

                                pPlayer->ADD_GOSSIP_ITEM(3, "Cocinero", GOSSIP_SENDER_MAIN, 9);
                                pPlayer->ADD_GOSSIP_ITEM(3, "Primeros Auxilios", GOSSIP_SENDER_MAIN, 10);
								pPlayer->ADD_GOSSIP_ITEM(3, "Pesca", GOSSIP_SENDER_MAIN, 14);
								pPlayer->ADD_GOSSIP_ITEM( 2, "<- Volver al Menu Principal.", GOSSIP_SENDER_MAIN, 2051);
								pPlayer->ADD_GOSSIP_ITEM( 2, "Olvidalo", GOSSIP_SENDER_MAIN, 2050);
                                pPlayer->PlayerTalkClass->SendGossipMenu(907, _creature->GetGUID());
                                break;		
								
                        case 1:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_ALCHEMY);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
                        case 2:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_BLACKSMITHING);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
                        case 3:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_LEATHERWORKING);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
                        case 4:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_TAILORING);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
                        case 5:
								//TRINITY
                                //CompleteLearnProfession(pPlayer, _creature, SKILL_ENGINEERING);
								//MYTH
								CompleteLearnProfession(pPlayer, _creature, SKILL_ENGINERING);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
                        case 6:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_ENCHANTING);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
                        case 7:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_JEWELCRAFTING);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
                        case 8:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_INSCRIPTION);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
                        case 9:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_COOKING);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
                        case 10:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_FIRST_AID);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
                        case 11:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_MINING);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
                        case 12:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_HERBALISM);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
                        case 13:
                                CompleteLearnProfession(pPlayer, _creature, SKILL_SKINNING);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
						case 14:
								CompleteLearnProfession(pPlayer, _creature, SKILL_FISHING);
 
                                pPlayer->CLOSE_GOSSIP_MENU();
                                break;
								
						case 2050:
								pPlayer->CLOSE_GOSSIP_MENU();
								break;	
								
						case 2051:
                        pPlayer->ADD_GOSSIP_ITEM(9, "[Profesiones Primarias] ->", GOSSIP_SENDER_MAIN, 196);
						pPlayer->ADD_GOSSIP_ITEM(9, "[Profesiones Secundarias] ->", GOSSIP_SENDER_MAIN, 197);
						pPlayer->ADD_GOSSIP_ITEM( 2, "Olvidalo", GOSSIP_SENDER_MAIN, 2050);
                        pPlayer->PlayerTalkClass->SendGossipMenu(907, _creature->GetGUID());	
								break;
                }
 
        
        }
         return true;
         }
         };
 
void AddSC_skill_npc()
{
    new skill_npc();
};