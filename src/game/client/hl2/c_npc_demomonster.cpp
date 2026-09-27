//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "dt_utlvector_recv.h"
#include "bone_setup.h"
#include "c_ai_basenpc.h"
#include "engine/IVDebugOverlay.h"
#include "c_surfacerender.h"

#include "IVRenderView.h"
#include "view_shared.h"
#include "iviewrender.h"

#include "tier0/vprof.h"
#include "soundinfo.h"

// TODO: These should be in public by the time the SDK ships
#if 1
#include "../../common/blobulator/Implicit/ImpDefines.h"
#include "../../common/blobulator/Implicit/ImpRenderer.h"
#include "../../common/blobulator/Implicit/ImpTiler.h"
#include "../../common/blobulator/Implicit/UserFunctions.h"
#else
#include "../common/blobulator/Implicit/ImpDefines.h"
#include "../../common/blobulator/Implicit/SweepRenderer2.h"
#include "../../common/blobulator/Implicit/ImpTiler2.h"
#include "../../common/blobulator/Implicit/UserFunctions2.h"
#endif
// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
#include <blob_networkbypass.h>
#include "c_npc_surface.h"

//Vector lastPoint0Pos;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_NPC_BlobDemoMonster : public  C_NPC_Surface
{
public:
	DECLARE_CLASS(C_NPC_BlobDemoMonster, C_NPC_Surface);
	DECLARE_CLIENTCLASS();
	DECLARE_INTERPOLATION();

	CUtlVector< float > m_flSurfaceV;
	CUtlVector< CInterpolatedVar< float > > m_iv_flSurfaceV;

	C_NPC_BlobDemoMonster()
		: C_NPC_Surface()
	{
		m_flSurfaceV.EnsureCount(MAX_SURFACE_ELEMENTS);
		m_iv_flSurfaceV.EnsureCount(MAX_SURFACE_ELEMENTS);

		for (int i = 0; i < MAX_SURFACE_ELEMENTS; i++)
		{
			IInterpolatedVar* pWatcher = &m_iv_flSurfaceV.Element(i);
			pWatcher->SetDebugName("m_iv_flSurfaceV");
			AddVar(&m_flSurfaceV.Element(i), pWatcher, LATCH_ANIMATION_VAR);
		}
	}

	void SetupParticles()
	{
		int n_particles = 0;
		for (int i = 0; i < m_nActiveParticles; i++)
		{
			ImpParticleWithOneInterpolant* imp_particle = &g_SurfaceRenderParticles[i];
			imp_particle->center = BLOBPARTICLEPOS_INTERP(m_iParticlePositionIndex[i]);
			imp_particle->setFieldScale(BLOBPARTICLERADIUS_INTERP(m_iParticlePositionIndex[i]));
			imp_particle->interpolants1[3] = m_flSurfaceV[i];
			n_particles++;
		}
		g_SurfaceRenderParticles.SetCountNonDestructively(n_particles);
	}

private:
	//C_NPC_Surface( const C_NPC_Surface & ); // not defined, not accessible
};


//-----------------------------------------------------------------------------
// Purpose: setup network receive table
//-----------------------------------------------------------------------------

IMPLEMENT_CLIENTCLASS_DT(C_NPC_BlobDemoMonster, DT_NPC_BlobDemoMonster, CNPC_BlobDemoMonster)
RecvPropUtlVector(
	RECVINFO_UTLVECTOR(m_flSurfaceV),
	MAX_SURFACE_ELEMENTS,
	RecvPropFloat(NULL, 0, sizeof(float))),
END_RECV_TABLE()
