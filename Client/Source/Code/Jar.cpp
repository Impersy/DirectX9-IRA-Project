#include "stdafx.h"
#include "..\Header\Jar.h"
#include "Export_Function.h"
#include "CollisionMgr.h"
#include "Effect_Stone_Hit.h"
#include "Effect_Tree_Hit.h"
#include <SoundMgr.h>
#include "Item_Coin.h"

CJar::CJar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
	, m_pColliderCom(nullptr)
{
}

CJar::CJar(const CJar& rhs)
	: CDynamicObject(rhs)
	, m_pColliderCom(rhs.m_pColliderCom)
	, m_bHit(rhs.m_bHit)
{
}

CJar::~CJar()
{
	Free();
}

HRESULT CJar::Ready_GameObject(void)
{
	Add_Component();

	return S_OK;
}

_int CJar::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_bSetCollider)
	{
		m_pColliderCom->Set_Radius(m_fColliderRadius);
		m_pColliderCom->Set_SpherePos(m_vColliderPos);

		m_bSetCollider = false;
	}

	if (m_bHit)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::POT_BREAK);
		CSoundMgr::Get_Instance()->PlaySound(L"Glass item Breaks 5.wav", CSoundMgr::POT_BREAK, 1.f);

		Create_Hit_Effect();
		m_bHit = false;

		Create_Coin();

		return OBJ_DEAD;
	}


	__super::Update_GameObject(fTimeDelta);

	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_LANDSCAPE, this);

	return 0;
}

void CJar::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CJar::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (-1 != m_byDrawID)
		m_pTextureCom[JAR_OBJ]->Set_Texture(m_byDrawID);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CJar* CJar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CJar* pInstance = new CJar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CJar::Free(void)
{
	__super::Free();
}

void CJar::Create_Hit_Effect(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	_vec3 vScale = m_pTransformCom->m_vScale;
	_vec3 vTemp = vPos;

	vTemp.y *= 1.6f;

	CGameObject* pGameObject;

	pGameObject = CEffect_Tree_Hit::Create(m_pGraphicDev, vTemp, vScale * 1.2);
	if (nullptr == pGameObject) return;
	pGameLogicLayer->Add_BulletObject(pGameObject);

	vTemp.z -= 0.1f;

	pGameObject = CEffect_Stone_Hit::Create(m_pGraphicDev, vTemp, vScale);
	if (nullptr == pGameObject) return;
	pGameLogicLayer->Add_BulletObject(pGameObject);



}

void CJar::Create_Coin(void)
{
	TIME_STOP_VOID

		CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	int iNum = rand() % 10;

	for (int i = 0; i < iNum; ++i)
	{
		pGameObject = CItem_Coin::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], false);
		if (pGameObject == nullptr)
			return;
		pGameLogicLayer->Add_BulletObject(pGameObject);
	}
}

HRESULT CJar::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Collider", pComponent });

	pComponent = m_pTextureCom[JAR_OBJ] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_HistoricSites_DecoObject_"));
	if (nullptr == m_pTextureCom[JAR_OBJ]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Spr_HistoricSites_DecoObject_", pComponent });


	return S_OK;
}
