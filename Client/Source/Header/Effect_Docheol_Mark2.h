#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Docheol_Mark2 : public CEffect
{
private:
	explicit CEffect_Docheol_Mark2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Docheol_Mark2(const CEffect_Docheol_Mark2& rhs);
	virtual ~CEffect_Docheol_Mark2();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale,const _float& fAliveTime);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Docheol_Mark2*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos, const _vec3& vScale,const _float& fAliveTime);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_float				m_fAliveTime = 0.f;

	_float				m_fFrameSpeed = 2.f;

	_bool				m_bFrameReturn = false;

};

