#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Player_Bow_Charge : public CEffect
{

private:
	explicit CEffect_Player_Bow_Charge(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Player_Bow_Charge(const CEffect_Player_Bow_Charge& rhs);
	virtual ~CEffect_Player_Bow_Charge();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	
private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Player_Bow_Charge*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	

};

