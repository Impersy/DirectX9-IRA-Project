#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_AlertCircle2 : public CEffect
{
private:
	explicit CEffect_AlertCircle2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_AlertCircle2(const CEffect_AlertCircle2& rhs);
	virtual ~CEffect_AlertCircle2();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos, const _vec3& vScale, const _float& fAliveTime, const _float& fTraceTime);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_AlertCircle2* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vScale, const _float& fAliveTime, const _float& fTraceTime);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_float				m_fAliveTime = 0.f;
	_float				m_fTraceTime = 0.f;
	_vec3				m_vScale;
};

