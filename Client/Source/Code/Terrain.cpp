#include "stdafx.h"
#include "..\Header\Terrain.h"
#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: Engine::CGameObject(rhs)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_eTerrainType(rhs.m_eTerrainType)
{
	m_pBufferCom[m_eTerrainType] = rhs.m_pBufferCom[m_eTerrainType];
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_GameObject(TERRAIN_TYPE eType)
{
	m_eTerrainType = eType;
	Add_Component(eType);

	return S_OK;
}

_int CTerrain::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	if(g_eCurScene == SCENE_BOSS2)
		Engine::Add_RenderGroup(RENDER_ALPHA, this);
	else
		Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

void CTerrain::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CTerrain::Key_Input(const _float& fTimeDelta)
{
	_vec3	vDir;

	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);


}

void CTerrain::Get_TerrainTexKey(_tchar* szBuf)
{
	switch (m_eTerrainType)
	{
	case TERRAIN_BOSS_DOEWOLE:
		lstrcpy(szBuf, L"Proto_TerrainTex");
		break;
	case TERRAIN_REGEN:
		lstrcpy(szBuf, L"Proto_TerrainTex_Regen");
		break;
	case TERRAIN_JAR:
		lstrcpy(szBuf, L"Proto_TerrainTex_Pot");
		break;
	case TERRAIN_NORMAL:
		lstrcpy(szBuf, L"Proto_TerrainTex_Normal");
		break;
	}
}

void CTerrain::Render_GameObject()
{
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	

	if (-1 != m_byDrawID)
		m_pTextureCom->Set_Texture(m_byDrawID);

	if (FAILED(SetUp_Material()))
		return;

	//for(int i = 0; i < 4; ++i)
	//	m_pBufferCom[i]->Render_Buffer();

	m_pBufferCom[m_eTerrainType]->Render_Buffer();

	
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
}

HRESULT CTerrain::Add_Component(TERRAIN_TYPE eType)
{
	Engine::CComponent*		pComponent = nullptr;

	if (TERRAIN_BOSS_DOEWOLE == eType)
	{
		pComponent = m_pBufferCom[TERRAIN_BOSS_DOEWOLE] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex"));
		if (nullptr == m_pBufferCom[TERRAIN_BOSS_DOEWOLE]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex", pComponent });
	}
	else if (TERRAIN_REGEN == eType)
	{
		pComponent = m_pBufferCom[TERRAIN_REGEN] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex_Regen"));
		if (nullptr == m_pBufferCom[TERRAIN_REGEN]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex_Regen", pComponent });
	}
	else if (TERRAIN_TUTORIAL == eType)
	{
		pComponent = m_pBufferCom[TERRAIN_TUTORIAL] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex_Tutorial"));
		if (nullptr == m_pBufferCom[TERRAIN_TUTORIAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex_Tutorial", pComponent });
	}
	else if (TERRAIN_DUNGEONLOBBY == eType)
	{
		pComponent = m_pBufferCom[TERRAIN_DUNGEONLOBBY] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex_DungeonLobby"));
		if (nullptr == m_pBufferCom[TERRAIN_DUNGEONLOBBY]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex_DungeonLobby", pComponent });
	}

	//pComponent = m_pBufferCom[1] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex_Add_Vert"));
	//NULL_CHECK_RETURN(m_pBufferCom[1], E_FAIL);
	//m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex_Add_Vert", pComponent });

	//pComponent = m_pBufferCom[2] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex_Add_Horz"));
	//NULL_CHECK_RETURN(m_pBufferCom[2], E_FAIL);
	//m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex_Add_Horz", pComponent });

	//pComponent = m_pBufferCom[3] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex_Add_Horz"));
	//NULL_CHECK_RETURN(m_pBufferCom[3], E_FAIL);
	//m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex_Add_Horz", pComponent });
	else if (TERRAIN_JAR == eType)
	{
		pComponent = m_pBufferCom[TERRAIN_JAR] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex_Pot"));
		if (nullptr == m_pBufferCom[TERRAIN_JAR]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex_Pot", pComponent });
	}

	else if (TERRAIN_NORMAL == eType)
	{
		pComponent = m_pBufferCom[TERRAIN_NORMAL] = dynamic_cast<CTerrainTex*>(Engine::Clone_ProtoComponent(L"Proto_TerrainTex_Normal"));
		if (nullptr == m_pBufferCom[TERRAIN_NORMAL]) 
			return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Proto_TerrainTex_Normal", pComponent });
	}

 	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
 	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
 
 	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Terrain"));
	if (nullptr == m_pTextureCom) return E_FAIL;
 	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Terrain", pComponent });

	return S_OK;
}

HRESULT CTerrain::SetUp_Material(void)
{
	D3DMATERIAL9			tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);


	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev, TERRAIN_TYPE eType)
{
	CTerrain * pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(eType)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTerrain::Free(void)
{
	__super::Free();
}
