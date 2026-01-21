//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Basic Teleport Grenade + Weapon Recreation 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "basegrenade_shared.h"
#include "basehlcombatweapon.h"
#include "in_buttons.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


#define GRENADE_MODEL "models/Weapons/w_grenade.mdl"

class CGrenadeTeleport : public CBaseGrenade
{
public:
	DECLARE_CLASS(CGrenadeTeleport, CBaseGrenade);
	//DECLARE_DATADESC();

	void SetVelocity(const Vector& velocity, const AngularImpulse& angVelocity)
	{
		IPhysicsObject* pPhysicsObject = VPhysicsGetObject();
		if (pPhysicsObject)
		{
			pPhysicsObject->AddVelocity(&velocity, &angVelocity);
		}
	}

	void VPhysicsCollision(int index, gamevcollisionevent_t* pEvent)
		//void TeleportTouch(CBaseEntity * pVictim)
	{
		CBaseEntity* pVictim = pEvent->pEntities[!index];

		CBaseCombatCharacter* thrower = GetThrower();

		//if (!pVictim->IsSolid())
		//	return;

		if (pVictim == thrower || pVictim == thrower->GetActiveWeapon())
		{
			DevMsg("Touched thrower");
		}
		else
		{
			Vector pos = GetAbsOrigin();
			thrower->Teleport(&pos, NULL, &vec3_origin);

			//Detonate();

			SetModelName(NULL_STRING);

			m_takedamage = DAMAGE_NO;
			SetThink(&CBaseEntity::SUB_Remove);
			SetTouch(NULL);
			AddSolidFlags(FSOLID_NOT_SOLID);
			SetSolid(SOLID_NONE);
			AddEffects(EF_NODRAW);
			SetAbsVelocity(vec3_origin);
			SetNextThink(gpGlobals->curtime + 0.1f);
			//DevMsg("goodbye");
		}


		BaseClass::VPhysicsCollision(index, pEvent);
	}

	void Precache(void)
	{
		PrecacheModel(GRENADE_MODEL);
	}

	void Spawn(void)
	{
		Precache();

		SetModel(GRENADE_MODEL);
		//SetTouch(&CGrenadeTeleport::TeleportTouch);

		m_takedamage = DAMAGE_YES;
		m_iHealth = 1;

		SetSize(-Vector(4, 4, 4), Vector(4, 4, 4));
		SetCollisionGroup(COLLISION_GROUP_WEAPON);
		CreateVPhysics();

		AddSolidFlags(FSOLID_NOT_STANDABLE);

		BaseClass::Spawn();
	}

	bool CreateVPhysics()
	{
		// Create the object in the physics system
		VPhysicsInitNormal(SOLID_BBOX, 0, false);
		return true;
	}

	int OnTakeDamage(const CTakeDamageInfo& inputInfo)
	{
		// Manually apply vphysics because BaseCombatCharacter takedamage doesn't call back to CBaseEntity OnTakeDamage
		VPhysicsTakeDamage(inputInfo);

		// Grenades only suffer blast damage and burn damage.
		if (!(inputInfo.GetDamageType() & (DMG_BLAST | DMG_BURN)))
			return 0;

		return BaseClass::OnTakeDamage(inputInfo);
	}
};

LINK_ENTITY_TO_CLASS(npc_grenade_teleport, CGrenadeTeleport);


CBaseGrenade* Telegrenade_Create(const Vector& position, const QAngle& angles, const Vector& velocity, const AngularImpulse& angVelocity, CBaseEntity* pOwner)
{
	// Don't set the owner here, or the player can't interact with grenades he's thrown
	CGrenadeTeleport* pGrenade = (CGrenadeTeleport*)CBaseEntity::Create("npc_grenade_teleport", position, angles, pOwner);

	pGrenade->SetVelocity(velocity, angVelocity);
	pGrenade->SetThrower(ToBaseCombatCharacter(pOwner));
	pGrenade->m_takedamage = DAMAGE_EVENTS_ONLY;

	return pGrenade;
}

//-----------------------------------------------------------------------------
// CWeaponTeleport
//-----------------------------------------------------------------------------

class CWeaponTeleport : public CBaseHLCombatWeapon
{
	DECLARE_CLASS(CWeaponTeleport, CBaseHLCombatWeapon);
public:
	DECLARE_SERVERCLASS();

	CWeaponTeleport(void);
	void	PrimaryAttack(void);
	//void	ItemPostFrame();
	void	Precache();
	void	Spawn();
	bool	IsSpecialSuitAbility()
	{//We are a special suit ability
		return true;
	}
	bool	HasAnyAmmo() { return true; }

	DECLARE_DATADESC();

	//CNetworkVector(m_targetPosition);
	//CNetworkVector(m_worldPosition);
	//CNetworkVar(int, m_active);
	//CNetworkVar(int, m_viewModelIndex);

	EHANDLE m_hTeleportGrenade;
};


LINK_ENTITY_TO_CLASS(weapon_teleport, CWeaponTeleport);

PRECACHE_WEAPON_REGISTER(weapon_teleport);

BEGIN_DATADESC(CWeaponTeleport)
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST(CWeaponTeleport, DT_WeaponTeleport)
END_SEND_TABLE()


CWeaponTeleport::CWeaponTeleport(void)
{
	m_bReloadsSingly = false;
	m_bFiresUnderwater = false;
}

void CWeaponTeleport::Precache()
{
	BaseClass::Precache();
}

void CWeaponTeleport::Spawn()
{
	Precache();

	BaseClass::Spawn();
}

/*void CWeaponTeleport::ItemPostFrame()
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
	}
}*/

ConVar teleport_velocity_multiplier("teleport_velocity_multiplier","1000.0");

void CWeaponTeleport::PrimaryAttack(void)
{
	if (!m_hTeleportGrenade)
	{
		CBasePlayer* pOwner = ToBasePlayer(GetOwner());
		if (!pOwner)
			return;
		Vector vecShoot = pOwner->Weapon_ShootPosition();

		Vector forward;

		pOwner->EyeVectors(&forward);

		Vector velocity = forward * teleport_velocity_multiplier.GetFloat();

		m_hTeleportGrenade = Telegrenade_Create(vecShoot, vec3_angle, velocity, vec3_origin, pOwner);
	}
}