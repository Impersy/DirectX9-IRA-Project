#include "stdafx.h"
#include "..\Header\Bow.h"
#include "Export_Function.h"

CBow::CBow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTransformCom(nullptr)
	,m_fSpeed(10.f)
{
}

CBow::CBow(const CBow & rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTransformCom(rhs.m_pTransformCom)
	,m_fSpeed(10.f)
{
}

CBow::~CBow()
{
	Free();
}

HRESULT CBow::Ready_GameObject(void)
{
	return S_OK;
}

_int CBow::Update_GameObject(const _float& fTimeDelta)
{	
	

	__super::Update_GameObject(fTimeDelta);


	return 0;
}

void CBow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBow::Render_GameObject()
{
}

void CBow::Free(void)
{
	__super::Free();
}
