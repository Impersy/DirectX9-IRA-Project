#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Docheol_ChargeMark : public CEffect
{
private:
	explicit CEffect_Docheol_ChargeMark(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Docheol_ChargeMark(const CEffect_Docheol_ChargeMark& rhs);
	virtual ~CEffect_Docheol_ChargeMark();

public:
	enum TYPE {CHARGE , SPECIAL};

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vDir, const _vec3& vScale,const _float& fSpeed , const _float& bMove , TYPE eType);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Docheol_ChargeMark*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos, const _vec3& vDir, const _vec3& vScale,const _float& fSpeed , const _float& bMove , TYPE eType = CHARGE);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;

	_float				m_fFrameSpeed = 3.f;
	_float				m_fSpeed = 0.f;

	_bool				m_bMove = false;
	_vec3				m_vDir;

	TYPE				m_eType = CHARGE;
};

