#include "stdafx.h"
#include "..\Header\Stage_Boss1.h"

#include "Export_Function.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "Doewole.h"
#include "Doewole_Body.h"
#include "Doewole_Shadow.h"
#include "Player_Shadow.h"
#include "Doewole_LeftClaw.h"
#include "Doewole_RightClaw.h"
#include "Effect_Doewole_Vanish.h"
#include "Ghost.h"
#include "SylphBow.h"
#include "SylphBowPair.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "SpriteObj.h"
#include "DestructibleObj.h"
#include "Jar.h"
#include "Lever.h"
#include <KeyMgr.h>
#include "CollisionMgr.h"
#include "AxeEliteEvilSoul.h"
#include "Monster_Shadow.h"
#include <MainApp.h>
#include "Tif_Circle.h"
#include "Rain.h"
#include <Black.h>
#include <SoundMgr.h>
#include "MyMouse.h"


CStage_Boss1::CStage_Boss1(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CStage_Boss1::~CStage_Boss1()
{
}

HRESULT CStage_Boss1::Ready_Scene(void)
{
	g_vNextPos = { 367.f , 7.5f  , 303.f };

	if (FAILED(Ready_Layer_Environment(L"Layer_Environment")))
		return E_FAIL;
	if (FAILED(Ready_Layer_GameLogic(L"Layer_GameLogic")))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;

	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_BOSS_Dowole_Start.wav");

	return S_OK;

}

_int CStage_Boss1::Update_Scene(const _float & fTimeDelta)
{
	if (!m_bTerrainInit)
	{
		//Load_Terrain_Info(L"../../Data/Terrain/TestMap.dat");
		Load_Terrain_Info(L"../../Data/Terrain/BossDowoleMap.dat");
		m_bTerrainInit = true;

		Load_Object_Info(L"../../Data/Object/BossDowoleMapObjStatic1.dat");	// 개구리밥
		Load_Object_Info(L"../../Data/Object/BossDowoleObjStatic2.dat");	// 나무
		Load_Dynamic_Object_Info(L"../../Data/Object/BossDowoleObjDynamic1.dat");	// 풀
		Load_Dynamic_Object_Info(L"../../Data/Object/BossDowoleObjDynamic2.dat");	// 가시
	}

	if (!g_bSphereMake)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_9))
			g_bSphereMake = true;
	}

	if (g_bSphereMake)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_0))
		{
			if (!g_bSphereRender)
				g_bSphereRender = true;
			else
				g_bSphereRender = false;
		}
	}


	return __super::Update_Scene(fTimeDelta);
}

void CStage_Boss1::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();

	if (CKeyMgr::Get_Instance()->Key_Down(KEY_F1))
	{
		g_eCurScene = SCENE_REGEN;
	}
}

void CStage_Boss1::Render_Scene(void)
{
	// _DEBUG용
}

HRESULT CStage_Boss1::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	 

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
	 
	if(FAILED(pLayer->Add_GameObject(L"StaticCamera", pGameObject)))
		return E_FAIL;

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	 
	if (FAILED(pLayer->Add_GameObject(L"SkyBox", pGameObject)))
		return E_FAIL;

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage_Boss1::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	 
	CGameObject*	pGameObject = nullptr;
	CGameObject*	pBulletObject = nullptr;

	CGameObject* pPlayer = CPlayer::Create(m_pGraphicDev);
	if (FAILED(pLayer->Add_GameObject(L"Player", pPlayer)))
		return E_FAIL;
	
	pGameObject = CPlayer_Shadow::Create(m_pGraphicDev);
	dynamic_cast<CPlayer_Shadow*>(pGameObject)->Set_Owner(pPlayer);
	if (FAILED(pLayer->Add_GameObject(L"Player_Shadow", pGameObject)))
		return E_FAIL;

	pGameObject = CSylphBow::Create(m_pGraphicDev,{0.f,0.f,0.f},0.f);
	if (FAILED(pLayer->Add_GameObject(L"SylphBow", pGameObject)))
		return E_FAIL;

	pGameObject = CSylphBowPair::Create(m_pGraphicDev, { 0.f,0.f,0.f }, 0.f);
	if (FAILED(pLayer->Add_GameObject(L"SylphBowPair", pGameObject)))
		return E_FAIL;

	pGameObject = CGhost::Create(m_pGraphicDev);
	if (FAILED(pLayer->Add_GameObject(L"Ghost", pGameObject)))
		return E_FAIL;

	CGameObject* pDoewole = CDoewole::Create(m_pGraphicDev);
	if (FAILED(pLayer->Add_GameObject(L"Doewole", pDoewole)))
		return E_FAIL;

	pGameObject = CDoewole_Body::Create(m_pGraphicDev);
	dynamic_cast<CDoewole_Body*>(pGameObject)->Set_Owner(pDoewole);
	if (FAILED(pLayer->Add_GameObject(L"Doewole_Body", pGameObject)))
		return E_FAIL;

	pGameObject = CDoewole_LeftClaw::Create(m_pGraphicDev);
	dynamic_cast<CDoewole_LeftClaw*>(pGameObject)->Set_Owner(pDoewole);
	if (FAILED(pLayer->Add_GameObject(L"Doewole_LeftClaw", pGameObject)))
		return E_FAIL;

	pGameObject = CDoewole_RightClaw::Create(m_pGraphicDev);
	dynamic_cast<CDoewole_RightClaw*>(pGameObject)->Set_Owner(pDoewole);
	if (FAILED(pLayer->Add_GameObject(L"Doewole_RightClaw", pGameObject)))
		return E_FAIL;

	pGameObject = CDoewole_Shadow::Create(m_pGraphicDev);
	dynamic_cast<CDoewole_Shadow*>(pGameObject)->Set_Owner(pDoewole);
	if (FAILED(pLayer->Add_GameObject(L"Doewole_Shadow", pGameObject)))
		return E_FAIL;
	 
	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage_Boss1::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	 

	// 오브젝트
	CGameObject*	pGameObject = nullptr;

	pGameObject = CStatus::Create(m_pGraphicDev);
	 
	if (FAILED(pLayer->Add_GameObject(L"FrontUI_Status", pGameObject)))
		return E_FAIL;

	pGameObject = CApostle::Create(m_pGraphicDev);
	 
	if (FAILED(pLayer->Add_GameObject(L"FrontUI_Apostle", pGameObject)))
		return E_FAIL;

	pGameObject = CQuickSlot::Create(m_pGraphicDev);
	 
	if (FAILED(pLayer->Add_GameObject(L"FrontUI_QuickSlot", pGameObject)))
		return E_FAIL;

	pGameObject = CMiniMap::Create(m_pGraphicDev);
	 
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


	pGameObject = CBoss_Bar::Create(m_pGraphicDev, BOSS_DOEWOLE);
	 
	if (FAILED(pLayer->Add_GameObject(L"FrontUI_Boss_Bar", pGameObject)))
		return E_FAIL;

	pGameObject = CRain::Create(m_pGraphicDev, 640.f * 1.5f, 480.f * 1.5f, { 0.f,-200.f,0.f });
	 
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



HRESULT CStage_Boss1::Ready_LightInfo()
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

HRESULT CStage_Boss1::Load_Terrain_Info(const _tchar* pPath)
{
	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));
	
	CGameObject* pGameObject = nullptr;
	pGameObject = CTerrain::Create(m_pGraphicDev, TERRAIN_BOSS_DOEWOLE);
	 

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

	//pTerrainBufCom->Set_VtxCnt(tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv);
	//pTerrainBufCom->Set_DetailLevel(tTerrainData.fDetailLevel);
	//pTerrainBufCom->Set_HeightMapLevel(tTerrainData.iHeightMapLevel);
	//pTerrainBufCom->Set_HeightMapFilePath(pHeightmapPath);
	//pTerrainBufCom->Set_VtxPos(tTerrainDataPointer.pPos, tTerrainDataPointer.dwVtxCnt);
	pTerrain->Set_DrawID(tTerrainData.byDrawID);
	pTerrain->Set_DrawOption(tTerrainData.byDrawOption);
	pTerrain->Set_TerrainType(TERRAIN_BOSS_DOEWOLE);

	CloseHandle(hFile);

	Safe_Delete_Array(pHeightmapFilePath);
	//Safe_Delete_Array(pHeightmapPath);

	//pTerrainBufCom->Update_Buffer_Data();

	return S_OK;
}

HRESULT CStage_Boss1::Load_Object_Info(const _tchar* pPath)
{
	HANDLE hFile = CreateFile(pPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	STATICOBJDATA tStaticObjData;

	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	 

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

		strComponentKey = strComponentKey.substr(0, iNum+1);

		dynamic_cast<CStaticObject*>(pGameObject)->Set_StaticObj_ID(dynamic_cast<CStaticObject*>(pGameObject)->CompareID(strComponentKey));
		if (FAILED(pLayer->Add_GameObject(dynamic_cast<CStaticObject*>(pGameObject)->Get_ObjKey(), pGameObject)))
			return E_FAIL;
		// dynamic은 Dynamic으로 따로 빼기
		Safe_Delete_Array<_tchar*>(pObjKey);

		CTransform* pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", dynamic_cast<CStaticObject*>(pGameObject)->Get_ObjKey(), L"Proto_Transform", ID_DYNAMIC));
		 

		pTransformCom->Set_Scale(tStaticObjData.vScale.x, tStaticObjData.vScale.y, tStaticObjData.vScale.z);
		pTransformCom->Set_Angle(tStaticObjData.vAngle.x, tStaticObjData.vAngle.y, tStaticObjData.vAngle.z);
		pTransformCom->Set_Pos(tStaticObjData.vPos.x + 257.f, tStaticObjData.vPos.y, tStaticObjData.vPos.z + 257.f);

		dynamic_cast<CStaticObject*>(pGameObject)->Set_DrawID(tStaticObjData.byDrawID);
		dynamic_cast<CStaticObject*>(pGameObject)->Set_DrawOption(tStaticObjData.byDrawOption);
		dynamic_cast<CStaticObject*>(pGameObject)->Set_ColliderPos(&tStaticObjData.vColliderPos);
		dynamic_cast<CStaticObject*>(pGameObject)->Set_ColliderRadius(tStaticObjData.fColliderRadius);

	}

	CloseHandle(hFile);

	return 0;
}

HRESULT CStage_Boss1::Load_Dynamic_Object_Info(const _tchar* pPath)
{
	HANDLE hFile = CreateFile(pPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	STATICOBJDATA tStaticObjData;

	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	 

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
		{
			pGameObject = CSpriteObj::Create(m_pGraphicDev);
			 
			tempSize = strComponentKey.length();
			lstrcpy(szComponentKey, strComponentKey.c_str());
			//dynamic_cast<CSpriteObj*>(pGameObject)->Set_ObjKey(szComponentKey, dwStrByte);
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_ObjKey(pObjKey, dwStrByte);
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_DynamicObj_ID(dynamic_cast<CSpriteObj*>(pGameObject)->CompareID(strComponentKey));
			if (FAILED(pLayer->Add_GameObject(dynamic_cast<CSpriteObj*>(pGameObject)->Get_ObjKey(), pGameObject)))
				return E_FAIL;
			Safe_Delete_Array<_tchar*>(pObjKey);

			pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", dynamic_cast<CSpriteObj*>(pGameObject)->Get_ObjKey(), L"Proto_Transform", ID_DYNAMIC));
			 
			pTransformCom->Set_Scale(tStaticObjData.vScale.x, tStaticObjData.vScale.y, tStaticObjData.vScale.z);
			pTransformCom->Set_Angle(tStaticObjData.vAngle.x, tStaticObjData.vAngle.y, tStaticObjData.vAngle.z);
			pTransformCom->Set_Pos(tStaticObjData.vPos.x + 257.f, tStaticObjData.vPos.y, tStaticObjData.vPos.z + 257.f);

			dynamic_cast<CSpriteObj*>(pGameObject)->Set_DrawID(tStaticObjData.byDrawID);
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_DrawOption(tStaticObjData.byDrawOption);
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_ColliderPos(&tStaticObjData.vColliderPos);
			dynamic_cast<CSpriteObj*>(pGameObject)->Set_ColliderRadius(tStaticObjData.fColliderRadius);
		}
			break;

		case THORN1: case THORN2: case THORN3: case THORN4: case THORN5: case THORN6:
		{
			pGameObject = CDestructibleObj::Create(m_pGraphicDev);
			 
			tempSize = strComponentKey.length();
			lstrcpy(szComponentKey, strComponentKey.c_str());
			dynamic_cast<CDestructibleObj*>(pGameObject)->Set_ObjKey(pObjKey, dwStrByte);
			dynamic_cast<CDestructibleObj*>(pGameObject)->Set_DynamicObj_ID(dynamic_cast<CDestructibleObj*>(pGameObject)->CompareID(strComponentKey));
			if (FAILED(pLayer->Add_GameObject(dynamic_cast<CDestructibleObj*>(pGameObject)->Get_ObjKey(), pGameObject)))
				return E_FAIL;
			Safe_Delete_Array<_tchar*>(pObjKey);

			pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", dynamic_cast<CDestructibleObj*>(pGameObject)->Get_ObjKey(), L"Proto_Transform", ID_DYNAMIC));
			 
			pTransformCom->Set_Scale(tStaticObjData.vScale.x, tStaticObjData.vScale.y, tStaticObjData.vScale.z);
			pTransformCom->Set_Angle(tStaticObjData.vAngle.x, tStaticObjData.vAngle.y, tStaticObjData.vAngle.z);
			pTransformCom->Set_Pos(tStaticObjData.vPos.x + 257.f, tStaticObjData.vPos.y, tStaticObjData.vPos.z + 257.f);

			dynamic_cast<CDestructibleObj*>(pGameObject)->Set_DrawID(tStaticObjData.byDrawID);
			dynamic_cast<CDestructibleObj*>(pGameObject)->Set_DrawOption(tStaticObjData.byDrawOption);
			_vec3 vColliderPos = tStaticObjData.vColliderPos;
			vColliderPos.x += 257.f;
			vColliderPos.z += 257.f;
			dynamic_cast<CDestructibleObj*>(pGameObject)->Set_ColliderPos(&vColliderPos);
			dynamic_cast<CDestructibleObj*>(pGameObject)->Set_ColliderRadius(tStaticObjData.fColliderRadius);
		}
			break;
		}
	}

	CloseHandle(hFile);

	return 0;
}

CStage_Boss1 * CStage_Boss1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage_Boss1 *	pInstance = new CStage_Boss1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{ 
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage_Boss1::Free(void)
{
	__super::Free();
}
