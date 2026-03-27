#include "..\..\Header\Component.h"

CComponent::CComponent()
	: m_bClone(false)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_bClone(false)
	, m_pGraphicDev(pGraphicDev)
{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

CComponent::CComponent(const CComponent & rhs)
	: m_bClone(true),
	m_pGraphicDev(rhs.m_pGraphicDev)
{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

CComponent::~CComponent()
{
}

void CComponent::Free(void)
{
	Safe_Release(m_pGraphicDev);
}
