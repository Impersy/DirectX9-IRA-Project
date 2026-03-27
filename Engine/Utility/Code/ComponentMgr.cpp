#include "..\..\Header\ComponentMgr.h"

IMPLEMENT_SINGLETON(CComponentMgr)

CComponentMgr::CComponentMgr()
{
}


CComponentMgr::~CComponentMgr()
{
	Free();
}

HRESULT CComponentMgr::Ready_ProtoComponent(const _tchar * pProtoTag, CComponent * pComponent)
{
	CComponent*		pPrototype = Find_ProtoComponent(pProtoTag);
	
	if (nullptr != pPrototype || nullptr == pComponent)
		return E_FAIL;

	m_uMapProto.insert({ pProtoTag, pComponent });


	return S_OK;
}

CComponent * CComponentMgr::Clone_ProtoComponent(const _tchar * pProtoTag)
{
	CComponent*		pPrototype = Find_ProtoComponent(pProtoTag);
	if (nullptr == pPrototype) return nullptr;
	
	return pPrototype->Clone();
}

void CComponentMgr::Delete_ProtoComponent()
{
	for_each(m_uMapProto.begin(), m_uMapProto.end(), CDeleteMap());
	m_uMapProto.clear();
}

CComponent * CComponentMgr::Find_ProtoComponent(const _tchar * pProtoTag)
{
	auto	iter = find_if(m_uMapProto.begin(), m_uMapProto.end(), CTag_Finder(pProtoTag));

	if (iter == m_uMapProto.end())
		return nullptr;

	return iter->second;
}

void CComponentMgr::Free(void)
{
	for_each(m_uMapProto.begin(), m_uMapProto.end(), CDeleteMap());
	m_uMapProto.clear();
}

