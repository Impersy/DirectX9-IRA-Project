#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Doewole_Circle : public CEffect
{
private:
	explicit CEffect_Doewole_Circle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Doewole_Circle(const CEffect_Doewole_Circle& rhs);
	virtual ~CEffect_Doewole_Circle();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Doewole_Circle*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;

};

