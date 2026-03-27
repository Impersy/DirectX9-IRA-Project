#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Docheol_FireCircle : public CEffect
{
public:
	enum TYPE {INTRO , SLAM};
private:
	explicit CEffect_Docheol_FireCircle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Docheol_FireCircle(const CEffect_Docheol_FireCircle& rhs);
	virtual ~CEffect_Docheol_FireCircle();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale,TYPE _e);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Docheol_FireCircle*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vScale,TYPE _e);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	TYPE				m_eType;

};

