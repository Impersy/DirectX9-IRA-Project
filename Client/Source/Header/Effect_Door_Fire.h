#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Door_Fire : public CEffect
{
private:
	explicit CEffect_Door_Fire(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Door_Fire(const CEffect_Door_Fire& rhs);
	virtual ~CEffect_Door_Fire();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale , const _float& fFrameSpeed);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Door_Fire*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vScale , const _float& fFrameSpeed);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_float				m_fFrameSpeed = 0.f;

};

