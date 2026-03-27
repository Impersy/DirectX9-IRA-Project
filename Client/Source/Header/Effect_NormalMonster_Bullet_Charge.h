#pragma once
#include "Effect.h"
#include "Base.h"
#include "AxeEliteEvilSoul.h"

class CEffect_NormalMonster_Bullet_Charge : public CEffect
{
private:
	explicit CEffect_NormalMonster_Bullet_Charge(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_NormalMonster_Bullet_Charge(const CEffect_NormalMonster_Bullet_Charge& rhs);
	virtual ~CEffect_NormalMonster_Bullet_Charge();

public:
	virtual HRESULT Ready_GameObject(CGameObject* pOwner);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_NormalMonster_Bullet_Charge*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner);

private:
	virtual void Free(void) override;

private:
	CGameObject*		m_pOwner;
	_float				m_fAccumulatedTime = 0.f;

};

