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

#include "DatabaseEnv.h"
#include "ObjectMgr.h"
#include "ObjectDefines.h"
#include "GridDefines.h"
#include "GridNotifiers.h"
#include "SpellMgr.h"
#include "GridNotifiersImpl.h"
#include "Cell.h"
#include "CellImpl.h"
#include "InstanceScript.h"
#include "ScriptedCreature.h"
#include "Group.h"

#include "SmartAI.h"
#include "ScriptPCH.h"

SmartAI::SmartAI(Creature* c) : CreatureAI(c)
{
    // copy script to local (protection for table reload)

    mWayPoints = NULL;
    mEscortState = SMART_ESCORT_NONE;
    mCurrentWPID = 0;//first wp id is 1 !!
    mWPReached = false;
    mWPPauseTimer = 0;
    mLastWP = NULL;

    mCanRepeatPath = false;

    // spawn in run mode
    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
    mRun = true;

    me->GetPosition(&mLastOOCPos);

    mCanAutoAttack = true;
    mCanCombatMove = true;

    mForcedPaused = false;
    mLastWPIDReached = 0;

    mEscortQuestID = 0;

    mDespawnTime = 0;
    mDespawnState = 0;

    mEscortInvokerCheckTimer = 1000;
    mFollowGuid = 0;
    mFollowDist = 0;
    mFollowAngle = 0;
    mFollowCredit = 0;
    mFollowArrivedEntry = 0;
    mFollowCreditType = 0;
}

void SmartAI::UpdateDespawn(const uint32 diff)
{
    if(mDespawnState <= 1 || mDespawnState > 3) return;
    if(mDespawnTime < diff)
    {
        if(mDespawnState == 2)
        {
            me->SetVisible(false);
            mDespawnTime = 5000;
            mDespawnState++;
        }
        else
            me->DespawnOrUnsummon();
    } else mDespawnTime -= diff;
}

void SmartAI::Reset()
{
    if(!HasEscortState(SMART_ESCORT_ESCORTING))//dont mess up escort movement after combat
        SetRun(true);
    GetScript()->OnReset();
}

WayPoint* SmartAI::GetNextWayPoint()
{
    if(!mWayPoints || mWayPoints->empty())
        return NULL;

    mCurrentWPID++;
    WPPath::const_iterator itr = mWayPoints->find(mCurrentWPID);
    if(itr != mWayPoints->end())
    {
        mLastWP = (*itr).second;
        if(mLastWP->id != mCurrentWPID)
        {
            sLog->outError("SmartAI::GetNextWayPoint: Got not expected waypoint id %u, expected %u", mLastWP->id, mCurrentWPID);
        }
        return (*itr).second;
    }
    return NULL;
}

void SmartAI::StartPath(bool run, uint32 path, bool repeat, Unit* /*invoker*/)
{
    if(me->isInCombat())// no wp movement in combat
    {
        sLog->outError("SmartAI::StartPath: Creature entry %u wanted to start waypoint movement while in combat, ignoring.", me->GetEntry());
        return;
    }
    if(HasEscortState(SMART_ESCORT_ESCORTING))
        StopPath();
    if(path)
        if(!LoadPath(path))
            return;
    if(!mWayPoints || mWayPoints->empty())
        return;

    AddEscortState(SMART_ESCORT_ESCORTING);
    mCanRepeatPath = repeat;

    SetRun(run);

    WayPoint* wp = GetNextWayPoint();
    if(wp)
    {
        me->GetPosition(&mLastOOCPos);
        me->GetMotionMaster()->MovePoint(wp->id, wp->x, wp->y, wp->z);
        GetScript()->ProcessEventsFor(SMART_EVENT_WAYPOINT_START, NULL, wp->id, GetScript()->GetPathId());
    }
}

bool SmartAI::LoadPath(uint32 entry)
{
    if(HasEscortState(SMART_ESCORT_ESCORTING))
        return false;
    mWayPoints = sSmartWaypointMgr->GetPath(entry);
    if(!mWayPoints)
    {
        GetScript()->SetPathId(0);
        return false;
    }
    GetScript()->SetPathId(entry);
    return true;
}

void SmartAI::PausePath(uint32 delay, bool forced)
{
    if(!HasEscortState(SMART_ESCORT_ESCORTING))
        return;
    if(HasEscortState(SMART_ESCORT_PAUSED))
    {
        sLog->outError("SmartAI::StartPath: Creature entry %u wanted to pause waypoint movement while already paused, ignoring.", me->GetEntry());
        return;
    }
    mForcedPaused = forced;
    me->GetPosition(&mLastOOCPos);
    AddEscortState(SMART_ESCORT_PAUSED);
    mWPPauseTimer = delay;
    if(forced)
    {
        SetRun(mRun);
        me->StopMoving();//force stop
        me->GetMotionMaster()->MoveIdle();//force stop
    }
    GetScript()->ProcessEventsFor(SMART_EVENT_WAYPOINT_PAUSED, NULL, mLastWP->id, GetScript()->GetPathId());
}

void SmartAI::StopPath(uint32 DespawnTime, uint32 quest, bool fail)
{
    if(!HasEscortState(SMART_ESCORT_ESCORTING))
        return;

    if(quest)
        mEscortQuestID = quest;
    SetDespawnTime(DespawnTime);
    //mDespawnTime = DespawnTime;

    me->GetPosition(&mLastOOCPos);
    me->StopMoving();//force stop
    me->GetMotionMaster()->MoveIdle();
    GetScript()->ProcessEventsFor(SMART_EVENT_WAYPOINT_STOPPED, NULL, mLastWP->id, GetScript()->GetPathId());
    EndPath(fail);
}

void SmartAI::EndPath(bool fail)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_WAYPOINT_ENDED, NULL, mLastWP->id, GetScript()->GetPathId());

    RemoveEscortState(SMART_ESCORT_ESCORTING | SMART_ESCORT_PAUSED | SMART_ESCORT_RETURNING);
    mWayPoints = NULL;
    mCurrentWPID = 0;
    mWPPauseTimer = 0;
    mLastWP = NULL;

    if(mCanRepeatPath)
        StartPath(mRun, GetScript()->GetPathId(), mCanRepeatPath);
    else
        GetScript()->SetPathId(0);

    ObjectList* targets = GetScript()->GetTargetList(SMART_ESCORT_TARGETS);
    if(targets && mEscortQuestID)
    {
        if(targets->size() == 1 && GetScript()->IsPlayer((*targets->begin())))
        {
            Player* plr = (*targets->begin())->ToPlayer();
            if(!fail && plr->IsAtGroupRewardDistance(me) && !plr->GetCorpse())
                plr->GroupEventHappens(mEscortQuestID, me);

            if(fail && plr->GetQuestStatus(mEscortQuestID) == QUEST_STATUS_INCOMPLETE)
                plr->FailQuest(mEscortQuestID);

            if(Group* pGroup = plr->GetGroup())
            {
                for(GroupReference* gr = pGroup->GetFirstMember(); gr != NULL; gr = gr->next())
                {
                    Player* pGroupGuy = gr->getSource();

                    if(!fail && pGroupGuy->IsAtGroupRewardDistance(me) && !pGroupGuy->GetCorpse())
                        pGroupGuy->AreaExploredOrEventHappens(mEscortQuestID);
                    if(fail && pGroupGuy->GetQuestStatus(mEscortQuestID) == QUEST_STATUS_INCOMPLETE)
                        pGroupGuy->FailQuest(mEscortQuestID);
                }
            }
        } else
        {
            for(ObjectList::iterator iter = targets->begin(); iter != targets->end(); iter++)
            {
                if(GetScript()->IsPlayer((*iter)))
                {
                    Player* plr = (*iter)->ToPlayer();
                    if(!fail && plr->IsAtGroupRewardDistance(me) && !plr->GetCorpse())
                        plr->AreaExploredOrEventHappens(mEscortQuestID);
                    if(fail && plr->GetQuestStatus(mEscortQuestID) == QUEST_STATUS_INCOMPLETE)
                        plr->FailQuest(mEscortQuestID);
                }
            }
        }
    }
    if(mDespawnState == 1)
        StartDespawn();
}

void SmartAI::ResumePath()
{
    //mWPReached = false;
    SetRun(mRun);
    if(mLastWP)
        me->GetMotionMaster()->MovePoint(mLastWP->id, mLastWP->x, mLastWP->y, mLastWP->z);
}

void SmartAI::ReturnToLastOOCPos()
{
    SetRun(mRun);
    me->GetMotionMaster()->MovePoint(SMART_ESCORT_LAST_OOC_POINT, mLastOOCPos);
}

void SmartAI::UpdatePath(const uint32 diff)
{
    if(!HasEscortState(SMART_ESCORT_ESCORTING))
        return;
    if(mEscortInvokerCheckTimer < diff)
    {
        if(!IsEscortInvokerInRange())
        {
            StopPath(mDespawnTime, mEscortQuestID, true);
        }
        mEscortInvokerCheckTimer = 1000;
    } else mEscortInvokerCheckTimer -= diff;
    // handle pause
    if(HasEscortState(SMART_ESCORT_PAUSED))
    {
        if(mWPPauseTimer < diff)
        {
            if(!me->isInCombat() && !HasEscortState(SMART_ESCORT_RETURNING) && (mWPReached || mLastWPIDReached == SMART_ESCORT_LAST_OOC_POINT || mForcedPaused))
            {
                GetScript()->ProcessEventsFor(SMART_EVENT_WAYPOINT_RESUMED, NULL, mLastWP->id, GetScript()->GetPathId());
                RemoveEscortState(SMART_ESCORT_PAUSED);
                if(mForcedPaused)// if paused between 2 wps resend movement
                {
                    ResumePath();
                    mWPReached = false;
                    mForcedPaused = false;
                }
                if(mLastWPIDReached == SMART_ESCORT_LAST_OOC_POINT)
                    mWPReached = true;
            }
            mWPPauseTimer = 0;
        } else {
            mWPPauseTimer -= diff;

        }
    }
    if(HasEscortState(SMART_ESCORT_RETURNING))
    {
        if(mWPReached)//reached OOC WP
        {
            RemoveEscortState(SMART_ESCORT_RETURNING);
            if(!HasEscortState(SMART_ESCORT_PAUSED))
                ResumePath();
            mWPReached = false;
        }
    }
    if(me->isInCombat() || HasEscortState(SMART_ESCORT_PAUSED | SMART_ESCORT_RETURNING))
        return;
    // handle next wp
    if(mWPReached)//reached WP
    {
        mWPReached = false;
        if(mCurrentWPID == GetWPCount())
        {
            EndPath();
        } else {
            WayPoint* wp = GetNextWayPoint();
            if(wp)
            {
                SetRun(mRun);
                me->GetMotionMaster()->MovePoint(wp->id, wp->x, wp->y, wp->z);
            }
        }

    }
}

void SmartAI::UpdateAI(const uint32 diff)
{
    GetScript()->OnUpdate(diff);
    UpdatePath(diff);
    UpdateDespawn(diff);

    //TODO move to void
    if(mFollowGuid)
    {
        if(mFollowArrivedTimer < diff)
        {
            if(me->FindNearestCreature(mFollowArrivedEntry, INTERACTION_DISTANCE, true))
            {
                if(Player* plr = me->GetPlayer(*me, mFollowGuid))
                {
                    if(!mFollowCreditType)
                        plr->RewardPlayerAndGroupAtEvent(mFollowCredit, me);
                    else
                        plr->GroupEventHappens(mFollowCredit, me);
                }
                mFollowGuid = 0;
                mFollowDist = 0;
                mFollowAngle = 0;
                mFollowCredit = 0;
                mFollowArrivedTimer = 1000;
                mFollowArrivedEntry = 0;
                mFollowCreditType = 0;
                SetDespawnTime(5000);
                me->StopMoving();
                me->GetMotionMaster()->MoveIdle();
                StartDespawn();
                GetScript()->ProcessEventsFor(SMART_EVENT_FOLLOW_COMPLETED);
                return;
            }
            mFollowArrivedTimer = 1000;
        } else mFollowArrivedTimer -= diff;
    }

    if(!UpdateVictim())
        return;

    if(mCanAutoAttack)
        DoMeleeAttackIfReady();
}

bool SmartAI::IsEscortInvokerInRange()
{
    ObjectList* targets = GetScript()->GetTargetList(SMART_ESCORT_TARGETS);
    if(targets)
    {
        if(targets->size() == 1 && GetScript()->IsPlayer((*targets->begin())))
        {
            Player* plr = (*targets->begin())->ToPlayer();
            if(me->GetDistance(plr) <= SMART_ESCORT_MAX_PLAYER_DIST)
                        return true;

            if(Group* pGroup = plr->GetGroup())
            {
                for(GroupReference* gr = pGroup->GetFirstMember(); gr != NULL; gr = gr->next())
                {
                    Player* pGroupGuy = gr->getSource();

                    if(me->GetDistance(pGroupGuy) <= SMART_ESCORT_MAX_PLAYER_DIST)
                        return true;
                }
            }
        } else
        {
            for(ObjectList::iterator iter = targets->begin(); iter != targets->end(); iter++)
            {
                if(GetScript()->IsPlayer((*iter)))
                {
                    if(me->GetDistance((*iter)->ToPlayer()) <= SMART_ESCORT_MAX_PLAYER_DIST)
                        return true;
                }
            }
        }
    }
    return true;//escort targets were not set, ignore range check
}

void SmartAI::MovepointReached(uint32 id)
{
    if(id != SMART_ESCORT_LAST_OOC_POINT && mLastWPIDReached != id)
        GetScript()->ProcessEventsFor(SMART_EVENT_WAYPOINT_REACHED, NULL, id);

    mLastWPIDReached = id;
    mWPReached = true;
}

void SmartAI::MovementInform(uint32 MovementType, uint32 Data)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_MOVEMENTINFORM, NULL, MovementType, Data);
    if(MovementType != POINT_MOTION_TYPE || !HasEscortState(SMART_ESCORT_ESCORTING))
        return;
    MovepointReached(Data);
}

void SmartAI::EnterEvadeMode()
{
    if(!me->isAlive())
        return;

    me->RemoveAllAuras();
    me->DeleteThreatList();
    me->CombatStop(true);
    me->LoadCreaturesAddon();
    me->SetLootRecipient(NULL);
    me->ResetPlayerDamageReq();

    GetScript()->ProcessEventsFor(SMART_EVENT_EVADE);//must be after aura clear so we can cast spells from db

    SetRun(mRun);
    if(HasEscortState(SMART_ESCORT_ESCORTING))
    {
        AddEscortState(SMART_ESCORT_RETURNING);
        ReturnToLastOOCPos();
    } else if(mFollowGuid) {
        if(Unit* target = me->GetUnit(*me, mFollowGuid))
            me->GetMotionMaster()->MoveFollow(target, mFollowDist, mFollowAngle);
    } else me->GetMotionMaster()->MoveTargetedHome();

    Reset();
}

void SmartAI::MoveInLineOfSight(Unit* pWho)
{
    if(!pWho)
        return;
    GetScript()->OnMoveInLineOfSight(pWho);
    if(me->HasReactState(REACT_PASSIVE) || AssistPlayerInCombat(pWho))
        return;

    if(!CanAIAttack(pWho))
        return;

    if(me->IsHostileTo(pWho))
    {
        float fAttackRadius = me->GetAttackDistance(pWho);
        if(me->IsWithinDistInMap(pWho, fAttackRadius) && me->IsWithinLOSInMap(pWho))
        {
            if(!me->getVictim())
            {
                pWho->RemoveAurasByType(SPELL_AURA_MOD_STEALTH);
                AttackStart(pWho);
            } else {
                pWho->SetInCombatWith(me);
                me->AddThreat(pWho, 0.0f);
            }
        }
    }
}

bool SmartAI::CanAIAttack(const Unit* /*pWho*/) const
{
    if(me->GetReactState() == REACT_PASSIVE)
        return false;
    return true;
}

bool SmartAI::AssistPlayerInCombat(Unit* pWho)
{
    if(!pWho || !pWho->getVictim())
        return false;

    //experimental (unknown) flag not present
    if(!(me->GetCreatureInfo()->type_flags & CREATURE_TYPEFLAGS_AID_PLAYERS))
        return false;

    //not a player
    if(!pWho->getVictim()->GetCharmerOrOwnerPlayerOrPlayerItself())
        return false;

    //never attack friendly
    if(me->IsFriendlyTo(pWho))
        return false;

    //too far away and no free sight?
    if(me->IsWithinDistInMap(pWho, SMART_MAX_AID_DIST) && me->IsWithinLOSInMap(pWho))
    {
        //already fighting someone?
        if(!me->getVictim())
        {
            AttackStart(pWho);
            return true;
        }
        else
        {
            pWho->SetInCombatWith(me);
            me->AddThreat(pWho, 0.0f);
            return true;
        }
    }

    return false;
}

void SmartAI::JustRespawned()
{
    mDespawnTime = 0;
    mDespawnState = 0;
    mEscortState = SMART_ESCORT_NONE;
    me->SetVisible(true);
    if(me->getFaction() != me->GetCreatureInfo()->faction_A)
        me->RestoreFaction();
    GetScript()->ProcessEventsFor(SMART_EVENT_RESPAWN);
    Reset();
    mFollowGuid = 0;//do not reset follower on Reset(), we need it after combat evade
    mFollowDist = 0;
    mFollowAngle = 0;
    mFollowCredit = 0;
    mFollowArrivedTimer = 1000;
    mFollowArrivedEntry = 0;
    mFollowCreditType = 0;
}

int SmartAI::Permissible(const Creature* creature)
{
    if(creature->GetAIName() == "SmartAI")
        return PERMIT_BASE_SPECIAL;
    return PERMIT_BASE_NO;
}

void SmartAI::JustReachedHome()
{
    GetScript()->ProcessEventsFor(SMART_EVENT_REACHED_HOME);
}

void SmartAI::EnterCombat(Unit* enemy)
{
    me->InterruptNonMeleeSpells(false);//msut be before ProcessEvents
    GetScript()->ProcessEventsFor(SMART_EVENT_AGGRO, enemy);
    me->GetPosition(&mLastOOCPos);
}

void SmartAI::JustDied(Unit* killer)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_DEATH, killer);
    if(HasEscortState(SMART_ESCORT_ESCORTING))
        EndPath(true);
}

void SmartAI::KilledUnit(Unit* victim)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_KILL, victim);
}

void SmartAI::JustSummoned(Creature* creature)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_SUMMONED_UNIT, creature);
}

void SmartAI::AttackStart(Unit* who)
{
    if(who && me->Attack(who, true))
    {
        SetRun(mRun);
        if(me->GetMotionMaster()->GetMotionSlotType(MOTION_SLOT_ACTIVE) == POINT_MOTION_TYPE)
            me->GetMotionMaster()->MovementExpired();

        if(mCanCombatMove)
            me->GetMotionMaster()->MoveChase(who);

        me->GetPosition(&mLastOOCPos);
    }
}

void SmartAI::SpellHit(Unit* pUnit, const SpellEntry* pSpell)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_SPELLHIT, pUnit, 0, 0, false, pSpell);
}

void SmartAI::SpellHitTarget(Unit* target, const SpellEntry* pSpell)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_SPELLHIT_TARGET, target, 0, 0, false, pSpell);
}

void SmartAI::DamageTaken(Unit* done_by, uint32& damage)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_DAMAGED, done_by, damage);
}

void SmartAI::HealReceived(Unit* done_by, uint32& addhealth)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_RECEIVE_HEAL, done_by, addhealth);
}

void SmartAI::ReceiveEmote(Player* pPlayer, uint32 text_emote)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_RECEIVE_EMOTE, pPlayer, text_emote);
}

void SmartAI::IsSummonedBy(Unit* summoner)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_JUST_SUMMONED, summoner);
}

void SmartAI::DamageDealt(Unit* done_to, uint32& damage, DamageEffectType /*damagetype*/)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_DAMAGED_TARGET, done_to, damage);
}

void SmartAI::SummonedCreatureDespawn(Creature* unit)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_SUMMON_DESPAWNED, unit);
}

void SmartAI::UpdateAIWhileCharmed(const uint32 /*diff*/) { }

void SmartAI::CorpseRemoved(uint32& respawnDelay)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_CORPSE_REMOVED, NULL, respawnDelay);
}

void SmartAI::PassengerBoarded(Unit* who, int8 seatId, bool apply)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_PASSENGER_BOARDED, who, (uint32)seatId, 0, apply);
}

void SmartAI::InitializeAI()
{
    GetScript()->OnInitialize(me);
    if(!me->isDead())
        Reset();
    GetScript()->ProcessEventsFor(SMART_EVENT_RESPAWN);
}

void SmartAI::OnCharmed(bool apply)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_CHARMED, NULL, 0, 0, apply);
}

void SmartAI::DoAction(const int32 /*param*/) { }

uint32 SmartAI::GetData(uint32 /*id*/)
{
    return 0;
}

void SmartAI::SetData(uint32 id, uint32 value)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_DATA_SET, NULL, id, value);
}

void SmartAI::SetGUID(const uint64& /*guid*/, int32 /*id*/) { }

uint64 SmartAI::GetGUID(int32 /*id*/)
{
    return 0;
}

void SmartAI::SetRun(bool run)
{
    if(run)
        me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
    else
        me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
    mRun = run;
}

void SmartAI::SetFly(bool bFly)
{
    me->SetFlying(bFly);
}

void SmartAI::SetSwimm(bool bSwimm)
{
    if(bSwimm)
        me->AddUnitMovementFlag(MOVEMENTFLAG_SWIMMING);
    else
        me->RemoveUnitMovementFlag(MOVEMENTFLAG_SWIMMING);
}

void SmartAI::sGossipHello(Player* player)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_GOSSIP_HELLO, player);
}

void SmartAI::sGossipSelect(Player* player, uint32 sender, uint32 action)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_GOSSIP_SELECT, player, sender, action);
}

void SmartAI::sGossipSelectCode(Player* /*pPlayer*/, uint32 /*sender*/, uint32 /*action*/, const char* /*code*/) { }

void SmartAI::sQuestAccept(Player* player, Quest const* quest)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_ACCEPTED_QUEST, player, quest->GetQuestId());
}

void SmartAI::sQuestReward(Player* player, Quest const* quest, uint32 opt)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_REWARD_QUEST, player, quest->GetQuestId(), opt);
}

bool SmartAI::sOnDummyEffect(Unit* caster, uint32 spellId, SpellEffIndex effIndex)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_DUMMY_EFFECT, caster, spellId, (uint32)effIndex);
    return true;
}

void SmartAI::SetCombatMove(bool on)
{
    if(mCanCombatMove == on)
        return;
    mCanCombatMove = on;
    if(!HasEscortState(SMART_ESCORT_ESCORTING))
    {
        if(on && me->getVictim())
        {
            if(me->GetMotionMaster()->GetCurrentMovementGeneratorType() == IDLE_MOTION_TYPE)
            {
                SetRun(mRun);
                me->GetMotionMaster()->MoveChase(me->getVictim());
                me->CastStop();
            }
        }
        else
        {
            me->StopMoving();
            me->GetMotionMaster()->MoveIdle();
        }
    }
}

void SmartAI::SetFollow(Unit* target, float dist, float angle, uint32 credit, uint32 end, uint32 creditType)
{
    if(!target)
        return;
    SetRun(mRun);
    mFollowGuid = target->GetGUID();
    mFollowDist = dist;
    mFollowAngle = angle;
    mFollowArrivedTimer = 1000;
    mFollowCredit = credit;
    mFollowArrivedEntry = end;
    me->GetMotionMaster()->MoveFollow(target, dist, angle);
    mFollowCreditType = creditType;
}

void SmartAI::SetScript9(SmartScriptHolder &e, uint32 entry, Unit* invoker)
{
    if(invoker)
        GetScript()->mLastInvoker = invoker->GetGUID();
    GetScript()->SetScript9(e, entry);
}

int GetValor()
{
	QueryResult valor = WorldDatabase.PQuery("SELECT start FROM evento_gurubashi_reliquias");

	if (!valor)
		return 0;
	else
	{
		Field *fields = valor->Fetch();
			return fields[0].GetUInt32();
	}
}

int Habilitada()
{
	QueryResult valor4 = WorldDatabase.PQuery("SELECT Deshabilitadas FROM arenas_habilitadas WHERE ID = 1");

	if (!valor4)
		return 0;
	else
	{
		Field *fields = valor4->Fetch();
			return fields[0].GetUInt32();
	}
}

int TextoUnico3()
{
	QueryResult valor5 = WorldDatabase.PQuery("SELECT texto_no_repetido FROM arenas_habilitadas WHERE ID = 1");

	if (!valor5)
		return 0;
	else
	{
		Field *fields = valor5->Fetch();
			return fields[0].GetUInt32();
	}
}

void SmartAI::sOnGameEvent(bool start, uint16 eventId)
{
	//Evento de las 16:30
	if (eventId == 75)
	{
		static bool no_repetir_texto_7 = true;
		if (start)
		{
			if (GetValor() != 1)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET start = 1");
				//if(GetTextoUnico() == 0)
				if(no_repetir_texto_7)
				{
					WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico = 1");
					sWorld->SendAllWorldText("""\xc2\xa1""El |cFFFF6060Cofre de Evento|r ha aparecido en la Arena Gurubashi!", NULL);	
					no_repetir_texto_7 = false;
				}
			}
		}
		else
		{
			if (GetValor() != 0)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET start = 0");
				if(no_repetir_texto_7 == false)
				{
					sWorld->SendGlobalText("El evento por el |cFFFF6060Cofre de Evento|r de Gurubashi ha llegado a su fin. A las 19:30 Hora del Servidor comenzar""\xC3\xA1"" de nuevo. ""\xc2\xa1""Estad atentos!", NULL);	
					WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico = 0");
					no_repetir_texto_7 = true;
				}
			}
		}
	}
	//Evento de las 19:30
	else if (eventId == 76)
	{
		static bool no_repetir_texto_8 = true;
		if (start)
		{
			if (GetValor() != 1)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET start = 1");
				//if(GetTextoUnico() == 0)
				if(no_repetir_texto_8)
				{
					sWorld->SendAllWorldText("""\xc2\xa1""El |cFFFF6060Cofre de Evento|r ha aparecido en la Arena Gurubashi!", NULL);	
					WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico = 1");
					no_repetir_texto_8 = false;
				}
			}
		}
		else
		{
			if (GetValor() != 0)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET start = 0");
				if(no_repetir_texto_8 == false)
				{
					sWorld->SendGlobalText("El evento por el |cFFFF6060Cofre de Evento|r de Gurubashi ha llegado a su fin. A las 23:30 Hora del Servidor comenzar""\xC3\xA1"" de nuevo. ""\xc2\xa1""Estad atentos!", NULL);	
					WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico = 0");
					no_repetir_texto_8 = true;
				}
			}
		}
	}
	//Evento de las 23:30
	else if (eventId == 88)
	{
		static bool no_repetir_texto_10 = true;
		if (start)
		{
			if (GetValor() != 1)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET start = 1");
				//if(GetTextoUnico() == 0)
				if(no_repetir_texto_10)
				{
					sWorld->SendAllWorldText("""\xc2\xa1""El |cFFFF6060Cofre de Evento|r ha aparecido en la Arena Gurubashi!", NULL);	
					WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico = 1");
					no_repetir_texto_10 = false;
				}
			}
		}
		else
		{
			if (GetValor() != 0)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET start = 0");
				if(no_repetir_texto_10 == false)
				{
					sWorld->SendGlobalText("El evento por el |cFFFF6060Cofre de Evento|r de Gurubashi ha llegado a su fin. Ma""\xC3\xB1""ana a las 16:30 Hora del Servidor comenzar""\xC3\xA1"" de nuevo. ""\xc2\xa1""Estad atentos!", NULL);	
					WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico = 0");
					no_repetir_texto_10 = true;
				}
			}
		}
	}
	//Aviso del Evento de las 16:30, 5 minutos antes
	else if (eventId == 77)
	{
		static bool no_repetir_texto = true;
		if (start)
		{
			if (no_repetir_texto)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 1");
				sWorld->SendGlobalText("El |cFFFF6060Cofre de Evento|r de Gurubashi aparecerá en |CFF8000805|R minutos. ""\xc2\xa1""Preparaos para luchar por el!", NULL);	
				no_repetir_texto = false;
			}
		}
		else
		{
			if(no_repetir_texto == false)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 0");
				no_repetir_texto = true;
			}
		}
	}
	//Aviso del Evento de las 19:30, 5 minutos antes
	else if (eventId == 78)
	{
		static bool no_repetir_texto_2 = true;
		if (start)
		{
			if(no_repetir_texto_2)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 1");
				sWorld->SendGlobalText("El |cFFFF6060Cofre de Evento|r de Gurubashi aparecerá en |CFF8000805|R minutos. ""\xc2\xa1""Preparaos para luchar por el!", NULL);	
				no_repetir_texto_2 = false;
			}
		}
		else
		{
			if(no_repetir_texto_2 == false)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 0");
				no_repetir_texto_2 = true;
			}
		}
	}
	//Aviso del Evento de las 23:30, 5 minutos antes
	else if (eventId == 85)
	{
		static bool no_repetir_texto_11 = true;
		if (start)
		{
			if(no_repetir_texto_11)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 1");
				sWorld->SendGlobalText("El |cFFFF6060Cofre de Evento|r de Gurubashi aparecerá en |CFF8000805|R minutos. ""\xc2\xa1""Preparaos para luchar por el!", NULL);	
				no_repetir_texto_11 = false;
			}
		}
		else
		{
			if(no_repetir_texto_11 == false)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 0");
				no_repetir_texto_11 = true;
			}
		}
	}
	//Aviso del Evento de las 16:30, 2 minutos antes
	else if (eventId == 79)
	{
		static bool no_repetir_texto_3 = true;
		if (start)
		{
			if(no_repetir_texto_3)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 1");
				sWorld->SendGlobalText("El |cFFFF6060Cofre de Evento|r de Gurubashi aparecerá en |CFF8000802|R minutos. ""\xc2\xa1""Preparaos para luchar por el!", NULL);	
				no_repetir_texto_3 = false;
			}
		}
		else
		{
			if(no_repetir_texto_3 == false)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 0");
				no_repetir_texto_3 = true;
			}
		}
	}
	//Aviso del Evento de las 19:30, 2 minutos antes
	else if (eventId == 80)
	{
		static bool no_repetir_texto_4 = true;
		if (start)
		{
			if(no_repetir_texto_4)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 1");
				sWorld->SendGlobalText("El |cFFFF6060Cofre de Evento|r de Gurubashi aparecerá en |CFF8000802|R minutos. ""\xc2\xa1""Preparaos para luchar por el!", NULL);	
				no_repetir_texto_4 = false;
			}
		}
		else
		{
			if(no_repetir_texto_4 == false)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 0");
				no_repetir_texto_4 = true;
			}
		}
	}
	//Aviso del Evento de las 23:30, 2 minutos antes
	else if (eventId == 86)
	{
		static bool no_repetir_texto_12 = true;
		if (start)
		{
			if(no_repetir_texto_12)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 1");
				sWorld->SendGlobalText("El |cFFFF6060Cofre de Evento|r de Gurubashi aparecerá en |CFF8000802|R minutos. ""\xc2\xa1""Preparaos para luchar por el!", NULL);	
				no_repetir_texto_12 = false;
			}
		}
		else
		{
			if(no_repetir_texto_12 == false)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 0");
				no_repetir_texto_12 = true;
			}
		}
	}
	//Aviso del Evento de las 16:30, 1 minuto antes
	else if (eventId == 81)
	{
		static bool no_repetir_texto_5 = true;
		if (start)
		{
			if(no_repetir_texto_5)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 1");
				sWorld->SendGlobalText("El |cFFFF6060Cofre de Evento|r de Gurubashi aparecerá en |CFF8000801|R minuto. ""\xc2\xa1""Preparaos para luchar por el!", NULL);	
				no_repetir_texto_5 = false;
			}
		}
		else
		{
			if(no_repetir_texto_5 == false)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 0");
				no_repetir_texto_5 = true;
			}
		}
	}
	//Aviso del Evento de las 19:30, 1 minuto antes
	else if (eventId == 82)
	{
		static bool no_repetir_texto_6 = true;
		if (start)
		{
			if(no_repetir_texto_6)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 1");
				sWorld->SendGlobalText("El |cFFFF6060Cofre de Evento|r de Gurubashi aparecerá en |CFF8000801|R minuto. ""\xc2\xa1""Preparaos para luchar por el!", NULL);	
				no_repetir_texto_6 = false;
			}
		}
		else
		{
			if(no_repetir_texto_6 == false)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 0");
				no_repetir_texto_6 = true;
			}
		}
	}
	//Aviso del Evento de las 23:30, 1 minuto antes
	else if (eventId == 87)
	{
		static bool no_repetir_texto_13 = true;
		if (start)
		{
			if(no_repetir_texto_13)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 1");
				sWorld->SendGlobalText("El |cFFFF6060Cofre de Evento|r de Gurubashi aparecerá en |CFF8000801|R minuto. ""\xc2\xa1""Preparaos para luchar por el!", NULL);	
				no_repetir_texto_13 = false;
			}
		}
		else
		{
			if(no_repetir_texto_13 == false)
			{
				WorldDatabase.PExecute("UPDATE evento_gurubashi_reliquias SET texto_unico2 = 0");
				no_repetir_texto_13 = true;
			}
		}
	}
	//Arenas Deshabilitadas
	else if (eventId == 83)
	{
		if (start)
		{
			if(Habilitada() == 0) //Habilitada
			{
				WorldDatabase.PExecute("UPDATE arenas_habilitadas SET Deshabilitadas = 1"); //Deshabilitar arenas cuando comience el evento
				if(TextoUnico3() == 0)
				{
					WorldDatabase.PExecute("UPDATE arenas_habilitadas SET texto_no_repetido = 1");
					sWorld->SendAllWorldText("|CFFFFFF01La Arenas se han |CFF87CEFAdeshabilitado|R|CFFFFFF01, ya no podréis apuntar a Cola de Arenas.|R", NULL);
					sWorld->SendGlobalText("|CFFFFFF01Las arenas se volverán a habilitar a las 12:00, Hora del Servidor.|R", NULL);
				}
			}
		}
		else if (!start)
		{
			if(Habilitada() == 1) //Deshabilitada
			{
				WorldDatabase.PExecute("UPDATE arenas_habilitadas SET Deshabilitadas = 0"); //Habilitar arenas cuando acabe el evento
				if(TextoUnico3() == 1)
				{
					WorldDatabase.PExecute("UPDATE arenas_habilitadas SET texto_no_repetido = 0");
					sWorld->SendAllWorldText("|CFFFFFF01La Arenas se han |CFF87CEFAhabilitado|R|CFFFFFF01, ya podéis apuntar a Cola de Arenas.|R", NULL);				
					sWorld->SendGlobalText("|CFFFFFF01Las arenas se volverán a deshabilitar a las 04:00 Hora del Servidor.|R", NULL);
				}
			}
		}
	}
	//Evento de Escalada, 5 minutos antes
	else if (eventId == 90)
	{
		static bool no_repetir_texto_21 = true;
		if (start)
		{
			if(no_repetir_texto_21)
			{
				sWorld->SendGlobalText("|CFFFFFF01El |cFFFF6060Evento de Escalada|r comenzará en |CFF8000805|R minutos. Para ir deberéis usar el teleport al Evento de Escalada en el Taxi (que se activará cuando queden 2 minutos).|R", NULL);
				no_repetir_texto_21 = false;
			}
		}
		else if (!start)
		{
			if(no_repetir_texto_21 == false)
			{
				no_repetir_texto_21 = true;
			}
		}
	}
	//Evento de Escalada, 3 minutos antes
	else if (eventId == 91)
	{
		static bool no_repetir_texto_20 = true;
		if (start)
		{
			if(no_repetir_texto_20)
			{
				sWorld->SendGlobalText("|CFFFFFF01El |cFFFF6060Evento de Escalada|r comenzará en |CFF8000803|R minutos. Para ir deberéis usar el teleport al Evento de Escalada en el Taxi (que se activará cuando queden 2 minutos).|R", NULL);
				no_repetir_texto_20 = false;
			}
		}
		else if (!start)
		{
			if(no_repetir_texto_20 == false)
			{
				no_repetir_texto_20 = true;
			}
		}
	}
	//Evento de Escalada 1, apertura del teleport
	else if (eventId == 92)
	{
		static bool no_repetir_texto_15 = true;
		if (start)
		{
			if (no_repetir_texto_15 == true)
			{
				WorldDatabase.PExecute("UPDATE eventos_custom SET Habilitado_Teleport = 1");
				WorldDatabase.PExecute("UPDATE eventos_custom SET evento_escalada1 = 1");

				sWorld->SendAllWorldText("""\xc2\xa1""El |cFFFF6060Evento de Escalada|r ya está activado, ya podéis ir desde el taxi!", NULL);
				no_repetir_texto_15 = false;
			}
		}
		else if (!start)
		{
			if(no_repetir_texto_15 == false)
			{
				WorldDatabase.PExecute("UPDATE eventos_custom SET Habilitado_Teleport = 0");
				WorldDatabase.PExecute("UPDATE eventos_custom SET evento_escalada1 = 0");
				sWorld->SendGlobalText("|CFFFFFF01Se acabó el tiempo, ya nadie más puede ir al |cFFFF6060Evento de Escalada|r.|R", NULL);
				no_repetir_texto_15 = true;
			}
		}
	}
	//Evento Laberinto 1, apertura del teleport
	else if (eventId == 93)
	{
		static bool no_repetir_texto_14 = true;
		if (start)
		{
			if (no_repetir_texto_14)
			{
				WorldDatabase.PExecute("UPDATE eventos_custom SET evento_laberinto1 = 1");
				WorldDatabase.PExecute("UPDATE eventos_custom SET Habilitado_Teleport_laberinto = 1");

				sWorld->SendAllWorldText("""\xc2\xa1""El |cFFFF6060Evento Laberinto|r ya está activado, ya podéis ir desde el taxi!", NULL);
				no_repetir_texto_14 = false;
			}
		}
		else if (!start)
		{
			if(no_repetir_texto_14 == false)
			{
				WorldDatabase.PExecute("UPDATE eventos_custom SET evento_laberinto1 = 0");
				WorldDatabase.PExecute("UPDATE eventos_custom SET Habilitado_Teleport_laberinto = 0");
				
				sWorld->SendGlobalText("|CFFFFFF01Se acabó el tiempo, ya nadie más puede ir al |cFFFF6060Evento Laberinto|r.|R", NULL);
				no_repetir_texto_14 = true;
			}
		}
	}
	//Eventos, expulsar players del evento
	else if (eventId == 94)
	{
		static bool no_repetir_texto_16 = true;
		if (start)
		{
			if (no_repetir_texto_16)
			{
				WorldDatabase.PExecute("UPDATE eventos_custom SET Expulsar_Jugadores = 1");
				no_repetir_texto_16 = false;
			}
		}
		else if (!start)
		{
			WorldDatabase.PExecute("UPDATE eventos_custom SET Expulsar_Jugadores = 0");
			no_repetir_texto_16 = true;
		}
	}
	//Evento de Escalada 2, apertura del teleport
	else if (eventId == 95)
	{
		static bool no_repetir_texto_17 = true;
		if (start)
		{
			if (no_repetir_texto_17)
			{
				WorldDatabase.PExecute("UPDATE eventos_custom SET Habilitado_Teleport = 1");
				WorldDatabase.PExecute("UPDATE eventos_custom SET evento_escalada2 = 1");
					
				sWorld->SendAllWorldText("""\xc2\xa1""El |cFFFF6060Evento de Escalada|r ya está activado, ya podéis ir desde el taxi!", NULL);
				no_repetir_texto_17 = false;
			}
		}
		else if (!start)
		{
			if(no_repetir_texto_17 == false)
			{
				WorldDatabase.PExecute("UPDATE eventos_custom SET Habilitado_Teleport = 0");
				WorldDatabase.PExecute("UPDATE eventos_custom SET evento_escalada2 = 0");
				sWorld->SendGlobalText("|CFFFFFF01Se acabó el tiempo, ya nadie más puede ir al |cFFFF6060Evento de Escalada|r.|R", NULL);
				no_repetir_texto_17 = true;
			}
		}
	}
	else
	{
		GetScript()->ProcessEventsFor(start ? SMART_EVENT_GAME_EVENT_START : SMART_EVENT_GAME_EVENT_END, NULL, eventId);
	}
}

/*
SMART_EVENT_UPDATE_OOC
SMART_EVENT_SPELLHIT
SMART_EVENT_RANGE
SMART_EVENT_RESPAWN
SMART_EVENT_SUMMONED_UNIT
SMART_EVENT_ACCEPTED_QUEST
SMART_EVENT_REWARD_QUEST
SMART_EVENT_TARGET_BUFFED
SMART_EVENT_SUMMON_DESPAWNED
SMART_EVENT_AI_INIT
SMART_EVENT_DATA_SET
SMART_EVENT_TEXT_OVER
SMART_EVENT_TIMED_EVENT_TRIGGERED
SMART_EVENT_UPDATE
SMART_EVENT_LINK
SMART_EVENT_GOSSIP_SELECT
SMART_EVENT_JUST_CREATED
SMART_EVENT_GOSSIP_HELLO
SMART_EVENT_DEATH
*/

int SmartGameObjectAI::Permissible(const GameObject* g)
{
    if(g->GetAIName() == "SmartGameObjectAI")
        return PERMIT_BASE_SPECIAL;
    return PERMIT_BASE_NO;
}

void SmartGameObjectAI::UpdateAI(const uint32 diff)
{
    GetScript()->OnUpdate(diff);
}

void SmartGameObjectAI::InitializeAI()
{
    GetScript()->OnInitialize(go);
    GetScript()->ProcessEventsFor(SMART_EVENT_RESPAWN);
    //Reset();
}

void SmartGameObjectAI::Reset()
{
    GetScript()->OnReset();
}

// Called when a player opens a gossip dialog with the gameobject.
bool SmartGameObjectAI::GossipHello(Player* player)
{
    sLog->outDebug(LOG_FILTER_DATABASE_AI, "SmartGameObjectAI::GossipHello");
    GetScript()->ProcessEventsFor(SMART_EVENT_GOSSIP_HELLO, player, 0, 0, false, NULL, go);
    return false;
}

// Called when a player selects a gossip item in the gameobject's gossip menu.
bool SmartGameObjectAI::GossipSelect(Player* player, uint32 sender, uint32 action)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_GOSSIP_SELECT, player, sender, action, false, NULL, go);
    return false;
}

// Called when a player selects a gossip with a code in the gameobject's gossip menu.
bool SmartGameObjectAI::GossipSelectCode(Player* /*pPlayer*/, uint32 /*sender*/, uint32 /*action*/, const char* /*code*/)
{
    return false;
}

// Called when a player accepts a quest from the gameobject.
bool SmartGameObjectAI::QuestAccept(Player* player, Quest const* quest)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_ACCEPTED_QUEST, player, quest->GetQuestId(), 0, false, NULL, go);
    return false;
}

// Called when a player selects a quest reward.
bool SmartGameObjectAI::QuestReward(Player* player, Quest const* quest, uint32 opt)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_REWARD_QUEST, player, quest->GetQuestId(), opt, false, NULL, go);
    return false;
}

// Called when the dialog status between a player and the gameobject is requested.
uint32 SmartGameObjectAI::GetDialogStatus(Player* /*pPlayer*/) { return 100; }

// Called when the gameobject is destroyed (destructible buildings only).
void SmartGameObjectAI::Destroyed(Player* player, uint32 eventId)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_DEATH, player, eventId, 0, false, NULL, go);
}

void SmartGameObjectAI::SetData(uint32 id, uint32 value)
{
    GetScript()->ProcessEventsFor(SMART_EVENT_DATA_SET, NULL, id, value);
}

void SmartGameObjectAI::SetScript9(SmartScriptHolder &e, uint32 entry, Unit* invoker)
{
    if(invoker)
        GetScript()->mLastInvoker = invoker->GetGUID();
    GetScript()->SetScript9(e, entry);
}

void SmartGameObjectAI::OnGameEvent(bool start, uint16 eventId)
{
	GetScript()->ProcessEventsFor(start ? SMART_EVENT_GAME_EVENT_START : SMART_EVENT_GAME_EVENT_END, NULL, eventId);
}

class SmartTrigger : public AreaTriggerScript
{
    public:
        SmartTrigger() : AreaTriggerScript("SmartTrigger") { }

        bool OnTrigger(Player* pPlayer, AreaTriggerEntry const* trigger)
        {
            if(!pPlayer->isAlive())
                return false;

            sLog->outDebug(LOG_FILTER_DATABASE_AI, "AreaTrigger %u is using SmartTrigger script", trigger->id);
            SmartScript script;
            script.OnInitialize(NULL, trigger);
            script.ProcessEventsFor(SMART_EVENT_AREATRIGGER_ONTRIGGER, pPlayer, trigger->id);
            return true;
        }
};

void AddSC_SmartSCripts()
{
    new SmartTrigger();
}