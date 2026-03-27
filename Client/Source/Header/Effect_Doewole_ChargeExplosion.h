#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Doewole_ChargeExplosion : public CEffect
{
private:
	explicit CEffect_Doewole_ChargeExplosion(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Doewole_ChargeExplosion(const CEffect_Doewole_ChargeExplosion& rhs);
	virtual ~CEffect_Doewole_ChargeExplosion();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Doewole_ChargeExplosion*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;

};

