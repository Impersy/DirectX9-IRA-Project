#include "stdafx.h"
#include "..\Header\Ending.h"
#include "Export_Function.h"

CEnding::CEnding(LPDIRECT3DDEVICE9 pGraphicDev)
	: CApostle_Skill_UI(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CEnding::CEnding(const CEnding& rhs)
	: CApostle_Skill_UI(rhs)
{
}

CEnding::~CEnding()
{
	Free();
}

HRESULT CEnding::Ready_GameObject(void)
{
	Add_Component();
	m_iAlpha = 255;
	m_fFrame = 0;
	
	g_UI_Render = false;



	return S_OK;
}

_int CEnding::Update_GameObject(const _float& fTimeDelta)
{	
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;


	__super::Update_GameObject(Time);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CEnding::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CEnding::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pTextureCom->Set_Texture(0);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Apostle_Skill_Time_illust(650 * 0.8f, 451 * 0.8f, { 0.f,0.f,0.f });

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CEnding::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Ending"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Ending", pComponent });

	return S_OK;
}

void CEnding::Change_State(void)
{

}

void CEnding::Frame_Check(const _float& fTimeDelta)
{
	
}

void CEnding::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CEnding* CEnding::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float size)
{
	CEnding* pInstance = new CEnding(pGraphicDev);

	pInstance->m_Size = size;
	pInstance->m_InitSize = size;


	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

void CEnding::Free(void)
{
	__super::Free();
}
