#pragma once
#include "Effect.h"
#include "Base.h"

class CMain_Lobby_Bow : public CEffect
{

private:
	explicit CMain_Lobby_Bow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMain_Lobby_Bow(const CMain_Lobby_Bow& rhs);
	virtual ~CMain_Lobby_Bow();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CMain_Lobby_Bow*		Create(LPDIRECT3DDEVICE9 pGraphicDev, int type, _vec3 pos);
	_vec3               m_vPos;
	int                 m_type = 0;
private:
	virtual void Free(void) override;
	CTexture* m_pTextureCom[6] = {};

private:
	_float				m_fAccTime = 0.f;
	_float				m_fDir = 1.f;
	int                 m_iAlpha = 100;
	bool                m_Is_Reverse = false;
	

};

