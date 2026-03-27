#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pScene(nullptr)

{
}

CManagement::~CManagement()
{
	Free();
}

CComponent * CManagement::Get_Component(const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	if (nullptr == m_pScene) return nullptr;

	return m_pScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

CGameObject * CManagement::Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	if (nullptr == m_pScene) return nullptr;

	return m_pScene->Get_GameObject(pLayerTag, pObjTag);
}

CLayer* CManagement::Get_Layer(const _tchar* pLayerTag)
{
	if (nullptr == m_pScene) return nullptr;

	return m_pScene->Get_Layer(pLayerTag);
}


HRESULT CManagement::Set_Scene(CScene * pScene)
{
	Safe_Release(m_pScene);
	Engine::Clear_RenderGroup();

	m_pScene = pScene;

	return S_OK;
}

_int CManagement::Update_Management(const _float & fTimeDelta)
{
	if (nullptr == m_pScene) return -1;

	return m_pScene->Update_Scene(fTimeDelta);
}

void CManagement::LateUpdate_Management()
{
	if (nullptr == m_pScene) return;
	m_pScene->LateUpdate_Scene();
}

void CManagement::Render_Management(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Render_GameObject(pGraphicDev);

	if (nullptr == m_pScene) return;
	m_pScene->Render_Scene();
}

void CManagement::Delete_Scene()
{
	Safe_Release(m_pScene);
}

void CManagement::Free(void)
{
	Safe_Release(m_pScene);
}
