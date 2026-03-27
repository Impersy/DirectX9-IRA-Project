#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Jar_Open : public CEffect
{
private:
	explicit CEffect_Jar_Open(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Jar_Open(const CEffect_Jar_Open& rhs);
	virtual ~CEffect_Jar_Open();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Jar_Open*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccumulatedTime = 0.f;

};

