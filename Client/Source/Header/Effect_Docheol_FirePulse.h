#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Docheol_FirePulse : public CEffect
{
private:
	explicit CEffect_Docheol_FirePulse(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Docheol_FirePulse(const CEffect_Docheol_FirePulse& rhs);
	virtual ~CEffect_Docheol_FirePulse();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Docheol_FirePulse*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vScale);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;

};

