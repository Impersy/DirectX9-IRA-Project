#include "stdafx.h"
#include "..\Header\Npc.h"
#include "Export_Function.h"

CNpc::CNpc(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr), m_pColliderCom(nullptr), m_pCalculatorCom(nullptr), m_bCheck(false)
	, m_fFrame(0.f), m_Radius(0.f)
{
}

CNpc::CNpc(const CNpc & rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTextureCom(rhs.m_pTextureCom), m_pTransformCom(rhs.m_pTransformCom), m_pColliderCom(rhs.m_pColliderCom), m_pCalculatorCom(rhs.m_pCalculatorCom)
	, m_bCheck(rhs.m_bCheck), m_fFrame(rhs.m_fFrame), m_Radius(rhs.m_Radius)
{
}

CNpc::~CNpc()
{

}

HRESULT CNpc::Ready_GameObject(void)
{
	return S_OK;
}

_int CNpc::Update_GameObject(const _float& fTimeDelta)
{	
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CNpc::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CNpc::Render_GameObject()
{
}

void CNpc::Free(void)
{
	__super::Free();
}
