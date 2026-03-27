#include "..\..\Header\Layer.h"
#include "Collider.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_uMapObject.begin(), m_uMapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_uMapObject.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

CGameObject * CLayer::Get_GameObject(const _tchar * pObjTag)
{
	auto	iter = find_if(m_uMapObject.begin(), m_uMapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_uMapObject.end())
		return nullptr;

	return iter->second;
}


HRESULT CLayer::Add_GameObject(const _tchar * pObjTag, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_uMapObject.insert({pObjTag, pGameObject});

	return S_OK;
}

void CLayer::Add_BulletObject(CGameObject* pGameObject)
{
	if (pGameObject == nullptr)
		return;

	m_ListBulletObject.push_back(pGameObject);
}

HRESULT CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	int iResult = 0;

	// 실제 TimeDelta 저장
	// Time Stop 예외 대상(플레이어/화살/스킬 이펙트 등)이 참조 가능하도록 유지
	m_fTimeDelta = fTimeDelta;

	for (auto iter = m_uMapObject.begin(); iter != m_uMapObject.end();)
	{
		if (m_fTimeStop == false) {
			// 일반 업데이트
			iResult = iter->second->Update_GameObject(fTimeDelta);
		}
		else {
			// Time Stop 시 TimeDelta를 0으로 전달해 업데이트 정지
			iResult = iter->second->Update_GameObject(0.f);
		}
		++iter;
	}


	for (auto iter = m_ListBulletObject.begin(); iter != m_ListBulletObject.end();)
	{
		if (m_fTimeStop == false) {
			iResult = (*iter)->Update_GameObject(fTimeDelta);
		}
		else {
			iResult = (*iter)->Update_GameObject(0.f);
		}

		if (OBJ_DEAD == iResult)
		{
			Safe_Delete <CGameObject*>(*iter);
			iter = m_ListBulletObject.erase(iter);
		}
		else
			++iter;
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(void)
{
	for (auto iter = m_uMapObject.begin(); iter != m_uMapObject.end(); ++iter)
		iter->second->LateUpdate_GameObject();

	for (auto iter = m_ListBulletObject.begin(); iter != m_ListBulletObject.end(); ++iter)
		(*iter)->LateUpdate_GameObject();
}

CLayer* CLayer::Create(void)
{
	CLayer *	pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLayer::Free(void)
{
	for_each(m_uMapObject.begin(), m_uMapObject.end(), CDeleteMap());
	m_uMapObject.clear();
	
	for_each(m_ListBulletObject.begin(), m_ListBulletObject.end(), CDeleteObj());
	m_ListBulletObject.clear();
}
