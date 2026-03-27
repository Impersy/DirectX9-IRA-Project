#include "stdafx.h"
#include "..\Header\MainApp.h"
#include "Loading_IRA.h"
#include "CollisionMgr.h"
#include <KeyMgr.h>
#include "Black.h"
#include "SoundMgr.h"


CMainApp::CMainApp()
	: m_eCurScene(g_eCurScene)
	, m_ePreScene(g_eCurScene)
{
}


CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp(void)
{
	CSoundMgr::Get_Instance()->Initialize();

	if (FAILED(Ready_DefaultSetting(&m_pGraphicDev)))
		return E_FAIL;
	if (FAILED(SetUp_Scene(m_pGraphicDev, &m_pManagementClass)))
		return E_FAIL;

	

	return S_OK;
}

int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	Engine::Update_DInput();



	CKeyMgr::Get_Instance()->Update();

	m_pManagementClass->Update_Management(fTimeDelta);

	if (GetAsyncKeyState('X'))
	{
		g_bDLobby_To_Boss = true;
		g_bNormal_To_Pot = true;
		g_bPot_To_Regen = true;
		g_bRegen_To_Pot = true;
	}

	return 0;
}

void CMainApp::LateUpdate_MainApp(void)
{
	m_pManagementClass->LateUpdate_Management();

	CCollisionMgr::GetInstance()->Collision_Update();

	Check_SceneChange();
}

void CMainApp::Render_MainApp(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	m_pManagementClass->Render_Management(m_pGraphicDev);

	Engine::Render_End();
}

HRESULT CMainApp::Ready_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	if (FAILED(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
		return E_FAIL;
	m_pDeviceClass->AddRef();		// 디바이스 클래스의 참조 카운트

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();		// 그래픽 디바이스 컴객체 참조 카운트

	if (FAILED(Engine::Ready_Font((*ppGraphicDev), L"Font_Default", L"바탕", 15, 20, FW_HEAVY)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font((*ppGraphicDev), L"Font_Jinji", L"궁서", 15, 20, FW_THIN)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Font((*ppGraphicDev), L"Font_Title", L"Yoon® 민국", 18, 24, FW_BOLD)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font((*ppGraphicDev), L"Font_Title_2", L"Yoon® 민국", 15, 20, FW_BOLD)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Font((*ppGraphicDev), L"Font_Title_3", L"서울한강 장체 L", 11, 23, FW_LIGHT)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Font((*ppGraphicDev), L"Font_Info_1", L"리디바탕", 15, 18, FW_THIN)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Font((*ppGraphicDev), L"Font_Info_2", L"리디바탕", 14, 16, FW_THIN)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Font((*ppGraphicDev), L"Font_Dialog_Name", L"리디바탕", 22, 24, FW_THIN)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font((*ppGraphicDev), L"Font_Dialog_Text", L"리디바탕", 18, 20, FW_THIN)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Font((*ppGraphicDev), L"Font_Money_01", L"Yoon® 민국", 14, 17, FW_REGULAR)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Font((*ppGraphicDev), L"Font_Money_02", L"서울한강 장체 L", 10, 20, FW_LIGHT)))
		return E_FAIL;

	// 출력 : Engine::Render_Font(L"Font_Default", m_pThreadMgr->Get_String(), &_vec2(20.f, 20.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	if (FAILED(Engine::Ready_DInput(g_hInst, g_hWnd)))
		return E_FAIL;


	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	

	return S_OK;
}

HRESULT CMainApp::SetUp_Scene(LPDIRECT3DDEVICE9 pGraphicDev, CManagement ** ppManagement)
{
	CScene*		pScene = nullptr;
	pScene = CLoading_IRA::Create(pGraphicDev , m_eCurScene);
	if (nullptr == pScene) return E_FAIL;

	if (FAILED(Engine::Create_Management(ppManagement)))
		return E_FAIL;
	(*ppManagement)->AddRef();

	if (FAILED((*ppManagement)->Set_Scene(pScene)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Check_SceneChange()
{
	m_eCurScene  = g_eCurScene ;

	if (m_eCurScene != g_ePreScene)
	{
		CSoundMgr::Get_Instance()->StopAll();
		Engine::Delete_Scene();
		SetUp_Scene(m_pGraphicDev, &m_pManagementClass);
	}

	g_ePreScene = m_eCurScene;
}

CMainApp * CMainApp::Create(void)
{
	CMainApp *		pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		Safe_Release<CMainApp*>(pInstance);
		return pInstance;
	}

	return pInstance;
}

void CMainApp::Free(void)
{
	CCollisionMgr::GetInstance()->DestroyInstance();
	CKeyMgr::Get_Instance()->Destroy_Instance();
	CSoundMgr::Get_Instance()->Destroy_Instance();

	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pManagementClass);
	Safe_Release(m_pGraphicDev);

	Engine::Release_Utility();
	Engine::Release_System();

	

}
