#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Docheol_FireBall : public CEffect
{
public:
	enum TYPE{ONE , TWO , THREE};
private:
	explicit CEffect_Docheol_FireBall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Docheol_FireBall(const CEffect_Docheol_FireBall& rhs);
	virtual ~CEffect_Docheol_FireBall();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale, TYPE _eType);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

	void			Create_Bullet();
	void			Create_MarkEffect();

public:
	static CEffect_Docheol_FireBall*	Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vScale , TYPE _eType );

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_int				m_iCnt = 0;
	TYPE				m_eType;

	_float				m_fRadius = 0.f;
	_float				m_fAngle = 0.f;

	_vec3				m_vCenterPos;

};

