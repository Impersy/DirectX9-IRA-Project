#include "stdafx.h"
#include "..\Header\Title.h"

#include "Export_Function.h"
#include <KeyMgr.h>
#include <SoundMgr.h>
#include "BackGround.h"
#include "Title_Font.h"
#include "MyMouse.h"


CTitle::CTitle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CTitle::~CTitle()
{
}

HRESULT CTitle::Ready_Scene(void)
{
	
	Ready_Layer_UI(L"Layer_UI");


	CSoundMgr::Get_Instance()->PlayBGM(L"MainTheme.wav");


	return S_OK;

}

_int CTitle::Update_Scene(const _float & fTimeDelta)
{



	

	return __super::Update_Scene(fTimeDelta);
}

void CTitle::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();


	// ¸ÞÀÎ·Îºñ
	if (CKeyMgr::Get_Instance()->Key_Down(KEY_H))
	{
		CSoundMgr::Get_Instance()->PlaySound(L"UI_Game start.wav", CSoundMgr::UI_CLICK, 0.7f);
		g_eCurScene = SCENE_MAINLOBBY;
	}


	// ´øÀü·Îºñ
	if (CKeyMgr::Get_Instance()->Key_Down(KEY_F1))
	{
		g_eCurScene = SCENE_DUNGEONLOBBY;
	}

	// ¸®Á¨¸Ê
	if (CKeyMgr::Get_Instance()->Key_Down(KEY_F2))
	{
		g_eCurScene = SCENE_REGEN;
	}

	// º¸½º µµ¿ï¸Ê
	if (CKeyMgr::Get_Instance()->Key_Down(KEY_F3))
	{
		g_eCurScene = SCENE_BOSS1;
	}

	// º¸½º µµÃ¶¸Ê
	if (CKeyMgr::Get_Instance()->Key_Down(KEY_F4))
	{
		g_eCurScene = SCENE_BOSS2;
	}

	// Æ©Åä¸®¾ó ¸Ê
	if (GetAsyncKeyState(VK_F6))
	{
		g_eCurScene = SCENE_TUTORIAL;
	}

	// »óÁ¡ ¸Ê
	if (GetAsyncKeyState(VK_F9))
	{
		g_eCurScene = SCENE_SHOP;

	}

	// Ç×¾Æ¸®¸Ê
	if (GetAsyncKeyState(VK_F7))
	{
		g_eCurScene = SCENE_POT;
	}

	// ÀÏ¹Ý¸Ê
	if (GetAsyncKeyState(VK_F10))
	{
		g_eCurScene = SCENE_NORMAL;
	}

}

void CTitle::Render_Scene(void)
{
	
	
}


HRESULT CTitle::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	if (nullptr == pLayer) return E_FAIL;

	

	// ¿ÀºêÁ§Æ®
	CGameObject* pGameObject = nullptr;

	pGameObject = CBackGround::Create(m_pGraphicDev);

	pLayer->Add_GameObject(L"BackGround", pGameObject);


	pGameObject = CTitle_Font::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Title_Font", pGameObject)))
		return E_FAIL;

	pGameObject = CMyMouse::Create(m_pGraphicDev);
	if (nullptr == pGameObject) return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"MyMouse_Cursor", pGameObject)))
		return E_FAIL;


	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CTitle * CTitle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTitle *	pInstance = new CTitle(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{ 
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTitle::Free(void)
{
	__super::Free();
}
