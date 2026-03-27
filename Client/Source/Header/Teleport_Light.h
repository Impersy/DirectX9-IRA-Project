#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Player_Foot : public CEffect
{

private:
	explicit CEffect_Player_Foot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Player_Foot(const CEffect_Player_Foot& rhs);
	virtual ~CEffect_Player_Foot();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Player_Foot*		Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 pos , FOOT_TYPE type , bool rev);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	FOOT_TYPE           m_Foot_Type = FOOT_TYPE_WATER;
	_vec3               m_vPos;
	int                 m_iAlpha = 100;
	bool                m_Is_Reverse = false;


};

