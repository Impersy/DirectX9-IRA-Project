#pragma once
#include "Effect.h"
#include "Base.h"
#include "AxeEliteEvilSoul.h"

class CEffect_Named_Monster_Dead : public CEffect
{
private:
	explicit CEffect_Named_Monster_Dead(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Named_Monster_Dead(const CEffect_Named_Monster_Dead& rhs);
	virtual ~CEffect_Named_Monster_Dead();

public:
	virtual HRESULT Ready_GameObject(CGameObject* pOwner);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Named_Monster_Dead*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner);

private:
	virtual void Free(void) override;

private:
	CGameObject*		m_pOwner;
	_float				m_fAccumulatedTime = 0.f;

};

