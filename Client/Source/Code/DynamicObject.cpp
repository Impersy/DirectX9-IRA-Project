#include "stdafx.h"
#include "..\Header\DynamicObject.h"
#include "Export_Function.h"

CDynamicObject::CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandscape(pGraphicDev)
{
}

CDynamicObject::CDynamicObject(const CDynamicObject& rhs)
	: CLandscape(rhs)
{
}

CDynamicObject::~CDynamicObject()
{
}

HRESULT CDynamicObject::Ready_GameObject(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CDynamicObject::Update_GameObject(const _float& fTimeDelta)
{	
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CDynamicObject::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	/*_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);*/
}

void CDynamicObject::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (-1 != m_byDrawID)
		m_pTextureCom[m_eID]->Set_Texture(m_byDrawID);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

DYNAMIC_OBJECT_ID CDynamicObject::CompareID(wstring strObjKey)
{
	if (strObjKey == L"door_")
		return DOOR;

	if (SCENE_BOSS1 == g_eCurScene)
	{
		if (strObjKey == L"spr_bush_01_")
			return BUSH1;
		else if (strObjKey == L"spr_bush_02_")
			return BUSH2;
		else if (strObjKey == L"spr_bush_03_")
			return BUSH3;
		else if (strObjKey == L"spr_bush_04_")
			return BUSH4;
		else if (strObjKey == L"spr_bush_05_")
			return BUSH5;
		else if (strObjKey == L"Spr_Deco_BushFlower01_")
			return BUSH_FLOWER1;
		else if (strObjKey == L"Spr_Deco_BushFlower02_")
			return BUSH_FLOWER2;
		else if (strObjKey == L"Spr_DestructibleObject_Infectionthorn01_")
			return THORN1;
		else if (strObjKey == L"Spr_DestructibleObject_Infectionthorn02_")
			return THORN2;
		else if (strObjKey == L"Spr_DestructibleObject_Infectionthorn03_")
			return THORN3;
		else if (strObjKey == L"Spr_DestructibleObject_Infectionthorn04_")
			return THORN4;
		else if (strObjKey == L"Spr_DestructibleObject_Infectionthorn05_")
			return THORN5;
		else if (strObjKey == L"Spr_DestructibleObject_Infectionthorn06_")
			return THORN6;
	}
	else if (SCENE_REGEN == g_eCurScene)
	{
		if (strObjKey == L"StoneWall01_")
			return STONE_WALL1;
		else if (strObjKey == L"StoneWall02_")
			return STONE_WALL2;
		else if (strObjKey == L"InfectionWall2_")
			return INFECTION_WALL2;
		else if (strObjKey == L"Pillar01_")
			return PILLAR1;
		else if (strObjKey == L"Pillar02_")
			return PILLAR2;
		else if (strObjKey == L"Pillar04_")
			return PILLAR4;
		else if (strObjKey == L"Temple_")
			return TEMPLE1;
		else if (strObjKey == L"Spr_Temple_Body_")
			return TEMPLE_BODY;
		else if (strObjKey == L"Spr_Temple_Head_")
			return TEMPLE_HEAD;
		else if (strObjKey == L"Regen_door_")
			return DOOR;
		else if (strObjKey == L"Regen_SideDoor_")
			return DOOR_SIDEDOOR;
		else if (strObjKey == L"Regen_NormalDoor_Pillar2_")
			return DOOR_PILLAR2_NORMAL;
		else if (strObjKey == L"Regen_NormalDoor_Fire_")
			return DOOR_FIRE_NORMAL;
	}
	else if (SCENE_BOSS2 == g_eCurScene)
	{
		if (strObjKey == L"DocheolObj")
			return FLOATING;
	}
	else if (SCENE_POT == g_eCurScene)
	{
		if (strObjKey == L"Jar_spr_bush_01_")
			return BUSH1;
		else if (strObjKey == L"Jar_spr_bush_02_")
			return BUSH2;
		else if (strObjKey == L"Jar_spr_bush_03_")
			return BUSH3;
		else if (strObjKey == L"Jar_spr_bush_04_")
			return BUSH4;
		else if (strObjKey == L"Jar_spr_bush_05_")
			return BUSH5;
		else if (strObjKey == L"Jar_Spr_Deco_BushFlower01_")
			return BUSH_FLOWER1;
		else if (strObjKey == L"Jar_Spr_Deco_BushFlower02_")
			return BUSH_FLOWER2;
		else if (strObjKey == L"Jar_Spr_Infectionthorn01_")
			return THORN1;
		else if (strObjKey == L"Jar_Spr_Infectionthorn02_")
			return THORN2;
		else if (strObjKey == L"Jar_Spr_Infectionthorn03_")
			return THORN3;
		else if (strObjKey == L"Jar_Spr_Infectionthorn04_")
			return THORN4;
		else if (strObjKey == L"Jar_Spr_Infectionthorn05_")
			return THORN5;
		else if (strObjKey == L"Jar_Spr_Infectionthorn06_")
			return THORN6;
		else if (strObjKey == L"Jar_InfectionWall2_")
			return INFECTION_WALL2;
		else if (strObjKey == L"HistoricSitesObj1_")
			return 	HISTORIC_SHINING1;
		else if (strObjKey == L"HistoricSitesObj2_")
			return 	HISTORIC_SHINING2;
		else if (strObjKey == L"HistoricSitesObj3_")
			return 	HISTORIC_SHINING3;
		else if (strObjKey == L"HistoricSitesObj4_")
			return 	HISTORIC_FLOATING;

		else if (strObjKey == L"Spr_HistoricSites_DecoObject_")
			return 	JAR_OBJ;
		else if (strObjKey == L"Spr_TreasureJar_Rare_Center_")
			return 	OPENJAR_CENTER;
		else if (strObjKey == L"Spr_TreasureJar_Rare_Body_")
			return 	OPENJAR_BODY;
		else if (strObjKey == L"Spr_TreasureJar_Rare_Head_")
			return 	OPENJAR_HEAD;

		else if (strObjKey == L"Jar_door_")
			return DOOR;
		else if (strObjKey == L"Jar_SideDoor_")
			return DOOR_SIDEDOOR;
		else if (strObjKey == L"Jar_NormalDoor_Pillar2_")
			return DOOR_PILLAR2_NORMAL;
		else if (strObjKey == L"Jar_NormalDoor_Fire_")
			return DOOR_FIRE_NORMAL;
		else if (strObjKey == L"Jar_NormalDoor_Head_")
			return DOOR_HEAD_NORMAL;
		else if (strObjKey == L"Jar_NormalDoor_Pillar_")
			return DOOR_PILLAR_NORMAL;
	}

	else if (SCENE_TUTORIAL == g_eCurScene)
	{
		if (strObjKey == L"Tuto_spr_bush_01_")
			return BUSH1;
		else if (strObjKey == L"Tuto_spr_bush_02_")
			return BUSH2;
		else if (strObjKey == L"Tuto_spr_bush_03_")
			return BUSH3;
		else if (strObjKey == L"Tuto_spr_bush_04_")
			return BUSH4;
		else if (strObjKey == L"Tuto_spr_bush_05_")
			return BUSH5;
		else if (strObjKey == L"Tuto_Spr_Deco_BushFlower01_")
			return BUSH_FLOWER1;
		else if (strObjKey == L"Tuto_Spr_Deco_BushFlower02_")
			return BUSH_FLOWER2;
	}
	
	else if (SCENE_DUNGEONLOBBY == g_eCurScene)
	{
		if (strObjKey == L"DLobby_spr_bush_01_")
			return BUSH1;
		else if (strObjKey == L"DLobby_spr_bush_02_")
			return BUSH2;	
		else if (strObjKey == L"DLobby_spr_bush_03_")
			return BUSH3;		
		else if (strObjKey == L"DLobby_spr_bush_04_")
			return BUSH4;		
		else if (strObjKey == L"DLobby_spr_bush_05_")
			return BUSH5;		
		else if (strObjKey == L"DLobby_Spr_Deco_BushFlower01_")
			return BUSH_FLOWER1;
		else if (strObjKey == L"DLobby_Spr_Deco_BushFlower02_")
			return BUSH_FLOWER2;
		else if (strObjKey == L"DLobby_door_")
			return DOOR;
		else if (strObjKey == L"DLobby_BossDoor_Head_")
			return DOOR_HEAD;
		else if (strObjKey == L"DLobby_BossDoor_Pillar_")
			return DOOR_PILLAR;
		else if (strObjKey == L"DLobby_BossDoor_Pillar2_")
			return DOOR_PILLAR2;
		else if (strObjKey == L"DLobby_BossDoor_Fire_")
			return DOOR_FIRE_BOSS;
		else if (strObjKey == L"DLobby_SideDoor_")
			return DOOR_SIDEDOOR;

		else if (strObjKey == L"DLobby_NormalDoor_Head_")
			return DOOR_HEAD_NORMAL;
		else if (strObjKey == L"DLobby_NormalDoor_Pillar_")
			return  DOOR_PILLAR_NORMAL;
		else if (strObjKey == L"DLobby_NormalDoor_Pillar2_")
			return  DOOR_PILLAR2_NORMAL;
		else if (strObjKey == L"DLobby_NormalDoor_Fire_")
			return  DOOR_FIRE_NORMAL;

		else if (strObjKey == L"DLobby_InfectionWall2_")
			return  INFECTION_WALL2;
		else if (strObjKey == L"DLobby_Pillar02_")
			return  PILLAR2;
		else if (strObjKey == L"DLobby_Spr_Infectionthorn01_")
			return  THORN1;
		else if (strObjKey == L"DLobby_Spr_Infectionthorn02_")
			return  THORN2;
		else if (strObjKey == L"DLobby_Spr_Infectionthorn03_")
			return  THORN3;
		else if (strObjKey == L"DLobby_Spr_Infectionthorn04_")
			return  THORN4;
		else if (strObjKey == L"DLobby_Spr_Infectionthorn05_")
			return  THORN5;
		else if (strObjKey == L"DLobby_Spr_Infectionthorn06_")
			return  THORN6;
		else if (strObjKey == L"DLobby_StoneWall02_")
			return  STONE_WALL2;
	}

	else if (SCENE_SHOP == g_eCurScene)
	{
		if (strObjKey == L"Shop_NormalDoor_Pillar2_")
			return  DOOR_PILLAR2_NORMAL;
		else if (strObjKey == L"Shop_NormalDoor_Fire_")
			return  DOOR_FIRE_NORMAL;
		else if (strObjKey == L"Shop_SideDoor_")
			return DOOR_SIDEDOOR;
		else if (strObjKey == L"Shop_door_")
			return DOOR;
		else if (strObjKey == L"Shop_spr_bush_01_")
			return BUSH1;
		else if (strObjKey == L"Shop_spr_bush_02_")
			return BUSH2;
		else if (strObjKey == L"Shop_spr_bush_03_")
			return BUSH3;
		else if (strObjKey == L"Shop_spr_bush_04_")
			return BUSH4;
		else if (strObjKey == L"Shop_spr_bush_05_")
			return BUSH5;
		else if (strObjKey == L"Shop_Spr_Deco_BushFlower01_")
			return BUSH_FLOWER1;
		else if (strObjKey == L"Shop_Spr_Deco_BushFlower02_")
			return BUSH_FLOWER2;
	}

	else if (SCENE_NORMAL == g_eCurScene)
	{
		if (strObjKey == L"Normal_spr_bush_01_")
			return BUSH1;
		else if (strObjKey == L"Normal_spr_bush_02_")
			return BUSH2;
		else if (strObjKey == L"Normal_spr_bush_03_")
			return BUSH3;
		else if (strObjKey == L"Normal_Spr_Deco_BushFlower01_")
			return BUSH_FLOWER1;
		else if (strObjKey == L"Normal_Spr_Deco_BushFlower02_")
			return BUSH_FLOWER2;
		else if (strObjKey == L"Normal_NormalDoor_Pillar2_")
			return  DOOR_PILLAR2_NORMAL;
		else if (strObjKey == L"Normal_NormalDoor_Fire_")
			return  DOOR_FIRE_NORMAL;
		else if (strObjKey == L"Normal_SideDoor_")
			return DOOR_SIDEDOOR;
		else if (strObjKey == L"Normal_NormalDoor_Head_")
			return DOOR_HEAD_NORMAL;
		else if (strObjKey == L"Normal_NormalDoor_Pillar_")
			return  DOOR_PILLAR_NORMAL;
		else if (strObjKey == L"Normal_door_")
			return  DOOR;
	}

	return DYNAMIC_OBJ_END;
}

_int CDynamicObject::Get_MaxFrameCnt(void)
{
	return m_pTextureCom[m_eID]->Get_MaxFrameCnt();
}

void CDynamicObject::Free(void)
{
	__super::Free();
}

HRESULT CDynamicObject::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_ProtoComponent(L"Proto_Calculator"));
	if (nullptr == m_pCalculatorCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });


	if (SCENE_BOSS1 == g_eCurScene)
	{
		pComponent = m_pTextureCom[BUSH1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"spr_bush_01_"));
		if (nullptr == m_pTextureCom[BUSH1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"spr_bush_01_", pComponent });

		pComponent = m_pTextureCom[BUSH2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"spr_bush_02_"));
		if (nullptr == m_pTextureCom[BUSH2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"spr_bush_02_", pComponent });

		pComponent = m_pTextureCom[BUSH3] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"spr_bush_03_"));
		if (nullptr == m_pTextureCom[BUSH3]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"spr_bush_03_", pComponent });

		pComponent = m_pTextureCom[BUSH4] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"spr_bush_04_"));
		if (nullptr == m_pTextureCom[BUSH4]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"spr_bush_04_", pComponent });

		pComponent = m_pTextureCom[BUSH5] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"spr_bush_05_"));
		if (nullptr == m_pTextureCom[BUSH5]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"spr_bush_05_", pComponent });

		pComponent = m_pTextureCom[BUSH_FLOWER1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_Deco_BushFlower01_"));
		if (nullptr == m_pTextureCom[BUSH_FLOWER1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Spr_Deco_BushFlower01_", pComponent });

		pComponent = m_pTextureCom[BUSH_FLOWER2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_Deco_BushFlower02_"));
		if (nullptr == m_pTextureCom[BUSH_FLOWER2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Spr_Deco_BushFlower02_", pComponent });

		pComponent = m_pTextureCom[DOOR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"door_"));
		if (nullptr == m_pTextureCom[DOOR]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"door_", pComponent });
	}
	else if (SCENE_TUTORIAL == g_eCurScene)
	{
		pComponent = m_pTextureCom[BUSH1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Tuto_spr_bush_01_"));
		if (nullptr == m_pTextureCom[BUSH1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Tuto_spr_bush_01_", pComponent });

		pComponent = m_pTextureCom[BUSH2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Tuto_spr_bush_02_"));
		if (nullptr == m_pTextureCom[BUSH2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Tuto_spr_bush_02_", pComponent });

		pComponent = m_pTextureCom[BUSH3] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Tuto_spr_bush_03_"));
		if (nullptr == m_pTextureCom[BUSH3]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Tuto_spr_bush_03_", pComponent });

		pComponent = m_pTextureCom[BUSH4] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Tuto_spr_bush_04_"));
		if (nullptr == m_pTextureCom[BUSH4]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Tuto_spr_bush_04_", pComponent });

		pComponent = m_pTextureCom[BUSH5] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Tuto_spr_bush_05_"));
		if (nullptr == m_pTextureCom[BUSH5]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Tuto_spr_bush_05_", pComponent });

		pComponent = m_pTextureCom[BUSH_FLOWER1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Tuto_Spr_Deco_BushFlower01_"));
		if (nullptr == m_pTextureCom[BUSH_FLOWER1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Tuto_Spr_Deco_BushFlower01_", pComponent });

		pComponent = m_pTextureCom[BUSH_FLOWER2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Tuto_Spr_Deco_BushFlower02_"));
		if (nullptr == m_pTextureCom[BUSH_FLOWER2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Tuto_Spr_Deco_BushFlower02_", pComponent });
	}
	else if (SCENE_REGEN == g_eCurScene)
	{
		// Regen
		pComponent = m_pTextureCom[STONE_WALL1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"StoneWall01_"));
		if (nullptr == m_pTextureCom[STONE_WALL1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"StoneWall01_", pComponent });

		pComponent = m_pTextureCom[STONE_WALL2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"StoneWall02_"));
		if (nullptr == m_pTextureCom[STONE_WALL2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"StoneWall02_", pComponent });

		pComponent = m_pTextureCom[INFECTION_WALL2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"InfectionWall2_"));
		if (nullptr == m_pTextureCom[INFECTION_WALL2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"InfectionWall2_", pComponent });

		pComponent = m_pTextureCom[PILLAR1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Pillar01_"));
		if (nullptr == m_pTextureCom[PILLAR1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Pillar01_", pComponent });

		pComponent = m_pTextureCom[PILLAR2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Pillar02_"));
		if (nullptr == m_pTextureCom[PILLAR2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Pillar02_", pComponent });

		pComponent = m_pTextureCom[PILLAR4] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Pillar04_"));
		if (nullptr == m_pTextureCom[PILLAR4]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Pillar04_", pComponent });

		pComponent = m_pTextureCom[TEMPLE_HEAD] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_Temple_Head_"));
		if (nullptr == m_pTextureCom[TEMPLE_HEAD]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Spr_Temple_Head_", pComponent });

		pComponent = m_pTextureCom[TEMPLE_BODY] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_Temple_Body_"));
		if (nullptr == m_pTextureCom[TEMPLE_BODY]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Spr_Temple_Body_", pComponent });

		pComponent = m_pTextureCom[DOOR_SIDEDOOR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Regen_SideDoor_"));
		if (nullptr == m_pTextureCom[DOOR_SIDEDOOR]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Regen_SideDoor_", pComponent });

		pComponent = m_pTextureCom[DOOR_PILLAR2_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Regen_NormalDoor_Pillar2_"));
		if (nullptr == m_pTextureCom[DOOR_PILLAR2_NORMAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Regen_NormalDoor_Pillar2_", pComponent });

		pComponent = m_pTextureCom[DOOR_FIRE_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Regen_NormalDoor_Fire_"));
		if (nullptr == m_pTextureCom[DOOR_FIRE_NORMAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Regen_NormalDoor_Fire_", pComponent });
	}
	
	else if (SCENE_POT== g_eCurScene)
	{
		pComponent = m_pTextureCom[BUSH1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_spr_bush_01_"));
		if (nullptr == m_pTextureCom[BUSH1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_spr_bush_01_", pComponent });

		pComponent = m_pTextureCom[BUSH2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_spr_bush_02_"));
		if (nullptr == m_pTextureCom[BUSH2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_spr_bush_02_", pComponent });

		pComponent = m_pTextureCom[BUSH3] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_spr_bush_03_"));
		if (nullptr == m_pTextureCom[BUSH3]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_spr_bush_03_", pComponent });

		pComponent = m_pTextureCom[BUSH4] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_spr_bush_04_"));
		if (nullptr == m_pTextureCom[BUSH4]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_spr_bush_04_", pComponent });

		pComponent = m_pTextureCom[BUSH5] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_spr_bush_05_"));
		if (nullptr == m_pTextureCom[BUSH5]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_spr_bush_05_", pComponent });

		pComponent = m_pTextureCom[BUSH_FLOWER1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_Spr_Deco_BushFlower01_"));
		if (nullptr == m_pTextureCom[BUSH_FLOWER1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_Spr_Deco_BushFlower01_", pComponent });

		pComponent = m_pTextureCom[BUSH_FLOWER2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_Spr_Deco_BushFlower02_"));
		if (nullptr == m_pTextureCom[BUSH_FLOWER2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_Spr_Deco_BushFlower02_", pComponent });

		pComponent = m_pTextureCom[HISTORIC_SHINING1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"HistoricSitesObj1_"));
		if (nullptr == m_pTextureCom[HISTORIC_SHINING1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"HistoricSitesObj1_", pComponent });

		pComponent = m_pTextureCom[HISTORIC_SHINING2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"HistoricSitesObj2_"));
		if (nullptr == m_pTextureCom[HISTORIC_SHINING2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"HistoricSitesObj2_", pComponent });

		pComponent = m_pTextureCom[HISTORIC_SHINING3] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"HistoricSitesObj3_"));
		if (nullptr == m_pTextureCom[HISTORIC_SHINING3]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"HistoricSitesObj3_", pComponent });

		pComponent = m_pTextureCom[OPENJAR_CENTER] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_TreasureJar_Rare_Center_"));
		if (nullptr == m_pTextureCom[OPENJAR_CENTER]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Spr_TreasureJar_Rare_Center_", pComponent });

		pComponent = m_pTextureCom[DOOR_SIDEDOOR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_SideDoor_"));
		if (nullptr == m_pTextureCom[DOOR_SIDEDOOR]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_SideDoor_", pComponent });

		pComponent = m_pTextureCom[DOOR_PILLAR2_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_NormalDoor_Pillar2_"));
		if (nullptr == m_pTextureCom[DOOR_PILLAR2_NORMAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_NormalDoor_Pillar2_", pComponent });

		pComponent = m_pTextureCom[DOOR_FIRE_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_NormalDoor_Fire_"));
		if (nullptr == m_pTextureCom[DOOR_FIRE_NORMAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_NormalDoor_Fire_", pComponent });

		pComponent = m_pTextureCom[DOOR_HEAD_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_NormalDoor_Head_"));
		if (nullptr == m_pTextureCom[DOOR_HEAD_NORMAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_NormalDoor_Head_", pComponent });

		pComponent = m_pTextureCom[DOOR_PILLAR_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_NormalDoor_Pillar_"));
		if (nullptr == m_pTextureCom[DOOR_PILLAR_NORMAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_NormalDoor_Pillar_", pComponent });
	}
	
	else if (SCENE_DUNGEONLOBBY == g_eCurScene)
	{
		pComponent = m_pTextureCom[BUSH1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_spr_bush_01_"));
		if (nullptr == m_pTextureCom[BUSH1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_spr_bush_01_", pComponent });

		pComponent = m_pTextureCom[BUSH2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_spr_bush_02_"));
		if (nullptr == m_pTextureCom[BUSH2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_spr_bush_02_", pComponent });

		pComponent = m_pTextureCom[BUSH3] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_spr_bush_03_"));
		if (nullptr == m_pTextureCom[BUSH3]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_spr_bush_03_", pComponent });

		pComponent = m_pTextureCom[BUSH4] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_spr_bush_04_"));
		if (nullptr == m_pTextureCom[BUSH4]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_spr_bush_04_", pComponent });

		pComponent = m_pTextureCom[BUSH5] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_spr_bush_05_"));
		if (nullptr == m_pTextureCom[BUSH5]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_spr_bush_05_", pComponent });

		pComponent = m_pTextureCom[BUSH_FLOWER1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_Spr_Deco_BushFlower01_"));
		if (nullptr == m_pTextureCom[BUSH_FLOWER1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_Spr_Deco_BushFlower01_", pComponent });

		pComponent = m_pTextureCom[BUSH_FLOWER2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_Spr_Deco_BushFlower02_"));
		if (nullptr == m_pTextureCom[BUSH_FLOWER2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_Spr_Deco_BushFlower02_", pComponent });

		pComponent = m_pTextureCom[DOOR_HEAD] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_BossDoor_Head_"));
		if (nullptr == m_pTextureCom[DOOR_HEAD]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_BossDoor_Head_", pComponent });

		pComponent = m_pTextureCom[DOOR_PILLAR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_BossDoor_Pillar_"));
		if (nullptr == m_pTextureCom[DOOR_PILLAR]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_BossDoor_Pillar_", pComponent });

		pComponent = m_pTextureCom[DOOR_PILLAR2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_BossDoor_Pillar2_"));
		if (nullptr == m_pTextureCom[DOOR_PILLAR2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_BossDoor_Pillar2_", pComponent });

		pComponent = m_pTextureCom[DOOR_FIRE_BOSS] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_BossDoor_Fire_"));
		if (nullptr == m_pTextureCom[DOOR_FIRE_BOSS]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_BossDoor_Fire_", pComponent });

		pComponent = m_pTextureCom[DOOR_SIDEDOOR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_SideDoor_"));
		if (nullptr == m_pTextureCom[DOOR_SIDEDOOR]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_SideDoor_", pComponent });

		pComponent = m_pTextureCom[DOOR_HEAD_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_NormalDoor_Head_"));
		if (nullptr == m_pTextureCom[DOOR_HEAD_NORMAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_NormalDoor_Head_", pComponent });

		pComponent = m_pTextureCom[DOOR_PILLAR_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_NormalDoor_Pillar_"));
		if (nullptr == m_pTextureCom[DOOR_PILLAR_NORMAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_NormalDoor_Pillar_", pComponent });

		pComponent = m_pTextureCom[DOOR_PILLAR2_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_NormalDoor_Pillar2_"));
		if (nullptr == m_pTextureCom[DOOR_PILLAR2_NORMAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_NormalDoor_Pillar2_", pComponent });

		pComponent = m_pTextureCom[DOOR_FIRE_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DLobby_NormalDoor_Fire_"));
		if (nullptr == m_pTextureCom[DOOR_FIRE_NORMAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_NormalDoor_Fire_", pComponent });
	}
	else if (SCENE_SHOP == g_eCurScene)
	{
		pComponent = m_pTextureCom[DOOR_SIDEDOOR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Shop_SideDoor_"));
		if (nullptr == m_pTextureCom[DOOR_SIDEDOOR]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_SideDoor_", pComponent });

		pComponent = m_pTextureCom[DOOR_PILLAR2_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Shop_NormalDoor_Pillar2_"));
		if (nullptr == m_pTextureCom[DOOR_PILLAR2_NORMAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_NormalDoor_Pillar2_", pComponent });

		pComponent = m_pTextureCom[DOOR_FIRE_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Shop_NormalDoor_Fire_"));
		if (nullptr == m_pTextureCom[DOOR_FIRE_NORMAL]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DLobby_NormalDoor_Fire_", pComponent });

		pComponent = m_pTextureCom[DOOR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Shop_door_"));
		if (nullptr == m_pTextureCom[DOOR]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Shop_door_", pComponent });

		pComponent = m_pTextureCom[BUSH1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Shop_spr_bush_01_"));
		if (nullptr == m_pTextureCom[BUSH1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Shop_spr_bush_01_", pComponent });

		pComponent = m_pTextureCom[BUSH2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Shop_spr_bush_02_"));
		if (nullptr == m_pTextureCom[BUSH2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Shop_spr_bush_02_", pComponent });

		pComponent = m_pTextureCom[BUSH3] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Shop_spr_bush_03_"));
		if (nullptr == m_pTextureCom[BUSH3]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Shop_spr_bush_03_", pComponent });

		pComponent = m_pTextureCom[BUSH4] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Shop_spr_bush_04_"));
		if (nullptr == m_pTextureCom[BUSH4]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Shop_spr_bush_04_", pComponent });

		pComponent = m_pTextureCom[BUSH5] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Shop_spr_bush_05_"));
		if (nullptr == m_pTextureCom[BUSH5]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Shop_spr_bush_05_", pComponent });

		pComponent = m_pTextureCom[BUSH_FLOWER1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Shop_Spr_Deco_BushFlower01_"));
		if (nullptr == m_pTextureCom[BUSH_FLOWER1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Shop_Spr_Deco_BushFlower01_", pComponent });

		pComponent = m_pTextureCom[BUSH_FLOWER2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Shop_Spr_Deco_BushFlower02_"));
		if (nullptr == m_pTextureCom[BUSH_FLOWER2]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Shop_Spr_Deco_BushFlower02_", pComponent });
	}

	else if (SCENE_NORMAL == g_eCurScene)
	{
		pComponent = m_pTextureCom[DOOR_SIDEDOOR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Normal_SideDoor_"));
		if (nullptr == m_pTextureCom[DOOR_SIDEDOOR]) 
			return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Normal_SideDoor_", pComponent });

		pComponent = m_pTextureCom[DOOR_PILLAR2_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Normal_NormalDoor_Pillar2_"));
		if (nullptr == m_pTextureCom[DOOR_PILLAR2_NORMAL])
			return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Normal_NormalDoor_Pillar2_", pComponent });

		pComponent = m_pTextureCom[DOOR_FIRE_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Normal_NormalDoor_Fire_"));
		if (nullptr == m_pTextureCom[DOOR_FIRE_NORMAL]) 
			return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Normal_NormalDoor_Fire_", pComponent });

		pComponent = m_pTextureCom[DOOR_HEAD_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Normal_NormalDoor_Head_"));
		if (nullptr == m_pTextureCom[DOOR_HEAD_NORMAL]) 
			return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Normal_NormalDoor_Head_", pComponent });

		pComponent = m_pTextureCom[DOOR_PILLAR_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Normal_NormalDoor_Pillar_"));
		if (nullptr == m_pTextureCom[DOOR_PILLAR_NORMAL])
			return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Normal_NormalDoor_Pillar_", pComponent });

		pComponent = m_pTextureCom[DOOR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Normal_door_"));
		if (nullptr == m_pTextureCom[DOOR]) 
			return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Normal_door_", pComponent });

		pComponent = m_pTextureCom[BUSH1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Normal_spr_bush_01_"));
		if (nullptr == m_pTextureCom[BUSH1])
			return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Normal_spr_bush_01_", pComponent });

		pComponent = m_pTextureCom[BUSH2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Normal_spr_bush_02_"));
		if (nullptr == m_pTextureCom[BUSH2])
			return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Normal_spr_bush_02_", pComponent });

		pComponent = m_pTextureCom[BUSH3] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Normal_spr_bush_03_"));
		if (nullptr == m_pTextureCom[BUSH3]) 
			return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Normal_spr_bush_03_", pComponent });

		pComponent = m_pTextureCom[BUSH_FLOWER1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Normal_Spr_Deco_BushFlower01_"));
		if (nullptr == m_pTextureCom[BUSH_FLOWER1])
			return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Normal_Spr_Deco_BushFlower01_", pComponent });

		pComponent = m_pTextureCom[BUSH_FLOWER2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Normal_Spr_Deco_BushFlower02_"));
		if (nullptr == m_pTextureCom[BUSH_FLOWER2])
			return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Normal_Spr_Deco_BushFlower02_", pComponent });
	}
	return S_OK;
}