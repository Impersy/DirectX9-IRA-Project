#include "stdafx.h"
#include "..\Header\MiniMap_Icon.h"
#include "Export_Function.h"

CMiniMap_Icon::CMiniMap_Icon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev), m_iRoomNumber(0), m_bInRoomCheck(false)
{
	//m_tINFO[0]->iMax = 5;
}

CMiniMap_Icon::CMiniMap_Icon(const CMiniMap_Icon& rhs)
	: CFrontUi(rhs), m_iRoomNumber(0), m_bInRoomCheck(false)
{
}

CMiniMap_Icon::~CMiniMap_Icon()
{
}

HRESULT CMiniMap_Icon::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;

	return S_OK;
}

_int CMiniMap_Icon::Update_GameObject(const _float& fTimeDelta)
{	
	Frame_Check(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CMiniMap_Icon::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMiniMap_Icon::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_iIconNumber);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	Ui_MiniMap_Icon_Print(m_iLocNumber);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CMiniMap_Icon::Ui_MiniMap_Icon_Print(_int _LocNumber)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	matWorld._11 = 0.3f;
	matWorld._22 = 0.3f;

	if (1 == _LocNumber)	
	{
		matWorld._41 = 15.9f;
		matWorld._42 = 9.4f;
	}
	else if (2 == _LocNumber) 
	{
		matWorld._41 = 17.2f;
		matWorld._42 = 9.4f;
	}
	else if (3 == _LocNumber) 
	{
		matWorld._41 = 18.5f;
		matWorld._42 = 9.4f;
	}
	else if (4 == _LocNumber) 
	{
		matWorld._41 = 15.9f;
		matWorld._42 = 8.1f;
	}
	else if (5 == _LocNumber)
	{
		matWorld._41 = 17.2f;
		matWorld._42 = 8.1f;
	}
	else if (6 == _LocNumber)
	{
		matWorld._41 = 18.5f;
		matWorld._42 = 8.1f;
	}
	else if (7 == _LocNumber)
	{
		matWorld._41 = 15.9f;
		matWorld._42 = 6.8f;
	}
	else if (8 == _LocNumber)
	{
		matWorld._41 = 17.2f;
		matWorld._42 = 6.8f;
	}
	else if (9 == _LocNumber)
	{
		matWorld._41 = 18.5f;
		matWorld._42 = 6.8f;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

HRESULT CMiniMap_Icon::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Ui_MiniMap_Icon"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Ui_MiniMap_Icon", pComponent });

	return S_OK;
}

void CMiniMap_Icon::Change_State(void)
{

}

void CMiniMap_Icon::Frame_Check(const _float& fTimeDelta)
{
}

void CMiniMap_Icon::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CMiniMap_Icon* CMiniMap_Icon::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iIconNumber, _int _LocNumber)
{
	CMiniMap_Icon* pInstance = new CMiniMap_Icon(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_iIconNumber = _iIconNumber;
	pInstance->m_iLocNumber = _LocNumber;

	return pInstance;
}

void CMiniMap_Icon::Free(void)
{
	__super::Free();
}
