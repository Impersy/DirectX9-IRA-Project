#include "stdafx.h"
#include "..\Header\StaticObject.h"
#include "Export_Function.h"

CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandscape(pGraphicDev)
{
}

CStaticObject::CStaticObject(const CStaticObject& rhs)
	: CLandscape(rhs)
{
}

CStaticObject::~CStaticObject()
{
}

HRESULT CStaticObject::Ready_GameObject(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CStaticObject::Update_GameObject(const _float& fTimeDelta)
{	
	//SetUp_OnTerrain();

	__super::Update_GameObject(fTimeDelta);

	//Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	//if (REGEN_OBJ == m_eID)
	//	Engine::Add_RenderGroup(RENDER_ALPHA, this);


	return 0;
}

void CStaticObject::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();


	//if (REGEN_OBJ == m_eID)
	//{
	//	_vec3	vPos;
	//	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	//	__super::Compute_ViewZ(&vPos);
	//}
}

void CStaticObject::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ***
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// 나중에 지워라 

	if (-1 != m_byDrawID)
		m_pTextureCom[m_eID]->Set_Texture(m_byDrawID);

	m_pBufferCom->Render_Buffer();

	// ***
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// 나중에 지워라
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

STATIC_OBJECT_ID CStaticObject::CompareID(wstring strObjKey)
{
	if (strObjKey == L"MainLobby")
		return MAIN_LOBBY;
	else if (strObjKey == L"BossDowoleObj")
		return BOSS_DOWOLE_OBJ;
	else if (strObjKey == L"RegenObj_")
		return REGEN_OBJ;
	else if (strObjKey == L"TutorialObj_")
		return TUTORIAL_OBJ;
	else if (strObjKey == L"DungeonLobbyObj_")
		return DUNGEONLOBBY_OBJ;
	else if (strObjKey == L"ShopObj_")
		return SHOP_OBJ;
	else if (strObjKey == L"JarMapObj_")
		return JARMAP_OBJ;
	else if (strObjKey == L"NormalObj_")
		return NORMAL_OBJ;

	return STATIC_OBJ_END;
}

HRESULT CStaticObject::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	//pComponent = m_pTextureCom[MAIN_LOBBY] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"MainLobby"));
	// 
	//m_uMapComponent[ID_STATIC].insert({ L"MainLobby", pComponent });

	if (SCENE_BOSS1 == g_eCurScene)
	{
		pComponent = m_pTextureCom[BOSS_DOWOLE_OBJ] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"BossDowoleObj"));
		if (nullptr == m_pTextureCom[BOSS_DOWOLE_OBJ]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"BossDowoleObj", pComponent });
	}
	else if (SCENE_REGEN == g_eCurScene)
	{
		pComponent = m_pTextureCom[REGEN_OBJ] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"RegenObj_"));
		if (nullptr == m_pTextureCom[REGEN_OBJ]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"RegenObj_", pComponent });
	}
	
	else if (SCENE_TUTORIAL == g_eCurScene)
	{
		pComponent = m_pTextureCom[TUTORIAL_OBJ] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"TutorialObj_"));
		if (nullptr == m_pTextureCom[TUTORIAL_OBJ]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"TutorialObj_", pComponent });
	}

	else if (SCENE_DUNGEONLOBBY == g_eCurScene)
	{
		pComponent = m_pTextureCom[DUNGEONLOBBY_OBJ] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DungeonLobbyObj_"));
		if (nullptr == m_pTextureCom[DUNGEONLOBBY_OBJ]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DungeonLobbyObj_", pComponent });
	}

	else if (SCENE_SHOP == g_eCurScene)
	{
		pComponent = m_pTextureCom[SHOP_OBJ] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"ShopObj_"));
		if (nullptr == m_pTextureCom[SHOP_OBJ]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"ShopObj_", pComponent });
	}

	else if (SCENE_POT == g_eCurScene)
	{
		pComponent = m_pTextureCom[JARMAP_OBJ] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"JarMapObj_"));
		if (nullptr == m_pTextureCom[JARMAP_OBJ]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"JarMapObj_", pComponent });
	}

	else if (SCENE_NORMAL == g_eCurScene)
	{
		pComponent = m_pTextureCom[NORMAL_OBJ] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"NormalObj_"));
		if (nullptr == m_pTextureCom[NORMAL_OBJ]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"NormalObj_", pComponent });
	}

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_ProtoComponent(L"Proto_Calculator"));
	if (nullptr == m_pCalculatorCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	return S_OK;
}

void CStaticObject::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Tool", L"MyTerrain", L"Proto_ToolTerrainTex", ID_STATIC));
	if (nullptr == pTerrainBufferCom) return;

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

CStaticObject* CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaticObject* pInstance = new CStaticObject(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStaticObject::Free(void)
{
	__super::Free();
}
