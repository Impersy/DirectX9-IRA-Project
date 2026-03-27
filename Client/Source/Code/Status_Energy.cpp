#include "stdafx.h"
#include "..\Header\Status_Energy.h"
#include "Export_Function.h"

#include "Player.h"

CStatus_Energy::CStatus_Energy(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CStatus_Energy::CStatus_Energy(const CStatus_Energy& rhs)
	: CFrontUi(rhs)
{
}

CStatus_Energy::~CStatus_Energy()
{
}

HRESULT CStatus_Energy::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CStatus_Energy::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Set_Receive_PlayerInfo();

	if (pReceiveInfo.MaxEnergy)
	{
		if (pReceiveInfo.MaxEnergy < m_tINFO.iNum)
		{
			m_bDead = true;
		}
		else
		{
			m_bDead = false;
		}
	}

	if (pReceiveInfo.NowEnergy)
	{
		if (pReceiveInfo.NowEnergy < m_tINFO.iNum)
		{
			m_bFillCheck = true;
		}
		else
		{
			m_bFillCheck = false;
		}
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CStatus_Energy::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CStatus_Energy::Render_GameObject()
{
	if (!g_UI_Render)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	Change_State();

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_Status_Print(m_tINFO.iNum, 2); //서순 주의

	if ((pReceiveInfo.NowEnergy + 1) != m_tINFO.iNum)
	{
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);

	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CStatus_Energy::Set_Receive_PlayerInfo(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	pReceiveInfo = pPlayer->Get_Player_Info();
}

HRESULT CStatus_Energy::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Energy"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Energy", pComponent });

	return S_OK;
}

void CStatus_Energy::Change_State(void)
{
	if (!m_bFillCheck)
	{
		m_pTextureCom->Set_Texture(0);
	}
	else
	{
		m_pTextureCom->Set_Texture(1);
	}
}

void CStatus_Energy::Frame_Check(const _float& fTimeDelta)
{

}

void CStatus_Energy::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CStatus_Energy* CStatus_Energy::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CStatus_Energy* pInstance = new CStatus_Energy(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CStatus_Energy::Free(void)
{
	__super::Free();
}
