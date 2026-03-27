#include "stdafx.h"
#include "..\Header\Effect_Ui_LevelUp_Text.h"
#include "Export_Function.h"

#include "LevelUp_Select.h"
#include "Player.h"

CEffect_Ui_LevelUp_Text::CEffect_Ui_LevelUp_Text(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Ui_LevelUp_Text::CEffect_Ui_LevelUp_Text(const CEffect_Ui_LevelUp_Text& rhs)
	: CEffect(rhs)
{
}

CEffect_Ui_LevelUp_Text::~CEffect_Ui_LevelUp_Text()
{
	Free();
}

HRESULT CEffect_Ui_LevelUp_Text::Ready_GameObject(void)
{
	Add_Component();

	m_fMaxFrame = 20.f;

	return S_OK;
}

_int CEffect_Ui_LevelUp_Text::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	m_fFrame += (m_fMaxFrame * fTimeDelta * 0.5);

	if ((m_fFrame > m_fMaxFrame))
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
		Create_LevelUp_Select();
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CEffect_Ui_LevelUp_Text::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CEffect_Ui_LevelUp_Text::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	m_pTextureCom->Set_Texture((_int)0);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	Ui_Print();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEffect_Ui_LevelUp_Text::Ui_Print(void)
{	
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 150.f + (m_fFrame * 1);
	matWorld._22 = 150.f + (m_fFrame * 1);
	matWorld._41 = 5.f;
	matWorld._42 = 50.f + (m_fFrame * 2);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CEffect_Ui_LevelUp_Text::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CEffect_Ui_LevelUp_Text* CEffect_Ui_LevelUp_Text::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Ui_LevelUp_Text* pInstance = new CEffect_Ui_LevelUp_Text(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CEffect_Ui_LevelUp_Text::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Ui_LevelUp_Text"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Ui_LevelUp_Text", pComponent });

	return S_OK;
}

void CEffect_Ui_LevelUp_Text::Create_LevelUp_Select(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pLevelObject = nullptr;

	pLevelObject = CLevelUp_Select::Create(m_pGraphicDev);
	pLayer->Add_BulletObject(pLevelObject);
}

void CEffect_Ui_LevelUp_Text::Free(void)
{
	__super::Free();
}



