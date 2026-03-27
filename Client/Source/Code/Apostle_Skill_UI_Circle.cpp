#include "stdafx.h"
#include "..\Header\Apostle_Skill_UI_Circle.h"
#include "Export_Function.h"

CApostle_Skill_UI_Circle::CApostle_Skill_UI_Circle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CApostle_Skill_UI(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CApostle_Skill_UI_Circle::CApostle_Skill_UI_Circle(const CApostle_Skill_UI_Circle& rhs)
	: CApostle_Skill_UI(rhs)
{
}

CApostle_Skill_UI_Circle::~CApostle_Skill_UI_Circle()
{
	Free();
}

HRESULT CApostle_Skill_UI_Circle::Ready_GameObject(void)
{
	Add_Component();
	m_iAlpha = 50;
	
	return S_OK;
}

_int CApostle_Skill_UI_Circle::Update_GameObject(const _float& fTimeDelta)
{	
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	Frame_Check(Time);

	m_Angle++;

	m_Size += 1400.f * Time * dir;

	if (m_Size > m_InitSize + 800) {
		m_Size = m_InitSize + 800;
	}
	
	m_fAccTime += 4.f * Time * 0.2f;

	if (m_fAccTime > 4.f) {
		dir = -5.f;
	}

	if (m_Size <= m_InitSize - 400) {
		m_Size = m_InitSize - 400;
		return OBJ_DEAD;
	}

	__super::Update_GameObject(Time);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CApostle_Skill_UI_Circle::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CApostle_Skill_UI_Circle::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 150, 200, 255));

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

HRESULT CApostle_Skill_UI_Circle::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Tif_UI_Circle"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Tif_UI_Circle", pComponent });

	return S_OK;
}

void CApostle_Skill_UI_Circle::Change_State(void)
{

}

void CApostle_Skill_UI_Circle::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += 16.f * fTimeDelta;

	if (16.f < m_fFrame)
	{
		m_fFrame = 0.f;
		m_bCheck = false;
	}
}

void CApostle_Skill_UI_Circle::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CApostle_Skill_UI_Circle* CApostle_Skill_UI_Circle::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float size)
{
	CApostle_Skill_UI_Circle* pInstance = new CApostle_Skill_UI_Circle(pGraphicDev);

	pInstance->m_Size = size;
	pInstance->m_InitSize = size;


	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

void CApostle_Skill_UI_Circle::Free(void)
{
	__super::Free();
}
