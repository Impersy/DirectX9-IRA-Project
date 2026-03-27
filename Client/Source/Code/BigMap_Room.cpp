#include "stdafx.h"
#include "..\Header\BigMap_Room.h"
#include "Export_Function.h"

#include "Player.h"

CBigMap_Room::CBigMap_Room(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
}

CBigMap_Room::CBigMap_Room(const CBigMap_Room& rhs)
	: CFrontUi(rhs)
{
}

CBigMap_Room::~CBigMap_Room()
{
	Free();
}

HRESULT CBigMap_Room::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;

	return S_OK;
}

_int CBigMap_Room::Update_GameObject(const _float& fTimeDelta)
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

void CBigMap_Room::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBigMap_Room::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	Ui_Print(m_iRoom_Number);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBigMap_Room::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Ui_BigMap_Room"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Ui_BigMap_Room", pComponent });

	return S_OK;
}

void CBigMap_Room::Change_State(void)
{

}

void CBigMap_Room::Frame_Check(const _float& fTimeDelta)
{
	if ((g_eCurScene == SCENE_TUTORIAL) && (m_iRoom_Number == 0))
	{
		m_fFrame += (m_fFrame_Max * fTimeDelta);
		if (m_fFrame > m_fFrame_Max)
		{
			m_fFrame = 0.f;
		}
	}
	if ((g_eCurScene == SCENE_MAINLOBBY) && (m_iRoom_Number == 1))
	{
		m_fFrame += (m_fFrame_Max * fTimeDelta);
		if (m_fFrame > m_fFrame_Max)
		{
			m_fFrame = 0.f;
		}
	}
	if ((g_eCurScene == SCENE_SHOP) && (m_iRoom_Number == 2))
	{
		m_fFrame += (m_fFrame_Max * fTimeDelta);
		if (m_fFrame > m_fFrame_Max)
		{
			m_fFrame = 0.f;
		}
	}
	if ((g_eCurScene == SCENE_DUNGEONLOBBY) && (m_iRoom_Number == 3))
	{
		m_fFrame += (m_fFrame_Max * fTimeDelta);
		if (m_fFrame > m_fFrame_Max)
		{
			m_fFrame = 0.f;
		}
	}
	if ((g_eCurScene == SCENE_NORMAL) && (m_iRoom_Number == 4))
	{
		m_fFrame += (m_fFrame_Max * fTimeDelta);
		if (m_fFrame > m_fFrame_Max)
		{
			m_fFrame = 0.f;
		}
	}
	if ((g_eCurScene == SCENE_POT) && (m_iRoom_Number == 5))
	{
		m_fFrame += (m_fFrame_Max * fTimeDelta);
		if (m_fFrame > m_fFrame_Max)
		{
			m_fFrame = 0.f;
		}
	}
	if ((g_eCurScene == SCENE_REGEN) && (m_iRoom_Number == 6))
	{
		m_fFrame += (m_fFrame_Max * fTimeDelta);
		if (m_fFrame > m_fFrame_Max)
		{
			m_fFrame = 0.f;
		}
	}
	if ((g_eCurScene == SCENE_BOSS1) && (m_iRoom_Number == 7))
	{
		m_fFrame += (m_fFrame_Max * fTimeDelta);
		if (m_fFrame > m_fFrame_Max)
		{
			m_fFrame = 0.f;
		}
	}
	if ((g_eCurScene == SCENE_BOSS2) && (m_iRoom_Number == 8))
	{
		m_fFrame += (m_fFrame_Max * fTimeDelta);
		if (m_fFrame > m_fFrame_Max)
		{
			m_fFrame = 0.f;
		}
	}
	
}

void CBigMap_Room::Ui_Print(_int _iNum)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 45.f;
	matWorld._22 = 45.f;

	switch (_iNum)
	{
	case 0:	//튜토리얼방
		matWorld._41 = -300.f;
		matWorld._42 = 190.f;	
		break;
	case 1:	// 사도공간
		matWorld._41 = -300.f;
		matWorld._42 = 90.f;
		break;
	case 2:	// 상점
		matWorld._41 = -100.f;
		matWorld._42 = -10.f;
		break;
	case 3:	// 던전로비
		matWorld._41 = 0.f;
		matWorld._42 = -10.f;
		break;
	case 4:	// 일반맵
		matWorld._41 = 100.f;
		matWorld._42 = -10.f;
		break;
	case 5:	// 보너스 맵(항아리)
		matWorld._41 = 100.f;
		matWorld._42 = -110.f;
		break;
	case 6:	// 리젠 맵
		matWorld._41 = 200.f;
		matWorld._42 = -110.f;
		break;
	case 7:	// 보스 맵 1(도올)
		matWorld._41 = 0.f;
		matWorld._42 = 90.f;
		break;
	case 8:	// 보스 맵 2(도올)
		matWorld._41 = 0.f;
		matWorld._42 = 190.f;
		break;

	default:
		break;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

_bool CBigMap_Room::BigMap_Close(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	if (!(pPlayer->Get_bBigMapUi_On()))
	{
		return true;
	}

	return true;
}

void CBigMap_Room::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBigMap_Room* CBigMap_Room::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _RoomNumber)
{
	CBigMap_Room* pInstance = new CBigMap_Room(pGraphicDev);

	pInstance->m_iRoom_Number = _RoomNumber;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBigMap_Room::Free(void)
{
	__super::Free();
}
