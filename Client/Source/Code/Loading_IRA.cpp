#include "stdafx.h"
#include "..\Header\Loading_IRA.h"

#include "Export_Function.h"
#include "Title.h"
#include "MyMouse.h"
#include "DungeonLobby.h"
#include "Stage_Boss1.h"
#include "Stage_Regen.h"
#include "Stage_Boss2.h"
#include "Tutorial.h"
#include "Stage_Shop.h"
#include "Begining_Credit.h"
#include "Main_Lobby.h"
#include "Stage_Pot.h"
#include <Stage_Normal.h>

CLoading_IRA::CLoading_IRA(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
	, m_pThreadMgr(nullptr)
{
}


CLoading_IRA::~CLoading_IRA()
{
}

HRESULT CLoading_IRA::Ready_Scene(SCENE_TYPE eCurScene)
{
	if (!g_bTest)
	{
		Ready_Prototype();
		g_bTest = true;
	}
	
	Ready_Layer_Environment(L"Layer_Environment");

	m_eCurScene = eCurScene;

	switch (m_eCurScene)
	{
	case SCENE_TITLE:
		m_pThreadMgr = CThreadMgr::Create(m_pGraphicDev, LOADING_TITLE);
		break;
	case SCENE_TUTORIAL:
		m_pThreadMgr = CThreadMgr::Create(m_pGraphicDev, LOADING_TUTORIAL);
		break;
	case SCENE_MAINLOBBY:
		m_pThreadMgr = CThreadMgr::Create(m_pGraphicDev, LOADING_MAINLOBBY);
		break;
	case SCENE_DUNGEONLOBBY:
		m_pThreadMgr = CThreadMgr::Create(m_pGraphicDev, LOADING_DUNGEONLOBBY);
		break;
	case SCENE_REGEN:
		m_pThreadMgr = CThreadMgr::Create(m_pGraphicDev, LOADING_REGEN);
		break;
	case SCENE_BOSS1:
		m_pThreadMgr = CThreadMgr::Create(m_pGraphicDev, LOADING_BOSS1);
		break;
	case SCENE_BOSS2:
		m_pThreadMgr = CThreadMgr::Create(m_pGraphicDev, LOADING_BOSS2);
		break;
	case SCENE_SHOP:
		m_pThreadMgr = CThreadMgr::Create(m_pGraphicDev, LOADING_SHOP);
		break;
	case SCENE_POT:
		m_pThreadMgr = CThreadMgr::Create(m_pGraphicDev, LOADING_POT);
		break;
	case SCENE_NORMAL:
		m_pThreadMgr = CThreadMgr::Create(m_pGraphicDev, LOADING_NORMAL);
		break;
	default:
		break;
	}
	


	return S_OK;
}

_int CLoading_IRA::Update_Scene(const _float & fTimeDelta)
{
	_int iExit = __super::Update_Scene(fTimeDelta);

	if (true == m_pThreadMgr->Get_Finish())
	{
		//if (GetAsyncKeyState(VK_RETURN))
		{
			CScene* pScene = nullptr;

			switch (m_eCurScene)
			{
			case SCENE_TITLE:
				pScene = CTitle::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return -1;
				Engine::Set_Scene(pScene);
				break;
			case SCENE_TUTORIAL:
				pScene = CTutorial::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return -1;
				Engine::Set_Scene(pScene);
				break;

			case SCENE_MAINLOBBY:
				pScene = CMain_Lobby::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return -1;
				Engine::Set_Scene(pScene);
				break;
			case SCENE_DUNGEONLOBBY:
				pScene = CDungeonLobby::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return -1;
				Engine::Set_Scene(pScene);
				break;
			case SCENE_REGEN:
				pScene = CStage_Regen::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return -1;
				Engine::Set_Scene(pScene);
				break;
			case SCENE_BOSS1:
				pScene = CStage_Boss1::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return -1;
				Engine::Set_Scene(pScene);
				break;
			case SCENE_BOSS2:
				pScene = CStage_Boss2::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return -1;
				Engine::Set_Scene(pScene);
				break;
			case SCENE_SHOP:
				pScene = CStage_Shop::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return -1;
				Engine::Set_Scene(pScene);
				break;
			case SCENE_POT:
				pScene = CStage_Pot::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return -1;
				Engine::Set_Scene(pScene);
				break;
			case SCENE_NORMAL:
				pScene = CStage_Normal::Create(m_pGraphicDev);
				if (pScene == nullptr)
					return -1;
				Engine::Set_Scene(pScene);
				break;

			case SCENE_END:
				break;
			default:
				break;

			}
		}
	}

	return iExit;
}

void CLoading_IRA::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CLoading_IRA::Render_Scene(void)
{
	// _DEBUG용
	/*if (g_eCurScene != SCENE_TITLE)
		Engine::Render_Font(L"Font_Default", m_pThreadMgr->Get_String(), &_vec2(20.f, WINCY - 50.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));*/



}

HRESULT CLoading_IRA::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	 

	// 여기서 로딩할때 로고들 띄울것 삽입
	if (g_eCurScene == SCENE_TITLE) {


		CGameObject* pGameObject = nullptr;

		pGameObject = CBegining_Credit::Create(m_pGraphicDev);

		pLayer->Add_GameObject(L"Begining_Credit", pGameObject);

		ShowCursor(false);
	}

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CLoading_IRA::Ready_Prototype(void)
{
	Engine::Ready_ProtoComponent(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev));
	Engine::Ready_ProtoComponent(L"Proto_Texture_Logo", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Spr_Tilte_Illustration.png"));
	Engine::Ready_ProtoComponent(L"Proto_Texture_Title_Font", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/BeginC_font.png"));
	Engine::Ready_ProtoComponent(L"Proto_Texture_Jusin", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Jusin.png"));
	Engine::Ready_ProtoComponent(L"Proto_Texture_Suna", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Suna.png"));
	Engine::Ready_ProtoComponent(L"Proto_Texture_Team", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Team.png"));
	Engine::Ready_ProtoComponent(L"Proto_Transform", CTransform::Create(m_pGraphicDev));
		
	
	return S_OK;
}

CLoading_IRA* CLoading_IRA::Create(LPDIRECT3DDEVICE9 pGraphicDev ,SCENE_TYPE eCurScene)
{
	CLoading_IRA*	pInstance = new CLoading_IRA(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene(eCurScene)))
	{ 
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLoading_IRA::Free(void)
{
	Safe_Release(m_pThreadMgr);

	__super::Free();
}
