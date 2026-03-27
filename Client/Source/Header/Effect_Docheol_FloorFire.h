#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Docheol_FloorFire : public CEffect
{
private:
	explicit CEffect_Docheol_FloorFire(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Docheol_FloorFire(const CEffect_Docheol_FloorFire& rhs);
	virtual ~CEffect_Docheol_FloorFire();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _float& fAliveTime);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Docheol_FloorFire*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _float& fAliveTime);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_int				m_iReplayCnt = 0;

	_float				m_fAliveTime = 0.f;

};

