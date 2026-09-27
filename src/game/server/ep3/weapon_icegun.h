//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Basic Icegun Recreation
//
// $NoKeywords: $
//=============================================================================//
#include "baseentity.h"


bool Icegun_IsPlayerIceSurfing();

struct icesphere_t// : public CBaseEntity
{
	

public:
	//DECLARE_CLASS(icesphere_t, CBaseEntity);

	//CWeaponPistol(void);

	//DECLARE_SERVERCLASS();

	Vector m_vecNextDelta;
};

class CIceSculpture : public CBaseEntity
{
public:
	void GetIceSphere(Vector pos, icesphere_t**) {};
	DECLARE_CLASS(CIceSculpture, CBaseEntity);
};