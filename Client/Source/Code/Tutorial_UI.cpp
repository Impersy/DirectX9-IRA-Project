#include "stdafx.h"
#include "..\Header\Tutorial_UI.h"
#include "Export_Function.h"
#include <KeyMgr.h>
#include <Black.h>
#include "Tutorial.h"

CTutorial_UI::CTutorial_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

CTutorial_UI::CTutorial_UI(const CTutorial_UI & rhs)
	: Engine::CGameObject(rhs)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pTransformCom(rhs.m_pTransformCom)
{
}

CTutorial_UI::~CTutorial_UI()
{
}

HRESULT CTutorial_UI::Ready_GameObject(void)
{
	Add_Component();
	
	return S_OK;
}

_int CTutorial_UI::Update_GameObject(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	if (m_fAccTime > 5.f)
	{
		++m_iFrameNum;
		m_fAccTime = 0.f;
	}

	if (m_iFrameNum > 5)
		m_bDead = true;

	if (m_bDead)
	{
		CBlack* pBlack = dynamic_cast<CBlack*>(Engine::Get_GameObject(L"Layer_UI", L"Black"));
		pBlack->Set_Alpha(0.f);

		CTutorial* pTutorialScene = dynamic_cast<CTutorial*>(Engine::Get_Scene());
		pTutorialScene->Set_ChasePlayer();

		return OBJ_DEAD;
	}

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_TUTORIALUI, this);

	return 0;
}

void CTutorial_UI::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

}

void CTutorial_UI::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(m_iFrameNum);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	Tutorial_UI_Print();

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTutorial_UI::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_CubeTex", pComponent });

 	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
 	 
 	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
 
 	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Tutorial_UI"));
 	 
 	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Tutorial_UI", pComponent });

	return S_OK;
}

void CTutorial_UI::Tutorial_UI_Print()
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 350.f;
	matWorld._22 = 350.f;
	matWorld._41 = 0.f;
	matWorld._42 = 0.f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CTutorial_UI::Reset_Proj_Matrix()
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CTutorial_UI * CTutorial_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTutorial_UI * pInstance = new CTutorial_UI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTutorial_UI::Free(void)
{
	__super::Free();
}
