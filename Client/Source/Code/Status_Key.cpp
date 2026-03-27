#include "stdafx.h"
#include "..\Header\Status_Key.h"
#include "Export_Function.h"

CStatus_Key::CStatus_Key(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CStatus_Key::CStatus_Key(const CStatus_Key& rhs)
	: CFrontUi(rhs)
{
}

CStatus_Key::~CStatus_Key()
{
}

HRESULT CStatus_Key::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CStatus_Key::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CStatus_Key::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CStatus_Key::Render_GameObject()
{
	if (!g_UI_Render)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_Status_Print(m_tINFO.iNum, 3); //서순 주의

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CStatus_Key::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Key"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Key", pComponent });

	return S_OK;
}

void CStatus_Key::Change_State(void)
{

}

void CStatus_Key::Frame_Check(const _float& fTimeDelta)
{

}

void CStatus_Key::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CStatus_Key* CStatus_Key::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CStatus_Key* pInstance = new CStatus_Key(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CStatus_Key::Free(void)
{
	__super::Free();
}
