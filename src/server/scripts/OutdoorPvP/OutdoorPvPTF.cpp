/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Copyright (C) 2012 SymphonyArt <http://symphonyart.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#include "OutdoorPvPTF.h"
#include "OutdoorPvPMgr.h"
#include "OutdoorPvP.h"
#include "WorldPacket.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "Language.h"
#include "World.h"
#include "ScriptPCH.h"

OutdoorPvPTF::OutdoorPvPTF()
{
    m_TypeId = OUTDOOR_PVP_TF;
}

OPvPCapturePointTF::OPvPCapturePointTF(OutdoorPvP *pvp, OutdoorPvPTF_TowerType type)
: OPvPCapturePoint(pvp), m_TowerType(type), m_TowerState(TF_TOWERSTATE_N)
{
    SetCapturePointData(TFCapturePoints[type].entry, TFCapturePoints[type].map, TFCapturePoints[type].x, TFCapturePoints[type].y, TFCapturePoints[type].z, TFCapturePoints[type].o, TFCapturePoints[type].rot0, TFCapturePoints[type].rot1, TFCapturePoints[type].rot2, TFCapturePoints[type].rot3);
}

void OPvPCapturePointTF::FillInitialWorldStates(WorldPacket &data)
{
    data << uint32(TFTowerWorldStates[m_TowerType].n) << uint32(bool(m_TowerState & TF_TOWERSTATE_N));
    data << uint32(TFTowerWorldStates[m_TowerType].h) << uint32(bool(m_TowerState & TF_TOWERSTATE_H));
    data << uint32(TFTowerWorldStates[m_TowerType].a) << uint32(bool(m_TowerState & TF_TOWERSTATE_A));
}

void OutdoorPvPTF::FillInitialWorldStates(WorldPacket &data)
{
    data << TF_UI_TOWER_SLIDER_POS << uint32(50);
    data << TF_UI_TOWER_SLIDER_N << uint32(100);
    data << TF_UI_TOWER_SLIDER_DISPLAY << uint32(0);

    data << TF_UI_TOWER_COUNT_H << m_HordeTowersControlled;
    data << TF_UI_TOWER_COUNT_A << m_AllianceTowersControlled;
    data << TF_UI_TOWERS_CONTROLLED_DISPLAY << uint32(!m_IsLocked);

    data << TF_UI_LOCKED_TIME_MINUTES_FIRST_DIGIT << first_digit;
    data << TF_UI_LOCKED_TIME_MINUTES_SECOND_DIGIT << second_digit;
    data << TF_UI_LOCKED_TIME_HOURS << hours_left;

    data << TF_UI_LOCKED_DISPLAY_NEUTRAL << uint32(m_IsLocked && !m_HordeTowersControlled && !m_AllianceTowersControlled);
    data << TF_UI_LOCKED_DISPLAY_HORDE << uint32(m_IsLocked && (m_HordeTowersControlled > m_AllianceTowersControlled));
    data << TF_UI_LOCKED_DISPLAY_ALLIANCE << uint32(m_IsLocked && (m_HordeTowersControlled < m_AllianceTowersControlled));

    for(OPvPCapturePointMap::iterator itr = m_capturePoints.begin(); itr != m_capturePoints.end(); ++itr)
    {
        itr->second->FillInitialWorldStates(data);
    }
}

void OutdoorPvPTF::SendRemoveWorldStates(Player* player)
{
    player->SendUpdateWorldState(TF_UI_TOWER_SLIDER_POS, uint32(0));
    player->SendUpdateWorldState(TF_UI_TOWER_SLIDER_N, uint32(0));
    player->SendUpdateWorldState(TF_UI_TOWER_SLIDER_DISPLAY, uint32(0));

    player->SendUpdateWorldState(TF_UI_TOWER_COUNT_H, uint32(0));
    player->SendUpdateWorldState(TF_UI_TOWER_COUNT_A, uint32(0));
    player->SendUpdateWorldState(TF_UI_TOWERS_CONTROLLED_DISPLAY, uint32(0));

    player->SendUpdateWorldState(TF_UI_LOCKED_TIME_MINUTES_FIRST_DIGIT, uint32(0));
    player->SendUpdateWorldState(TF_UI_LOCKED_TIME_MINUTES_SECOND_DIGIT, uint32(0));
    player->SendUpdateWorldState(TF_UI_LOCKED_TIME_HOURS, uint32(0));

    player->SendUpdateWorldState(TF_UI_LOCKED_DISPLAY_NEUTRAL, uint32(0));
    player->SendUpdateWorldState(TF_UI_LOCKED_DISPLAY_HORDE, uint32(0));
    player->SendUpdateWorldState(TF_UI_LOCKED_DISPLAY_ALLIANCE, uint32(0));

    for(int i = 0; i < TF_TOWER_NUM; ++i)
    {
        player->SendUpdateWorldState(uint32(TFTowerWorldStates[i].n), uint32(0));
        player->SendUpdateWorldState(uint32(TFTowerWorldStates[i].h), uint32(0));
        player->SendUpdateWorldState(uint32(TFTowerWorldStates[i].a), uint32(0));
    }
}

void OPvPCapturePointTF::UpdateTowerState()
{
    m_PvP->SendUpdateWorldState(uint32(TFTowerWorldStates[m_TowerType].n), uint32(bool(m_TowerState & TF_TOWERSTATE_N)));
    m_PvP->SendUpdateWorldState(uint32(TFTowerWorldStates[m_TowerType].h), uint32(bool(m_TowerState & TF_TOWERSTATE_H)));
    m_PvP->SendUpdateWorldState(uint32(TFTowerWorldStates[m_TowerType].a), uint32(bool(m_TowerState & TF_TOWERSTATE_A)));
}

bool OPvPCapturePointTF::HandlePlayerEnter(Player* player)
{
    if(OPvPCapturePoint::HandlePlayerEnter(player))
    {
        player->SendUpdateWorldState(TF_UI_TOWER_SLIDER_DISPLAY, 1);
        uint32 phase = (uint32)ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f);
        player->SendUpdateWorldState(TF_UI_TOWER_SLIDER_POS, phase);
        player->SendUpdateWorldState(TF_UI_TOWER_SLIDER_N, m_neutralValuePct);
        return true;
    }
    return false;
}

void OPvPCapturePointTF::HandlePlayerLeave(Player* player)
{
    player->SendUpdateWorldState(TF_UI_TOWER_SLIDER_DISPLAY, 0);
    OPvPCapturePoint::HandlePlayerLeave(player);
}

bool OutdoorPvPTF::Update(uint32 diff)
{
    bool changed = OutdoorPvP::Update(diff);

    if(changed)
    {
        if(m_AllianceTowersControlled == TF_TOWER_NUM)
        {
            TeamApplyBuff(TEAM_ALLIANCE, TF_CAPTURE_BUFF);
            m_IsLocked = true;
            SendUpdateWorldState(TF_UI_LOCKED_DISPLAY_NEUTRAL, uint32(0));
            SendUpdateWorldState(TF_UI_LOCKED_DISPLAY_HORDE, uint32(0));
            SendUpdateWorldState(TF_UI_LOCKED_DISPLAY_ALLIANCE, uint32(1));
            SendUpdateWorldState(TF_UI_TOWERS_CONTROLLED_DISPLAY, uint32(0));
        }
        else if(m_HordeTowersControlled == TF_TOWER_NUM)
        {
            TeamApplyBuff(TEAM_HORDE, TF_CAPTURE_BUFF);
            m_IsLocked = true;
            SendUpdateWorldState(TF_UI_LOCKED_DISPLAY_NEUTRAL, uint32(0));
            SendUpdateWorldState(TF_UI_LOCKED_DISPLAY_HORDE, uint32(1));
            SendUpdateWorldState(TF_UI_LOCKED_DISPLAY_ALLIANCE, uint32(0));
            SendUpdateWorldState(TF_UI_TOWERS_CONTROLLED_DISPLAY, uint32(0));
        }
        else
        {
            TeamCastSpell(TEAM_ALLIANCE, -TF_CAPTURE_BUFF);
            TeamCastSpell(TEAM_HORDE, -TF_CAPTURE_BUFF);
        }
        SendUpdateWorldState(TF_UI_TOWER_COUNT_A, m_AllianceTowersControlled);
        SendUpdateWorldState(TF_UI_TOWER_COUNT_H, m_HordeTowersControlled);
    }
    if(m_IsLocked)
    {
        // lock timer is down, release lock
        if(m_LockTimer < diff)
        {
            m_LockTimer = TF_LOCK_TIME;
            m_LockTimerUpdate = 0;
            m_IsLocked = false;
            SendUpdateWorldState(TF_UI_TOWERS_CONTROLLED_DISPLAY, uint32(1));
            SendUpdateWorldState(TF_UI_LOCKED_DISPLAY_NEUTRAL, uint32(0));
            SendUpdateWorldState(TF_UI_LOCKED_DISPLAY_HORDE, uint32(0));
            SendUpdateWorldState(TF_UI_LOCKED_DISPLAY_ALLIANCE, uint32(0));
        }
        else
        {
            // worldstateui update timer is down, update ui with new time data
            if(m_LockTimerUpdate < diff)
            {
                m_LockTimerUpdate = TF_LOCK_TIME_UPDATE;
                uint32 minutes_left = m_LockTimer / 60000;
                hours_left = minutes_left / 60;
                minutes_left -= hours_left * 60;
                second_digit = minutes_left % 10;
                first_digit = minutes_left / 10;

                SendUpdateWorldState(TF_UI_LOCKED_TIME_MINUTES_FIRST_DIGIT, first_digit);
                SendUpdateWorldState(TF_UI_LOCKED_TIME_MINUTES_SECOND_DIGIT, second_digit);
                SendUpdateWorldState(TF_UI_LOCKED_TIME_HOURS, hours_left);
            } else m_LockTimerUpdate -= diff;
            m_LockTimer -= diff;
        }
    }
    return changed;
}

void OutdoorPvPTF::HandlePlayerEnterZone(Player* player, uint32 zone)
{
    if(player->GetTeam() == ALLIANCE)
    {
        if(m_AllianceTowersControlled >= TF_TOWER_NUM)
            player->CastSpell(player, TF_CAPTURE_BUFF, true);
    }
    else
    {
        if(m_HordeTowersControlled >= TF_TOWER_NUM)
            player->CastSpell(player, TF_CAPTURE_BUFF, true);
    }
    OutdoorPvP::HandlePlayerEnterZone(player, zone);
}

void OutdoorPvPTF::HandlePlayerLeaveZone(Player* player, uint32 zone)
{
    // remove buffs
    player->RemoveAurasDueToSpell(TF_CAPTURE_BUFF);
    OutdoorPvP::HandlePlayerLeaveZone(player, zone);
}

bool OutdoorPvPTF::SetupOutdoorPvP()
{
    m_AllianceTowersControlled = 0;
    m_HordeTowersControlled = 0;

    m_IsLocked = false;
    m_LockTimer = TF_LOCK_TIME;
    m_LockTimerUpdate = 0;
    hours_left = 6;
    second_digit = 0;
    first_digit = 0;

    // add the zones affected by the pvp buff
    for(uint8 i = 0; i < OutdoorPvPTFBuffZonesNum; ++i)
        RegisterZone(OutdoorPvPTFBuffZones[i]);

    AddCapturePoint(new OPvPCapturePointTF(this, TF_TOWER_NW));
    AddCapturePoint(new OPvPCapturePointTF(this, TF_TOWER_N));
    AddCapturePoint(new OPvPCapturePointTF(this, TF_TOWER_NE));
    AddCapturePoint(new OPvPCapturePointTF(this, TF_TOWER_SE));
    AddCapturePoint(new OPvPCapturePointTF(this, TF_TOWER_S));

    return true;
}

bool OPvPCapturePointTF::Update(uint32 diff)
{
    // can update even in locked state if gathers the controlling faction
    bool canupdate = ((((OutdoorPvPTF*)m_PvP)->m_AllianceTowersControlled > 0) && m_activePlayers[0].size() > m_activePlayers[1].size()) ||
            ((((OutdoorPvPTF*)m_PvP)->m_HordeTowersControlled > 0) && m_activePlayers[0].size() < m_activePlayers[1].size());
    // if gathers the other faction, then only update if the pvp is unlocked
    canupdate = canupdate || !((OutdoorPvPTF*)m_PvP)->m_IsLocked;
    return canupdate && OPvPCapturePoint::Update(diff);
}

void OPvPCapturePointTF::ChangeState()
{
    // if changing from controlling alliance to horde
    if(m_OldState == OBJECTIVESTATE_ALLIANCE)
    {
        if(((OutdoorPvPTF*)m_PvP)->m_AllianceTowersControlled)
            ((OutdoorPvPTF*)m_PvP)->m_AllianceTowersControlled--;
        sWorld->SendZoneText(OutdoorPvPTFBuffZones[0], sObjectMgr->GetTrinityStringForDBCLocale(LANG_OPVP_TF_LOSE_A));
    }
    // if changing from controlling horde to alliance
    else if(m_OldState == OBJECTIVESTATE_HORDE)
    {
        if(((OutdoorPvPTF*)m_PvP)->m_HordeTowersControlled)
            ((OutdoorPvPTF*)m_PvP)->m_HordeTowersControlled--;
        sWorld->SendZoneText(OutdoorPvPTFBuffZones[0], sObjectMgr->GetTrinityStringForDBCLocale(LANG_OPVP_TF_LOSE_H));
    }

    uint32 artkit = 21;

    switch(m_State)
    {
    case OBJECTIVESTATE_ALLIANCE:
        m_TowerState = TF_TOWERSTATE_A;
        artkit = 2;
        if(((OutdoorPvPTF*)m_PvP)->m_AllianceTowersControlled<TF_TOWER_NUM)
            ((OutdoorPvPTF*)m_PvP)->m_AllianceTowersControlled++;
        sWorld->SendZoneText(OutdoorPvPTFBuffZones[0], sObjectMgr->GetTrinityStringForDBCLocale(LANG_OPVP_TF_CAPTURE_A));
        for(PlayerSet::iterator itr = m_activePlayers[0].begin(); itr != m_activePlayers[0].end(); ++itr)
            (*itr)->AreaExploredOrEventHappens(TF_ALLY_QUEST);
        break;
    case OBJECTIVESTATE_HORDE:
        m_TowerState = TF_TOWERSTATE_H;
        artkit = 1;
        if(((OutdoorPvPTF*)m_PvP)->m_HordeTowersControlled<TF_TOWER_NUM)
            ((OutdoorPvPTF*)m_PvP)->m_HordeTowersControlled++;
        sWorld->SendZoneText(OutdoorPvPTFBuffZones[0], sObjectMgr->GetTrinityStringForDBCLocale(LANG_OPVP_TF_CAPTURE_H));
        for(PlayerSet::iterator itr = m_activePlayers[1].begin(); itr != m_activePlayers[1].end(); ++itr)
            (*itr)->AreaExploredOrEventHappens(TF_HORDE_QUEST);
        break;
    case OBJECTIVESTATE_NEUTRAL:
    case OBJECTIVESTATE_NEUTRAL_ALLIANCE_CHALLENGE:
    case OBJECTIVESTATE_NEUTRAL_HORDE_CHALLENGE:
    case OBJECTIVESTATE_ALLIANCE_HORDE_CHALLENGE:
    case OBJECTIVESTATE_HORDE_ALLIANCE_CHALLENGE:
        m_TowerState = TF_TOWERSTATE_N;
        break;
    }

    GameObject* flag = HashMapHolder<GameObject>::Find(m_capturePointGUID);
    if(flag)
        flag->SetGoArtKit(artkit);

    UpdateTowerState();
}

void OPvPCapturePointTF::SendChangePhase()
{
    // send this too, sometimes the slider disappears, dunno why :(
    SendUpdateWorldState(TF_UI_TOWER_SLIDER_DISPLAY, 1);
    // send these updates to only the ones in this objective
    uint32 phase = (uint32)ceil((m_value + m_maxValue) / (2 * m_maxValue) * 100.0f);
    SendUpdateWorldState(TF_UI_TOWER_SLIDER_POS, phase);
    // send this too, sometimes it resets :S
    SendUpdateWorldState(TF_UI_TOWER_SLIDER_N, m_neutralValuePct);
}

class OutdoorPvP_terokkar_forest : public OutdoorPvPScript
{
    public:
        OutdoorPvP_terokkar_forest() : OutdoorPvPScript("outdoorpvp_tf") { }

        OutdoorPvP* GetOutdoorPvP() const
        {
            return new OutdoorPvPTF();
        }
};

void AddSC_outdoorpvp_tf()
{
    new OutdoorPvP_terokkar_forest;
}
