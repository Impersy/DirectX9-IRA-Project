#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_StoneDestruction_General : public CEffect
{
private:
	explicit CEffect_StoneDestruction_General(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_StoneDestruction_General(const CEffect_StoneDestruction_General& rhs);
	virtual ~CEffect_StoneDestruction_General();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos, _bool bIsLast);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_StoneDestruction_General*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _bool bIsLast = false);

private:
	virtual void Free(void) override;
	void	Create_Debris(void);

private:
	_float				m_fAccTime = 0.f;
	_bool				m_bIsLast = false;

};

