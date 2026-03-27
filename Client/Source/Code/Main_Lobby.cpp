#include "stdafx.h"
#include "..\Header\Main_Lobby.h"

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
#include <KeyMgr.h>
#include "CollisionMgr.h"
#include "Docheol.h"
#include "Docheol_Tile.h"
#include "Docheol_FloatingObj.h"
#include "Main_Lobby_Tile.h"
#include "Main_Lobby_Ball.h"
#include "Main_Lobby_Altar.h"
#include "Main_Lobby_Grail.h"
#include "Main_Lobby_Bow.h"
#include "Main_Lobby_Cloud.h"
#include "Main_Lobby_Book.h"
#include "Main_Lobby_Portal.h"
#include "Main_Lobby_Portal_Back.h"
#include "Main_Lobby_Portal_Effect.h"
#include "Main_Lobby_Column.h"
#include "Npc_ApostleNpc_Time.h"
#include "Inventory.h"
#include <Black.h>
#include <MovingObj.h>
#include <SoundMgr.h>
#include "Interaction_Dialog.h"
#include "MyMouse.h"


CMain_Lobby::CMain_Lobby(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CMain_Lobby::~CMain_Lobby()
{
}

HRESULT CMain_Lobby::Ready_Scene(void)
{
	if (FAILED(Ready_Layer_Environment(L"Layer_Environment")))
		return E_FAIL;
	if (FAILED(Ready_Layer_GameLogic(L"Layer_GameLogic")))
		return E_FAIL;
	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;

	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	CSoundMgr::Get_Instance()->PlayBGM(L"MainLobby.wav");


	
	
	

	return S_OK;

}

_int CMain_Lobby::Update_Scene(const _float & fTimeDelta)
{
	if (!m_bTerrainInit)
	{
		m_bTerrainInit = true;
	}

	if (g_Game_Clear == true && CKeyMgr::Get_Instance()->Key_Down(KEY_L)) {
		CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera")); \
			pCam->m_bEnding = true;

		// 브금 교체(웅장한 브금)
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"MainTheme_Ending.wav");

	}


	if (g_Game_Clear == false) {



		if (g_UI_Render == false && g_Story_Book == false) {

			if (g_Dialog_Cnt == 0)
				Set_String(L"티프 :  이..이라님..?! 드디어 돌아오셨군요!");
			else if (g_Dialog_Cnt == 1)
				Set_String(L"티프가 깜짝 놀라며 미소짓는다.");
			else if (g_Dialog_Cnt == 2)
				Set_String(L" 연  :  당신은 누구죠?");
			else if (g_Dialog_Cnt == 3)
				Set_String(L"티프 :  아... 역시나 기억이 온전하지 않으시군요.");
			else if (g_Dialog_Cnt == 4)
				Set_String(L"티프 :  이라님의 마지막 기억은 무엇인가요?");
			else if (g_Dialog_Cnt == 5)
				Set_String(L" 연  :  분명 자습실이었는데...여기는 어디인가요..?");
			else if (g_Dialog_Cnt == 6)
				Set_String(L"연이 머리를 부여잡으며 혼란스러워한다.");
			else if (g_Dialog_Cnt == 7)
				Set_String(L"티프 :  여기는 천계.");
			else if (g_Dialog_Cnt == 8)
				Set_String(L"티프 :  이라님이 당신의 사도들을 위해 만든 공간입니다.");
			else if (g_Dialog_Cnt == 9)
				Set_String(L" 연  :  왜 저를 '이라'라고 부르시나요? 처음 듣는 이름인데..");
			else if (g_Dialog_Cnt == 10)
				Set_String(L"티프 :  당신은 세상에 하나뿐인 질서의 신, '이라' 입니다.");
			else if (g_Dialog_Cnt == 11)
				Set_String(L"연이 화들짝 놀라며");
			else if (g_Dialog_Cnt == 12)
				Set_String(L" 연  :  제가 신이라구요? 그게 대체 무슨 소리에요!");
			else if (g_Dialog_Cnt == 13)
				Set_String(L"티프 :  지금은 설명할 시간이 없습니다. ");
			else if (g_Dialog_Cnt == 14)
				Set_String(L"티프 :  혼돈의 신, '도철' 이 이라님이 없는 틈을 타 봉인을 풀고 날뛰고 있습니다.");
			else if (g_Dialog_Cnt == 15)
				Set_String(L"티프 :  하루빨리 소멸시키지 않으면 세계의 균형이 무너질 겁니다.");
			else if (g_Dialog_Cnt == 16)
				Set_String(L" 연  :  윽...(흐릿한 기억들이 들어오며 머리가 깨질듯이 아파온다)");
			else if (g_Dialog_Cnt == 17)
				Set_String(L"연을 부축해준다.");
			else if (g_Dialog_Cnt == 18)
				Set_String(L"티프 :  너무 걱정마세요. 뒤에 있는 책이 당신이 기억을 찾는데 도움을 줄 겁니다.");
			else if (g_Dialog_Cnt == 19)
				Set_String(L"티프 :  저희 사도들은 항상 당신 곁에 있습니다.");
			else if (g_Dialog_Cnt == 20)
				Set_String(L"티프 :  다시 한번 이 세상을 구해주세요..!");

			m_SzFrame[g_Dialog_Cnt] += 13.f * fTimeDelta * 1.f;

			if (m_SzFrame[g_Dialog_Cnt] < m_szTotalString.length() + 1) {

				CSoundMgr::Get_Instance()->PlaySound(L"typewriter.wav", CSoundMgr::TEXT_SOUND, 0.5f);
				m_szString.clear();
				for (int i = 0; i < (int)m_SzFrame[g_Dialog_Cnt] + 1; i++) {
					m_szString.push_back(m_szTotalString[i]);
				}
			}
			else {
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TEXT_SOUND);
			}


		}
		else {

			for (int i = 0; i < 21; i++) {
				m_SzFrame[i] = 0.f;
			}
		}
	}
	else {
		CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera"));
		bool m_bEnding = pCam->m_bEnding;

		if (g_Ending_Logo != true && m_bEnding != true) {

			if (g_UI_Render == false && g_Story_Book == false) {

				if (g_Dialog_Cnt == 0)
					Set_String(L"티프가 안도의 한숨을 내쉰다.");
				else if (g_Dialog_Cnt == 1)
					Set_String(L"티프 : 후..드디어 우리들의 오랜 숙적인 도철이 소멸했습니다.");
				else if (g_Dialog_Cnt == 2)
					Set_String(L"티프 : 이제 이 세상은 평화로 가득 채워질 겁니다.");
				else if (g_Dialog_Cnt == 3)
					Set_String(L"티프 : 모두 이라님 덕분입니다. 감사합니다.");
				else if (g_Dialog_Cnt == 4)
					Set_String(L" 연  : 제가 없는 동안 모두들 애써줘서 고맙습니다.");
				else if (g_Dialog_Cnt == 5)
					Set_String(L" 연  : 저는 질서의 신 '이라' 이며 그와 동시에 '연' 이기도 합니다.");
				else if (g_Dialog_Cnt == 6)
					Set_String(L" 연  : 이곳은 이제 안전해진 것 같군요..하지만 '연' 이 살던 세상은 아직입니다.");
				else if (g_Dialog_Cnt == 7)
					Set_String(L" 연  : 아직도 자습실에서 고통받는 친구들이 많습니다.");
				else if (g_Dialog_Cnt == 8)
					Set_String(L" 연  : 그들을 위해 저는 다시 가봐야 할 것 같습니다.");
				else if (g_Dialog_Cnt == 9)
					Set_String(L"티프가 아쉬워한다.");
				else if (g_Dialog_Cnt == 10)
					Set_String(L"티프 : 이라..아니 연님의 생각을 존중합니다.");
				else if (g_Dialog_Cnt == 11)
					Set_String(L"티프 : 떠나신다니 슬프지만..이곳은 저희 사도들이 잘 수호하겠습니다.");
				else if (g_Dialog_Cnt == 12)
					Set_String(L"티프 : 언제든지 마음이 바뀌면 다시 오세요. 기다리고 있겠습니다.");
				else if (g_Dialog_Cnt == 13)
					Set_String(L" 연  : 물론이죠. 모든 세상이 평화로워지는 날에 만나요..!");
				else if (g_Dialog_Cnt == 14)
					Set_String(L"연은 옅은 미소를 지으며 떠나간다.");


				m_SzFrame[g_Dialog_Cnt] += 11.f * fTimeDelta * 1.f;

				if (m_SzFrame[g_Dialog_Cnt] < m_szTotalString.length() + 1) {

					CSoundMgr::Get_Instance()->PlaySound(L"typewriter.wav", CSoundMgr::TEXT_SOUND, 0.5f);
					m_szString.clear();
					for (int i = 0; i < (int)m_SzFrame[g_Dialog_Cnt] + 1; i++) {
						m_szString.push_back(m_szTotalString[i]);
					}
				}
				else {
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TEXT_SOUND);
				}


			}
			else {

				for (int i = 0; i < 21; i++) {
					m_SzFrame[i] = 0.f;
				}

				

				


			}

		}
	}



	Check_PlayerPos();


	return __super::Update_Scene(fTimeDelta);
}

void CMain_Lobby::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CMain_Lobby::Render_Scene(void)
{
	// _DEBUG용

	 CStaticCamera * pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera"));
	 bool m_bEnding_Logo = pCam->m_bEnding_Logo;
	 bool m_bEnding = pCam->m_bEnding;

	if (g_UI_Render == false && g_Story_Book == false && m_bEnding_Logo != true && m_bEnding != true) {
		Engine::Render_Font(L"Font_Title", m_szString.c_str(), &_vec2(WINCX * 0.5f - 7.f * m_szString.length(), WINCY - 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}


}

void CMain_Lobby::Check_PlayerPos()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	CBlack* pBlack = dynamic_cast<CBlack*>(Engine::Get_GameObject(L"Layer_UI", L"Black"));

	_vec3 vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];

	if (vPlayerPos.z > 461.f) {
		//pBlack->Scene_Change(SCENE_TUTORIAL, TUTORIAL_TELEPORT);
		pBlack->Scene_Change(SCENE_DUNGEONLOBBY, DUNGEONLOBBY_TELEPORT);
	}
	

}

HRESULT CMain_Lobby::Ready_Layer_Environment(const _tchar* pLayerTag)
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

	if (FAILED(pLayer->Add_GameObject(L"StaticCamera", pGameObject)))
		return E_FAIL;


	pGameObject = CSkyBox::Create(m_pGraphicDev);
	
	if (FAILED(pLayer->Add_GameObject(L"SkyBox", pGameObject)))
		return E_FAIL;

	


	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMain_Lobby::Ready_Layer_GameLogic(const _tchar * pLayerTag)
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


	pGameObject = CNpc_ApostleNpc_Time::Create(m_pGraphicDev, { 383.f,5.f * PUBLIC_SCALE,340.f });
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"NPC_Tif", pGameObject)))
		return E_FAIL;




	for (int i = 0; i < 30; i++) {
		pGameObject = CMain_Lobby_Tile::Create(m_pGraphicDev, { 128.f + 256.f, 0.05f - 2.f * i , 128.f + 256.f }, 1.f - 1.f * (0.007f * i));
		pLayer->Add_BulletObject(pGameObject);
			
	}
	

	pGameObject = CMain_Lobby_Portal::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"MainLobby_Portal", pGameObject)))
		return E_FAIL;

	pGameObject = CMain_Lobby_Portal_Back::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"MainLobby_Portal_Back", pGameObject)))
		return E_FAIL;
	
	pGameObject = CMain_Lobby_Portal_Effect::Create(m_pGraphicDev);

	pLayer->Add_BulletObject(pGameObject);


	for (int i = 0; i < 7; i++) {
		pGameObject = CMain_Lobby_Column::Create(m_pGraphicDev, { 270.f + 13.f * i, 34.f, 398.f + 8.f * i });
		pLayer->Add_BulletObject(pGameObject);

	}

	for (int i = 0; i < 7; i++) {
		pGameObject = CMain_Lobby_Column::Create(m_pGraphicDev, { 384.f + 384.f - 270.f - 13.f * i, 34.f, 398.f + 8.f * i });
		pLayer->Add_BulletObject(pGameObject);

	}
	


	pGameObject = CMain_Lobby_Ball::Create(m_pGraphicDev, { 317.5f + 3.f,13.f - 1.f,378.8f }, { 9.f,9.f,1.f });
	pLayer->Add_GameObject(L"MainLobby_Ball_Left", pGameObject);

	pGameObject = CMain_Lobby_Alter::Create(m_pGraphicDev, { 318.f + 3.f,9.f - 1.f,379.f });
	pLayer->Add_GameObject(L"MainLobby_Altar_Left", pGameObject);

	pGameObject = CMain_Lobby_Ball::Create(m_pGraphicDev, { 382.f + 384.f - (317.5f + 3.f),13.f - 1.f,378.8f }, { 9.f,9.f,1.f });
	pLayer->Add_GameObject(L"MainLobby_Ball_Right", pGameObject);

	pGameObject = CMain_Lobby_Alter::Create(m_pGraphicDev, { 383.f + 384.f - (318.f + 3.f),9.f - 1.f,379.f });
	pLayer->Add_GameObject(L"MainLobby_Altar_Right", pGameObject);


	for (int i = 0; i < 3; i++) {
		pGameObject = CMain_Lobby_Grail::Create(m_pGraphicDev, i, { 365.f - 16.f * i, 7.f, 413.f - 10.f * i });

		pLayer->Add_BulletObject(pGameObject);

	}

	for (int i = 0; i < 3; i++) {
		pGameObject = CMain_Lobby_Grail::Create(m_pGraphicDev, i + 3, { 384.f + 384.f - (365.f - 16.f * i), 7.f, 413.f - 10.f * i });

		pLayer->Add_BulletObject(pGameObject);

	}

	for (int i = 0; i < 3; i++) {
		pGameObject = CMain_Lobby_Bow::Create(m_pGraphicDev, i, { 365.f - 16.f * i, 9.f, 413.f - 10.f * i });

		pLayer->Add_BulletObject(pGameObject);

	}

	for (int i = 0; i < 3; i++) {
		pGameObject = CMain_Lobby_Bow::Create(m_pGraphicDev, i + 3, { 384.f + 384.f - (365.f - 16.f * i), 9.f, 413.f - 10.f * i });

		pLayer->Add_BulletObject(pGameObject);

	}

	pGameObject = CMain_Lobby_Book::Create(m_pGraphicDev);

	pLayer->Add_BulletObject(pGameObject);



	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMain_Lobby::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	if (nullptr == pLayer) return E_FAIL;

	CGameObject* pGameObject = nullptr;

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

HRESULT CMain_Lobby::Ready_LightInfo()
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

HRESULT CMain_Lobby::Load_Terrain_Info(const _tchar* pPath)
{
	CLayer* pLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));
	
	CGameObject* pGameObject = nullptr;
	pGameObject = CTerrain::Create(m_pGraphicDev, TERRAIN_BOSS_DOEWOLE);
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
	//Safe_Delete_Array(pHeightmapPath);

	//pTerrainBufCom->Update_Buffer_Data();

	return S_OK;
}

HRESULT CMain_Lobby::Load_Dynamic_Object_Info(const _tchar* pPath)
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
		case FLOATING:
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

CMain_Lobby * CMain_Lobby::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMain_Lobby *	pInstance = new CMain_Lobby(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{ 
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMain_Lobby::Free(void)
{
	__super::Free();
}
