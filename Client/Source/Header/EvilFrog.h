#pragma once
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CEvilFrog : public CMonster
{
	enum DIRECTION { LEFTTOP, LEFTBOTTOM, RIGHTTOP, RIGHTBOTTOM, DIREND };

private:
	explicit CEvilFrog(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEvilFrog(const CEvilFrog& rhs);
	virtual ~CEvilFrog();

public:
	virtual HRESULT Ready_GameObject(void) { return S_OK; }
	virtual HRESULT Ready_GameObject(const _vec3 vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	_vec3*	Get_TargetPos(void) { return &m_vTargetPos; }
	_bool	Get_IsReturnBulletAttack(void) { return m_bReturnBulletAttack; }

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	SetUp_OnTerrain(void) override;
	virtual void	Change_State(void) {}
	virtual void	Frame_Check(const _float& fTimeDelta) override;

	void	Change_State(const _float& fTimeDelta);
	void	Check_Direction(const _vec3	vDir);
	HRESULT	Create_Return_Bullet(const _float& fTimeDelta);
	void	Set_Texture(void);
	void	Look_At_Player(void);
	void	Look_Moving_Dir(void);
	void	Create_Effect_Birth_LandMark(void);
	void	Create_Effect_Birth_Charge(void);
	void	Create_Effect_Summon(void);
	void	Create_Effect_Summon_Wave(void);
	void	Create_Effect_NormalMonster_Bullet_Charge(void);
	void	Move(const _float& fTimeDelta);


public:
	static CEvilFrog* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3	vPos);

	void			Create_Coin(void);

private:
	DIRECTION		m_eDir;
	CTexture*		m_pTextureComStand[HEAD_END];
	CTexture*		m_pTextureComAttack[HEAD_END];
	CTexture*		m_pTextureComJump[HEAD_END];
	_vec3			m_vScaleOrigin;
	_vec3			m_vTargetPos;

	_float			m_fAccumulatedTime = 0.f;
	_float			m_fAccumulatedTimeBullet = 0.f;
	_bool			m_bReturnBulletAttack = false;
	_bool			m_bChargeEffect = false;
	_bool			m_bDeadEffect = false;

	_bool			m_bBirthEffect[BIRTH_END] = { };

	_float			m_fYSpeed = 60.f;
	_bool			m_bJump = false;

private:
	void	Create_Dead_Effect(void);
	virtual void Free(void) override;
};

