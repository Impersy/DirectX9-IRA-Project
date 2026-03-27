#include "..\..\Header\Collider.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pTransformCom(nullptr)
{
}


CCollider::~CCollider()
{
}

HRESULT CCollider::Ready_CCollider()
{


	return S_OK;
}

_int CCollider::Update_Component(const _float& fTimeDelta)
{
	m_vSpherePos = m_pTransformCom->m_vInfo[INFO_POS] + m_vOffset;

	return 0;
}

void CCollider::LateUpdate_Component()
{
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollider *	pInstance = new CCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_CCollider()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	
	return pInstance;
}

CComponent * CCollider::Clone(void)
{
	return new CCollider(*this);
}

void CCollider::Free(void)
{
	__super::Free();
}
