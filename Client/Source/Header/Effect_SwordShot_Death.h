#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_SwordShot_Death : public CEffect
{
private:
	explicit CEffect_SwordShot_Death(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_SwordShot_Death(const CEffect_SwordShot_Death& rhs);
	virtual ~CEffect_SwordShot_Death();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_SwordShot_Death*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;

};

