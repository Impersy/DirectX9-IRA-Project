#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Docheol_Fire : public CEffect
{
public:
	enum TYPE{INTRO , SLAM ,RAGE_UP , SPECIAL};
private:
	explicit CEffect_Docheol_Fire(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Docheol_Fire(const CEffect_Docheol_Fire& rhs);
	virtual ~CEffect_Docheol_Fire();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale, TYPE _eType , const _float& fDelayTime);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Docheol_Fire*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vScale , TYPE _eType , const _float& fDelayTime = 0.f);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	TYPE				m_eType;

	_float				m_fDelayTime = 0.f;

};

