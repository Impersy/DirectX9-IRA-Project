#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CTerrainTex;
class CTransform;
class CTexture;

END

class CTerrain : public Engine::CGameObject
{	
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain();

public:
	virtual HRESULT Ready_GameObject(void) override { return S_OK; }
	virtual HRESULT Ready_GameObject(TERRAIN_TYPE eType);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(TERRAIN_TYPE eType);
	HRESULT			SetUp_Material(void);
	void			Key_Input(const _float& fTimeDelta);

public:
	void	Set_DrawID(_byte id) { m_byDrawID = id; }
	void	Set_DrawOption(_byte option) { m_byDrawOption = option; }
	void	Set_TerrainType(TERRAIN_TYPE eType) { m_eTerrainType = eType; }
	void	Get_TerrainTexKey(_tchar* szBuf);

private:
	CTerrainTex*	m_pBufferCom[TERRAIN_TYPE_END];
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;

	_byte			m_byDrawID;
	_byte			m_byDrawOption;

	TERRAIN_TYPE	m_eTerrainType = TERRAIN_BOSS_DOEWOLE;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev, TERRAIN_TYPE eType);

private:
	virtual void Free(void) override;

};

