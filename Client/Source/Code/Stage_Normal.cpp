#include "stdafx.h"
#include "..\Header\Stage_Normal.h"

#include "Export_Function.h"
#include "StaticCamera.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Player_Shadow.h"
#include "Ghost.h"
#include "SylphBow.h"
#include "SylphBowPair.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "SpriteObj.h"
#include "DestructibleObj.h"
#include "Portal.h"
#include "Door.h"
#include "MovingObj.h"
#include "Jar.h"
#include "Lever.h"
#include <KeyMgr.h>
#include "CollisionMgr.h"
#include "Rain.h"
#include <Black.h>

#include "EvilSoul.h"
#include "GreenEvilSlime.h"
#include "MutationEvilSoul.h"
#include "GreenEvilBat.h"

#include <MyMouse.h>
#include <SoundMgr.h>

CStage_Normal::CStage_Normal(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CStage_Normal::~CStage_Normal()
{
}

HRESULT CStage_Normal::Ready_Scene(void)
{
	//g_vNextPos = { 383.f , 7.5f , 383.f };

	if (FAILED(Ready_Layer_Environment(L"Layer_Environment")))
		return E_FAIL;
	if (FAILED(Ready_Layer_GameLogic(L"Layer_GameLogic")))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;


	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Normal.wav");

	return S_OK;

}

_int CStage_Normal::Update_Scene(const _float & fTimeDelta)
{
	if (!m_bTerrainInit)
	{
		Load_Terrain_Info(L"../../Data/Terrain/Stage_NormalMap.dat");
		m_bTerrainInit = true;

		Load_Static_Object_Info(L"../../Data/Object/Stage_NormalObjectStatic.dat");
		Load_Dynamic_Object_Info(L"../../Data/Object/Stage_NormalObjectDynamic.dat");
	}

	if (!m_bDoorInit)
	{
		Door_Init();
		m_bDoorInit = true;
	}

	Door_Activate_Check();

	Check_PlayerPos();


	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	int Mon_Cnt = pLayer->m_Normal_Stage_Mon_Cnt;                                                                     

	CGameObject* pGameObject = nullptr;


	if (Mon_Cnt == 0 && m_Wave_1 == false) {
		for (int i = 0; i < 7; i++) {


			pGameObject = CEvilSoul::Create(m_pGraphicDev,{});

			if (nullptr == pGameObject) return E_FAIL;

			pLayer->Add_BulletObject(pGameObject);

		}
		m_Wave_1 = true;
	}

	if (Mon_Cnt == 7 && m_Wave_2 == false) {
		for (int i = 0; i < 7; i++) {
			pGameObject = CGreenEvilSlime::Create(m_pGraphicDev,{});

			if (nullptr == pGameObject) return E_FAIL;

			pLayer->Add_BulletObject(pGameObject);

		}
		m_Wave_2 = true;
	}

	if (Mon_Cnt == 14 && m_Wave_3 == false) {
		for (int i = 0; i < 7; i++) {
			pGameObject = CMutationEvilSoul::Create(m_pGraphicDev,{});

			if (nullptr == pGameObject) return E_FAIL;

			pLayer->Add_BulletObject(pGameObject);
		}
		m_Wave_3 = true;
	}

	
	if (Mon_Cnt == 21 && m_Wave_4 == false) {
		for (int i = 0; i < 3; i++) {

			pGameObject = CMutationEvilSoul::Create(m_pGraphicDev,{});

			if (nullptr == pGameObject) return E_FAIL;

			pLayer->Add_BulletObject(pGameObject);

			pGameObject = CEvilSoul::Create(m_pGraphicDev,{});

			if (nullptr == pGameObject) return E_FAIL;

			pLayer->Add_BulletObject(pGameObject);

			pGameObject = CGreenEvilSlime::Create(m_pGraphicDev,{});

			if (nullptr == pGameObject) return E_FAIL;

			pLayer->Add_BulletObject(pGameObject);

		}


		m_Wave_4 = true;
	}

	if (Mon_Cnt == 30)
	{
		g_bNormal_To_Pot = true;
	}






	return __super::Update_Scene(fTimeDelta);
}

void CStage_Normal::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CStage_Normal::Render_Scene(void)
{
	// _DEBUG용
}

HRESULT CStage_Normal::Ready_Layer_Environment(const _tchar* pLayerTag)
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

HRESULT CStage_Normal::Ready_Layer_GameLogic(const _tchar * pLayerTag)
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

	pGameObject = CSylphBow::Create(m_pGraphicDev, { 0.f,0.f,0.f }, 0.f);
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

	
	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage_Normal::Ready_Layer_UI(const _tchar * pLayerTag)
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

	pGameObject = CInventory::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"UI_Inventory", pGameObject)))
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

HRESULT CStage_Normal::Ready_LightInfo()
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

HRESULT CStage_Normal::Load_Terrain_Info(const _tchar* pPath)
{
	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));
	
	CGameObject* pGameObject = nullptr;
	pGameObject = CTerrain::Create(m_pGraphicDev, TERRAIN_NORMAL);
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

	CTerrain* pTerrain = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Environment", L"Terrain"));
	if (nullptr == pTerrain) return E_FAIL;

	
	pTerrain->Set_DrawID(tTerrainData.byDrawID);
	pTerrain->Set_DrawOption(tTerrainData.byDrawOption);

	CloseHandle(hFile);

	Safe_Delete_Array(pHeightmapFilePath);
	return S_OK;
}

HRESULT CStage_Normal::Load_Static_Object_Info(const _tchar* pPath)
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

HRESULT CStage_Normal::Load_Dynamic_Object_Info(const _tchar* pPath)
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
		case BUSH1: case BUSH2: case BUSH3: case BUSH4: case BUSH5: case BUSH_FLOWER1: case BUSH_FLOWER2: 
		case DOOR_HEAD : case DOOR_PILLAR: case DOOR_PILLAR2: case DOOR_FIRE_BOSS:
		case DOOR_SIDEDOOR: case DOOR_HEAD_NORMAL: case DOOR_PILLAR_NORMAL: case DOOR_PILLAR2_NORMAL: case DOOR_FIRE_NORMAL:
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
			pTransformCom->Set_Pos(tStaticObjData.vPos.x , tStaticObjData.vPos.y, tStaticObjData.vPos.z);

			dynamic_cast<CSpriteObj*>(pGameObject)->Set_DrawID(tStaticObjData.byDrawID);
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_DrawOption(tStaticObjData.byDrawOption);
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_ColliderPos(&tStaticObjData.vColliderPos);
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_ColliderRadius(tStaticObjData.fColliderRadius);
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
		}
	}
	CloseHandle(hFile);

	return 0;
}

void CStage_Normal::Check_PlayerPos()
{
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	CBlack* pBlack = dynamic_cast<CBlack*>(Engine::Get_GameObject(L"Layer_UI", L"Black"));

	_vec3 vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];
	_vec3 vPotRoomPos = { 373.f, vPlayerPos.y , 277.f };
	
	_vec3 vDiff1 = vPlayerPos - vPotRoomPos;

	if (D3DXVec3Length(&vDiff1) < 5.f)
		pBlack->Scene_Change(SCENE_POT , { 159.33f , vPlayerPos.y , 409.22f });
}

void CStage_Normal::Door_Init()
{
	
}

void CStage_Normal::Door_Activate_Check()
{
	CSpriteObj* pObj = nullptr;

	if (g_bNormal_To_Pot)
	{
		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Normal_SideDoor_0"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Normal_NormalDoor_Pillar2_0"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Normal_NormalDoor_Pillar2_0|Clone0"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Normal_NormalDoor_Pillar2_0|Clone1"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Normal_NormalDoor_Pillar2_0|Clone2"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Normal_NormalDoor_Head_0"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Normal_NormalDoor_Pillar_0"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Normal_NormalDoor_Pillar_0|Clone0"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Normal_NormalDoor_Fire_0"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Normal_NormalDoor_Fire_0|Clone0"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Normal_NormalDoor_Fire_0|Clone1"));
		pObj->Set_DoorActive();

		pObj = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Normal_NormalDoor_Fire_0|Clone2"));
		pObj->Set_DoorActive();
	}
}



CStage_Normal * CStage_Normal::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage_Normal *	pInstance = new CStage_Normal(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{ 
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage_Normal::Free(void)
{
	__super::Free();
}
