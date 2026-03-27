#pragma once
#include "Effect.h"
#include "Base.h"

class CTime_Big : public CEffect
{
	
private:
	explicit CTime_Big(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTime_Big(const CTime_Big& rhs);
	virtual ~CTime_Big();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CTime_Big*		Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 vPos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;
	
	_float          m_Alert_Time = 2.f;
	bool            m_Is_Alert = false;
};

