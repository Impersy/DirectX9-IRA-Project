#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Key : public CEffect
{
private:
	explicit CEffect_Key(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Key(const CEffect_Key& rhs);
	virtual ~CEffect_Key();

public:
	virtual HRESULT Ready_GameObject(CGameObject* pOwner);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Key*	Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccumulatedTime = 0.f;

};

