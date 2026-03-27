#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Player_Arrow_Hit : public CEffect
{
	
private:
	explicit CEffect_Player_Arrow_Hit(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Player_Arrow_Hit(const CEffect_Player_Arrow_Hit& rhs);
	virtual ~CEffect_Player_Arrow_Hit();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Player_Arrow_Hit*		Create(LPDIRECT3DDEVICE9 pGraphicDev , ARROW_TYPE ArrowType , _vec3 vPos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
    ARROW_TYPE          m_Arrow_Type = ARROW_TYPE_SYLPH;
	_vec3               m_vPos;

};

