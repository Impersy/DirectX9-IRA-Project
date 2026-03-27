#include "stdafx.h"
#include "..\Header\Inventory_ItemIcon.h"
#include "Export_Function.h"

#include "Player.h"

#include "SoundMgr.h"

CInventory_ItemIcon::CInventory_ItemIcon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev), m_pReceive_PlayerInven(nullptr), m_bPrint(false), m_bDrag(false), m_fRadius(45.f), m_bDown(false), m_bDrop(false), m_iSlotNumber(0), m_tRect{ 0,0,0,0 },
	m_Drop_pt{ 0, 0 }, m_Down_pt{ 0, 0 }, m_PrintNumber(0), m_vPos_Origin(0.f, 0.f, 0.f), m_iDownSpace(0), m_iDropSpace(0)

{
}

CInventory_ItemIcon::CInventory_ItemIcon(const CInventory_ItemIcon& rhs)
	: CFrontUi(rhs), m_pReceive_PlayerInven(nullptr), m_bPrint(false), m_bDrag(false), m_fRadius(45.f), m_bDown(false), m_bDrop(false), m_iSlotNumber(0), m_tRect{ 0, 0, 0, 0 },
	m_Drop_pt{ 0, 0 }, m_Down_pt{ 0, 0 }, m_PrintNumber(0), m_vPos_Origin(0.f, 0.f, 0.f), m_iDownSpace(0), m_iDropSpace(0)
{
}

CInventory_ItemIcon::~CInventory_ItemIcon()
{
}

HRESULT CInventory_ItemIcon::Ready_GameObject(void)
{	
	Add_Component();

	//Set_Receive_PlayerInven();

	vTemp[0] = { 323.f, 40.f, 0.f };
	vTemp[1] = { 415.f, 40.f, 0.f };
	vTemp[2] = { 507.f, 40.f, 0.f };
	vTemp[3] = { 599.f, 40.f, 0.f };
	vTemp[4] = { 691.f, 40.f, 0.f };
	vTemp[5] = { 323.f, -52.f, 0.f };
	vTemp[6] = { 415.f, -52.f, 0.f };
	vTemp[7] = { 507.f, -52.f, 0.f };
	vTemp[8] = { 599.f, -52.f, 0.f };
	vTemp[9] = { 691.f, -52.f, 0.f };
	vTemp[10] = { -150.f, 48.f, 0.f };
	vTemp[11] = { -50.f, 118.f, 0.f };
	vTemp[12] = { 50.f, 118.f, 0.f };
	vTemp[13] = { 150.f, 48.f, 0.f };
	vTemp[14] = { -150.f, -60.f, 0.f };
	vTemp[15] = { -50.f, -130.f, 0.f };
	vTemp[16] = { 50.f, -130.f, 0.f };
	vTemp[17] = { 150.f, -60.f, 0.f };

	m_eCategory = UI_FRONT;

	Set_Receive_PlayerInven();

	return S_OK;
}

_int CInventory_ItemIcon::Update_GameObject(const _float& fTimeDelta)
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	_bool bInvenClose = pPlayer->Get_bItemUi_On();
	if (bInvenClose == false)
		return OBJ_DEAD;

	Change_State();
	Slot_Frame_Check();

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CInventory_ItemIcon::LateUpdate_GameObject()
{	
	if (m_pReceive_PlayerInven->Get_Down())
	{
		m_bDown = false;
	}

	__super::LateUpdate_GameObject();
}

void CInventory_ItemIcon::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	if ((m_iDownSpace == m_PrintNumber) && m_bDrag)
	{
		Ui_Print_Drag();
	}
	else
	{
		Ui_Print(m_PrintNumber);
	}

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CInventory_ItemIcon::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Inventory_Icon"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Inventory_Icon", pComponent });

	return S_OK;
}

void CInventory_ItemIcon::Change_State(void)
{	
	if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_LBUTTON))
	{
		if (m_bDrag == false && m_bDown == false)
		{
			GetCursorPos(&m_Down_pt);
			ScreenToClient(g_hWnd, &m_Down_pt);
			m_bDown = true;
			m_pReceive_PlayerInven->Set_Down(false);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_CLICK);
			CSoundMgr::Get_Instance()->PlaySound(L"UI_item select.wav", CSoundMgr::UI_CLICK, 0.8f);
		}
		if (m_bDrag == false && m_bDown)
		{
			for (int i = 0; i < 18; i++)
			{
				m_tRect.left = LONG(800.f + vTemp[i].x - (50.f));
				m_tRect.top = LONG(450.f - vTemp[i].y - (50.f));
				m_tRect.right = LONG(800.f + vTemp[i].x + (50.f));
				m_tRect.bottom = LONG(450.f - vTemp[i].y + (50.f));

				if (PtInRect(&m_tRect, m_Down_pt))
				{
					m_iDownSpace = i;

					if ((m_vPos_Origin.x == vTemp[i].x) && (m_vPos_Origin.y == vTemp[i].y))
					{
						m_bDrag = true;
					}
				}
			}
		}
	}
	if (m_bDown)
	{
		if (m_iDownSpace < 10)
		{
			if ((m_pReceive_PlayerInven->Space[(m_iDownSpace)].iItemNumber) == 0)
			{
				m_bDown = false;
				m_bDrag = false;
				m_pReceive_PlayerInven->Set_Down(true);
				m_iDownSpace = 0;
			}
		}
		else if ((10 <= m_iDownSpace) && (m_iDownSpace < 14))
		{
			if ((m_pReceive_PlayerInven->Weapon[(m_iDownSpace - 10)].iItemNumber) == 0)
			{
				m_bDown = false;
				m_bDrag = false;
				m_pReceive_PlayerInven->Set_Down(true);
				m_iDownSpace = 0;
			}
		}
		else if ((14 <= m_iDownSpace) && (m_iDownSpace < 18))
		{
			if ((m_pReceive_PlayerInven->Acc[(m_iDownSpace - 14)].iItemNumber) == 0)
			{
				m_bDown = false;
				m_bDrag = false;
				m_pReceive_PlayerInven->Set_Down(true);
				m_iDownSpace = 0;
			}
		}
	}
	
	if (m_bDrag)
	{
		POINT	pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		m_Drop_pt.x = pt.x - 800;
		m_Drop_pt.y = -(pt.y - 450);
	}

	if (m_bDrag && (CKeyMgr::Get_Instance()->Key_Up(KEY_LBUTTON)))
	{
		m_bDrag = false;
		m_Drop_pt.x = (LONG)0.f;
		m_Drop_pt.y = (LONG)0.f;

		POINT	pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		if (0 != pt.x)
		{
			m_Drop_pt.x = pt.x;
			m_Drop_pt.y = pt.y;
		}
		m_bDrop = true;

		if (m_bDrop)
		{
			for (int i = 0; i < 18; i++)
			{
				m_tRect.left = LONG(800.f + vTemp[i].x - (50.f));
				m_tRect.top = LONG(450.f - vTemp[i].y - (50.f));
				m_tRect.right = LONG(800.f + vTemp[i].x + (50.f));
				m_tRect.bottom = LONG(450.f - vTemp[i].y + (50.f));

				if (PtInRect(&m_tRect, m_Drop_pt))
				{
					m_iDropSpace = i;
				}
			}

			if (m_iDownSpace != m_iDropSpace)
			{
				Compare_In(m_iDropSpace, m_iDownSpace);
			}
			else if (m_iDownSpace == m_iDropSpace)
			{
				//m_bDown = false;
				//m_bDrag = false;
				m_bDrop = false;

				m_iDownSpace = 0;
				m_iDropSpace = 0;

			}
			m_Drop_pt = { 0, 0 };
			m_Down_pt = { 0, 0 };
		}

		m_bDrag = false;
		m_bDrop = false;
		m_bDown = false;

		m_pReceive_PlayerInven->Set_Down(true);
	}
}

void CInventory_ItemIcon::Frame_Check(const _float& fTimeDelta)
{	
	
}

void CInventory_ItemIcon::Compare_In(_int _iTemp, _int _iSlotNum)
{
	_int iTemp_Index = _iTemp;
	_int iSlot_Index = _iSlotNum;

	ITEM_INFO vTemp_Look;

	if (iTemp_Index < 10) // 1. 목표가 인벤일 경우
	{
		vTemp_Look = m_pReceive_PlayerInven->Space[(iTemp_Index)];

		if (iSlot_Index < 10)								// 1-1. 내 슬롯도 인벤인 경우
		{
			m_pReceive_PlayerInven->Set_Space(iTemp_Index, m_pReceive_PlayerInven->Space[(iSlot_Index)]);
			m_pReceive_PlayerInven->Set_Space(iSlot_Index, vTemp_Look);
		}
		else if ((10 <= iSlot_Index) && (iSlot_Index < 14)) // 1-2. 내 슬롯은 웨폰인 경우
		{
			if ((vTemp_Look.tType == ITEM_WEAPON) || (vTemp_Look.iItemNumber == 0))
			{
				m_pReceive_PlayerInven->Set_Space(iTemp_Index, m_pReceive_PlayerInven->Weapon[(iSlot_Index - 10)]);
				m_pReceive_PlayerInven->Set_Weapon(iSlot_Index - 10, vTemp_Look);
			}
		}
		else if ((14 <= iSlot_Index) && (iSlot_Index < 18)) // 1-3. 내 슬롯은 악세인 경우
		{
			if ((vTemp_Look.tType == ITEM_ACC) || (vTemp_Look.iItemNumber == 0))
			{
				m_pReceive_PlayerInven->Set_Space(iTemp_Index, m_pReceive_PlayerInven->Acc[(iSlot_Index - 14)]);
				m_pReceive_PlayerInven->Set_Acc(iSlot_Index - 14, vTemp_Look);
			}
		}
	}
	else if ((10 <= iTemp_Index) && (iTemp_Index < 14)) // 2. 목표의 값이 웨폰인 경우
	{
		vTemp_Look = m_pReceive_PlayerInven->Weapon[(iTemp_Index - 10)];

		if (iSlot_Index < 10)								// 2-1. 내 슬롯이 인벤인 경우
		{
			if ((m_pReceive_PlayerInven->Space[(iSlot_Index)].tType == ITEM_WEAPON)) // 인벤 슬롯이지만 무기일때
			{
				m_pReceive_PlayerInven->Set_Weapon((iTemp_Index - 10), m_pReceive_PlayerInven->Space[(iSlot_Index)]);
				m_pReceive_PlayerInven->Set_Space((iSlot_Index), vTemp_Look);
			}
		}
		else if ((10 <= iSlot_Index) && (iSlot_Index < 14)) // 2-2. 내 슬롯도 웨폰인 경우
		{
			m_pReceive_PlayerInven->Set_Weapon((iTemp_Index - 10), m_pReceive_PlayerInven->Weapon[(iSlot_Index - 10)]);
			m_pReceive_PlayerInven->Set_Weapon((iSlot_Index - 10), vTemp_Look);
		}
		else if ((14 <= iSlot_Index) && (iSlot_Index < 18)) // 2-3. 내 슬롯은 악세인 경우
		{
			// 값 변경이 없어야함
		}
	}
	else if ((14 <= iTemp_Index) && (iTemp_Index < 18)) // 3. 목표의 값이 악세인 경우
	{
		vTemp_Look = m_pReceive_PlayerInven->Acc[(iTemp_Index - 14)];

		if (iSlot_Index < 10)								// 3-1. 내 슬롯이 인벤인 경우
		{
			if ((m_pReceive_PlayerInven->Space[(iSlot_Index)].tType == ITEM_ACC)) // 인벤 슬롯이지만 악세일때
			{
				m_pReceive_PlayerInven->Set_Acc((iTemp_Index - 14), m_pReceive_PlayerInven->Space[(iSlot_Index)]);
				m_pReceive_PlayerInven->Set_Space((iSlot_Index), vTemp_Look);
			}
		}
		else if ((10 <= iSlot_Index) && (iSlot_Index < 14)) // 3-2. 내 슬롯이 웨폰인 경우
		{
			// 값 변경이 없어야함
		}
		else if ((14 <= iSlot_Index) && (iSlot_Index < 18)) // 3-3. 내 슬롯도 악세인 경우
		{
			m_pReceive_PlayerInven->Set_Acc((iTemp_Index - 14), m_pReceive_PlayerInven->Acc[(iSlot_Index - 14)]);
			m_pReceive_PlayerInven->Set_Acc((iSlot_Index - 14), vTemp_Look);
		}
	}

	m_bDown = false;
	m_bDrag = false;
	m_bDrop = false;

	m_iDropSpace = 0;
	m_iDownSpace = 0;

	m_pReceive_PlayerInven->Weapon;
	m_pReceive_PlayerInven->Space;
	m_pReceive_PlayerInven->Acc;

	if (m_PrintNumber)
	{
		int ia = m_PrintNumber;
	}
}


_bool CInventory_ItemIcon::Inventory_Close(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	if (!(pPlayer->Get_bItemUi_On()))
	{
		return true;
	}

	return true;
}

void CInventory_ItemIcon::Slot_Frame_Check(void)
{
	m_pReceive_PlayerInven->Weapon;
	m_pReceive_PlayerInven->Space;
	m_pReceive_PlayerInven->Acc;

	if (m_iSlotNumber < 10)
	{
		m_bPrint = true;
		_int iTemp_iTemnumber = m_pReceive_PlayerInven->Space[m_iSlotNumber].iItemNumber;

		if (0 != iTemp_iTemnumber)
			m_fFrame = (_float)iTemp_iTemnumber;

		if (0 == iTemp_iTemnumber)
			m_fFrame = 0;
	}

	if ((10 <= m_iSlotNumber) && (14 > m_iSlotNumber))
	{
		m_bPrint = true;

		_int iTemp_iTemnumber = m_pReceive_PlayerInven->Weapon[(m_iSlotNumber - 10)].iItemNumber;

		if (0 != iTemp_iTemnumber)
			m_fFrame = (_float)iTemp_iTemnumber;

		if (0 == iTemp_iTemnumber)
			m_fFrame = 1;
	}

	if ((14 <= m_iSlotNumber) && (m_iSlotNumber < 18))
	{
		m_bPrint = true;

		_int iTemp_iTemnumber = m_pReceive_PlayerInven->Acc[(m_iSlotNumber - 14)].iItemNumber;

		if (0 != iTemp_iTemnumber)
			m_fFrame = (_float)iTemp_iTemnumber;

		if (0 == iTemp_iTemnumber)
			m_fFrame = 1;
	}
}

void CInventory_ItemIcon::Ui_Print(_int _iNum)
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
	case 0:
		matWorld._41 = 323.f;
		matWorld._42 = 40.f;
		break;
	case 1:
		matWorld._41 = 415.f;
		matWorld._42 = 40.f;
		break;
	case 2:
		matWorld._41 = 507.f;
		matWorld._42 = 40.f;
		break;
	case 3:
		matWorld._41 = 599.f;
		matWorld._42 = 40.f;
		break;
	case 4:
		matWorld._41 = 691.f;
		matWorld._42 = 40.f;
		break;
	case 5:
		matWorld._41 = 323.f;
		matWorld._42 = -52.f;
		break;
	case 6:
		matWorld._41 = 415.f;
		matWorld._42 = -52.f;
		break;
	case 7:
		matWorld._41 = 507.f;
		matWorld._42 = -52.f;
		break;
	case 8:
		matWorld._41 = 599.f;
		matWorld._42 = -52.f;
		break;
	case 9:
		matWorld._41 = 691.f;
		matWorld._42 = -52.f;
		break;

	case 10:
		matWorld._41 = -150.f;
		matWorld._42 = 48.f;
		break;

	case 11:
		matWorld._41 = -50.f;
		matWorld._42 = 118.f;
		break;

	case 12:
		matWorld._41 = 50.f;
		matWorld._42 = 118.f;
		break;

	case 13:
		matWorld._41 = 150.f;
		matWorld._42 = 48.f;
		break;

	case 14:
		matWorld._41 = -150.f;
		matWorld._42 = -60.f;
		break;

	case 15:
		matWorld._41 = -50.f;
		matWorld._42 = -130.f;
		break;

	case 16:
		matWorld._41 = 50.f;
		matWorld._42 = -130.f;
		break;

	case 17:
		matWorld._41 = 150.f;
		matWorld._42 = -60.f;
		break;
	default:
		break;
	}

	m_vPos_Origin.x = matWorld._41;
	m_vPos_Origin.y = matWorld._42;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CInventory_ItemIcon::Ui_Print_Drag(void)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 46.f;
	matWorld._22 = 46.f;
	matWorld._41 = (_float)m_Drop_pt.x;
	matWorld._42 = (_float)m_Drop_pt.y;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CInventory_ItemIcon::Set_Receive_PlayerInven(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_pReceive_PlayerInven = pPlayer->Get_InvenInfo();
}

void CInventory_ItemIcon::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CInventory_ItemIcon* CInventory_ItemIcon::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CInventory_ItemIcon* pInstance = new CInventory_ItemIcon(pGraphicDev);

	pInstance->m_iSlotNumber = _iNumber;
	pInstance->m_PrintNumber = _iNumber;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInventory_ItemIcon::Free(void)
{
	__super::Free();
}
