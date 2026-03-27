#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Docheol_Electronic : public CEffect
{
private:
	explicit CEffect_Docheol_Electronic(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Docheol_Electronic(const CEffect_Docheol_Electronic& rhs);
	virtual ~CEffect_Docheol_Electronic();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Docheol_Electronic*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vScale);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_float				m_fAccTime2 = 0.f;

	_vec3				m_vTemp;
};

