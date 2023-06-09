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

#include "PetAI.h"
#include "Errors.h"
#include "Pet.h"
#include "Player.h"
#include "DBCStores.h"
#include "Spell.h"
#include "ObjectAccessor.h"
#include "SpellMgr.h"
#include "Creature.h"
#include "World.h"
#include "Util.h"
#include "Group.h"

int PetAI::Permissible(const Creature* creature)
{
    if(creature->isPet())
        return PERMIT_BASE_SPECIAL;

    return PERMIT_BASE_NO;
}

PetAI::PetAI(Creature* c) : CreatureAI(c), i_tracker(TIME_INTERVAL_LOOK)
{
    m_AllySet.clear();
    UpdateAllies();
}

void PetAI::EnterEvadeMode() { }

bool PetAI::_needToStop()
{
    // This is needed for charmed creatures, as once their target was reset other effects can trigger threat
    if(me->isCharmed() && me->getVictim() == me->GetCharmer())
        return true;

    return !me->canAttack(me->getVictim());
}

void PetAI::_stopAttack()
{
    if(!me->isAlive())
    {
        sLog->outDebug(LOG_FILTER_UNITS, "Creature stoped attacking cuz his dead [guid=%u]", me->GetGUIDLow());
        me->GetMotionMaster()->Clear();
        me->GetMotionMaster()->MoveIdle();
        me->CombatStop();
        me->getHostileRefManager().deleteReferences();

        return;
    }

    me->AttackStop();
    me->GetCharmInfo()->SetIsCommandAttack(false);
    HandleReturnMovement();
}

void PetAI::UpdateAI(const uint32 diff)
{
    if(!me->isAlive())
        return;

    Unit* owner = me->GetCharmerOrOwner();

    if(m_updateAlliesTimer <= diff)
        // UpdateAllies self set update timer
        UpdateAllies();
    else
        m_updateAlliesTimer -= diff;

    // me->getVictim() can't be used for check in case stop fighting, me->getVictim() clear at Unit death etc.
    if(me->getVictim())
    {
        // is only necessary to stop casting, the pet must not exit combat
        if(me->getVictim()->HasBreakableByDamageCrowdControlAura(me))
        {
            me->InterruptNonMeleeSpells(false);
            return;
        }

        if(_needToStop())
        {
            sLog->outDebug(LOG_FILTER_UNITS, "Pet AI stopped attacking [guid=%u]", me->GetGUIDLow());
            _stopAttack();
            return;
        }

        DoMeleeAttackIfReady();
    } else if(owner && me->GetCharmInfo()) { // No Victim
        Unit* nextTarget = SelectNextTarget();

        if(me->HasReactState(REACT_PASSIVE))
            _stopAttack();
        else if(nextTarget)
            AttackStart(nextTarget);
        else
            HandleReturnMovement();
    } else if(owner && !me->HasUnitState(UNIT_STAT_FOLLOW)) // no charm info and no victim
        me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle());

    if(!me->GetCharmInfo())
        return;

    // Autocast (casted only in combat or persistent spells in any state)
    if(!me->HasUnitState(UNIT_STAT_CASTING))
    {
        typedef std::vector<std::pair<Unit*, Spell*> > TargetSpellList;
        TargetSpellList targetSpellStore;

        for(uint8 i = 0; i < me->GetPetAutoSpellSize(); ++i)
        {
            uint32 spellID = me->GetPetAutoSpellOnPos(i);
            if(!spellID)
                continue;

            SpellEntry const *spellInfo = sSpellStore.LookupEntry(spellID);
            if(!spellInfo)
                continue;

            if(me->GetCharmInfo() && me->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
                continue;

			if(!IsPositiveSpell(spellInfo->Id))
            {
                if(!IsNonCombatSpell(spellInfo))
                {
                    /*// allow only spell without spell cost or with spell cost but not duration limit
                    int32 duration = GetSpellDuration(spellInfo);
                    if((spellInfo->manaCost || spellInfo->ManaCostPercentage || spellInfo->manaPerSecond) && duration > 0)*/
					// check spell cooldown
                    if (me->HasSpellCooldown(spellInfo->Id))
                        continue;

                    /*// allow only spell without cooldown > duration
                    int32 cooldown = GetSpellRecoveryTime(spellInfo);
                    if(cooldown >= 0 && duration >= 0 && cooldown > duration)*/
					// Check if we're in combat or commanded to attack
					if (!me->isInCombat() && !me->GetCharmInfo()->IsCommandAttack())
                        continue;
                }

                Spell* pSpell = new Spell(me, spellInfo, false, 0);
                bool spellUsed = false;
                /*for(std::set<uint64>::const_iterator tar = m_AllySet.begin(); tar != m_AllySet.end(); ++tar)
                {
                    Unit* pTarget = ObjectAccessor::GetUnit(*me, *tar);

                    //only buff targets that are in combat, unless the spell can only be cast while out of combat
                    if(!pTarget)
                        continue;

                    if(pSpell->CanAutoCast(pTarget))*/
					// Some spells can target enemy or friendly (DK Ghoul's Leap)
				// Check for enemy first (pet then owner)
				if (Unit* target = me->getAttackerForHelper())
				{
					if (_CanAttack(target) && pSpell->CanAutoCast(target))
					{
						targetSpellStore.push_back(std::make_pair(target, pSpell));
						spellUsed = true;
					}
				}
				else if (Unit* target = me->GetCharmerOrOwner()->getAttackerForHelper())
				{
					if (_CanAttack(target) && pSpell->CanAutoCast(target))
					{
                        targetSpellStore.push_back(std::make_pair(target, pSpell));
                        spellUsed = true;
                    }
                }
				
				// No enemy, check friendly
                if (!spellUsed)
                {
                    for (std::set<uint64>::const_iterator tar = m_AllySet.begin(); tar != m_AllySet.end(); ++tar)
                    {
                        Unit* target = ObjectAccessor::GetUnit(*me, *tar);

                        //only buff targets that are in combat, unless the spell can only be cast while out of combat
                        if (!target)
                            continue;

                        if (pSpell->CanAutoCast(target))
                        {
                            targetSpellStore.push_back(std::make_pair(target, pSpell));
                            spellUsed = true;
                            break;
                        }
                    }
                }

                // No valid targets at all
				if(!spellUsed)
					delete pSpell;
            }
            else if(me->getVictim() && _CanAttack(me->getVictim()) && !IsNonCombatSpell(spellInfo))
            {
                Spell* pSpell = new Spell(me, spellInfo, false, 0);
                if (pSpell->CanAutoCast(me->getVictim()))
                    targetSpellStore.push_back(std::make_pair(me->getVictim(), pSpell));
                else
                    delete pSpell;
            }
        }

        //found units to cast on to
        if(!targetSpellStore.empty())
        {
            uint32 index = urand(0, targetSpellStore.size() - 1);

            Spell* spell  = targetSpellStore[index].second;
            Unit* target = targetSpellStore[index].first;

            targetSpellStore.erase(targetSpellStore.begin() + index);

            SpellCastTargets targets;
            targets.SetUnitTarget(target);

            if(!me->HasInArc(M_PI, target))
            {
                me->SetInFront(target);
                if(target && target->GetTypeId() == TYPEID_PLAYER)
                    me->SendUpdateToPlayer(target->ToPlayer());

                if(owner && owner->GetTypeId() == TYPEID_PLAYER)
                    me->SendUpdateToPlayer(owner->ToPlayer());
            }

            me->AddCreatureSpellCooldown(spell->m_spellInfo->Id);

            spell->prepare(&targets);
        }

        // deleted cached Spell objects
        for(TargetSpellList::const_iterator itr = targetSpellStore.begin(); itr != targetSpellStore.end(); ++itr)
            delete itr->second;
    }
}

void PetAI::UpdateAllies()
{
    Unit* pOwner = me->GetCharmerOrOwner();
    Group *pGroup = NULL;

    m_updateAlliesTimer = 10*IN_MILLISECONDS;                //update friendly targets every 10 seconds, lesser checks increase performance

    if(!pOwner)
        return;
    else if(pOwner->GetTypeId() == TYPEID_PLAYER)
        pGroup = pOwner->ToPlayer()->GetGroup();

    //only pet and owner/not in group->ok
    if(m_AllySet.size() == 2 && !pGroup)
        return;
    //owner is in group; group members filled in already (no raid -> subgroupcount = whole count)
    if(pGroup && !pGroup->isRaidGroup() && m_AllySet.size() == (pGroup->GetMembersCount() + 2))
        return;

    m_AllySet.clear();
    m_AllySet.insert(me->GetGUID());
    if(pGroup)                                              //add group
    {
        for(GroupReference *itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* Target = itr->getSource();
            if(!Target || !pGroup->SameSubGroup((Player*)pOwner, Target))
                continue;

            if(Target->GetGUID() == pOwner->GetGUID())
                continue;

            m_AllySet.insert(Target->GetGUID());
        }
    }
    else                                                    //remove group
        m_AllySet.insert(pOwner->GetGUID());
}

void PetAI::KilledUnit(Unit* victim)
{
    // Called from Unit::Kill() in case where pet or owner kills something
    // if owner killed this victim, pet may still be attacking something else
    if(me->getVictim() && me->getVictim() != victim)
        return;

    // Clear target just in case. May help problem where health / focus / mana
    // regen gets stuck. Also resets attack command.
    // Can't use _stopAttack() because that activates movement handlers and ignores
    // next target selection
    me->AttackStop();
    me->GetCharmInfo()->SetIsCommandAttack(false);

    Unit* nextTarget = SelectNextTarget();

    if(nextTarget)
        AttackStart(nextTarget);
    else
        HandleReturnMovement(); // Return
}

void PetAI::AttackStart(Unit* pTarget)
{
    // Check all pet states to decide if we can attack this target
    if(!_CanAttack(pTarget))
        return;

    if(Unit* pOwner = me->GetOwner())
        pOwner->SetInCombatWith(pTarget);

    DoAttack(pTarget, true);
}

Unit* PetAI::SelectNextTarget()
{
    // Provides next target selection after current target death

    // Passive pets don't do next target selection
    if(me->HasReactState(REACT_PASSIVE))
        return NULL;

    Unit* target = me->getAttackerForHelper();

    // Check pet's attackers first to prevent dragging mobs back to owner
    if(target && !target->HasBreakableByDamageCrowdControlAura())
        return target;

    if(me->GetCharmerOrOwner())
    {
        // Check owner's attackers if pet didn't have any
        target = me->GetCharmerOrOwner()->getAttackerForHelper();
        if(target && !target->HasBreakableByDamageCrowdControlAura())
            return target;

        // 3.0.2 - Pets now start attacking their owners target in defensive mode as soon as the hunter does
        target = me->GetCharmerOrOwner()->getVictim();
        if(target && !target->HasBreakableByDamageCrowdControlAura())
            return target;
    }

    // Default
    return NULL;
}

void PetAI::HandleReturnMovement()
{
    // Handles moving the pet back to stay or owner

    if(me->GetCharmInfo()->HasCommandState(COMMAND_STAY))
    {
        if(!me->GetCharmInfo()->IsAtStay() && !me->GetCharmInfo()->IsReturning())
        {
            // Return to previous position where stay was clicked
            if(!me->GetCharmInfo()->IsCommandAttack())
            {
                float x, y, z;

                me->GetCharmInfo()->GetStayPosition(x, y, z);
                me->GetCharmInfo()->SetIsReturning(true);
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint(me->GetGUIDLow(), x, y, z);
            }
        }
    }
    else // COMMAND_FOLLOW
    {
        if(!me->GetCharmInfo()->IsFollowing() && !me->GetCharmInfo()->IsReturning())
        {
            if(!me->GetCharmInfo()->IsCommandAttack())
            {
                me->GetCharmInfo()->SetIsReturning(true);
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MoveFollow(me->GetCharmerOrOwner(), PET_FOLLOW_DIST, me->GetFollowAngle());
            }
        }
    }
}

void PetAI::DoAttack(Unit* target, bool chase)
{
    // Handles attack with or without chase and also resets all
    // PetAI flags for next update / creature kill
    if(chase)
    {
        if(me->Attack(target, true))
        {
            me->GetCharmInfo()->SetIsAtStay(false);
            me->GetCharmInfo()->SetIsFollowing(false);
            me->GetCharmInfo()->SetIsReturning(false);
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MoveChase(target);
        }
    } else {
        me->GetCharmInfo()->SetIsAtStay(true);
        me->GetCharmInfo()->SetIsFollowing(false);
        me->GetCharmInfo()->SetIsReturning(false);
        me->Attack(target, true);
    }
}

void PetAI::MovementInform(uint32 moveType, uint32 data)
{
    // Receives notification when pet reaches stay or follow owner
    switch(moveType)
    {
        case POINT_MOTION_TYPE:
        {
            // Pet is returning to where stay was clicked. data should be
            // pet's GUIDLow since we set that as the waypoint ID
            if(data == me->GetGUIDLow() && me->GetCharmInfo()->IsReturning())
            {
                me->GetCharmInfo()->SetIsAtStay(true);
                me->GetCharmInfo()->SetIsReturning(false);
                me->GetCharmInfo()->SetIsFollowing(false);
                me->GetCharmInfo()->SetIsCommandAttack(false);
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MoveIdle();
            }
            break;
        }
        case FOLLOW_MOTION_TYPE:
        {
            // If data is owner's GUIDLow then we've reached follow point,
            // otherwise we're probably chasing a creature
            if(me->GetCharmerOrOwner() && me->GetCharmInfo() && data == me->GetCharmerOrOwner()->GetGUIDLow() && me->GetCharmInfo()->IsReturning())
            {
                me->GetCharmInfo()->SetIsAtStay(false);
                me->GetCharmInfo()->SetIsReturning(false);
                me->GetCharmInfo()->SetIsFollowing(true);
                me->GetCharmInfo()->SetIsCommandAttack(false);
            }
            break;
        }
        default:
            break;
    }
}

bool PetAI::_CanAttack(Unit* target)
{
    // Evaluates wether a pet can attack a specific
    // target based on CommandState, ReactState and other flags

    // Returning - check first since pets returning ignore attacks
    if(me->GetCharmInfo()->IsReturning())
        return false;

    // Passive - check now so we don't have to worry about passive in later checks
    if(me->HasReactState(REACT_PASSIVE))
        return me->GetCharmInfo()->IsCommandAttack();

    //  Pets commanded to attack should not stop their approach if attacked by another creature
    if(me->getVictim() && (me->getVictim() != target))
        return !me->GetCharmInfo()->IsCommandAttack();

    // From this point on, pet will always be either aggressive or defensive

    // Stay - can attack if target is within range or commanded to
    if(me->GetCharmInfo()->HasCommandState(COMMAND_STAY))
        return (me->IsWithinMeleeRange(target, MIN_MELEE_REACH) || me->GetCharmInfo()->IsCommandAttack());

    // Follow
    if(me->GetCharmInfo()->HasCommandState(COMMAND_FOLLOW))
        return true;

    // default, though we shouldn't ever get here
    return false;
}
