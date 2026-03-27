#pragma once
#include "Effect.h"
#include "Base.h"



class CNight_Big : public CEffect
{
	
private:
	explicit CNight_Big(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNight_Big(const CNight_Big& rhs);
	virtual ~CNight_Big();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CNight_Big*		Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 vPos);

	bool Final_Damage(void);
	void Create_Hit_Effect(void);
	void Create_Damage_Font(void);
	void Create_Font_List(int damage);
private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;


	CCollider* m_pColliderCom;


	list<int> m_Font_List;

	float m_fPower = 5358.f;
	int   m_fAttack_Num = 1;
	float m_fRandom_Value = 0.f;
	float m_fDamage = 0.f;

	bool  m_Is_Cri = false;
public:
	list<int> m_Damage_List;
	list<bool> m_Cri_List;
	bool m_bHit = false;
	vector<_vec3> TargetPos_List;

	_float m_fTime = 3.f;
	bool   m_bHiting = false;


};

