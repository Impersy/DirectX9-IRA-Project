#include "stdafx.h"
#include "..\Header\MiniMap_Room.h"
#include "Export_Function.h"

CMiniMap_Room::CMiniMap_Room(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev), m_iRoomNumber(0), m_bInRoomCheck(false)
{
	//m_tINFO[0]->iMax = 5;
}

CMiniMap_Room::CMiniMap_Room(const CMiniMap_Room& rhs)
	: CFrontUi(rhs), m_iRoomNumber(0), m_bInRoomCheck(false)
{
}

CMiniMap_Room::~CMiniMap_Room()
{
}

HRESULT CMiniMap_Room::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;

	return S_OK;
}

_int CMiniMap_Room::Update_GameObject(const _float& fTimeDelta)
{	
	if (5 == m_iRoomNumber)
	{
		m_bInRoomCheck = true;
	}

	Frame_Check(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CMiniMap_Room::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMiniMap_Room::Render_GameObject()
{
	if (!g_UI_Render)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_MiniMap_Print(m_tINFO.iNum, g_eCurScene, m_iRoomNumber, nullptr); //서순 주의

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMiniMap_Room::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MiniMap_Room"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MiniMap_Room", pComponent });

	return S_OK;
}

void CMiniMap_Room::Change_State(void)
{

}

void CMiniMap_Room::Frame_Check(const _float& fTimeDelta)
{
	if (m_bInRoomCheck)
	{
		m_fFrame += 2.f * fTimeDelta;

		if (2.f < m_fFrame)
		{
			m_fFrame = 0.f;
			//m_bCheck = false;
		}
	}

}

void CMiniMap_Room::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CMiniMap_Room* CMiniMap_Room::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber, _int _iRoomNumber)
{
	CMiniMap_Room* pInstance = new CMiniMap_Room(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;
	pInstance->m_iRoomNumber = _iRoomNumber;

	return pInstance;
}

void CMiniMap_Room::Free(void)
{
	__super::Free();
}
