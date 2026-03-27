#include "stdafx.h"
#include "..\Header\Stage_Regen.h"

#include "Export_Function.h"
#include "StaticCamera.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "StaticObject.h"
#include "DestructibleObj.h"
#include "Door.h"
#include "MovingObj.h"
#include "SpriteObj.h"

#include "Player_Shadow.h"
#include "Ghost.h"
#include "SylphBow.h"
#include "SylphBowPair.h"
#include "Player.h"

#include "Monster.h"
#include "Monster_Shadow.h"
#include "AxeEliteEvilSoul.h"
#include "GreenEvilBat.h"
#include "EvilFrog.h"
#include "BlueEvilSoulBall.h"
#include "GreenEvilSlime.h"
#include "EvilSoul.h"
#include "MutationEvilSoul.h"

#include "Status.h"
#include "Status_Hp.h"
#include "Apostle.h"
#include "QuickSlot.h"
#include "MiniMap.h"
#include "Inventory.h"
#include "BigMap.h"
#include "Rain.h"
#include "MyMouse.h"
#include "Npc_NonePrint.h"

#include <KeyMgr.h>
#include "CollisionMgr.h"
#include <Black.h>
#include <SoundMgr.h>

CStage_Regen::CStage_Regen(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CStage_Regen::~CStage_Regen()
{
}

HRESULT CStage_Regen::Ready_Scene(void)
{
	g_vNextPos = { 380.f , 7.5f , 380.f };

	if (FAILED(Ready_Layer_Environment(L"Layer_Environment")))
		return E_FAIL;
	if (FAILED(Ready_Layer_GameLogic(L"Layer_GameLogic")))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;
	if (FAILED(Ready_Layer_Monster(L"Layer_Monster")))
		return E_FAIL;
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	CSoundMgr::Get_Instance()->PlayBGM(L"Bgm_Stage1-2_Start.wav");
	
	return S_OK;

}

_int CStage_Regen::Update_Scene(const _float & fTimeDelta)
{
	if(GetAsyncKeyState('U'))
		CSoundMgr::Get_Instance()->PlaySound(L"Ambience_Rain.wav", CSoundMgr::RAIN, 0.75f);

	if (!m_bTerrainInit)
	{
		Load_Terrain_Info(L"../../Data/Terrain/RegenMap.dat");
		m_bTerrainInit = true;


		Load_Object_Info(L"../../Data/Object/RegenObjectStatic.dat");
		Load_Object_Info(L"../../Data/Object/RegenObjectStatic2.dat");	// 나무
		Load_Dynamic_Object_Info(L"../../Data/Object/RegenObjectWalls.dat");
		Load_Dynamic_Object_Info(L"../../Data/Object/RegenObjectDoor.dat");
		Load_Dynamic_Object_Info(L"../../Data/Object/RegenObjectTemple.dat");
	}

	//========================== 디버깅용 구체만들기 =============================/
	if (!g_bSphereMake)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_C))
			g_bSphereMake = true;
	}

	if (g_bSphereMake)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_V))
		{
			if (!g_bSphereRender)
				g_bSphereRender = true;
			else
				g_bSphereRender = false;
		}
	}
	//============================================================================/

	m_fTimeWaveTime += fTimeDelta;

	StartWave1(fTimeDelta);		// 박쥐
	StartWave2(fTimeDelta);		// 공
	StartWave3(fTimeDelta);		// 개구리
	StartWave4(fTimeDelta);		// 네임드 + 박쥐
	StartWave5(fTimeDelta);
	//StartTimeWave1(fTimeDelta);
	//StartTimeWave2(fTimeDelta);
	//if (!Engine::Get_GameObject(L"Layer_Monster", L"GreenEvilSlime"))
	//{
	//	CGameObject* pGameObject = nullptr;
	//	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
	//	CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	//	CGameObject* pMonster = nullptr;

	//	pGameObject = pMonster = CGreenEvilSlime::Create(m_pGraphicDev);

	//	if (FAILED(pLayer->Add_GameObject(L"GreenEvilSlime", pGameObject))) return E_FAIL;

	//	pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));

	//	if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilSlime_Shadow", pGameObject))) return E_FAIL;

	//}

	//if (!Engine::Get_GameObject(L"Layer_Monster", L"EvilSoul"))
	//{
	//	CGameObject* pGameObject = nullptr;
	//	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
	//	CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	//	CGameObject* pMonster = nullptr;

	//	pGameObject = pMonster = CEvilSoul::Create(m_pGraphicDev);

	//	if (FAILED(pLayer->Add_GameObject(L"EvilSoul", pGameObject))) return E_FAIL;

	//	pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));

	//	if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilSoul_Shadow", pGameObject))) return E_FAIL;

	//}

	//if (!Engine::Get_GameObject(L"Layer_Monster", L"MutationEvilSoul"))
	//{
	//	CGameObject* pGameObject = nullptr;
	//	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
	//	CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	//	CGameObject* pMonster = nullptr;

	//	pGameObject = pMonster = CMutationEvilSoul::Create(m_pGraphicDev);

	//	if (FAILED(pLayer->Add_GameObject(L"MutationEvilSoul", pGameObject))) return E_FAIL;

	//	pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));

	//	if (FAILED(pLayerGameLogic->Add_GameObject(L"MutationEvilSoul_Shadow", pGameObject))) return E_FAIL;

	//}

	Door_Activate_Check();

	Check_PlayerPos();

	return __super::Update_Scene(fTimeDelta);
}

void CStage_Regen::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();

	Restrict_Monster_Activity_Scope();
}

void CStage_Regen::Render_Scene(void)
{
	// _DEBUG용
}

HRESULT CStage_Regen::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	if (nullptr == pLayer) return E_FAIL;

	// 오브젝트
	CGameObject*	pGameObject = nullptr;

	// 오브젝트 
	pGameObject = CStaticCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 10.f, -10.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f),
		(_float)WINCX / WINCY,
		0.1f,
		1000.f);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"StaticCamera", pGameObject)))
		return E_FAIL;

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"SkyBox", pGameObject)))
		return E_FAIL;

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage_Regen::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	if (nullptr == pLayer) return E_FAIL;

	CGameObject*	pGameObject = nullptr;
	CGameObject*	pBulletObject = nullptr;

	CGameObject* pPlayer = CPlayer::Create(m_pGraphicDev);
	if (nullptr == pPlayer) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Player", pPlayer)))
		return E_FAIL;
	
	pGameObject = CPlayer_Shadow::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	dynamic_cast<CPlayer_Shadow*>(pGameObject)->Set_Owner(pPlayer);
	if (FAILED(pLayer->Add_GameObject(L"Player_Shadow", pGameObject)))
		return E_FAIL;

	pGameObject = CSylphBow::Create(m_pGraphicDev,{0.f,0.f,0.f},0.f);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"SylphBow", pGameObject)))
		return E_FAIL;

	pGameObject = CSylphBowPair::Create(m_pGraphicDev, { 0.f,0.f,0.f }, 0.f);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"SylphBowPair", pGameObject)))
		return E_FAIL;

	pGameObject = CGhost::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Ghost", pGameObject)))
		return E_FAIL;

	pGameObject = CNpc_NonePrint::Create(m_pGraphicDev, NPC_REGEN_STONE);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Npc_None_RegenStone", pGameObject)))
		return E_FAIL;

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage_Regen::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	if (nullptr == pLayer) return E_FAIL;

	// 오브젝트
	CGameObject*	pGameObject = nullptr;

	pGameObject = CStatus::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"FrontUI_Status", pGameObject)))
		return E_FAIL;

	pGameObject = CApostle::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"FrontUI_Apostle", pGameObject)))
		return E_FAIL;

	pGameObject = CQuickSlot::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"FrontUI_QuickSlot", pGameObject)))
		return E_FAIL;

	pGameObject = CMiniMap::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"FrontUI_MiniMap", pGameObject)))
		return E_FAIL;

	pGameObject = CInventory::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"UI_Inventory", pGameObject)))
		return E_FAIL;

	pGameObject = CBigMap::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"UI_BigMap", pGameObject)))
		return E_FAIL;

	pGameObject = CRain::Create(m_pGraphicDev, 640.f * 1.5f, 480.f * 1.5f, { 0.f,-200.f,0.f });
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Rain", pGameObject)))
		return E_FAIL;

	// Fade In Out 효과
	pGameObject = CBlack::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Black", pGameObject)))
		return E_FAIL;

	pGameObject = CMyMouse::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"MyMouse_Cursor", pGameObject)))
		return E_FAIL;

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage_Regen::Ready_Layer_Monster(const _tchar* pLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (nullptr == pLayer) return E_FAIL;

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage_Regen::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
	
	tLightInfo.Type    = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage_Regen::Load_Terrain_Info(const _tchar* pPath)
{
	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));
	
	CGameObject* pGameObject = nullptr;
	pGameObject = CTerrain::Create(m_pGraphicDev, TERRAIN_REGEN);
	if (nullptr == pGameObject) return E_FAIL;

	if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
		return E_FAIL;

	HANDLE hFile = CreateFile(pPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	TERRAINDATA tTerrainData;
	ZeroMemory(&tTerrainData, sizeof(TERRAINDATA));
	TERRAINDATA_P tTerrainDataPointer;
	ZeroMemory(&tTerrainDataPointer, sizeof(TERRAINDATA_P));

	ReadFile(hFile, &tTerrainData, sizeof(TERRAINDATA), &dwByte, nullptr);

	ReadFile(hFile, &(tTerrainDataPointer.dwVtxCnt), sizeof(_ulong), &dwByte, nullptr);

	tTerrainDataPointer.pPos = new _vec3[tTerrainDataPointer.dwVtxCnt];

	for (int i = 0; i < tTerrainDataPointer.dwVtxCnt; ++i)
		ReadFile(hFile, &(tTerrainDataPointer.pPos[i]), sizeof(_vec3), &dwByte, nullptr);

	Safe_Delete_Array(tTerrainDataPointer.pPos);

	ReadFile(hFile, &dwStrByte, sizeof(_ulong), &dwByte, nullptr);

	TCHAR* pHeightmapFilePath = nullptr;
	pHeightmapFilePath = new TCHAR[dwStrByte];

	ReadFile(hFile, pHeightmapFilePath, dwStrByte, &dwByte, nullptr);

	//wstring strFilePath;
	//wstring strHeightmapFilePath;
	//strFilePath = pHeightmapFilePath;
	//strHeightmapFilePath = strFilePath.substr(0, 3);
	//strHeightmapFilePath += strFilePath.substr(10);

	//_tchar* pHeightmapPath = nullptr;
	//pHeightmapPath = new TCHAR[dwStrByte];
	//lstrcpy(pHeightmapPath, strHeightmapFilePath.c_str());

	//CTerrainTex* pTerrainBufCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	//NULL_CHECK_RETURN(pTerrainBufCom, E_FAIL);

	CTerrain* pTerrain = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Environment", L"Terrain"));
	if (nullptr == pTerrain) return E_FAIL;

	//pTerrainBufCom->Set_VtxCnt(tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv);
	//pTerrainBufCom->Set_DetailLevel(tTerrainData.fDetailLevel);
	//pTerrainBufCom->Set_HeightMapLevel(tTerrainData.iHeightMapLevel);
	//pTerrainBufCom->Set_HeightMapFilePath(pHeightmapPath);
	//pTerrainBufCom->Set_VtxPos(tTerrainDataPointer.pPos, tTerrainDataPointer.dwVtxCnt);
	pTerrain->Set_DrawID(tTerrainData.byDrawID);
	pTerrain->Set_DrawOption(tTerrainData.byDrawOption);
	pTerrain->Set_TerrainType(TERRAIN_REGEN);

	CloseHandle(hFile);

	Safe_Delete_Array(pHeightmapFilePath);
	//Safe_Delete_Array(pHeightmapPath);

	//pTerrainBufCom->Update_Buffer_Data();

	return S_OK;
}
HRESULT CStage_Regen::Load_Object_Info(const _tchar* pPath)
{
	HANDLE hFile = CreateFile(pPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	STATICOBJDATA tStaticObjData;

	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	if (nullptr == pLayer) return E_FAIL;

	CGameObject* pGameObject = nullptr;

	while (true)
	{
		ZeroMemory(&tStaticObjData, sizeof(STATICOBJDATA));

		ReadFile(hFile, &tStaticObjData, sizeof(STATICOBJDATA), &dwByte, nullptr);

		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		_tchar* pObjKey = nullptr;
		pObjKey = new TCHAR[dwStrByte];

		ReadFile(hFile, pObjKey, dwStrByte, &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete_Array(pObjKey);
			break;
		}

		pGameObject = CStaticObject::Create(m_pGraphicDev);
		if (nullptr == pGameObject) return E_FAIL;

		dynamic_cast<CStaticObject*>(pGameObject)->Set_ObjKey(pObjKey, dwStrByte);

		wstring strComponentKey = pObjKey;

		if (string::npos != strComponentKey.find('|'))	// 클론인경우
		{
			size_t startPos = strComponentKey.find('|');
			strComponentKey = strComponentKey.substr(0, startPos);
		}

		int iNum = strComponentKey.length() - 1;
		for (; iNum >= 0; --iNum)
		{
			if (0 == isdigit(pObjKey[iNum]))	// 숫자가 아닌 경우
				break;
		}

		strComponentKey = strComponentKey.substr(0, iNum + 1);

		dynamic_cast<CStaticObject*>(pGameObject)->Set_StaticObj_ID(dynamic_cast<CStaticObject*>(pGameObject)->CompareID(strComponentKey));
		if (FAILED(pLayer->Add_GameObject(dynamic_cast<CStaticObject*>(pGameObject)->Get_ObjKey(), pGameObject)))
			return E_FAIL;
		// dynamic은 Dynamic으로 따로 빼기
		Safe_Delete_Array<_tchar*>(pObjKey);

		CTransform* pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", dynamic_cast<CStaticObject*>(pGameObject)->Get_ObjKey(), L"Proto_Transform", ID_DYNAMIC));
		if (nullptr == pTransformCom) return E_FAIL;

		pTransformCom->Set_Scale(tStaticObjData.vScale.x, tStaticObjData.vScale.y, tStaticObjData.vScale.z);
		pTransformCom->Set_Angle(tStaticObjData.vAngle.x, tStaticObjData.vAngle.y, tStaticObjData.vAngle.z);
		pTransformCom->Set_Pos(tStaticObjData.vPos.x, tStaticObjData.vPos.y, tStaticObjData.vPos.z);

		dynamic_cast<CStaticObject*>(pGameObject)->Set_DrawID(tStaticObjData.byDrawID);
		dynamic_cast<CStaticObject*>(pGameObject)->Set_DrawOption(tStaticObjData.byDrawOption);
		dynamic_cast<CStaticObject*>(pGameObject)->Set_ColliderPos(&tStaticObjData.vColliderPos);
		dynamic_cast<CStaticObject*>(pGameObject)->Set_ColliderRadius(tStaticObjData.fColliderRadius);
	}

	CloseHandle(hFile);

	return 0;
}
HRESULT CStage_Regen::Load_Dynamic_Object_Info(const _tchar* pPath)
{
	HANDLE hFile = CreateFile(pPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	STATICOBJDATA tStaticObjData;

	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	if (nullptr == pLayer) return E_FAIL;

	CGameObject* pGameObject = nullptr;

	while (true)
	{
		ZeroMemory(&tStaticObjData, sizeof(STATICOBJDATA));

		ReadFile(hFile, &tStaticObjData, sizeof(STATICOBJDATA), &dwByte, nullptr);

		ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

		_tchar* pObjKey = nullptr;
		pObjKey = new TCHAR[dwStrByte];

		ReadFile(hFile, pObjKey, dwStrByte, &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete_Array(pObjKey);
			break;
		}

		wstring strComponentKey = pObjKey;
		_bool	bIsClone = false;
		
		if (string::npos != strComponentKey.find('|'))	// 클론인경우
		{
			size_t startPos = strComponentKey.find('|');
			strComponentKey = strComponentKey.substr(0, startPos);
			bIsClone = true;
		}

		int iNum = strComponentKey.length() - 1;
		for (; iNum >= 0; --iNum)
		{
			if (0 == isdigit(pObjKey[iNum]))	// 숫자가 아닌 경우
				break;
		}

		strComponentKey = strComponentKey.substr(0, iNum + 1);

		CTransform* pTransformCom = nullptr;
		size_t tempSize = 0;
		_tchar szComponentKey[MAX_STR];


		DYNAMIC_OBJECT_ID eID = CDynamicObject::CompareID(strComponentKey);
		switch (eID)
		{
		case TEMPLE_BODY: case TEMPLE_HEAD: case DOOR_SIDEDOOR: case DOOR_PILLAR2_NORMAL: case DOOR_FIRE_NORMAL:
		{
			pGameObject = CSpriteObj::Create(m_pGraphicDev);
			if (nullptr == pGameObject) return E_FAIL;
			tempSize = strComponentKey.length();
			lstrcpy(szComponentKey, strComponentKey.c_str());
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_ObjKey(pObjKey, dwStrByte);
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_DynamicObj_ID(dynamic_cast<CSpriteObj*>(pGameObject)->CompareID(strComponentKey));
			if (FAILED(pLayer->Add_GameObject(dynamic_cast<CSpriteObj*>(pGameObject)->Get_ObjKey(), pGameObject)))
				return E_FAIL;
			Safe_Delete_Array<_tchar*>(pObjKey);

			pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", dynamic_cast<CSpriteObj*>(pGameObject)->Get_ObjKey(), L"Proto_Transform", ID_DYNAMIC));
			if (nullptr == pTransformCom) return E_FAIL;
			pTransformCom->Set_Scale(tStaticObjData.vScale.x, tStaticObjData.vScale.y, tStaticObjData.vScale.z);
			pTransformCom->Set_Angle(tStaticObjData.vAngle.x, tStaticObjData.vAngle.y, tStaticObjData.vAngle.z);
			pTransformCom->Set_Pos(tStaticObjData.vPos.x, tStaticObjData.vPos.y, tStaticObjData.vPos.z);

			dynamic_cast<CSpriteObj*>(pGameObject)->Set_DrawID(tStaticObjData.byDrawID);
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_DrawOption(tStaticObjData.byDrawOption);
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_ColliderPos(&tStaticObjData.vColliderPos);
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_ColliderRadius(tStaticObjData.fColliderRadius);
		}
		break;
		case STONE_WALL2: case INFECTION_WALL2: case PILLAR2:
		{
			pGameObject = CDestructibleObj::Create(m_pGraphicDev);
			if (nullptr == pGameObject) return E_FAIL;
			tempSize = strComponentKey.length();
			lstrcpy(szComponentKey, strComponentKey.c_str());
			dynamic_cast<CDestructibleObj*>(pGameObject)->Set_ObjKey(pObjKey, dwStrByte);
			dynamic_cast<CDestructibleObj*>(pGameObject)->Set_DynamicObj_ID(dynamic_cast<CDestructibleObj*>(pGameObject)->CompareID(strComponentKey));
			if (FAILED(pLayer->Add_GameObject(dynamic_cast<CDestructibleObj*>(pGameObject)->Get_ObjKey(), pGameObject)))
				return E_FAIL;
			Safe_Delete_Array<_tchar*>(pObjKey);

			pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", dynamic_cast<CDestructibleObj*>(pGameObject)->Get_ObjKey(), L"Proto_Transform", ID_DYNAMIC));
			if (nullptr == pTransformCom) return E_FAIL;
			pTransformCom->Set_Scale(tStaticObjData.vScale.x, tStaticObjData.vScale.y, tStaticObjData.vScale.z);
			pTransformCom->Set_Angle(tStaticObjData.vAngle.x, tStaticObjData.vAngle.y, tStaticObjData.vAngle.z);
			pTransformCom->Set_Pos(tStaticObjData.vPos.x, tStaticObjData.vPos.y, tStaticObjData.vPos.z);

			dynamic_cast<CDestructibleObj*>(pGameObject)->Set_DrawID(tStaticObjData.byDrawID);
			dynamic_cast<CDestructibleObj*>(pGameObject)->Set_DrawOption(tStaticObjData.byDrawOption);
			dynamic_cast<CDestructibleObj*>(pGameObject)->Set_ColliderPos(&tStaticObjData.vColliderPos);
			dynamic_cast<CDestructibleObj*>(pGameObject)->Set_ColliderRadius(tStaticObjData.fColliderRadius*1.15);
		}
		break;
		case DOOR:
		{
			pGameObject = CDoor::Create(m_pGraphicDev);
			if (nullptr == pGameObject) return E_FAIL;
			tempSize = strComponentKey.length();
			lstrcpy(szComponentKey, strComponentKey.c_str());
			dynamic_cast<CDoor*>(pGameObject)->Set_ObjKey(pObjKey, dwStrByte);
			dynamic_cast<CDoor*>(pGameObject)->Set_DynamicObj_ID(dynamic_cast<CDoor*>(pGameObject)->CompareID(strComponentKey));
			if (FAILED(pLayer->Add_GameObject(dynamic_cast<CDoor*>(pGameObject)->Get_ObjKey(), pGameObject)))
				return E_FAIL;
			Safe_Delete_Array<_tchar*>(pObjKey);

			pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", dynamic_cast<CDoor*>(pGameObject)->Get_ObjKey(), L"Proto_Transform", ID_DYNAMIC));
			if (nullptr == pTransformCom) return E_FAIL;
			pTransformCom->Set_Scale(tStaticObjData.vScale.x, tStaticObjData.vScale.y, tStaticObjData.vScale.z);
			pTransformCom->Set_Angle(tStaticObjData.vAngle.x, tStaticObjData.vAngle.y, tStaticObjData.vAngle.z);
			pTransformCom->Set_Pos(tStaticObjData.vPos.x, tStaticObjData.vPos.y, tStaticObjData.vPos.z);

			dynamic_cast<CDoor*>(pGameObject)->Set_DrawID(tStaticObjData.byDrawID);
			dynamic_cast<CDoor*>(pGameObject)->Set_DrawOption(tStaticObjData.byDrawOption);
			dynamic_cast<CDoor*>(pGameObject)->Set_ColliderPos(&tStaticObjData.vColliderPos);
			dynamic_cast<CDoor*>(pGameObject)->Set_ColliderRadius(tStaticObjData.fColliderRadius);
		}
		break;
		case TEMPLE1:
		{
			pGameObject = CMovingObj::Create(m_pGraphicDev);
			if (nullptr == pGameObject) return E_FAIL;
			tempSize = strComponentKey.length();
			lstrcpy(szComponentKey, strComponentKey.c_str());
			dynamic_cast<CMovingObj*>(pGameObject)->Set_ObjKey(pObjKey, dwStrByte);
			dynamic_cast<CMovingObj*>(pGameObject)->Set_DynamicObj_ID(dynamic_cast<CMovingObj*>(pGameObject)->CompareID(strComponentKey));
			if (FAILED(pLayer->Add_GameObject(dynamic_cast<CMovingObj*>(pGameObject)->Get_ObjKey(), pGameObject)))
				return E_FAIL;
			Safe_Delete_Array<_tchar*>(pObjKey);

			pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", dynamic_cast<CMovingObj*>(pGameObject)->Get_ObjKey(), L"Proto_Transform", ID_DYNAMIC));
			if (nullptr == pTransformCom) return E_FAIL;
			pTransformCom->Set_Scale(tStaticObjData.vScale.x, tStaticObjData.vScale.y, tStaticObjData.vScale.z);
			pTransformCom->Set_Angle(tStaticObjData.vAngle.x, tStaticObjData.vAngle.y, tStaticObjData.vAngle.z);
			pTransformCom->Set_Pos(tStaticObjData.vPos.x, tStaticObjData.vPos.y, tStaticObjData.vPos.z);

			dynamic_cast<CMovingObj*>(pGameObject)->Set_DrawID(tStaticObjData.byDrawID);
			dynamic_cast<CMovingObj*>(pGameObject)->Set_DrawOption(tStaticObjData.byDrawOption);
			dynamic_cast<CMovingObj*>(pGameObject)->Set_ColliderPos(&tStaticObjData.vColliderPos);
			dynamic_cast<CMovingObj*>(pGameObject)->Set_ColliderRadius(tStaticObjData.fColliderRadius);
		}
		break;
		}
	}

	CloseHandle(hFile);

	return 0;
}

CStage_Regen * CStage_Regen::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage_Regen *	pInstance = new CStage_Regen(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{ 
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
void CStage_Regen::Free(void)
{
	__super::Free();
}

void CStage_Regen::StartWave1(const _float& fTimeDelta)
{
	if (1 == m_iCurWave)
	{
		m_fAccumulatedTime += fTimeDelta;

		if (!m_bMonsterSummoned[0] && 5.f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(1);

			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat_Shadow", pGameObject))) return;

			m_bMonsterSummoned[0] = true;
		}

		if (!m_bMonsterSummoned[1] && 6.2f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(200.f + 257.f, 0.f, 120.f + 257.f));


			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat2", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat2_Shadow", pGameObject))) return;

			m_bMonsterSummoned[1] = true;
		}

		if (!m_bMonsterSummoned[2] && 7.1f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(125.f + 257.f, 0.f, 60.f + 257.f));


			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat3", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat3_Shadow", pGameObject))) return;

			m_bMonsterSummoned[2] = true;
		}

		if (!m_bMonsterSummoned[3] && 8.7f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(50.f + 257.f, 0.f, 100.f + 257.f));


			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat4", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat4_Shadow", pGameObject))) return;

			m_bMonsterSummoned[3] = true;
		}

		if (!m_bMonsterSummoned[4] && 9.1f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));


			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat5", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat5_Shadow", pGameObject))) return;

			m_bMonsterSummoned[4] = true;
		}

		if (!m_bMonsterSummoned[5] && 10.8f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(200.f + 257.f, 0.f, 120.f + 257.f));


			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat6", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat6_Shadow", pGameObject))) return;

			m_bMonsterSummoned[5] = true;
		}

		if (!m_bMonsterSummoned[6] && 11.6f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(125.f + 257.f, 0.f, 60.f + 257.f));


			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat7", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat7_Shadow", pGameObject))) return;

			m_bMonsterSummoned[6] = true;
		}

		if (!m_bMonsterSummoned[7] && 12.6f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(50.f + 257.f, 0.f, 100.f + 257.f));


			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat8", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat8_Shadow", pGameObject))) return;

			m_bMonsterSummoned[7] = true;
		}

		if (!m_bMonsterSummoned[8] && 13.4f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));


			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat9", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat9_Shadow", pGameObject))) return;

			m_bMonsterSummoned[8] = true;
		}

		if (!m_bMonsterSummoned[9] && 14.2f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(200.f + 257.f, 0.f, 120.f + 257.f));


			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat10", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat10_Shadow", pGameObject))) return;

			m_bMonsterSummoned[9] = true;
		}

		CScene* pScene = Get_Scene();
		m_iMonsterCnt = pScene->Get_MonsterCnt();

		if (m_bMonsterSummoned[9] && m_iMonsterCnt == 0)
		{
			++m_iCurWave;
			ZeroMemory(m_bMonsterSummoned, sizeof(_bool) * 10);
			m_fAccumulatedTime = 0.f;
		}
	}
}
void CStage_Regen::StartWave2(const _float& fTimeDelta)
{
	if (2 == m_iCurWave)
	{
		m_fAccumulatedTime += fTimeDelta;

		if (!m_bMonsterSummoned[0] && 5.f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));


			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall_Shadow", pGameObject))) return;

			m_bMonsterSummoned[0] = true;
		}

		if (!m_bMonsterSummoned[1] && 6.2f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(200.f + 257.f, 0.f, 120.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(2);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall2", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall2_Shadow", pGameObject))) return;

			m_bMonsterSummoned[1] = true;
		}

		if (!m_bMonsterSummoned[2] && 7.1f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(125.f + 257.f, 0.f, 60.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(3);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall3", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall3_Shadow", pGameObject))) return;

			m_bMonsterSummoned[2] = true;
		}

		if (!m_bMonsterSummoned[3] && 8.7f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(50.f + 257.f, 0.f, 100.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(4);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall4", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall4_Shadow", pGameObject))) return;

			m_bMonsterSummoned[3] = true;
		}

		if (!m_bMonsterSummoned[4] && 9.1f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(5);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall5", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall5_Shadow", pGameObject))) return;

			m_bMonsterSummoned[4] = true;
		}

		if (!m_bMonsterSummoned[5] && 10.8f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(200.f + 257.f, 0.f, 120.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(6);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall6", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall6_Shadow", pGameObject))) return;

			m_bMonsterSummoned[5] = true;
		}

		if (!m_bMonsterSummoned[6] && 11.6f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(125.f + 257.f, 0.f, 60.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(7);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall7", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall7_Shadow", pGameObject))) return;

			m_bMonsterSummoned[6] = true;
		}

		if (!m_bMonsterSummoned[7] && 12.6f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(50.f + 257.f, 0.f, 100.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(8);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall8", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall8_Shadow", pGameObject))) return;

			m_bMonsterSummoned[7] = true;
		}

		if (!m_bMonsterSummoned[8] && 13.4f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(9);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall9", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall9_Shadow", pGameObject))) return;

			m_bMonsterSummoned[8] = true;
		}

		if (!m_bMonsterSummoned[9] && 14.2f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(200.f + 257.f, 0.f, 120.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(10);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall10", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall10_Shadow", pGameObject))) return;

			m_bMonsterSummoned[9] = true;
		}

		CScene* pScene = Get_Scene();
		m_iMonsterCnt = pScene->Get_MonsterCnt();

		if (m_bMonsterSummoned[9] && m_iMonsterCnt == 0)
		{
			++m_iCurWave;
			ZeroMemory(m_bMonsterSummoned, sizeof(_bool) * 10);
			m_fAccumulatedTime = 0.f;
		}
	}
}
void CStage_Regen::StartWave3(const _float& fTimeDelta)
{
	if (3 == m_iCurWave)
	{
		m_fAccumulatedTime += fTimeDelta;

		if (!m_bMonsterSummoned[0] && 5.f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"EvilFrog", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog_Shadow", pGameObject))) return;

			m_bMonsterSummoned[0] = true;
		}

		if (!m_bMonsterSummoned[1] && 6.2f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(200.f + 257.f, 0.f, 120.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"EvilFrog2", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog2_Shadow", pGameObject))) return;

			m_bMonsterSummoned[1] = true;
		}

		if (!m_bMonsterSummoned[2] && 7.1f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(125.f + 257.f, 0.f, 60.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"EvilFrog3", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog3_Shadow", pGameObject))) return;

			m_bMonsterSummoned[2] = true;
		}

		if (!m_bMonsterSummoned[3] && 8.7f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(50.f + 257.f, 0.f, 100.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"EvilFrog4", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog4_Shadow", pGameObject))) return;

			m_bMonsterSummoned[3] = true;
		}

		if (!m_bMonsterSummoned[4] && 9.1f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"EvilFrog5", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog5_Shadow", pGameObject))) return;

			m_bMonsterSummoned[4] = true;
		}

		if (!m_bMonsterSummoned[5] && 10.8f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(200.f + 257.f, 0.f, 120.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"EvilFrog6", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog6_Shadow", pGameObject))) return;

			m_bMonsterSummoned[5] = true;
		}

		if (!m_bMonsterSummoned[6] && 11.6f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(125.f + 257.f, 0.f, 60.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"EvilFrog7", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog7_Shadow", pGameObject))) return;

			m_bMonsterSummoned[6] = true;
		}

		//if (!m_bMonsterSummoned[7] && 12.6f < m_fAccumulatedTime)
		//{
		//	CGameObject* pGameObject = nullptr;
		//	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
		//	CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		//	CGameObject* pMonster = nullptr;

		//	pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(50.f + 257.f, 0.f, 100.f + 257.f));

		//	if (FAILED(pLayer->Add_GameObject(L"EvilFrog8", pGameObject))) return;

		//	pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));

		//	if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog8_Shadow", pGameObject))) return;

		//	m_bMonsterSummoned[7] = true;
		//}

		//if (!m_bMonsterSummoned[8] && 13.4f < m_fAccumulatedTime)
		//{
		//	CGameObject* pGameObject = nullptr;
		//	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
		//	CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		//	CGameObject* pMonster = nullptr;

		//	pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));

		//	if (FAILED(pLayer->Add_GameObject(L"EvilFrog9", pGameObject))) return;

		//	pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));

		//	if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog9_Shadow", pGameObject))) return;

		//	m_bMonsterSummoned[8] = true;
		//}

		//if (!m_bMonsterSummoned[9] && 14.2f < m_fAccumulatedTime)
		//{
		//	CGameObject* pGameObject = nullptr;
		//	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
		//	CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		//	CGameObject* pMonster = nullptr;

		//	pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(200.f + 257.f, 0.f, 120.f + 257.f));

		//	if (FAILED(pLayer->Add_GameObject(L"EvilFrog10", pGameObject))) return;

		//	pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));

		//	if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog10_Shadow", pGameObject))) return;

		//	m_bMonsterSummoned[9] = true;
		//}

		CScene* pScene = Get_Scene();
		m_iMonsterCnt = pScene->Get_MonsterCnt();

		if (m_bMonsterSummoned[6] && m_iMonsterCnt == 0)
		{
			++m_iCurWave;
			ZeroMemory(m_bMonsterSummoned, sizeof(_bool) * 10);
			m_fAccumulatedTime = 0.f;
		}
	}
}
void CStage_Regen::StartWave4(const _float& fTimeDelta)
{
	if (4 == m_iCurWave)
	{
		m_fAccumulatedTime += fTimeDelta;

		if (!m_bMonsterSummoned[0] && 2.f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(50.f + 257.f, 0.f, 100.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(1);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall11", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall11_Shadow", pGameObject))) return;

			m_bMonsterSummoned[0] = true;
		}

		if (!m_bMonsterSummoned[1] && 3.3f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(125.f + 257.f, 0.f, 60.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat11", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat11_Shadow", pGameObject))) return;

			m_bMonsterSummoned[1] = true;
		}

		if (!m_bMonsterSummoned[2] && 4.6f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"EvilFrog11", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog11_Shadow", pGameObject))) return;

			m_bMonsterSummoned[2] = true;
		}

		if (!m_bMonsterSummoned[3] && 7.2f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(200.f + 257.f, 0.f, 120.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"EvilFrog12", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog12_Shadow", pGameObject))) return;

			m_bMonsterSummoned[3] = true;
		}

		if (!m_bMonsterSummoned[4] && 8.5f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(125.f + 257.f, 0.f, 60.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(9);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall12", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall12_Shadow", pGameObject))) return;

			m_bMonsterSummoned[4] = true;
		}

		if (!m_bMonsterSummoned[5] && 9.7f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(1);

			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat12", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat12_Shadow", pGameObject))) return;

			m_bMonsterSummoned[5] = true;
		}

		if (!m_bMonsterSummoned[6] && 10.6f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(50.f + 257.f, 0.f, 100.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(9);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall13", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall13_Shadow", pGameObject))) return;

			m_bMonsterSummoned[6] = true;
		}

		if (!m_bMonsterSummoned[7] && 11.2f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(200.f + 257.f, 0.f, 120.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(1);

			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat13", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat13_Shadow", pGameObject))) return;

			m_bMonsterSummoned[7] = true;
		}

		if (!m_bMonsterSummoned[8] && 12.8f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(125.f + 257.f, 0.f, 60.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"EvilFrog13", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog13_Shadow", pGameObject))) return;

			m_bMonsterSummoned[8] = true;
		}

		if (!m_bMonsterSummoned[9] && 13.8f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CBlueEvilSoulBall::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(9);

			if (FAILED(pLayer->Add_GameObject(L"BlueEvilSoulBall14", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"BlueEvilSoulBall14_Shadow", pGameObject))) return;

			m_bMonsterSummoned[9] = true;
		}


		CScene* pScene = Get_Scene();
		m_iMonsterCnt = pScene->Get_MonsterCnt();

		if (m_bMonsterSummoned[9] && m_iMonsterCnt == 0)
		{
			++m_iCurWave;
			ZeroMemory(m_bMonsterSummoned, sizeof(_bool) * 10);
			m_fAccumulatedTime = 0.f;
		}
	}
}
void CStage_Regen::StartWave5(const _float& fTimeDelta)
{
	if (5 == m_iCurWave)
	{
		m_fAccumulatedTime += fTimeDelta;

		if (!m_bMonsterSummoned[0] && 3.f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CAxeEliteEvilSoul::Create(m_pGraphicDev);

			if (FAILED(pLayer->Add_GameObject(L"AxeEliteEvilSoul", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"AxeEliteEvilSoul_Shadow", pGameObject))) return;

			m_bMonsterSummoned[0] = true;
		}
	
		if (!m_bMonsterSummoned[1] && 5.8f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(50.f + 257.f, 0.f, 100.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"EvilFrog14", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog14_Shadow", pGameObject))) return;

			m_bMonsterSummoned[1] = true;
		}

		if (!m_bMonsterSummoned[2] && 6.1f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CEvilFrog::Create(m_pGraphicDev, _vec3(200.f + 257.f, 0.f, 120.f + 257.f));

			if (FAILED(pLayer->Add_GameObject(L"EvilFrog15", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"EvilFrog15_Shadow", pGameObject))) return;

			m_bMonsterSummoned[2] = true;
		}

		if (!m_bMonsterSummoned[3] && 10.f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(130.f + 257.f, 0.f, 160.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(1);

			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat14", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat14_Shadow", pGameObject))) return;

			m_bMonsterSummoned[3] = true;
		}

		if (!m_bMonsterSummoned[4] && 11.4f < m_fAccumulatedTime)
		{
			CGameObject* pGameObject = nullptr;
			CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
			CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			CMonster* pMonster = nullptr;

			pGameObject = pMonster = CGreenEvilBat::Create(m_pGraphicDev, _vec3(125.f + 257.f, 0.f, 60.f + 257.f));
			//dynamic_cast<CMonster*>(pMonster)->Set_SoundChannel(1);

			if (FAILED(pLayer->Add_GameObject(L"GreenEvilBat15", pGameObject))) return;

			pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
			pMonster->Set_Shadow(dynamic_cast<CMonster_Shadow*>(pGameObject));
			if (FAILED(pLayerGameLogic->Add_GameObject(L"GreenEvilBat15_Shadow", pGameObject))) return;

			m_bMonsterSummoned[4] = true;
		}

		CScene* pScene = Get_Scene();
		m_iMonsterCnt = pScene->Get_MonsterCnt();

		if (m_bMonsterSummoned[0] && m_iMonsterCnt == 0)
		{
			++m_iCurWave;
			ZeroMemory(m_bMonsterSummoned, sizeof(_bool) * 10);
			m_fAccumulatedTime = 0.f;
		}
	}
}

void CStage_Regen::StartTimeWave1(const _float& fTimeDelta)
{
	if (15.f < m_fTimeWaveTime && !m_bTimeWave1)
	{
		_bool bTrue = true;
		for (int i = 0; i < 10; ++i)
		{
			while (bTrue)
			{
				_float randNum1 = rand() % 257 + 257;
				_float randNum2 = rand() % 257 + 257;

				_vec3 vTemp = { randNum1, 0.f, randNum2 };

				if (Is_In_Area(vTemp))
				{
					if (rand() % 2 == 0)
					{
						CGameObject* pGameObject = nullptr;
						CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
						CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
						CGameObject* pMonster = nullptr;
						wstring strKey = L"EvilSoul" + i;

						pGameObject = pMonster = CEvilSoul::Create(m_pGraphicDev, vTemp);

						if (FAILED(pLayer->Add_GameObject(strKey.c_str(), pGameObject))) return;

						pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));
						
						strKey + L"_Shadow";
						if (FAILED(pLayerGameLogic->Add_GameObject(strKey.c_str(), pGameObject))) return;

						bTrue = false;
					}
					else
					{
						CGameObject* pGameObject = nullptr;
						CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
						CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
						CGameObject* pMonster = nullptr;
						wstring strKey = L"MutationEvilSoul" + i;

						pGameObject = pMonster = CMutationEvilSoul::Create(m_pGraphicDev, vTemp);

						if (FAILED(pLayer->Add_GameObject(strKey.c_str(), pGameObject))) return;

						pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));

						strKey + L"_Shadow";
						if (FAILED(pLayerGameLogic->Add_GameObject(strKey.c_str(), pGameObject))) return;

						bTrue = false;
					}
				}
			}
		}
		m_bTimeWave1 = true;
	}
}

void CStage_Regen::StartTimeWave2(const _float& fTimeDelta)
{
	if (30.f < m_fTimeWaveTime && !m_bTimeWave2)
	{
		_bool bTrue = true;
		for (int i = 0; i < 15; ++i)
		{
			while (bTrue)
			{
				_float randNum1 = rand() % 257 + 257;
				_float randNum2 = rand() % 257 + 257;

				_vec3 vTemp = { randNum1, 0.f, randNum2 };

				if (Is_In_Area(vTemp))
				{
					if (rand() % 2 == 0)
					{
						CGameObject* pGameObject = nullptr;
						CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
						CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
						CGameObject* pMonster = nullptr;
						wstring strKey = L"EvilSoul" + i;

						pGameObject = pMonster = CEvilSoul::Create(m_pGraphicDev, vTemp);

						if (FAILED(pLayer->Add_GameObject(strKey.c_str(), pGameObject))) return;

						pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));

						strKey + L"_Shadow";
						if (FAILED(pLayerGameLogic->Add_GameObject(strKey.c_str(), pGameObject))) return;
						bTrue = false;
					}
					else
					{
						CGameObject* pGameObject = nullptr;
						CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
						CLayer* pLayerGameLogic = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
						CGameObject* pMonster = nullptr;
						wstring strKey = L"MutationEvilSoul" + i;

						pGameObject = pMonster = CMutationEvilSoul::Create(m_pGraphicDev, vTemp);

						if (FAILED(pLayer->Add_GameObject(strKey.c_str(), pGameObject))) return;

						pGameObject = CMonster_Shadow::Create(m_pGraphicDev, pMonster, _vec3(8.f, 8.f, 0.f));

						strKey + L"_Shadow";
						if (FAILED(pLayerGameLogic->Add_GameObject(strKey.c_str(), pGameObject))) return;
						bTrue = false;
					}
				}
			}
		}
		m_bTimeWave2 = true;
	}
}

void CStage_Regen::Restrict_Monster_Activity_Scope(void)
{
	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Monster"));
	_vec3 vPos;
	_vec3 vDir;
	for (auto& iter : pLayer->Get_ObjectMap())
	{
		vPos = dynamic_cast<CMonster*>(iter.second)->Get_Transform()->m_vInfo[INFO_POS];
		if (!Is_In_Area(vPos))
		{
			vDir = *dynamic_cast<CMonster*>(iter.second)->Get_Dir();
			vDir *= -1;
			dynamic_cast<CMonster*>(iter.second)->Set_Dir(vDir);
		}
	}
}

_bool CStage_Regen::Is_In_Area(const _vec3& vPos)
{
	// 12시, 3시. 6시, 9시
	_vec3 vPoint[4]{
		{401.9f, 0.f, 466.98f},
		{538.37f, 0.f, 392.51f},
		{385.7f, 0.f, 289.f},
		{251.89f, 0.f, 369.87f}
	};

	_vec3 vDir[4]{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	_vec3 vNormal[4]{
		{ -vDir[0].z, 0.f, vDir[0].x },
		{ -vDir[1].z, 0.f, vDir[1].x },
		{ -vDir[2].z, 0.f, vDir[2].x },
		{ -vDir[3].z, 0.f, vDir[3].x },
	};

	_vec3 vObjectDir[4]{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vObjectDir[i], &vObjectDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (0.f < D3DXVec3Dot(&vNormal[i], &vObjectDir[i]))
			return false;
	}

	return true;
}

void CStage_Regen::Check_PlayerPos()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));


	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	CBlack* pBlack = dynamic_cast<CBlack*>(Engine::Get_GameObject(L"Layer_UI", L"Black"));

	_vec3 vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];
	_vec3 vDungeonLobbyPos = {242.f , vPlayerPos.y , 373.f};
	
	_vec3 vDiff1 = vPlayerPos - vDungeonLobbyPos;

	_vec3 vNextMapPos;

	if (D3DXVec3Length(&vDiff1) < 5.f)
	{
		vNextMapPos = { 384.f , vPlayerPos.y , 384.f };
		pBlack->Scene_Change(SCENE_DUNGEONLOBBY, vNextMapPos);
	}
	
}

void CStage_Regen::Door_Init()
{
}

void CStage_Regen::Door_Activate_Check()
{
	CSpriteObj* pObj = nullptr;

	if (g_bRegen_To_Pot)
	{
		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Regen_SideDoor_0"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Regen_NormalDoor_Pillar2_0"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Regen_NormalDoor_Fire_0"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Regen_NormalDoor_Fire_0|Clone0"));
		pObj->Set_DoorActive();
	}
}
