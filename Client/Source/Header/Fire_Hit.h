#pragma once
#include "Effect.h"
#include "Base.h"

class CFire_Hit : public CEffect
{

private:
	explicit CFire_Hit(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFire_Hit(const CFire_Hit& rhs);
	virtual ~CFire_Hit();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CFire_Hit*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos, bool IsSmall);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;
	int                 m_iAlpha = 255;
	
	bool                m_Is_Small = false;

};

