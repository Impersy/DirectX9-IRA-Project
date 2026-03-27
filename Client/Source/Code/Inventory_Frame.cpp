#include "stdafx.h"
#include "..\Header\Inventory_Frame.h"
#include "Export_Function.h"

#include "Player.h"

CInventory_Frame::CInventory_Frame(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev), m_fRadius(46.f), m_tRect {0, 0, 0, 0}
{
	//m_tINFO[0]->iMax = 5;
}

CInventory_Frame::CInventory_Frame(const CInventory_Frame& rhs)
	: CFrontUi(rhs), m_fRadius(46.f), m_tRect{ 0, 0, 0, 0 }
{
}

CInventory_Frame::~CInventory_Frame()
{
}

HRESULT CInventory_Frame::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CInventory_Frame::Update_GameObject(const _float& fTimeDelta)
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	_bool bInvenClose = pPlayer->Get_bItemUi_On();
	if (bInvenClose == false)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CInventory_Frame::LateUpdate_GameObject()
{	
	Picking_Check();

	__super::LateUpdate_GameObject();
}

void CInventory_Frame::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	Ui_Print(m_tINFO.iNum);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CInventory_Frame::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Inventory_Frame"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Inventory_Frame", pComponent });

	return S_OK;
}

void CInventory_Frame::Change_State(void)
{

}

void CInventory_Frame::Frame_Check(const _float& fTimeDelta)
{

}

void CInventory_Frame::Ui_Print(_int _iNum)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 49.f;
	matWorld._22 = 49.f;

	switch (_iNum)
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

	m_vPos_frame.x = matWorld._41 + 800.f;
	m_vPos_frame.y = 450.f - matWorld._42;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CInventory_Frame::Picking_Check(void)
{
	m_tRect.left = LONG(m_vPos_frame.x - (m_fRadius));
	m_tRect.top = LONG(m_vPos_frame.y - (m_fRadius));
	m_tRect.right = LONG(m_vPos_frame.x + (m_fRadius));
	m_tRect.bottom = LONG(m_vPos_frame.y + (m_fRadius));

	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		if (m_tINFO.iNum < 11)	// 인벤창 프레임
		{
			m_fFrame = 1;
		}
		if (m_tINFO.iNum >= 11)	// 장비창 프레임
		{
			m_fFrame = 2;
		}
	}
	else
	{
		m_fFrame = 0;
	}
}

void CInventory_Frame::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CInventory_Frame* CInventory_Frame::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CInventory_Frame* pInstance = new CInventory_Frame(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CInventory_Frame::Free(void)
{
	__super::Free();
}
