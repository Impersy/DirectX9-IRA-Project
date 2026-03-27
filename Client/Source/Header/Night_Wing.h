#pragma once
#include "Effect.h"
#include "Base.h"

class CNight_Wing : public CEffect
{

private:
	explicit CNight_Wing(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNight_Wing(const CNight_Wing& rhs);
	virtual ~CNight_Wing();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CNight_Wing*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;
	int                 m_iAlpha = 255;
	


};

