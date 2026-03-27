#include "stdafx.h"
#include "..\Header\Arrow.h"
#include "Export_Function.h"

CArrow::CArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTransformCom(nullptr), m_pColliderCom(nullptr)
	, m_pCalculatorCom(nullptr), m_fSpeed(90.f)
{
}

CArrow::CArrow(const CArrow & rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTransformCom(rhs.m_pTransformCom), m_pColliderCom(rhs.m_pColliderCom)
	, m_pCalculatorCom(rhs.m_pCalculatorCom), m_fSpeed(90.f)
{
}

CArrow::~CArrow()
{
	Free();
}

HRESULT CArrow::Ready_GameObject(void)
{
	return S_OK;
}

_int CArrow::Update_GameObject(const _float& fTimeDelta)
{	
	

	__super::Update_GameObject(fTimeDelta);


	return 0;
}

void CArrow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CArrow::Render_GameObject()
{
}

void CArrow::Free(void)
{
	__super::Free();
}
