#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Docheol_Fire3 : public CEffect
{
private:
	explicit CEffect_Docheol_Fire3(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Docheol_Fire3(const CEffect_Docheol_Fire3& rhs);
	virtual ~CEffect_Docheol_Fire3();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale , const _float& fFrameSpeed);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Docheol_Fire3*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vScale , const _float& fFrameSpeed);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_float				m_fFrameSpeed = 0.f;

};

