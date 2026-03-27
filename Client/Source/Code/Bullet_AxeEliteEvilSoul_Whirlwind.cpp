#include "stdafx.h"
#include "..\Header\Bullet_AxeEliteEvilSoul_Whirlwind.h"
#include "Export_Function.h"
#include "Monster.h"

CBullet_AxeEvilSoul_Whirlwind::CBullet_AxeEvilSoul_Whirlwind(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CBullet_AxeEvilSoul_Whirlwind::CBullet_AxeEvilSoul_Whirlwind(const CBullet_AxeEvilSoul_Whirlwind& rhs)
	: CBullet(rhs)
{
}

CBullet_AxeEvilSoul_Whirlwind::~CBullet_AxeEvilSoul_Whirlwind()
{
}


HRESULT CBullet_AxeEvilSoul_Whirlwind::Ready_GameObject(CGameObject* pOwner)
{
	Add_Component();

	m_pOwner = pOwner;

	//m_fSpeed = rand() % 10 * 3.f + 10.f;

	//m_fMaxframe = 7.f;

	m_pTransformCom->m_vScale = { 8.f , 8.f , 8.f };

	//CTransform* pTransformCom = dynamic_cast<CTransform*>( Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	// 

	//CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	// 

	//if (pTransformCom->m_vInfo[INFO_POS].x >= pPlayerTransformCom->m_vInfo[INFO_POS].x)
	//	m_pTransformCom->m_vInfo[INFO_POS] = { pTransformCom->m_vInfo[INFO_POS].x - (rand() % 30) * 0.02f ,3.f,  pTransformCom->m_vInfo[INFO_POS].z - (rand() % 30) * 0.02f };
	//else
	//	m_pTransformCom->m_vInfo[INFO_POS] = { pTransformCom->m_vInfo[INFO_POS].x + (rand() % 30) * 0.02f ,3.f,  pTransformCom->m_vInfo[INFO_POS].z - (rand() % 30) * 0.02f };

	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y = 5.f;

	//m_vDir = m_pTransformCom->m_vInfo[INFO_POS] - pTransformCom->m_vInfo[INFO_POS];
	//D3DXVec3Normalize(&m_vDir, &m_vDir);

	return S_OK;
}

_int CBullet_AxeEvilSoul_Whirlwind::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pOwner->Get_Dead())
		return OBJ_DEAD;

	if (m_bDead || m_bHit)
	{
		//Create_DeathEffect();
		return OBJ_DEAD;
	}

	Frame_Check(fTimeDelta);

	//Check_Boss_Dead();

	//m_fAccTime += fTimeDelta;

	//m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

	CGameObject::Update_GameObject(fTimeDelta);

	_vec3 vPos = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	_vec3 vDir = vPos - m_pTransformCom->m_vInfo[INFO_POS];
	if (3.f < D3DXVec3Length(&vDir))
		m_pTransformCom->Move_Pos(&vPos, 110.f, fTimeDelta);

	//m_fAccumulatedTime += fTimeDelta;

	//if (1.2f < m_fAccumulatedTime)
	//{
	//	m_fAccumulatedTime = 0.f;
	//	m_bDead = true;
	//}

	Check_Monster_State();

	//Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CBullet_AxeEvilSoul_Whirlwind::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBullet_AxeEvilSoul_Whirlwind::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//m_pTextureCom->Set_Texture((_uint)m_fFrame);

	//m_pBufferCom->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBullet_AxeEvilSoul_Whirlwind::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	//pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	// 
	//m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(8.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

//void CBullet_AxeEvilSoul_Whirlwind::Frame_Check(const _float& fTimeDelta)
//{
//	m_fFrame += m_fMaxframe * fTimeDelta;
//
//	if (m_fMaxframe < m_fFrame)
//		m_fFrame = 0.f;
//
//	if (m_fAccTime > 5.f)
//		m_bDead = true;
//}

void CBullet_AxeEvilSoul_Whirlwind::Check_Monster_State()
{
	if (dynamic_cast<CMonster*>(m_pOwner)->Get_State() != MONSTER_MOVE)
		m_bDead = true;
}

CBullet_AxeEvilSoul_Whirlwind* CBullet_AxeEvilSoul_Whirlwind::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner)
{
	CBullet_AxeEvilSoul_Whirlwind* pInstance = new CBullet_AxeEvilSoul_Whirlwind(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

//void CBullet_AxeEvilSoul_Whirlwind::Create_DeathEffect()
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


void CBullet_AxeEvilSoul_Whirlwind::Free(void)
{
	__super::Free();
}
