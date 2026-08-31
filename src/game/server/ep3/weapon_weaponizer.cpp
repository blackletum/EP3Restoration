//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Basic Weaponizer Recreation 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "npcevent.h"
#include "basehlcombatweapon.h"
#include "basecombatcharacter.h"
#include "ai_basenpc.h"
#include "player.h"
#include "gamerules.h"
#include "in_buttons.h"
#include "soundent.h"
#include "game.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "te_effect_dispatch.h"
#include "gamestats.h"
#include "particle_parse.h"
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// CWeaponWeaponizer
//-----------------------------------------------------------------------------

class CWeaponWeaponizer : public CBaseHLCombatWeapon
{
	DECLARE_CLASS(CWeaponWeaponizer, CBaseHLCombatWeapon);
public:
	DECLARE_SERVERCLASS();

	CWeaponWeaponizer(void);
	void	PrimaryAttack(void);
	//void	ItemPostFrame();
	void	Precache();
	void	Spawn();

	DECLARE_DATADESC();

	int animIndex = 0;
};


LINK_ENTITY_TO_CLASS(weapon_weaponizer, CWeaponWeaponizer);

PRECACHE_WEAPON_REGISTER(weapon_weaponizer);

BEGIN_DATADESC(CWeaponWeaponizer)
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST(CWeaponWeaponizer, DT_WeaponWeaponizer)
END_SEND_TABLE()



CWeaponWeaponizer::CWeaponWeaponizer(void)
{
	m_bReloadsSingly = false;
	m_bFiresUnderwater = false;
}

void CWeaponWeaponizer::Precache()
{
	BaseClass::Precache();
}

void CWeaponWeaponizer::Spawn()
{
	Precache();

	BaseClass::Spawn();
}

/*void CWeaponWeaponizer::ItemPostFrame()
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (pOwner == NULL)
	{
		return;
	}

	if ((pOwner->m_nButtons & IN_ATTACK) && (m_flNextPrimaryAttack <= gpGlobals->curtime))
	{
		PrimaryAttack();
	}
	else
	{
		//m_active = false;
	}
}*/

int weaponizer_anims[] = {
		ACT_VM_IDLE,
			ACT_VM_DRAW,
				ACT_VM_DRYFIRE,
					ACT_VM_PULLBACK,
	ACT_VM_PRIMARYATTACK,
		ACT_VM_SECONDARYATTACK,
			ACT_VM_RELOAD,
};

void CWeaponWeaponizer::PrimaryAttack(void)
{
	animIndex++;
	if (animIndex >= 7)
		animIndex = 0;
	SendWeaponAnim(weaponizer_anims[animIndex]);
	m_flNextPrimaryAttack = gpGlobals->curtime + 0.5;
}