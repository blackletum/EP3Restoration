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

Vector lastPoint0Pos;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_NPC_BlobFountain : public  C_NPC_Surface
{
public:
	DECLARE_CLASS(C_NPC_BlobFountain, C_NPC_Surface);
	DECLARE_CLIENTCLASS();
	DECLARE_INTERPOLATION();



	void SetupParticles()
	{
		Vector fountainOrigin = GetRenderOrigin();//?
		int n_particles = 0;
		for (int i = 0; i < m_nActiveParticles; i++)
		{
			ImpParticleWithOneInterpolant* imp_particle = &g_SurfaceRenderParticles[i];
			imp_particle->center = BLOBPARTICLEPOS_INTERP(m_iParticlePositionIndex[i]);
			imp_particle->setFieldScale(BLOBPARTICLERADIUS_INTERP(m_iParticlePositionIndex[i]));
			imp_particle->interpolants1[3] = 0;//m_flSurfaceV[i];
			n_particles++;
		}
		
		
			static float time = 0.0f;

			bool paused = BLOBPARTICLEPOS_INTERP(m_iParticlePositionIndex[0]) == lastPoint0Pos;
			lastPoint0Pos = BLOBPARTICLEPOS_INTERP(m_iParticlePositionIndex[0]); //((CAI_BaseNPC::m_nDebugBits & bits_debugDisableAI) != 0);
			if (!paused) time += 0.1f;

			for (int i = -7; i <= 7; i++)
				for (int j = -7; j <= 7; j++)
				{
					ImpParticleWithOneInterpolant* imp_particle = &g_SurfaceRenderParticles[n_particles++];
					imp_particle->center = fountainOrigin + Vector(i * 2.0f * m_flRadius, j * 2.0f * m_flRadius, 15.0f);
					float dist = sqrtf(sqr(imp_particle->center[0]) + sqr(imp_particle->center[1]));

					imp_particle->center[2] += 2.0f * sin(2.0f * time + 2.0f * dist);
					imp_particle->setFieldScale(1.0f);
					imp_particle->interpolants1[3] = 0.0f;
				}
			for (int i = -2; i <= 2; i++)
				for (int j = -2; j <= 2; j++)
				{
					ImpParticleWithOneInterpolant* imp_particle = &g_SurfaceRenderParticles[n_particles++];
					imp_particle->center = fountainOrigin + Vector(i * 2.0f * m_flRadius, j * 2.0f * m_flRadius, 15.0f - 2.0f * m_flRadius);
					imp_particle->setFieldScale(1.0f);
					imp_particle->interpolants1[3] = 0.0f;
				}
			ImpParticleWithOneInterpolant* imp_particle = &g_SurfaceRenderParticles[n_particles++];
			imp_particle->center = fountainOrigin + Vector(0.0f, 0.0f, 15.0f - 4.0f * m_flRadius);
			imp_particle->setFieldScale(1.0f);
			imp_particle->interpolants1[3] = 0.0f;
		g_SurfaceRenderParticles.SetCountNonDestructively(n_particles);

	}

private:
	//C_NPC_Surface( const C_NPC_Surface & ); // not defined, not accessible
};


//-----------------------------------------------------------------------------
// Purpose: setup network receive table
//-----------------------------------------------------------------------------

IMPLEMENT_CLIENTCLASS_DT(C_NPC_BlobFountain, DT_NPC_BlobFountain, CNPC_BlobFountain)
END_RECV_TABLE()
