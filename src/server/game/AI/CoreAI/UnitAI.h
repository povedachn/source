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

#ifndef TRINITY_UNITAI_H
#define TRINITY_UNITAI_H

#include "Define.h"
#include <list>
#include "Unit.h"

class Unit;
class Player;
struct AISpellInfoType;

// Default script texts
enum GeneralScriptTexts
{
    DEFAULT_TEXT                = -1000000,
    EMOTE_GENERIC_FRENZY_KILL   = -1000001,
    EMOTE_GENERIC_FRENZY        = -1000002,
    EMOTE_GENERIC_ENRAGED       = -1000003,
    EMOTE_GENERIC_BERSERK       = -1000004,
    EMOTE_GENERIC_BERSERK_RAID  = -1000005, // RaidBossEmote version of the previous one
};

//Selection method used by SelectTarget
enum SelectAggroTarget
{
    SELECT_TARGET_RANDOM = 0,                               //Just selects a random target
    SELECT_TARGET_TOPAGGRO,                                 //Selects targes from top aggro to bottom
    SELECT_TARGET_BOTTOMAGGRO,                              //Selects targets from bottom aggro to top
    SELECT_TARGET_NEAREST,
    SELECT_TARGET_FARTHEST,
};

enum SelectArc
{
    SELECT_ARC_NONE = 0,
    SELECT_ARC_FRONT,           // select front of target
    SELECT_ARC_BACK,            //  select behind target
};

// default predicate function to select target based on distance, player and/or aura criteria
struct DefaultTargetSelector : public std::unary_function<Unit*, bool>
{
    const Unit* me;
    float m_dist;
    bool m_playerOnly;
    int32 m_aura;
    SelectArc m_arc_type;
    float m_arc;

    // pUnit: the reference unit
    // dist: if 0: ignored, if > 0: maximum distance to the reference unit, if < 0: minimum distance to the reference unit
    // playerOnly: self explaining
    // aura: if 0: ignored, if > 0: the target shall have the aura, if < 0, the target shall NOT have the aura
    DefaultTargetSelector(Unit const* pUnit, float dist, bool playerOnly, int32 aura, SelectArc m_arc_type = SELECT_ARC_NONE, float arc = 0.0f) : me(pUnit)
                           , m_dist(dist), m_playerOnly(playerOnly), m_aura(aura), m_arc_type(m_arc_type), m_arc(arc) { }

    bool operator() (Unit const* pTarget)
    {
        if(!me)
            return false;

        if(!pTarget)
            return false;

        if(m_playerOnly && (pTarget->GetTypeId() != TYPEID_PLAYER))
            return false;

        if(m_dist > 0.0f && !me->IsWithinCombatRange(pTarget, m_dist))
            return false;

        if(m_dist < 0.0f && me->IsWithinCombatRange(pTarget, -m_dist))
            return false;

        if(m_aura)
        {
            if(m_aura > 0)
            {
                if(!pTarget->HasAura(m_aura))
                    return false;
            } else if(pTarget->HasAura(-m_aura))
                    return false;
        }

        if(m_arc_type)
        {
            if(m_arc_type == SELECT_ARC_FRONT)
            {
                if(!me->HasInArc(m_arc, pTarget))
                    return false;
            } else if(m_arc_type == SELECT_ARC_BACK && me->HasInArc(2 * static_cast<float>(M_PI) - m_arc, pTarget))
                return false;
        }

        return true;
    }
};

class UnitAI
{
protected:
    Unit* const me;
public:
    explicit UnitAI(Unit* unit) : me(unit) { }
    virtual ~UnitAI() { }

    virtual bool CanAIAttack(Unit const* /*target*/) const { return true; }
    virtual void AttackStart(Unit* /*pTarget*/);
    virtual void UpdateAI(uint32 const diff) = 0;

    virtual void InitializeAI() { if(!me->isDead()) Reset(); }

    virtual void Reset() { };

    // Called when unit is charmed
    virtual void OnCharmed(bool apply) = 0;

    // Pass parameters between AI
    virtual void DoAction(int32 const /*param*/) { }
    virtual uint32 GetData(uint32 /*id = 0*/) { return 0; }
    virtual void SetData(uint32 /*id*/, uint32 /*value*/) { }
    virtual void SetGUID(uint64 const&/*guid*/, int32 /*id*/ = 0) { }
    virtual uint64 GetGUID(int32 /*id*/ = 0) { return 0; }

    Unit* SelectTarget(SelectAggroTarget targetType, uint32 position = 0, float dist = 0.0f, bool playerOnly = false, int32 aura = 0, SelectArc targetarctype = SELECT_ARC_NONE, float arc = 0.0f);
    // Select the targets satifying the predicate.
    template <class PREDICATE> Unit* SelectTarget(SelectAggroTarget targetType, uint32 position, PREDICATE predicate)
    {
        const std::list<HostileReference* > &threatlist = me->getThreatManager().getThreatList();
        if(position >= threatlist.size())
            return NULL;

        std::list<Unit*> targetList;
        for(std::list<HostileReference*>::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
            if(predicate((*itr)->getTarget()))
                targetList.push_back((*itr)->getTarget());

        if(position >= targetList.size())
            return NULL;

        if(targetType == SELECT_TARGET_NEAREST || targetType == SELECT_TARGET_FARTHEST)
            targetList.sort(Trinity::ObjectDistanceOrderPred(me));

        switch(targetType)
        {
            case SELECT_TARGET_NEAREST:
            case SELECT_TARGET_TOPAGGRO:
            {
                std::list<Unit*>::iterator itr = targetList.begin();
                std::advance(itr, position);
                return *itr;
            }
            case SELECT_TARGET_FARTHEST:
            case SELECT_TARGET_BOTTOMAGGRO:
            {
                std::list<Unit*>::reverse_iterator ritr = targetList.rbegin();
                std::advance(ritr, position);
                return *ritr;
            }
            case SELECT_TARGET_RANDOM:
            {
                std::list<Unit*>::iterator itr = targetList.begin();
                std::advance(itr, urand(position, targetList.size()-1));
                return *itr;
            }
            default:
                break;
        }

        return NULL;
    }

    void SelectTargetList(std::list<Unit*> &targetList, uint32 num, SelectAggroTarget targetType, float dist = 0.0f, bool playerOnly = false, int32 aura = 0);

    // Select the targets satifying the predicate.
    // predicate shall extend std::unary_function<Unit*, bool>
    template <class PREDICATE> void SelectTargetList(std::list<Unit*> &targetList, PREDICATE predicate, uint32 maxTargets, SelectAggroTarget targetType)
    {
        std::list<HostileReference*> const& threatlist = me->getThreatManager().getThreatList();
        if(threatlist.empty())
            return;

        for(std::list<HostileReference*>::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
            if(predicate((*itr)->getTarget()))
                targetList.push_back((*itr)->getTarget());

        if(targetList.size() < maxTargets)
            return;

        if(targetType == SELECT_TARGET_NEAREST || targetType == SELECT_TARGET_FARTHEST)
            targetList.sort(Trinity::ObjectDistanceOrderPred(me));

        if(targetType == SELECT_TARGET_FARTHEST || targetType == SELECT_TARGET_BOTTOMAGGRO)
            targetList.reverse();

        if(targetType == SELECT_TARGET_RANDOM)
            Trinity::RandomResizeList(targetList, maxTargets);
        else
            targetList.resize(maxTargets);
    }

    // Called at any Damage to any victim (before damage apply)
    virtual void DamageDealt(Unit* /*pVictim*/, uint32& /*damage*/, DamageEffectType /*damageType*/) { }

    // Called at any Damage from any attacker (before damage apply)
    // Note: it for recalculation damage or special reaction at damage
    // for attack reaction use AttackedBy called for not DOT damage in Unit::DealDamage also
    virtual void DamageTaken(Unit* /*pAttacker*/, uint32& /*damage*/) { }

    // Called when the creature receives heal
    virtual void HealReceived(Unit* /*done_by*/, uint32& /*addhealth*/) { }

    // Called when the unit heals
    virtual void HealDone(Unit* /*done_to*/, uint32& /*addhealth*/) { }

    // Called when a spell is interrupted by Spell::EffectInterruptCast
    // Use to reschedule next planned cast of spell.
    virtual void SpellInterrupted(uint32 /*spellId*/, uint32 /*unTimeMs*/) {}

    void AttackStartCaster(Unit* victim, float dist);

    void DoAddAuraToAllHostilePlayers(uint32 spellid);
    void DoCast(uint32 spellId);
    void DoCast(Unit* victim, uint32 spellId, bool triggered = false);
    void DoCastToAllHostilePlayers(uint32 spellid, bool triggered = false);
    void DoCastVictim(uint32 spellId, bool triggered = false);
    void DoCastAOE(uint32 spellId, bool triggered = false);

    float DoGetSpellMaxRange(uint32 spellId, bool positive = false);

    void DoMeleeAttackIfReady();
    bool DoSpellAttackIfReady(uint32 spell);

    static AISpellInfoType* AISpellInfo;
    static void FillAISpellInfo();

    virtual void sGossipHello(Player* /*pPlayer*/) { }
    virtual void sGossipSelect(Player* /*pPlayer*/, uint32 /*sender*/, uint32 /*action*/) { }
    virtual void sGossipSelectCode(Player* /*pPlayer*/, uint32 /*sender*/, uint32 /*action*/, char const* /*code*/) { }
    virtual void sQuestAccept(Player* /*pPlayer*/, Quest const* /*quest*/) { }
    virtual void sQuestSelect(Player* /*pPlayer*/, Quest const* /*quest*/) { }
    virtual void sQuestComplete(Player* /*pPlayer*/, Quest const* /*quest*/) { }
    virtual void sQuestReward(Player* /*pPlayer*/, Quest const* /*quest*/, uint32 /*opt*/) { }
    virtual bool sOnDummyEffect(Unit* /*pCaster*/, uint32 /*spellId*/, SpellEffIndex /*effIndex*/) { return false; }
	virtual void sOnGameEvent(bool /*start*/, uint16 /*eventId*/) {}
};

class PlayerAI : public UnitAI
{
protected:
    Player* const me;
public:
    explicit PlayerAI(Player* p) : UnitAI((Unit*)p), me(p) { }

    void OnCharmed(bool apply);
};

class SimpleCharmedAI : public PlayerAI
{
public:
    void UpdateAI(uint32 const diff);
    SimpleCharmedAI(Player* player): PlayerAI(player) { }
};

#endif
