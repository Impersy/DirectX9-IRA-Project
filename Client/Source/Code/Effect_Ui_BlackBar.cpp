#include "stdafx.h"
#include "..\Header\Effect_Ui_BlackBar.h"
#include "Export_Function.h"

#include "Player.h"
#include "StaticCamera.h"

CEffect_Ui_BlackBar::CEffect_Ui_BlackBar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Ui_BlackBar::CEffect_Ui_BlackBar(const CEffect_Ui_BlackBar& rhs)
	: CEffect(rhs)
{
}

CEffect_Ui_BlackBar::~CEffect_Ui_BlackBar()
{
	Free();
}

HRESULT CEffect_Ui_BlackBar::Ready_GameObject(void)
{
	Add_Component();

	m_fMaxFrame = 10.f;

	g_UI_Render = false;


	CAMERA_ZOOM_IN;


	return S_OK;
}

_int CEffect_Ui_BlackBar::Update_GameObject(const _float& fTimeDelta)
{

	if (m_bDead)
		return OBJ_DEAD;




	m_fFrame += (m_fMaxFrame * fTimeDelta * 0.5f);

	if (m_iAlpha < 255)
	{
		m_iAlpha = (_int)((255 / m_fMaxFrame) * m_fFrame);
	}
	else if (m_iAlpha > 255)
	{
		m_iAlpha = 255;
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CEffect_Ui_BlackBar::LateUpdate_GameObject()
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_bPlayer_Dialog = pPlayer->Get_bDialog_On();

	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{	
		m_fFrame = m_fMaxFrame;
	}

	if (m_bPlayer_Dialog == false)
	{
		m_bDead = true;
	}
}

void CEffect_Ui_BlackBar::Render_GameObject()
{	
	if (g_eCurScene != SCENE_MAINLOBBY)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pTextureCom->Set_Texture(0);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	if (m_bDead == false)
	{
		Ui_Print();
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);

	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEffect_Ui_BlackBar::Ui_Print(void)
{	
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	matWorld._11 = 20.f;
	matWorld._22 = 11.25f;
	matWorld._41 = 0;
	matWorld._42 = 0;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CEffect_Ui_BlackBar::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CEffect_Ui_BlackBar* CEffect_Ui_BlackBar::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CEffect_Ui_BlackBar* pInstance = new CEffect_Ui_BlackBar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CEffect_Ui_BlackBar::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Ui_BlackBar"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Ui_BlackBar", pComponent });

	return S_OK;
}

void CEffect_Ui_BlackBar::Free(void)
{
	g_UI_Render = true;
	CAMERA_ZOOM_IN_RETURN;
	__super::Free();
}



