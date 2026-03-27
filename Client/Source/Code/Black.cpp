#include "stdafx.h"
#include "..\Header\Black.h"
#include "Export_Function.h"
#include <KeyMgr.h>

CBlack::CBlack(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

CBlack::CBlack(const CBlack & rhs)
	: Engine::CGameObject(rhs)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pTransformCom(rhs.m_pTransformCom)
{
}

CBlack::~CBlack()
{
}

void CBlack::Set_Alpha(float fAlpha)
{
	m_fAlpha = fAlpha;
}

HRESULT CBlack::Ready_GameObject(void)
{
	Add_Component();
	
	return S_OK;
}

_int CBlack::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	if (m_bFadeOut)
	{
		m_fAlpha += m_fAlphaStep;

		if (m_fAlpha > 255.f)
		{
			m_fAlpha = 255.f;
			m_bFadeOut = false;
		}
	}

	if (m_bFadeIn)
	{
		m_fAlpha -= m_fAlphaStep;

		if (m_fAlpha < 0.f)
		{
			m_fAlpha = 0.f;
			m_bFadeIn = false;
		}
	}

	Engine::Add_RenderGroup(RENDER_BLACK, this);

	return 0;
}

void CBlack::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	// ´øÀü·Îºñ
	if (g_eCurScene != SCENE_DUNGEONLOBBY && CKeyMgr::Get_Instance()->Key_Down(KEY_F1) || m_bChangeScene[SCENE_DUNGEONLOBBY])
	{
		m_bFadeOut = true;
		m_bChangeScene[SCENE_DUNGEONLOBBY] = true;

		if(m_fAlpha == 255.f)
			g_eCurScene = SCENE_DUNGEONLOBBY;
	}

	// ¸®Á¨¸Ê
	if (g_eCurScene != SCENE_REGEN && CKeyMgr::Get_Instance()->Key_Down(KEY_F2) || m_bChangeScene[SCENE_REGEN])
	{
		m_bFadeOut = true;
		m_bChangeScene[SCENE_REGEN] = true;

		if (m_fAlpha == 255.f)
			g_eCurScene = SCENE_REGEN;
	}

	// º¸½º µµ¿ï¸Ê
	if (g_eCurScene != SCENE_BOSS1 && CKeyMgr::Get_Instance()->Key_Down(KEY_F3) || m_bChangeScene[SCENE_BOSS1])
	{
		m_bFadeOut = true;
		m_bChangeScene[SCENE_BOSS1] = true;

		if (m_fAlpha == 255.f)
			g_eCurScene = SCENE_BOSS1;
	}

	// º¸½º µµÃ¶¸Ê
	if (g_eCurScene != SCENE_BOSS2 && CKeyMgr::Get_Instance()->Key_Down(KEY_F4) || m_bChangeScene[SCENE_BOSS2])
	{
		m_bFadeOut = true;
		m_bChangeScene[SCENE_BOSS2] = true;

		if (m_fAlpha == 255.f)
			g_eCurScene = SCENE_BOSS2;
	}

	// Æ©Åä¸®¾ó¸Ê
	if (g_eCurScene != SCENE_TUTORIAL && GetAsyncKeyState(VK_F6) || m_bChangeScene[SCENE_TUTORIAL])
	{
		m_bFadeOut = true;
		m_bChangeScene[SCENE_TUTORIAL] = true;

		if (m_fAlpha == 255.f)
			g_eCurScene = SCENE_TUTORIAL;
	}

	// »óÁ¡¸Ê
	if (g_eCurScene != SCENE_SHOP && GetAsyncKeyState(VK_F9) || m_bChangeScene[SCENE_SHOP])
	{
		m_bFadeOut = true;
		m_bChangeScene[SCENE_SHOP] = true;

		if (m_fAlpha == 255.f)
			g_eCurScene = SCENE_SHOP;
	}

	// µµÀÚ±â¸Ê
	if (g_eCurScene != SCENE_POT && GetAsyncKeyState(VK_F7) || m_bChangeScene[SCENE_POT])
	{
		m_bFadeOut = true;
		m_bChangeScene[SCENE_POT] = true;

		if (m_fAlpha == 255.f)
			g_eCurScene = SCENE_POT;
	}

	// ÀÏ¹Ý¸Ê
	if (g_eCurScene != SCENE_NORMAL && GetAsyncKeyState(VK_F10) || m_bChangeScene[SCENE_NORMAL])
	{
		m_bFadeOut = true;
		m_bChangeScene[SCENE_NORMAL] = true;

		if (m_fAlpha == 255.f)
			g_eCurScene = SCENE_NORMAL;
	}

	// ¸ÞÀÎ·Îºñ
	if (g_eCurScene != SCENE_MAINLOBBY && m_bChangeScene[SCENE_MAINLOBBY])
	{
		m_bFadeOut = true;
		m_bChangeScene[SCENE_MAINLOBBY] = true;

		if (m_fAlpha == 255.f)
			g_eCurScene = SCENE_MAINLOBBY;
	}
}

void CBlack::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((_uint)m_fAlpha, 255, 255, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	Black_Print();

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBlack::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_CubeTex", pComponent });

 	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
 	 
 	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
 
 	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Black"));
 	 
 	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Black", pComponent });

	return S_OK;
}

void CBlack::Black_Print()
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 1600.f;
	matWorld._22 = 900.f;
	matWorld._41 = 0.f;
	matWorld._42 = 0.f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CBlack::Reset_Proj_Matrix()
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBlack * CBlack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBlack * pInstance = new CBlack(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBlack::Free(void)
{
	__super::Free();
}
