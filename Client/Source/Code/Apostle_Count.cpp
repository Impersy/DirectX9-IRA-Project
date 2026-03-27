#include "stdafx.h"
#include "..\Header\Apostle_Count.h"
#include "Export_Function.h"

#include "Player.h"

CApostle_Count::CApostle_Count(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CApostle_Count::CApostle_Count(const CApostle_Count& rhs)
	: CFrontUi(rhs)
{
}

CApostle_Count::~CApostle_Count()
{
}

HRESULT CApostle_Count::Ready_GameObject(void)
{
	Add_Component();

	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CApostle_Count::Update_GameObject(const _float& fTimeDelta)
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	_int TempCount = pPlayer->Get_Apostle_Count();

	if (TempCount < m_tINFO.iNum)
	{
		m_bNonePrint = true;
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CApostle_Count::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CApostle_Count::Render_GameObject()
{

	if (!g_UI_Render)
		return;

	if (m_bNonePrint)
	{
		return;
	}
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_Apostle_Print(m_tINFO.iNum, 4); //서순 주의

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CApostle_Count::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Apostle_Count"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Apostle_Count", pComponent });

	return S_OK;
}

void CApostle_Count::Change_State(void)
{

}

void CApostle_Count::Frame_Check(const _float& fTimeDelta)
{

}

void CApostle_Count::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CApostle_Count* CApostle_Count::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CApostle_Count* pInstance = new CApostle_Count(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CApostle_Count::Free(void)
{
	__super::Free();
}
