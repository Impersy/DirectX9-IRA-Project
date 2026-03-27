#include "stdafx.h"
#include "..\Header\Lever.h"
#include "Export_Function.h"

CLever::CLever(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CLever::CLever(const CLever& rhs)
	: CDynamicObject(rhs)
{
}

CLever::~CLever()
{
}

HRESULT CLever::Ready_GameObject(void)
{
	__super::Ready_GameObject();

	return S_OK;
}

_int CLever::Update_GameObject(const _float& fTimeDelta)
{	
	//SetUp_OnTerrain();

	__super::Update_GameObject(fTimeDelta);

	

	return 0;
}

void CLever::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CLever::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//if (-1 != m_byDrawID)
	//	m_pTextureCom->Set_Texture(m_byDrawID);

	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CLever* CLever::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLever* pInstance = new CLever(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLever::Free(void)
{
	__super::Free();
}

HRESULT CLever::Add_Component(void)
{
	return E_NOTIMPL;
}

void CLever::SetUp_OnTerrain(void)
{
}
