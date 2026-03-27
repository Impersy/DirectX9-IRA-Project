#pragma once
#include "Effect.h"
#include "Base.h"
#include "AxeEliteEvilSoul.h"

class CEffect_Bullet_BackHit : public CEffect
{
private:
	explicit CEffect_Bullet_BackHit(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Bullet_BackHit(const CEffect_Bullet_BackHit& rhs);
	virtual ~CEffect_Bullet_BackHit();

public:
	virtual HRESULT Ready_GameObject(CGameObject* pOwner, const _float& fTimeDelta);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Bullet_BackHit*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _float& fTimeDelta);

private:
	virtual void Free(void) override;

private:
	CGameObject*		m_pOwner;

};

