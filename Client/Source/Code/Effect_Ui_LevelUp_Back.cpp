#include "stdafx.h"
#include "..\Header\Effect_Ui_LevelUp_Back.h"
#include "Export_Function.h"

#include "Player.h"

CEffect_Ui_LevelUp_Back::CEffect_Ui_LevelUp_Back(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Ui_LevelUp_Back::CEffect_Ui_LevelUp_Back(const CEffect_Ui_LevelUp_Back& rhs)
	: CEffect(rhs)
{
}

CEffect_Ui_LevelUp_Back::~CEffect_Ui_LevelUp_Back()
{
	Free();
}

HRESULT CEffect_Ui_LevelUp_Back::Ready_GameObject(void)
{
	Add_Component();

	m_fMaxFrame = 6.f;

	return S_OK;
}

_int CEffect_Ui_LevelUp_Back::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += (m_fMaxFrame * fTimeDelta * 2);

	if ((m_fFrame > m_fMaxFrame))
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CEffect_Ui_LevelUp_Back::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CEffect_Ui_LevelUp_Back::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	m_pTextureCom->Set_Texture((_int)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	Ui_Print();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEffect_Ui_LevelUp_Back::Ui_Print(void)
{	
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 800.f;
	matWorld._22 = 800.f;
	matWorld._41 = 0;
	matWorld._42 = 0;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CEffect_Ui_LevelUp_Back::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CEffect_Ui_LevelUp_Back* CEffect_Ui_LevelUp_Back::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Ui_LevelUp_Back* pInstance = new CEffect_Ui_LevelUp_Back(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CEffect_Ui_LevelUp_Back::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Ui_LevelUp_Back"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Ui_LevelUp_Back", pComponent });

	return S_OK;
}

void CEffect_Ui_LevelUp_Back::Free(void)
{
	__super::Free();
}



