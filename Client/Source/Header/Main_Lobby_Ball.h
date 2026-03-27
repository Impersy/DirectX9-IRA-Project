#pragma once
#include "Effect.h"
#include "Base.h"

class CMain_Lobby_Ball : public CEffect
{

private:
	explicit CMain_Lobby_Ball(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMain_Lobby_Ball(const CMain_Lobby_Ball& rhs);
	virtual ~CMain_Lobby_Ball();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CMain_Lobby_Ball*		Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 pos , _vec3 scale);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;
	_vec3               m_vScale;
	int                 m_iAlpha = 255;
	bool                m_Is_Reverse = false;


};

