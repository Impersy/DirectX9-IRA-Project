#pragma once
#include "Effect.h"
#include "Base.h"

class CTutorial_Arrow : public CEffect
{
private:
	explicit CTutorial_Arrow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTutorial_Arrow(const CTutorial_Arrow& rhs);
	virtual ~CTutorial_Arrow();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CTutorial_Arrow*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vScale);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;

	_float				m_fFrameSpeed = 1.f;

};

