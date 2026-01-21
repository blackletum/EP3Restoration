//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Test weapon for some ep3 particles (uses the paint gun spray code which looks like it was copied from ep3)
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
// CWeaponIcegun
//-----------------------------------------------------------------------------

class CWeaponParticleTest : public CBaseHLCombatWeapon
{
	DECLARE_CLASS(CWeaponParticleTest, CBaseHLCombatWeapon);
public:
	DECLARE_SERVERCLASS();

	CWeaponParticleTest(void);
	void	PrimaryAttack(void);
	void	ItemPostFrame();
	void	SecondaryAttack();
	void	Precache();
	void	NextParticle();
	void	Spawn();
	// float	WeaponAutoAimScale()	{ return 0.6f; }

	DECLARE_DATADESC();

	CNetworkVar(bool, m_bFiring);
	CNetworkString(m_szParticleName, 260);
	CNetworkVector(endspawn);

	int currentIndex;

};


LINK_ENTITY_TO_CLASS(weapon_particle_test, CWeaponParticleTest);

PRECACHE_WEAPON_REGISTER(weapon_particle_test);

BEGIN_DATADESC(CWeaponParticleTest)
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST(CWeaponParticleTest, DT_WeaponParticleTest)
SendPropBool(SENDINFO(m_bFiring)),
SendPropString(SENDINFO(m_szParticleName)),
//SendPropVector(SENDINFO(endspawn)),
END_SEND_TABLE()

CWeaponParticleTest::CWeaponParticleTest(void)
{
	m_bReloadsSingly = false;
	m_bFiresUnderwater = false;
}

const char* particleTypes[] =
{
"world_water_source",
"world_fuel_source",
"weaponizer_fire",
"water_hose",
"water_hose_splash",
"water",
"tube_water",
"tube_modifier_explosive",
"tube_modified_flechette",
"tube_matter_2",
"tube_fuel",
"tube_flechette",
"tube_fire",
"matter_explosive",
"fuel_hose_splash",
"fuel_hose",
"water_puddle_arcs",
"water_splash_01_droplets",
"water_splash_01_surface3",
"water_splash_01_surface4",
};

void CWeaponParticleTest::Precache()
{
	for (int i = 0; i < 20; i++)
	{
		PrecacheParticleSystem(particleTypes[i]);
	}
	BaseClass::Precache();
}

void CWeaponParticleTest::Spawn()
{
	Precache();

	Q_strncpy(m_szParticleName.GetForModify(), particleTypes[currentIndex], 260);

	BaseClass::Spawn();
}

void CWeaponParticleTest::NextParticle()
{
	if (currentIndex == 19)
	{
		currentIndex = 0;
	}
	else
	{
		currentIndex++;
	}

	Q_strncpy(m_szParticleName.GetForModify(), particleTypes[currentIndex], 260);
}

static void NextParticle()
{
	CBasePlayer* pPlayer = UTIL_GetLocalPlayer();
	if (!pPlayer)
		return;

	CWeaponParticleTest * pPaintGun = dynamic_cast<CWeaponParticleTest*>(pPlayer->GetActiveWeapon());
	if (pPaintGun)
	{
		pPaintGun->NextParticle();
	}
}

static ConCommand nextparticle("nextparticle", NextParticle);

void CWeaponParticleTest::ItemPostFrame()
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (pOwner == NULL)
	{
		return;
	}

	if ((pOwner->m_nButtons & IN_ATTACK))// && (m_flNextPrimaryAttack <= gpGlobals->curtime))
	{
		PrimaryAttack();
	}
	else
	{
		m_bFiring = false;
	}
		
	if ((pOwner->m_nButtons & IN_ATTACK2))// && (m_flNextSecondaryAttack <= gpGlobals->curtime))
	{
		SecondaryAttack();
	}
	//else
	//{
	//	m_bFiring2 = false;
	//}
}

void CWeaponParticleTest::PrimaryAttack(void)
{
	m_bFiring = true;
	//CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	//if (!pOwner)
	//	return;

	// Take the Player's EyeAngles and turn it into a direction
	//Vector vecDir;
	//AngleVectors(pOwner->EyeAngles(), &vecDir);

	// Get the Start/End
	//Vector vecAbsStart = pOwner->EyePosition();
	//Vector vecAbsEnd = vecAbsStart + (vecDir * MAX_TRACE_LENGTH);

	//Ray_t ray;
	//ray.Init(vecAbsStart, vecAbsEnd);

	//trace_t tr; // Create our trace_t class to hold the end result
	// Do the TraceLine, and write our results to our trace_t class, tr.
	//UTIL_TraceRay(ray, MASK_ALL, pOwner, COLLISION_GROUP_NONE, &tr);

	//if (tr.DidHit())
	//{
	//	DispatchParticleEffect("fuel_hose", tr.endpos, pOwner->EyeAngles());
	//}
}

void CWeaponParticleTest::SecondaryAttack(void)
{
	CBasePlayer* pOwner = ToBasePlayer(GetOwner());
	if (!pOwner)
		return;

	// Take the Player's EyeAngles and turn it into a direction
	Vector vecDir;
	AngleVectors(pOwner->EyeAngles(), &vecDir);

	// Get the Start/End
	Vector vecAbsStart = pOwner->EyePosition();
	Vector vecAbsEnd = vecAbsStart + (vecDir * MAX_TRACE_LENGTH);

	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);

	trace_t tr; // Create our trace_t class to hold the end result
	// Do the TraceLine, and write our results to our trace_t class, tr.
	UTIL_TraceRay(ray, MASK_ALL, pOwner, COLLISION_GROUP_NONE, &tr);

	if (tr.DidHit())
	{
		//endspawn = tr.endpos;
		DispatchParticleEffect(m_szParticleName,tr.endpos,pOwner->EyeAngles());
	}



	//m_bFiring2 = true;
}