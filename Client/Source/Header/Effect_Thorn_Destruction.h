#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Thorn_Destruction : public CEffect
{
private:
	explicit CEffect_Thorn_Destruction(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Thorn_Destruction(const CEffect_Thorn_Destruction& rhs);
	virtual ~CEffect_Thorn_Destruction();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos, const _vec3& vScale);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Thorn_Destruction*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vScale);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;

};

