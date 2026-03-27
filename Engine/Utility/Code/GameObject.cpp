#include "..\..\Header\GameObject.h"
#include "Transform.h"


CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	if (m_pGraphicDev)
		m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)	
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(pComponentTag, eID);
	if (nullptr == pComponent) return nullptr;
	
	return pComponent;
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	for (auto& iter : m_uMapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return _int();
}

void CGameObject::LateUpdate_GameObject()
{
	for (auto& iter : m_uMapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component ();
}

void CGameObject::Compute_ViewZ(const _vec3* pPos)
{
	_matrix		matCamWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, 0, &matCamWorld);

	_vec3		vCamPos;
    memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	vCamPos.x = pPos->x;

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));

}

Engine::CComponent* Engine::CGameObject::Find_Component(const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_uMapComponent[eID].begin(), m_uMapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_uMapComponent[eID].end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free(void)
{
	Safe_Release(m_pGraphicDev);

	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_uMapComponent[i].begin(), m_uMapComponent[i].end(), CDeleteMap());
		m_uMapComponent[i].clear();
	}

}
