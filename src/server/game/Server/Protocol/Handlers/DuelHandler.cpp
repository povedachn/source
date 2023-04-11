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

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Log.h"
#include "Opcodes.h"
#include "UpdateData.h"
#include "Player.h"

void WorldSession::HandleDuelAcceptedOpcode(WorldPacket& recvPacket)
{
    uint64 guid;
    Player* pl;
    Player* plTarget;

    if(!GetPlayer()->duel)                                  // ignore accept from duel-sender
        return;

    recvPacket >> guid;

    pl       = GetPlayer();
    plTarget = pl->duel->opponent;

    if(pl == pl->duel->initiator || !plTarget || pl == plTarget || pl->duel->startTime != 0 || plTarget->duel->startTime != 0)
        return;

    time_t now = time(NULL);
    pl->duel->startTimer = now;
    plTarget->duel->startTimer = now;

    pl->SendDuelCountdown(3000);
    plTarget->SendDuelCountdown(3000);
	
	if(pl->getPowerType() == POWER_MANA)
		pl->SetPower(POWER_MANA, pl->GetMaxPower(POWER_MANA));
	else if(pl->getPowerType() == POWER_ENERGY)
		pl->SetPower(POWER_ENERGY, pl->GetMaxPower(POWER_ENERGY));

	if(plTarget->getPowerType() == POWER_MANA)
		plTarget->SetPower(POWER_MANA, plTarget->GetMaxPower(POWER_MANA));
	else if(plTarget->getPowerType() == POWER_ENERGY)
		plTarget->SetPower(POWER_ENERGY, plTarget->GetMaxPower(POWER_ENERGY));
		
	if (pl->GetPet())
		pl->GetPet()->SetHealth(pl->GetPet()->GetMaxHealth());
	if (plTarget->GetPet())
		plTarget->GetPet()->SetHealth(plTarget->GetPet()->GetMaxHealth());
		
	if (pl->GetPet())
		if (pl->GetPet()->getPowerType() == POWER_MANA)
			pl->GetPet()->SetPower(POWER_MANA, pl->GetPet()->GetMaxPower(POWER_MANA));
	if (plTarget->GetPet())
		if (plTarget->GetPet()->getPowerType() == POWER_MANA)
			plTarget->GetPet()->SetPower(POWER_MANA, plTarget->GetPet()->GetMaxPower(POWER_MANA));
		
	if (pl->HasAura(66233))
		pl->RemoveAurasDueToSpell(66233);
	if (plTarget->HasAura(66233))
		plTarget->RemoveAurasDueToSpell(66233);
	if (pl->HasAura(25771))
		pl->RemoveAurasDueToSpell(25771);
	if (plTarget->HasAura(25771))
		plTarget->RemoveAurasDueToSpell(25771);
		
	//Custom, script phase duel
	/*pl->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, 0);
	pl->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GM, 0);
	pl->SetPhaseMask(1, true);
	if(pl->GetPet())
	{
		pl->GetPet()->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, 0);
		pl->GetPet()->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GM, 0);
		pl->GetPet()->SetPhaseMask(1, true);
	}
	
	plTarget->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, 0);
	plTarget->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GM, 0);
	plTarget->SetPhaseMask(1, true);
	if(plTarget->GetPet())
	{
		plTarget->GetPet()->m_serverSideVisibility.SetValue(SERVERSIDE_VISIBILITY_GM, 0);
		plTarget->GetPet()->m_serverSideVisibilityDetect.SetValue(SERVERSIDE_VISIBILITY_GM, 0);
		plTarget->GetPet()->SetPhaseMask(1, true);
	}*/
}

void WorldSession::HandleDuelCancelledOpcode(WorldPacket& recvPacket)
{
    // no duel requested
    if(!GetPlayer()->duel)
        return;

    // player surrendered in a duel using /forfeit
    if(GetPlayer()->duel->startTime != 0)
    {
        GetPlayer()->CombatStopWithPets(true);
        if(GetPlayer()->duel->opponent)
            GetPlayer()->duel->opponent->CombatStopWithPets(true);

        GetPlayer()->CastSpell(GetPlayer(), 7267, true);    // beg
        GetPlayer()->DuelComplete(DUEL_WON);
        return;
    }

    // player either discarded the duel using the "discard button"
    // or used "/forfeit" before countdown reached 0
    uint64 guid;
    recvPacket >> guid;

    GetPlayer()->DuelComplete(DUEL_INTERRUPTED);
}
