#include "stdafx.h"
#include "..\Header\Fire_Filter.h"
#include "Export_Function.h"


CFire_Filter::CFire_Filter(LPDIRECT3DDEVICE9 pGraphicDev)
	: CApostle_Skill_UI(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CFire_Filter::CFire_Filter(const CFire_Filter& rhs)
	: CApostle_Skill_UI(rhs)
{
}

CFire_Filter::~CFire_Filter()
{
	Free();
}

HRESULT CFire_Filter::Ready_GameObject(void)
{
	Add_Component();

	m_iAlpha = 145;
	
	return S_OK;
}

_int CFire_Filter::Update_GameObject(const _float& fTimeDelta)
{	
	
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	CGameObject* pGameObject = nullptr;

	m_Size += 800.f * Time * dir;

	if (m_Size > m_InitSize + 1500) {
		m_Size = m_InitSize + 1500;

		m_iAlpha = 55;
	}
	
	m_fAccTime += 2.f * Time * 0.7f;


	if (m_fAccTime > 6.f) {
		return OBJ_DEAD;
	}
	
	__super::Update_GameObject(Time);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CFire_Filter::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CFire_Filter::Render_GameObject()
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

	m_pTransformCom->Apostle_Skill_Time_Circle(m_Size , m_Angle);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFire_Filter::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Courage_Filter"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Courage_Filter", pComponent });

	return S_OK;
}

void CFire_Filter::Change_State(void)
{

}

void CFire_Filter::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += 16.f * fTimeDelta;

	if (16.f < m_fFrame)
	{
		m_fFrame = 0.f;
		m_bCheck = false;
	}
}

void CFire_Filter::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CFire_Filter* CFire_Filter::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float size)
{
	CFire_Filter* pInstance = new CFire_Filter(pGraphicDev);

	pInstance->m_Size = size;
	pInstance->m_InitSize = size;


	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

void CFire_Filter::Free(void)
{
	__super::Free();
}
