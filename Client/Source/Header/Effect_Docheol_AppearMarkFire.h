#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Docheol_AppearMarkFire : public CEffect
{
private:
	explicit CEffect_Docheol_AppearMarkFire(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Docheol_AppearMarkFire(const CEffect_Docheol_AppearMarkFire& rhs);
	virtual ~CEffect_Docheol_AppearMarkFire();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	void			Create_FloorFireRayUp_Effect();

public:
	static CEffect_Docheol_AppearMarkFire*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;

	_bool				m_bEffectMake = false;

	_bool				m_bSound = false;
};

