#pragma once
#include "Arrow.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CSylphChargeArrow : public CArrow
{

	enum { ARROW_IDLE, ARROW_DEATH, ARROW_HIT, ARROW_END };

private:
	explicit CSylphChargeArrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSylphChargeArrow(const CSylphChargeArrow& rhs);
	virtual ~CSylphChargeArrow();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;					
	virtual void	SetUp_OnTerrain(void) override;					
	virtual void	Change_State(void) override;					
	virtual void	Frame_Check(const _float& fTimeDelta) override;

public:
	static CSylphChargeArrow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir,_float Angle);

public:
	void Create_Hit_Effect(void);
	bool Final_Damage(void);
	void Create_Damage_Font(void);
	void Create_Font_List(int damage);


private:
	virtual void Free(void) override;

	bool  m_bDisappear = false;
	float m_AccTime = 0.f;
	float m_AccMaxTime = 15.f;

	float m_fPower = 12170.f;
	int   m_fAttack_Num = 3;
	float m_fRandom_Value = 0.f;
	float m_fDamage = 0.f;
	bool  m_Is_Cri = false;

	list<int> m_Font_List;

	ARROW_TYPE m_Arrow_Type = ARROW_TYPE_CHARGE_SYLPH;

	_float m_Dis = 5.f;

	_vec3  m_Size = { 3.f * PUBLIC_SCALE, 3.f * PUBLIC_SCALE, 1.f };

	
	bool  m_Is_Alter = false;
	bool  m_Is_Alter_2 = false;

	// 타임 활 스킬 
	_float          m_Alert_Time = 0.f;
	bool            m_Is_Alert = false;

};

