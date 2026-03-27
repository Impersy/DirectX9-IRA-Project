#pragma once
#include "Landscape.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CDocheol_Tile : public CLandscape
{
private:
	explicit CDocheol_Tile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDocheol_Tile(const CDocheol_Tile& rhs);
	virtual ~CDocheol_Tile();

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
	static CDocheol_Tile* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

