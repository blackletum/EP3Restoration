
extern int	g_interactionAdvisorImmobilize;
extern int	g_interactionAdvisorRelease;

class CAdvisorCooperationDefaultImpl
{
public:
	bool m_advisor_bCooperate;
	bool m_advisor_bLevitating;
	EHANDLE  m_advisor_hEnt;
	float  m_shadow_flTeleportDist;
	float m_shadow_maxSpeed;
	float m_shadow_maxAngSpeed;
	int m_shadow_flags;
};

bool AdvisorRoaming_IsPullingPlayer(CBasePlayer*);

bool AdvisorRoaming_RemapPlayerPhysDmg(CBasePlayer*, const CTakeDamageInfo& info, CTakeDamageInfo& dmgNew);

bool Advisor_IsLevitating();

bool Advisor_AllowCooperation(bool);