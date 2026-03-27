#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Player_Charge_Gauge : public CEffect
{

private:
	explicit CEffect_Player_Charge_Gauge(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Player_Charge_Gauge(const CEffect_Player_Charge_Gauge& rhs);
	virtual ~CEffect_Player_Charge_Gauge();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	
private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Player_Charge_Gauge*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_float              m_iAlpha = 0.f;
	_float              m_fSize = 1.f;
	

};

