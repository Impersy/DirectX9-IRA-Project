#include "stdafx.h"
#include "../Header/Bullet_GreenEvilBat_DoubleLeaf.h"
#include "Export_Function.h"
#include "Monster.h"
#include "GreenEvilBat.h"
#include "Effect_DoubleLeaf_Bullet_Dead.h"
#include "Effect_Leaf_Bullet_BackHit.h"

CBullet_GreenEvilBat_DoubleLeaf::CBullet_GreenEvilBat_DoubleLeaf(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CBullet_GreenEvilBat_DoubleLeaf::CBullet_GreenEvilBat_DoubleLeaf(const CBullet_GreenEvilBat_DoubleLeaf& rhs)
	: CBullet(rhs)
{
}

CBullet_GreenEvilBat_DoubleLeaf::~CBullet_GreenEvilBat_DoubleLeaf()
{
}


HRESULT CBullet_GreenEvilBat_DoubleLeaf::Ready_GameObject(CGameObject* pOwner, const _vec3& vDir)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pOwner = pOwner;

	m_fSpeed = 55.f;

	m_fMaxframe = 6.f;

	m_pTransformCom->m_vScale = { 3.f , 3.f, 0.f };

	m_vDir = vDir;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(45.f));

	_matrix matRot;
	D3DXMatrixIdentity(&matRot);

	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y = 5.f;

	return S_OK;
}

_int CBullet_GreenEvilBat_DoubleLeaf::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pOwner->Get_Dead())
		m_bDead = true;

	if (m_bDead || m_bHit)
	{
		Create_DeathEffect(fTimeDelta);
		Create_BackHitEffect(fTimeDelta);
		return OBJ_DEAD;
	}

	Frame_Check(fTimeDelta);

	//Check_Boss_Dead();

	m_fAccumulatedTime += fTimeDelta;

	if (dynamic_cast<CGreenEvilBat*>(m_pOwner)->Get_IsDoubleLeafBulletAttack())
		m_bAtttack = true;

	if(m_bAtttack)
		m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;	

	CGameObject::Update_GameObject(fTimeDelta);

	//Check_Monster_State();

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CBullet_GreenEvilBat_DoubleLeaf::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fAccumulatedTime > 8.f)
		m_bDead = true;
}

void CBullet_GreenEvilBat_DoubleLeaf::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBullet_GreenEvilBat_DoubleLeaf::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_DoubleLeaf"));
	if (nullptr == m_pTextureCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_DoubleLeaf", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	if (nullptr == m_pColliderCom) return E_FAIL;
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(2.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CBullet_GreenEvilBat_DoubleLeaf::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxframe * fTimeDelta;

	if (m_fMaxframe < m_fFrame)
		m_fFrame = 0.f;
}

CBullet_GreenEvilBat_DoubleLeaf* CBullet_GreenEvilBat_DoubleLeaf::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _vec3& vDir)
{
	CBullet_GreenEvilBat_DoubleLeaf* pInstance = new CBullet_GreenEvilBat_DoubleLeaf(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner, vDir)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBullet_GreenEvilBat_DoubleLeaf::Create_DeathEffect(_float fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_DoubleLeaf_Bullet_Dead::Create(m_pGraphicDev, this, fTimeDelta);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CBullet_GreenEvilBat_DoubleLeaf::Create_BackHitEffect(_float fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Leaf_Bullet_BackHit::Create(m_pGraphicDev, this, fTimeDelta);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

//void CBullet_GreenEvilBat_DoubleLeaf::Create_DeathEffect()
//{
//	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
//
//	CGameObject* pGameObject;
//
//	pGameObject = CEffect_StandardBullet_Death::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);
//
//	if (pGameObject == nullptr)
//		return;
//
//	pGameLogicLayer->Add_BulletObject(pGameObject);
//}


void CBullet_GreenEvilBat_DoubleLeaf::Free(void)
{
	__super::Free();
}
