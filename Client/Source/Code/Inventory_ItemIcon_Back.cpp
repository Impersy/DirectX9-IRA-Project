#include "stdafx.h"
#include "..\Header\Inventory_ItemIcon_Back.h"
#include "Export_Function.h"

#include "Player.h"

CInventory_ItemIcon_Back::CInventory_ItemIcon_Back(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev), m_pReceive_PlayerInven(nullptr), m_bPrint(false), m_fRadius(45.f), m_iSlotNumber(0), m_tRect {0,0,0,0}, m_PrintNumber(0)

{
}

CInventory_ItemIcon_Back::CInventory_ItemIcon_Back(const CInventory_ItemIcon_Back& rhs)
	: CFrontUi(rhs), m_pReceive_PlayerInven(nullptr), m_bPrint(false), m_fRadius(45.f), m_iSlotNumber(0), m_tRect{ 0,0,0,0 }, m_PrintNumber(0)
{
}

CInventory_ItemIcon_Back::~CInventory_ItemIcon_Back()
{

}

HRESULT CInventory_ItemIcon_Back::Ready_GameObject(void)
{	
	Add_Component();

	m_eCategory = UI_FRONT;

	return S_OK;
}

_int CInventory_ItemIcon_Back::Update_GameObject(const _float& fTimeDelta)
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	_bool bInvenClose = pPlayer->Get_bItemUi_On();
	if (bInvenClose == false)
		return OBJ_DEAD;

	Slot_Check();

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CInventory_ItemIcon_Back::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CInventory_ItemIcon_Back::Render_GameObject()
{	
	if (m_bPrint)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		_matrix matCamWorld;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
		m_pTextureCom->Set_Texture((_uint)m_fFrame);

		Ui_Print(m_PrintNumber);

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
		Reset_Proj_Matrix();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

HRESULT CInventory_ItemIcon_Back::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Inventory_Icon_Back"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Inventory_Icon_Back", pComponent });

	return S_OK;
}

void CInventory_ItemIcon_Back::Change_State(void)
{

}

void CInventory_ItemIcon_Back::Frame_Check(const _float& fTimeDelta)
{	
	
}


void CInventory_ItemIcon_Back::Slot_Check(void)
{
	//if ((1 <= m_iSlotNumber) && (11 > m_iSlotNumber))
	//{
	//	m_tINFO_Now = m_pReceive_PlayerInven->Space[(m_iSlotNumber - 1)];
	//	_int iTemp_iTemnumber = m_tINFO_Now.iItemNumber;

	//	if (0 != iTemp_iTemnumber)
	//	{
	//		m_bPrint = false;
	//	}

	//}

	if ((11 <= m_iSlotNumber) && (15 > m_iSlotNumber))
	{
		m_bPrint = true;
		m_fFrame = 1;
	}

	if ((15 <= m_iSlotNumber) && (19 > m_iSlotNumber))
	{
		m_bPrint = true;
		m_fFrame = 0;
	}
}

void CInventory_ItemIcon_Back::Ui_Print(_int _iNum)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 46.f;
	matWorld._22 = 46.f;

	switch ((_iNum))
	{
	case 1:
		matWorld._41 = 323.f;
		matWorld._42 = 40.f;
		break;
	case 2:
		matWorld._41 = 415.f;
		matWorld._42 = 40.f;
		break;
	case 3:
		matWorld._41 = 507.f;
		matWorld._42 = 40.f;
		break;
	case 4:
		matWorld._41 = 599.f;
		matWorld._42 = 40.f;
		break;
	case 5:
		matWorld._41 = 691.f;
		matWorld._42 = 40.f;
		break;
	case 6:
		matWorld._41 = 323.f;
		matWorld._42 = -52.f;
		break;
	case 7:
		matWorld._41 = 415.f;
		matWorld._42 = -52.f;
		break;
	case 8:
		matWorld._41 = 507.f;
		matWorld._42 = -52.f;
		break;
	case 9:
		matWorld._41 = 599.f;
		matWorld._42 = -52.f;
		break;
	case 10:
		matWorld._41 = 691.f;
		matWorld._42 = -52.f;
		break;

	case 11:
		matWorld._41 = -150.f;
		matWorld._42 = 48.f;
		break;

	case 12:
		matWorld._41 = -50.f;
		matWorld._42 = 118.f;
		break;

	case 13:
		matWorld._41 = 50.f;
		matWorld._42 = 118.f;
		break;

	case 14:
		matWorld._41 = 150.f;
		matWorld._42 = 48.f;
		break;

	case 15:
		matWorld._41 = -150.f;
		matWorld._42 = -60.f;
		break;

	case 16:
		matWorld._41 = -50.f;
		matWorld._42 = -130.f;
		break;

	case 17:
		matWorld._41 = 50.f;
		matWorld._42 = -130.f;
		break;

	case 18:
		matWorld._41 = 150.f;
		matWorld._42 = -60.f;
		break;
	default:
		break;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CInventory_ItemIcon_Back::Set_Receive_PlayerInven(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_pReceive_PlayerInven = pPlayer->Get_InvenInfo();
}

void CInventory_ItemIcon_Back::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CInventory_ItemIcon_Back* CInventory_ItemIcon_Back::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CInventory_ItemIcon_Back* pInstance = new CInventory_ItemIcon_Back(pGraphicDev);

	pInstance->m_iSlotNumber = _iNumber;
	pInstance->m_PrintNumber = _iNumber;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInventory_ItemIcon_Back::Free(void)
{
	__super::Free();
}
