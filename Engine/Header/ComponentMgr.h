#pragma once

#include "Component.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponentMgr :public CBase
{
	DECLARE_SINGLETON(CComponentMgr)

private:
	explicit CComponentMgr();
	virtual ~CComponentMgr();

public:
	HRESULT			Ready_ProtoComponent(const _tchar* pProtoTag, CComponent* pComponent);
	CComponent*		Clone_ProtoComponent(const _tchar* pProtoTag);
	void			Delete_ProtoComponent();

private:
	CComponent*		Find_ProtoComponent(const _tchar* pProtoTag);

private:
	unordered_map<const _tchar*, CComponent*>		m_uMapProto;

private:
	virtual void Free(void);
};

END