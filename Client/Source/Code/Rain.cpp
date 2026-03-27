#include "stdafx.h"
#include "..\Header\Rain.h"
#include "Export_Function.h"

CRain::CRain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CApostle_Skill_UI(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CRain::CRain(const CRain& rhs)
	: CApostle_Skill_UI(rhs)
{
}

CRain::~CRain()
{
	Free();
}

HRESULT CRain::Ready_GameObject(void)
{
	Add_Component();


	
	return S_OK;
}

_int CRain::Update_GameObject(const _float& fTimeDelta)
{	
	
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;
	
	if (pGameLogicLayer->m_fTimeStop == true)
		Time *= 0.05f;


	Frame_Check(Time);

	
	
	__super::Update_GameObject(Time);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CRain::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CRain::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((int)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Apostle_Skill_Time_illust(m_Size_X, m_Size_Y, m_vPos);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CRain::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Rain"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Rain", pComponent });

	return S_OK;
}

void CRain::Change_State(void)
{

}

void CRain::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += 39.f * fTimeDelta;

	if (39.f < m_fFrame)
	{
		m_fFrame = 0.f;
	}
}

void CRain::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CRain* CRain::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float size_x, _float size_y, _vec3 pos)
{
	CRain* pInstance = new CRain(pGraphicDev);

	pInstance->m_Size_X = size_x;
	pInstance->m_Size_Y = size_y;
	pInstance->m_vPos = pos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

void CRain::Free(void)
{
	__super::Free();
}
