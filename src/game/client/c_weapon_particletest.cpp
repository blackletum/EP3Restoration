//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Test weapon for some ep3 particles (uses the paint gun spray code which looks like it was copied from ep3)
//
// $NoKeywords: $
//===========================================================================//

#include "cbase.h"
#include "hud.h"
#include "in_buttons.h"
#include "beamdraw.h"
#include "c_weapon__stubs.h"
#include "basehlcombatweapon_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


class C_WeaponParticleTest : public C_BaseHLCombatWeapon
{
	DECLARE_CLASS(C_WeaponParticleTest, C_BaseHLCombatWeapon);
public:
	C_WeaponParticleTest() {}

	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();

	void OnDataChanged(DataUpdateType_t updateType);

	void StartHoseEffect();
	void StopHoseEffect();

private:
	CUtlReference<CNewParticleEffect> m_pStreamEffect;
	C_WeaponParticleTest(const C_WeaponParticleTest&);

	bool m_bFiring;

	bool m_bFiring2;

	char m_szParticleName[260];

	CUtlReference<CNewParticleEffect> m_pStreamEffect2;
};

STUB_WEAPON_CLASS_IMPLEMENT(weapon_particle_test, C_WeaponParticleTest);

IMPLEMENT_CLIENTCLASS_DT(C_WeaponParticleTest, DT_WeaponParticleTest, CWeaponParticleTest)
RecvPropBool(RECVINFO(m_bFiring)),
RecvPropString(RECVINFO(m_szParticleName)),
END_RECV_TABLE()

void C_WeaponParticleTest::StartHoseEffect()
{
	// Choose the correct particle effect.
	const char* particleEffectName = m_szParticleName;//"fuel_hose";

	Color paintColor;

	C_BasePlayer* pOwner = ToBasePlayer(GetOwner());

	if (!m_pStreamEffect.IsValid())
	{
		const char* pAttachmentName = "muzzle";

		m_pStreamEffect = pOwner->GetViewModel()->ParticleProp()->Create(particleEffectName, PATTACH_POINT_FOLLOW, pAttachmentName);
	}
}

void C_WeaponParticleTest::StopHoseEffect()
{
	if (m_pStreamEffect.IsValid())
	{
		C_BasePlayer* pOwner = ToBasePlayer(GetOwner());

		if (pOwner->GetViewModel())
		{
			pOwner->GetViewModel()->ParticleProp()->StopEmission(m_pStreamEffect);
			m_pStreamEffect = 0;
		}
	}
}

void C_WeaponParticleTest::OnDataChanged(DataUpdateType_t updateType)
{
	BaseClass::OnDataChanged(updateType);
	SetNextClientThink(CLIENT_THINK_ALWAYS);

	if (IsCarrierAlive() && (WeaponState() == WEAPON_IS_ACTIVE))
	{
		if (m_bFiring)
		{
			StartHoseEffect();
		}
		else
		{
			StopHoseEffect();
		}
	}
	else
	{
		StopHoseEffect();
	}
}