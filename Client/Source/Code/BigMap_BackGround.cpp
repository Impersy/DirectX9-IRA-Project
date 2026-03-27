#include "stdafx.h"
#include "..\Header\BigMap_BackGround.h"
#include "Export_Function.h"

#include "Player.h"

CBigMap_BackGround::CBigMap_BackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
}

CBigMap_BackGround::CBigMap_BackGround(const CBigMap_BackGround& rhs)
	: CFrontUi(rhs)
{
}

CBigMap_BackGround::~CBigMap_BackGround()
{
	Free();
}

HRESULT CBigMap_BackGround::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CBigMap_BackGround::Update_GameObject(const _float& fTimeDelta)
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	_bool bInvenClose = pPlayer->Get_bBigMapUi_On();
	if (bInvenClose == false)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBigMap_BackGround::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBigMap_BackGround::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	Ui_Print(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBigMap_BackGround::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Ui_BigMap_Back"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Ui_BigMap_Back", pComponent });

	return S_OK;
}

void CBigMap_BackGround::Change_State(void)
{

}

void CBigMap_BackGround::Frame_Check(const _float& fTimeDelta)
{

}

void CBigMap_BackGround::Ui_Print(_int _iNum)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 450.f;
	matWorld._22 = 450.f;
	matWorld._41 = 0.f;
	matWorld._42 = 25.f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

_bool CBigMap_BackGround::BigMap_Close(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	if (!(pPlayer->Get_bBigMapUi_On()))
	{
		return true;
	}
}

void CBigMap_BackGround::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBigMap_BackGround* CBigMap_BackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBigMap_BackGround* pInstance = new CBigMap_BackGround(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBigMap_BackGround::Free(void)
{
	__super::Free();
}
