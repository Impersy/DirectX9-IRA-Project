#include "stdafx.h"
#include "..\Header\Landscape.h"
#include "Export_Function.h"

CLandscape::CLandscape(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_byDrawID(-1)
	, m_byDrawOption(0)
	, m_pObjKey(nullptr)
	, m_vColliderPos({0.f, 0.f, 0.f})
	, m_fColliderRadius(0.f)
{
}

CLandscape::CLandscape(const CLandscape & rhs)
	: Engine::CGameObject(rhs)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_byDrawID(rhs.m_byDrawID)
	, m_byDrawOption(rhs.m_byDrawOption)
	, m_pObjKey(rhs.m_pObjKey)
	, m_vColliderPos(rhs.m_vColliderPos)
	, m_fColliderRadius(rhs.m_fColliderRadius)
{
}

CLandscape::~CLandscape()
{
}

HRESULT CLandscape::Ready_GameObject(void)
{
	//Add_Component();

	return S_OK;
}

_int CLandscape::Update_GameObject(const _float& fTimeDelta)
{	
	//SetUp_OnTerrain();

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return 0;
}

void CLandscape::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CLandscape::Render_GameObject()
{
}

void CLandscape::Set_ObjKey(_tchar* pKey, _ulong strLength)
{
	if (m_pObjKey)
		Safe_Delete_Array(m_pObjKey);

	m_pObjKey = new _tchar[strLength];
	
	lstrcpyW(m_pObjKey, pKey);
}

void CLandscape::Free(void)
{
	Safe_Delete_Array(m_pObjKey);

	__super::Free();
}
