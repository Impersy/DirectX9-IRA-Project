#pragma once
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CGreenEvilBat : public CMonster
{
	enum DIRECTION { LEFTTOP, LEFTBOTTOM, RIGHTTOP, RIGHTBOTTOM, DIREND };

private:
	explicit CGreenEvilBat(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGreenEvilBat(const CGreenEvilBat& rhs);
	virtual ~CGreenEvilBat();

public:
	virtual HRESULT Ready_GameObject(void) { return S_OK; }
	virtual HRESULT Ready_GameObject(const _vec3 vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	_vec3*	Get_TargetPos(void) { return &m_vTargetPos; }
	_bool	Get_IsDoubleLeafBulletAttack(void) { return m_bDoubleLeafBulletAttack; }

private:
	virtual HRESULT	Add_Component(void) override;
	void			Create_Coin(void);
	virtual void	SetUp_OnTerrain(void) override;
	virtual void	Change_State(void) {}
	virtual void	Frame_Check(const _float& fTimeDelta) override;

	void	Change_State(const _float& fTimeDelta);
	void	Check_Direction(const _vec3	vDir);
	void	Create_Bullet_DoubleLeaf(void);
	HRESULT	Create_Bullet(void);
	void	Set_Texture(void);
	void	Look_At_Player(void);
	void	Look_Moving_Dir(void);
	void	Create_Effect_Birth_LandMark(void);
	void	Create_Effect_Birth_Charge(void);
	void	Create_Effect_Summon(void);
	void	Create_Effect_Summon_Wave(void);
	void	Move(const _float& fTimeDelta);
	void	MovingShot(const _float& fTimeDelta);


public:
	static CGreenEvilBat* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3	vPos);

private:
	DIRECTION		m_eDir;
	CTexture*		m_pTextureCom[HEAD_END];
	_vec3			m_vScaleOrigin;
	_vec3			m_vTargetPos;

	_float			m_fAccumulatedTime = 0.f;
	_float			m_fAccumulatedTimeBullet = 0.f;
	_float			m_fAccumulatedTimeMove = 0.f;
	_bool			m_bDoubleLeafBulletAttack = false;

	_bool			m_bDeadEffect = false;
	_bool			m_bBirthEffect[BIRTH_END] = { };

	_byte			m_byLastChannel = 1;

private:
	void	Create_Dead_Effect(void);
	virtual void Free(void) override;
};

