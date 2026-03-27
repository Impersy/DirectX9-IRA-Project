#pragma once
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CAxeEliteEvilSoul : public CMonster
{
	enum DIRECTION { LEFTTOP, LEFTBOTTOM, RIGHTTOP, RIGHTBOTTOM, DIREND };

private:
	explicit CAxeEliteEvilSoul(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAxeEliteEvilSoul(const CAxeEliteEvilSoul& rhs);
	virtual ~CAxeEliteEvilSoul();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	_vec3* Get_TargetPos(void) { return &m_vTargetPos; }

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	SetUp_OnTerrain(void) override;
	virtual void	Change_State(void) {}
	virtual void	Frame_Check(const _float& fTimeDelta) override;

	void	Change_State(const _float& fTimeDelta);
	void	Check_Direction(const _vec3	vDir);
	HRESULT	Create_Bullet(void);
	void	Set_Texture(void);
	void	Look_At_Player(void);
	void	Create_Effect_Whirlwind(void);
	void	Create_Effect_Birth_LandMark(void);
	void	Create_Effect_Birth_Charge(void);
	void	Create_Effect_Summon(void);
	void	Create_Effect_Summon_Wave(void);
	void	Create_Effect_Key(void);
	void	Create_Bullet_Whirlwind(void);
	void	Create_Bullet_Leaf(_int iCnt);


public:
	static CAxeEliteEvilSoul* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	DIRECTION		m_eDir;
	CTexture*		m_pTextureComStand[HEAD_END];
	CTexture*		m_pTextureComAttack[HEAD_END];
	CTexture*		m_pTextureComAttackRot;
	CTexture*		m_pTextureComDeath;
	_vec3			m_vScaleOrigin;
	_vec3			m_vTargetPos;

	_float			m_fAccumulatedTime = 0.f;
	_bool			m_bLeafBulletAttack = false;
	_bool			m_bDeadEffect = false;
	_bool			m_bBirthEffect[BIRTH_END] = { };


private:
	void	Create_Dead_Effect(void);
	virtual void Free(void) override;
};

