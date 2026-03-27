#include "stdafx.h"
#include "..\Header\MiniMap_Back.h"
#include "Export_Function.h"

CMiniMap_Back::CMiniMap_Back(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CMiniMap_Back::CMiniMap_Back(const CMiniMap_Back& rhs)
	: CFrontUi(rhs)
{
}

CMiniMap_Back::~CMiniMap_Back()
{
}

HRESULT CMiniMap_Back::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CMiniMap_Back::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CMiniMap_Back::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMiniMap_Back::Render_GameObject()
{
	if (!g_UI_Render)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_MiniMap_Print(m_tINFO.iNum, g_eCurScene, 0); //서순 주의

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMiniMap_Back::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MiniMap_Back"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MiniMap_Back", pComponent });

	return S_OK;
}

void CMiniMap_Back::Change_State(void)
{

}

void CMiniMap_Back::Frame_Check(const _float& fTimeDelta)
{

}

void CMiniMap_Back::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CMiniMap_Back* CMiniMap_Back::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CMiniMap_Back* pInstance = new CMiniMap_Back(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CMiniMap_Back::Free(void)
{
	__super::Free();
}
