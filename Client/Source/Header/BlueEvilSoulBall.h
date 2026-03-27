#pragma once
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CBlueEvilSoulBall : public CMonster
{
	enum DIRECTION { LEFTTOP, LEFTBOTTOM, RIGHTTOP, RIGHTBOTTOM, DIREND };
	enum BLUEEVILSOULBALL_STATE { MONSTER_IDLE, MONSTER_READY_ATTACK, MONSTER_MOVE, MONSTER_ATTACK, 
		MONSTER_DASH, MONSTER_SHAKE, MONSTER_READY_ATTACK_4WAY, MONSTER_ATTACK_4WAY, MONSTER_DEAD};

private:
	explicit CBlueEvilSoulBall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBlueEvilSoulBall(const CBlueEvilSoulBall& rhs);
	virtual ~CBlueEvilSoulBall();

public:
	virtual HRESULT Ready_GameObject(void) { return S_OK; }
	virtual HRESULT Ready_GameObject(const _vec3 vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	_vec3*	Get_TargetPos(void) { return &m_vTargetPos; }
	_bool	Get_IsStop(void) { return m_bStop; }

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	SetUp_OnTerrain(void) override;
	virtual void	Change_State(void) {}
	virtual void	Frame_Check(const _float& fTimeDelta) override;

	void	Change_State(const _float& fTimeDelta);
	void	Check_Direction(const _vec3	vDir);
	void	Create_Bullet_Chain();
	void	Set_Texture(void);
	void	Look_At_Player(void);
	void	Look_Moving_Dir(void);
	void	Create_Effect_Birth_LandMark(void);
	void	Create_Effect_Birth_Charge(void);
	void	Create_Effect_Summon(void);
	void	Create_Effect_Summon_Wave(void);
	void	Set_Target_Direction(_float fHeightY = 10.f);	
	void	Check_Target_Distance();	
	void	Move(const _float& fTimeDelta);


public:
	static CBlueEvilSoulBall* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3	vPos);

	void			Create_Coin(void);

private:
	DIRECTION		m_eDir;
	CTexture*		m_pTextureComDeath;
	CTexture*		m_pTextureComIdle;
	_vec3			m_vScaleOrigin;
	_vec3			m_vTargetPos;
	_float			m_fDistanceToTarget = 0.f;

	_float			m_fAccumulatedTime = 0.f;
	_float			m_fAccumulatedTimeMove = 0.f;

	_bool			m_bDeadEffect = false;
	_bool			m_bBirthEffect[BIRTH_END] = { };

	_bool			m_bStop = false;
	BLUEEVILSOULBALL_STATE	m_eState;


private:
	void	Create_Dead_Effect(void);
	virtual void Free(void) override;
};

