#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Stone_Destruction : public CEffect
{
private:
	explicit CEffect_Stone_Destruction(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Stone_Destruction(const CEffect_Stone_Destruction& rhs);
	virtual ~CEffect_Stone_Destruction();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos, const _vec3& vScale);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Stone_Destruction*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vScale);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;

};

