#pragma once
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CEvilSoul : public CMonster
{
	enum DIRECTION { LEFTTOP, LEFTBOTTOM, RIGHTTOP, RIGHTBOTTOM, DIREND };

private:
	explicit CEvilSoul(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEvilSoul(const CEvilSoul& rhs);
	virtual ~CEvilSoul();

public:
	virtual HRESULT Ready_GameObject(void) { return S_OK; }
	virtual HRESULT Ready_GameObject(const _vec3 vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;					
	virtual void	SetUp_OnTerrain(void) override;					
	//virtual void	SetUp_OnTerrain(void) {}
	virtual void	Change_State(void) override;					
	virtual void	Frame_Check(const _float& fTimeDelta) override;	

	void	Head_Check(const _vec3	vDir);

	//void	Bullet_Test(void);
	HRESULT	Create_Bullet(void);

	void	Create_Effect_Birth_LandMark(void);
	void	Create_Effect_Birth_Charge(void);
	void	Create_Effect_Summon(void);
	void	Create_Effect_Summon_Wave(void);


public:
	static CEvilSoul* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3	vPos);

private:
	CTexture* m_pTextureCom_135_1;	//45는 상속받은 Tex Ptr에, 135는 현재 클래스의 변수에 저장
	CTexture* m_pTextureCom_135_2;
	MONSTER_HEAD	m_eHead;
	_float			m_fAccumulatedTimeMove = 0.f;

	DIRECTION		m_eDir;

	_bool			m_bBirthEffect[BIRTH_END] = { };
	_float			m_fAccumulatedTime = 0.f;

private:
	void	Create_Dead_Effect(void);
	void	Create_Coin(void);
	virtual void Free(void) override;

};

