#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Tree_Hit : public CEffect
{
private:
	explicit CEffect_Tree_Hit(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Tree_Hit(const CEffect_Tree_Hit& rhs);
	virtual ~CEffect_Tree_Hit();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos, _bool bIsLast);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Tree_Hit*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _bool bIsLast = false);

private:
	virtual void Free(void) override;
	void	Create_Debris(void);

private:
	_float				m_fAccTime = 0.f;
	_bool				m_bIsLast = false;

};

