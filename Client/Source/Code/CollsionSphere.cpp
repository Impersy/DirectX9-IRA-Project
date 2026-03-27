#include "stdafx.h"
#include "..\Header\CollisionSphere.h"
#include "Export_Function.h"

CCollisionSphere::CCollisionSphere(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pSphereBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pOwner(nullptr)

{
}

CCollisionSphere::CCollisionSphere(const CCollisionSphere & rhs)
	: Engine::CGameObject(rhs)
	, m_pSphereBufferCom(rhs.m_pSphereBufferCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pOwner(rhs.m_pOwner)
{
}

CCollisionSphere::~CCollisionSphere()
{
}

HRESULT CCollisionSphere::Ready_GameObject(CGameObject* pOwner , COLLIDER_TYPE _e)
{
	Add_Component();

	m_pOwner = pOwner;

	m_eCollideType = _e;

	_float fRadius = 0.f;
	
	if(m_eCollideType == COLLIDER_DYNAMIC)
		fRadius = dynamic_cast<CCollider*>(m_pOwner->Get_Component(L"Proto_Collider", ID_DYNAMIC))->Get_Radius();
	else if (m_eCollideType == COLLIDER_STATIC)
	{
		fRadius = dynamic_cast<CCollider*>(m_pOwner->Get_Component(L"Proto_Collider", ID_STATIC))->Get_Radius();
		m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CCollider*>(m_pOwner->Get_Component(L"Proto_Collider", ID_STATIC))->Get_SpherePos();
	}

	m_pTransformCom->m_vScale = { fRadius , fRadius, fRadius };

	return S_OK;
}

_int CCollisionSphere::Update_GameObject(const _float& fTimeDelta)
{
	if (m_eCollideType == COLLIDER_DYNAMIC)
		m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CCollider*>(m_pOwner->Get_Component(L"Proto_Collider", ID_DYNAMIC))->Get_SpherePos();
	
		

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CCollisionSphere::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CCollisionSphere::Render_GameObject()
{
	if (!g_bSphereRender)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pSphereBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CCollisionSphere::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pSphereBufferCom = dynamic_cast<CSphereTex*>(Engine::Clone_ProtoComponent(L"Proto_SphereTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_SphereTex", pComponent });

	return S_OK;
}

CCollisionSphere* CCollisionSphere::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner , COLLIDER_TYPE _e)
{
	CCollisionSphere* pInstance = new CCollisionSphere(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner , _e)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}


void CCollisionSphere::Free(void)
{
	__super::Free();
}


