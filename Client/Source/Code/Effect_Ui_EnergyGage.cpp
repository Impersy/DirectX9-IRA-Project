#include "stdafx.h"
#include "..\Header\Effect_Ui_EnergyGage.h"
#include "Export_Function.h"

#include "Player.h"
#include "Status.h"

CEffect_Ui_EnergyGage::CEffect_Ui_EnergyGage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Ui_EnergyGage::CEffect_Ui_EnergyGage(const CEffect_Ui_EnergyGage& rhs)
	: CEffect(rhs)
{
}

CEffect_Ui_EnergyGage::~CEffect_Ui_EnergyGage()
{
	Free();
}

HRESULT CEffect_Ui_EnergyGage::Ready_GameObject(void)
{
	Add_Component();

	m_fMaxFrame = 21.f;

	return S_OK;
}

_int CEffect_Ui_EnergyGage::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{	
		return OBJ_DEAD;
	}

	Set_Receive_PlayerInfo();

	m_fFrame += (m_fMaxFrame * fTimeDelta * 0.5);

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CEffect_Ui_EnergyGage::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{	
		Create_Perk_And_Stack();
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}

	if (pReceiveInfo.NowEnergy == pReceiveInfo.MaxEnergy)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}
}

void CEffect_Ui_EnergyGage::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	if (m_bDead == false)
	{
		Ui_Print(pReceiveInfo.NowEnergy);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);

	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEffect_Ui_EnergyGage::Set_Receive_PlayerInfo(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	pReceiveInfo = pPlayer->Get_Player_Info();
}

void CEffect_Ui_EnergyGage::Ui_Print(_int _iNowEng)
{	
	_iNowEng += 1;

	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	matWorld._11 = 0.6f;
	matWorld._22 = 0.6f;
	matWorld._41 = -(18.6f - ((_iNowEng * 1.2f) - 1.2f));
	matWorld._42 = 10.3f - (1 * 1.2);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CEffect_Ui_EnergyGage::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CEffect_Ui_EnergyGage::Create_Perk_And_Stack(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	pReceiveInfo = pPlayer->Get_Player_Info();
	
	if (pReceiveInfo.NowEnergy < pReceiveInfo.MaxEnergy)
	{
		pReceiveInfo.NowEnergy += 1;
		pPlayer->Set_Player_Info(pReceiveInfo);
	}

	CStatus* pStatus = dynamic_cast<CStatus*>(Engine::Get_GameObject(L"Layer_UI", L"FrontUI_Status"));
	pStatus->Set_Stack_Eng_Charge(false);
}

CEffect_Ui_EnergyGage* CEffect_Ui_EnergyGage::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CEffect_Ui_EnergyGage* pInstance = new CEffect_Ui_EnergyGage(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CEffect_Ui_EnergyGage::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Ui_EnergyGage"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Ui_EnergyGage", pComponent });

	return S_OK;
}

void CEffect_Ui_EnergyGage::Free(void)
{
	__super::Free();
}



