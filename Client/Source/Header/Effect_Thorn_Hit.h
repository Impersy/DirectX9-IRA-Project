#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Thorn_Hit : public CEffect
{
private:
	explicit CEffect_Thorn_Hit(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Thorn_Hit(const CEffect_Thorn_Hit& rhs);
	virtual ~CEffect_Thorn_Hit();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Thorn_Hit*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;

};

