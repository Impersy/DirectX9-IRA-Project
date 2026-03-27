#include "stdafx.h"
#include "..\Header\Door.h"
#include "Export_Function.h"
#include "CollisionMgr.h"
#include "CollisionMgr.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CDoor::CDoor(const CDoor& rhs)
	: CDynamicObject(rhs)
{
}

CDoor::~CDoor()
{
}

HRESULT CDoor::Ready_GameObject(void)
{
	//__super::Ready_GameObject();

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CDoor::Update_GameObject(const _float& fTimeDelta)
{	
	//SetUp_OnTerrain();

	__super::Update_GameObject(fTimeDelta);

	
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_LANDSCAPE, this);

	return 0;
}

void CDoor::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CDoor::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (-1 != m_byDrawID)
		m_pTextureCom[m_eID]->Set_Texture(m_byDrawID);
	//m_pTextureCom[m_eID]->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoor* pInstance = new CDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoor::Free(void)
{
	__super::Free();
}

HRESULT CDoor::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_ProtoComponent(L"Proto_Calculator"));
	if (nullptr == m_pCalculatorCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	if (nullptr == m_pColliderCom) return E_FAIL;
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Collider", pComponent });

	if (SCENE_REGEN == g_eCurScene)
	{
		pComponent = m_pTextureCom[DOOR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Regen_door_"));
		if (nullptr == m_pTextureCom[DOOR]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Regen_door_", pComponent });
	}
	else if (SCENE_POT == g_eCurScene)
	{
		pComponent = m_pTextureCom[DOOR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Jar_door_"));
		if (nullptr == m_pTextureCom[DOOR]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Jar_door_", pComponent });
	}
	else if (SCENE_DUNGEONLOBBY == g_eCurScene || SCENE_SHOP == g_eCurScene || SCENE_NORMAL == g_eCurScene)
	{
		pComponent = m_pTextureCom[DOOR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"door_"));
		m_uMapComponent[ID_STATIC].insert({ L"door_", pComponent });
	}
	
	return S_OK;
}

void CDoor::SetUp_OnTerrain(void)
{
}
