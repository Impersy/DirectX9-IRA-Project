#pragma once
#include "Effect.h"
#include "Base.h"
#include "Bullet_AxeEliteEvilSoul_Leaf.h"

class CEffect_DoubleLeaf_Bullet_Dead : public CEffect
{
private:
	explicit CEffect_DoubleLeaf_Bullet_Dead(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_DoubleLeaf_Bullet_Dead(const CEffect_DoubleLeaf_Bullet_Dead& rhs);
	virtual ~CEffect_DoubleLeaf_Bullet_Dead();

public:
	virtual HRESULT Ready_GameObject(CGameObject* pOwner, _float fTimeDelta);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_DoubleLeaf_Bullet_Dead*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, _float fTimeDelta);

private:
	virtual void Free(void) override;

private:
	CGameObject*	m_pOwner;
	_float			m_fAccumulatedTime = 0.f;

};

