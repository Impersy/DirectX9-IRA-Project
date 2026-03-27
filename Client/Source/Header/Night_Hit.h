#pragma once
#include "Effect.h"
#include "Base.h"

class CNight_Hit : public CEffect
{
	
private:
	explicit CNight_Hit(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNight_Hit(const CNight_Hit& rhs);
	virtual ~CNight_Hit();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CNight_Hit*		Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 vPos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;

public:
	bool                m_Is_Boss2_Apostle = false;

};

