#pragma once
#include "Effect.h"
#include "Base.h"

class CMain_Lobby_Column : public CEffect
{

private:
	explicit CMain_Lobby_Column(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMain_Lobby_Column(const CMain_Lobby_Column& rhs);
	virtual ~CMain_Lobby_Column();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CMain_Lobby_Column* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;
	int                 m_iAlpha = 255;
	


};

