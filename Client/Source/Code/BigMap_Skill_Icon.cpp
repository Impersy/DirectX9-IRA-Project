#include "stdafx.h"
#include "..\Header\BigMap_Skill_Icon.h"
#include "Export_Function.h"

#include "Player.h"

CBigMap_Skill_Icon::CBigMap_Skill_Icon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
}

CBigMap_Skill_Icon::CBigMap_Skill_Icon(const CBigMap_Skill_Icon& rhs)
	: CFrontUi(rhs)
{
}

CBigMap_Skill_Icon::~CBigMap_Skill_Icon()
{
	Free();
}

HRESULT CBigMap_Skill_Icon::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CBigMap_Skill_Icon::Update_GameObject(const _float& fTimeDelta)
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	_bool bInvenClose = pPlayer->Get_bBigMapUi_On();
	if (bInvenClose == false)
		return OBJ_DEAD;

	Frame_Check(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBigMap_Skill_Icon::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBigMap_Skill_Icon::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	Ui_Print(m_iIcon_Number);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBigMap_Skill_Icon::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Ui_BigMap_Icon"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Ui_BigMap_Icon", pComponent });

	return S_OK;
}

void CBigMap_Skill_Icon::Change_State(void)
{

}

void CBigMap_Skill_Icon::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame = m_iIcon_Number;
}

void CBigMap_Skill_Icon::Ui_Print(_int _iNum)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 38.f;
	matWorld._22 = 38.f;
	matWorld._41 = -154.f + (100.f * _iNum);
	matWorld._42 = -365.f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

_bool CBigMap_Skill_Icon::BigMap_Close(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	if (!(pPlayer->Get_bBigMapUi_On()))
	{
		return true;
	}
	return true;

}

void CBigMap_Skill_Icon::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBigMap_Skill_Icon* CBigMap_Skill_Icon::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _IconNumber)
{
	CBigMap_Skill_Icon* pInstance = new CBigMap_Skill_Icon(pGraphicDev);

	pInstance->m_iIcon_Number = _IconNumber;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBigMap_Skill_Icon::Free(void)
{
	__super::Free();
}
