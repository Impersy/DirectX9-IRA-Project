#pragma once
#include "Effect.h"
#include "Base.h"

class CTime_Charge : public CEffect
{

private:
	explicit CTime_Charge(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTime_Charge(const CTime_Charge& rhs);
	virtual ~CTime_Charge();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	
private:
	HRESULT			Add_Component(void);

public:
	static CTime_Charge*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	

};

