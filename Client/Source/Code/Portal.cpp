#include "stdafx.h"
#include "..\Header\Portal.h"
#include "Export_Function.h"

CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CPortal::CPortal(const CPortal& rhs)
	: CDynamicObject(rhs)
{
}

CPortal::~CPortal()
{
}

HRESULT CPortal::Ready_GameObject(void)
{
	__super::Ready_GameObject();

	return S_OK;
}

_int CPortal::Update_GameObject(const _float& fTimeDelta)
{	
	//SetUp_OnTerrain();

	__super::Update_GameObject(fTimeDelta);

	

	return 0;
}

void CPortal::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CPortal::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//if (-1 != m_byDrawID)
	//	m_pTextureCom->Set_Texture(m_byDrawID);

	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CPortal* CPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPortal* pInstance = new CPortal(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPortal::Free(void)
{
	__super::Free();
}

HRESULT CPortal::Add_Component(void)
{
	return E_NOTIMPL;
}

void CPortal::SetUp_OnTerrain(void)
{
}
