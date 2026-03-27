#pragma once
#include "Effect.h"
#include "Base.h"

class CTime_Hit : public CEffect
{
	
private:
	explicit CTime_Hit(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTime_Hit(const CTime_Hit& rhs);
	virtual ~CTime_Hit();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CTime_Hit*		Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 vPos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;

public:
	bool   m_Is_Charge = false;

};

