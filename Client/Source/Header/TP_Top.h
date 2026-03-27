#pragma once
#include "Effect.h"
#include "Base.h"

class CTP_Top : public CEffect
{

private:
	explicit CTP_Top(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTP_Top(const CTP_Top& rhs);
	virtual ~CTP_Top();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CTP_Top*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;
	int                 m_iAlpha = 255;
	


};

