#pragma once
#include "Effect.h"
#include "Base.h"

class CTP_Bot : public CEffect
{

private:
	explicit CTP_Bot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTP_Bot(const CTP_Bot& rhs);
	virtual ~CTP_Bot();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CTP_Bot*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;
	int                 m_iAlpha = 255;
	


};

