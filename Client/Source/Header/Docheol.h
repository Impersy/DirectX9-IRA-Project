#pragma once
#include "Boss.h"
#include "Base.h"
#include "Effect_Docheol_FireCircle.h"

class CDocheol : public CBoss
{
public:
	enum DOCHEOL_STATE { INTRO1, INTRO2, IDLE, SWING, TWOHANDSLAM , FULLSWING, RAGE_UP, BOSS_DEAD, 
						RAGE_IDLE , RAGE_SWING, RAGE_TWOHANDSLAM, RAGE_CHARGE, RAGE_AREASLAM, SPECIAL_PRATTERN, STATE_END };
	enum DOCHEOL_TEXTURE { TEX_APPEAR, TEX_HANDUP_APPEAR,  TEX_IDLE, TEX_SWING, TEX_TWOHANDSLAM ,TEX_FULLSWING , TEX_RAGE_UP,
							TEX_RAGE_IDLE , TEX_RAGE_SWING, TEX_RAGE_TWOHANDSLAM, TEX_RAGE_CHARGE, TEX_RAGE_SLAMSHAKE,TEX_DEAD,   TEX_END };

private:
	explicit CDocheol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDocheol(const CDocheol& rhs);
	virtual ~CDocheol();

public:
	void			Set_State(DOCHEOL_STATE _eState) { m_eCurState = _eState; }
	void			Set_vTemp(const _vec3& vPos) { m_vTemp[0] = vPos; }
	void			Set_CreateMark(_bool b) { m_bCreateMark = b; }
	void			Set_Hp_Up(_int iHP) { m_iBossCurHP += iHP; }

	DOCHEOL_STATE	Get_State() { return m_eCurState; }
	_bool			Get_BulletGo() { return m_bBulletGo; }
	_bool			Get_ChargeBulletDead() { return m_bChargeBulletDead; }

	void			Add_PosList(const _vec3& vPos) { m_PosList.push_back(vPos); }

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	LateUpdate_GameObject();
	virtual void	Render_GameObject() override;

public:
	static CDocheol*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	void			Initialize_Variable();

private:
	virtual void Free(void) override;

private:
	CTexture*			m_pTextureCom[TEX_END];

	DOCHEOL_STATE		m_eCurState;
	DOCHEOL_STATE		m_ePreState;
	_float				m_fAccTime = 0.f;

	// Hit 관련
	bool            m_HitBlend = false;
	bool            m_HitBlendRender = false;
	float           m_HitBlendFrame = 0.f;
	float           m_HitMaxFrame = 2.f;
	int             m_HitBlendCnt = 0;
	int             m_HitBlendMaxCnt = 1;

	DWORD           AlphaValue = 100;
	DWORD           R = 255;
	DWORD           G = 255;
	DWORD           B = 255;

private:
	void			Intro_HandUpAppear(const _float& fTimeDelta);
	void			Intro_Appear(const _float& fTimeDelta);
	void			Idle(const _float& fTimeDelta);
	void			Swing(const float& fTimeDelta);
	void			TwoHandSlam(const float& fTimeDelta);
	void			FullSwing(const float& fTimeDelta);
	void			Rage_Up(const float& fTimeDelta);
	void			Special_Pattern(const float& fTimeDelta);

	void			Rage_Idle(const float& fTimeDelta);
	void			Rage_Swing(const float& fTimeDelta);
	void			Rage_TwoHandSlam(const float& fTimeDelta);
	void			Rage_Charge(const float& fTimeDelta);
	void			Rage_AreaSlam(const float& fTimeDelta);
	void			Boss_Dead(const _float& fTimeDelta);

private:
	HRESULT			Add_Component(void);
	void			State_Update(const _float& fTimeDelta);

	void            Frame_Manage(const _float& fTimeDelta);
	void			GetDamage_Update(const _float& fTimeDelta);

	void			Create_IntroEffect(const _float& fTimeDelta);
	void			Create_IntroEffect2(const _float& fTimeDelta);
	void			Create_TileEffect(const _float& fTimeDelta);
	void			Create_SwingEffect(const _float& fTimeDelta);
	void			Create_SlamEffect(const _float& fTimeDelta);
	void			Create_SlamAlertEffect(const _float& fTimeDelta);
	void			Create_FullSwingEffect(const _float& fTimeDelta);
	void			Create_ElctronicEffect(const _float& fTimeDelta);
	void			Create_FireBall(const _float& fTimeDelta);
	void			Create_Explosion3();
	void			Create_RageUp_FireEffect(const _float& fTimeDelta);
	void			Create_FireCircleEffect(const _float& fTimeDelta , const _vec3& vScale , CEffect_Docheol_FireCircle::TYPE eType);
	void			Create_Fire3Effect();
	void			Create_CircleEffect();
	void			Create_Mark3();
	void			Create_ElctronicEffect2(const _float& fTimeDelta);
	void			Create_DeathEffect();

	void			Create_SwingBullet(const _float& fTimeDelta);
	void			Create_SlamExplosion(const _float& fTimeDelta);
	void			Create_Meteor(const _float& fTimeDelta);
	void			Create_FullSwingBullet(const _float& fTimeDelta);
	void			Create_RageUpBullet(const _float& fTimeDelta);
	void			Create_ChargeBullet(const _float& fTimeDelta);

	void			Create_AreaSlam(const _float& fTimeDelta);

	void			Create_RageUpGimic(const _float& fTimeDelta);

	void			Create_DoorBullet(const _float& fTimeDelta);
	void			Create_SpecialDoor(const _float& fTimeDelta);
	void			Create_SpecialChargeBullet();

	void			PlayerCheck();

private:
	_uint					m_iPattern = 1;
	_uint					m_iMaxPattern = 5;
	_float					m_fAccTime2 = 0.f;
	_float					m_fAccTime3 = 0.f;

	_bool					m_bMake[10] = {};
	_uint					m_iCnt = 0;
	_uint					m_iCnt2 = 0;
	_vec3					m_vTemp[5];
	_int					m_iHandSlamCnt = 0;
	_int					m_iPatternRelpayCnt = 0;
	_float					m_fSwingEffectAngle = 0.f;
	_float					m_fSwingEffectScale = 0.f;

	_float					m_fFrameSpeed = 0.f;
	_bool					m_bRageUp = false;

	_float					m_fAlphaAngle = 0.f;

	_bool					m_bMotionIdle = false;

	_bool					m_bTest = false;
	_bool					m_bBulletGo = false;
	_bool					m_bChargeBulletDead = false;

	list<_vec3>				m_PosList;

	_bool					m_bCreateMark = false;
	_bool					m_bSlamShake = true;

	_uint					m_iExplosionCnt = 0;

	// 광역 Slam 공격 변수
	_vec3					m_vDir = { 1.f , 0.f , 0.f };
	_float					m_fRadius = 10.f;

	_float					m_fTemp = 0.f;
	_float					m_fAngle = 0.f;

	_uint					m_iRotCnt = 0.f;

	// Special Pattern

	_bool					m_bChargeMotion = true;

	_float					m_fSoundTime = 0.f;

	_bool					m_bSound[55] = {};

	_bool					m_bChargePattern = false;

};