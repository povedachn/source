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

#include "ScriptPCH.h"

enum WarriorSpells
{
    WARRIOR_SPELL_LAST_STAND_TRIGGERED           = 12976,
};

class spell_warr_last_stand : public SpellScriptLoader
{
public:
    spell_warr_last_stand() : SpellScriptLoader("spell_warr_last_stand") { }

    class spell_warr_last_stand_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_last_stand_SpellScript);

        bool Validate(SpellEntry const* /*pSpellEntry*/)
        {
            if(!sSpellStore.LookupEntry(WARRIOR_SPELL_LAST_STAND_TRIGGERED))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            int32 healthModSpellBasePoints0 = int32(GetCaster()->CountPctFromMaxHealth(30));
            GetCaster()->CastCustomSpell(GetCaster(), WARRIOR_SPELL_LAST_STAND_TRIGGERED, &healthModSpellBasePoints0, NULL, NULL, true, NULL);
        }

        void Register()
        {
            // add dummy effect spell handler to Last Stand
            OnEffect += SpellEffectFn(spell_warr_last_stand_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warr_last_stand_SpellScript();
    }
};

// Fix carga del guerrero
/*enum Charge
{
    SPELL_JUGGERNAUT_CRIT_BONUS_TALENT      = 64976,
    SPELL_JUGGERNAUT_CRIT_BONUS_BUFF        = 65156,
    SPELL_CHARGE                            = 34846,
};

class spell_warr_charge : public SpellScriptLoader
{
    public:
        spell_warr_charge() : SpellScriptLoader("spell_warr_charge") { }

        class spell_warr_charge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_charge_SpellScript);

            bool Validate(SpellEntry const*)
            {
                if (!sSpellStore.LookupEntry(SPELL_JUGGERNAUT_CRIT_BONUS_TALENT) || !sSpellStore.LookupEntry(SPELL_JUGGERNAUT_CRIT_BONUS_BUFF) || !sSpellStore.LookupEntry(SPELL_CHARGE))
                    return false;
                return true;
            }
            void HandleDummy(SpellEffIndex)
            {
                int32 chargeBasePoints0 = GetEffectValue();
                Unit* caster = GetCaster();
                caster->CastCustomSpell(caster, SPELL_CHARGE, &chargeBasePoints0, NULL, NULL, true);

                // Juggernaut crit bonus
                if (caster->HasAura(SPELL_JUGGERNAUT_CRIT_BONUS_TALENT))
                    caster->CastSpell(caster, SPELL_JUGGERNAUT_CRIT_BONUS_BUFF, true);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_warr_charge_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_charge_SpellScript();
        }
};*/

class spell_warr_improved_spell_reflection : public SpellScriptLoader
{
public:
    spell_warr_improved_spell_reflection() : SpellScriptLoader("spell_warr_improved_spell_reflection") { }

    class spell_warr_improved_spell_reflection_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_improved_spell_reflection_SpellScript);

        void FilterTargets(std::list<Unit*>& unitList)
        {
            unitList.remove(GetCaster());
        }

        void Register()
        {
            OnUnitTargetSelect += SpellUnitTargetFn(spell_warr_improved_spell_reflection_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_PARTY_CASTER);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warr_improved_spell_reflection_SpellScript();
    }
};

void AddSC_warrior_spell_scripts()
{
    new spell_warr_last_stand;
	//new spell_warr_charge;
    new spell_warr_improved_spell_reflection;
}