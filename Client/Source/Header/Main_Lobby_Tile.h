#pragma once
#include "Landscape.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CCubeTex;
class CTransform;
class CTexture;

END

class CMain_Lobby_Tile : public CLandscape
{
private:
	explicit CMain_Lobby_Tile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMain_Lobby_Tile(const CMain_Lobby_Tile& rhs);
	virtual ~CMain_Lobby_Tile();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT		Add_Component(void) override;
	virtual void SetUp_OnTerrain(void){};

private:
	CTexture*			m_pTextureCom;


public:
	static CMain_Lobby_Tile* Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 pos , _float Scale);

private:
	virtual void Free(void) override;
	_vec3 m_vPos;
	_float m_Scale = 0.f;

};

