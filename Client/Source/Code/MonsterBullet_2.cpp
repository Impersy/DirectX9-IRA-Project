#include "stdafx.h"
#include "..\Header\MonsterBullet_2.h"
#include "Export_Function.h"

#include "Effect_Monster_Bullet_1_Dead.h"
#include "Effect_Monster_Bullet_2_Dead.h"


CMonsterBullet_2::CMonsterBullet_2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev), m_iDirCount(0)
{
}

CMonsterBullet_2::CMonsterBullet_2(const CMonsterBullet_2& rhs)
	: CBullet(rhs), m_iDirCount(0)
{
}

CMonsterBullet_2::~CMonsterBullet_2()
{
	Free();
}


HRESULT CMonsterBullet_2::Ready_GameObject(void)
{
	Add_Component();

	m_fSpeed = 10.f;
	m_pTransformCom->Set_Pos(m_vOriginPos.x, m_vOriginPos.y, m_vOriginPos.z);
	m_pTransformCom->UpdatePos_OnWorld();

	return S_OK;
}

_int CMonsterBullet_2::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		Create_Dead_Effect();
		return OBJ_DEAD;
	}

	CGameObject::Update_GameObject(fTimeDelta);

	Frame_Check(fTimeDelta);

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3	vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];
	m_pTransformCom->Bullet_Move(vDir, m_fSpeed, fTimeDelta, m_iDirCount);
	SetUp_OnTerrain();
	Distance_Dead_Check();

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CMonsterBullet_2::LateUpdate_GameObject()
{	
	if (m_bHit)
	{
		m_bDead = true;
	}
	__super::LateUpdate_GameObject();
}

void CMonsterBullet_2::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonsterBullet_2::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Monster_Bullet_2"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Monster_Bullet_2", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(2.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CMonsterBullet_2::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	 
	m_pTransformCom->Set_Pos(vPos.x, 5.f, vPos.z);
}

void CMonsterBullet_2::Change_State(void)
{
}

void CMonsterBullet_2::Frame_Check(const _float& fTimeDelta)
{
	if (m_eState == BULLET_IDLE)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bCheck = false;
		}
	}
}

void CMonsterBullet_2::Distance_Dead_Check(void)
{
	_vec3		vPosCheck;

	vPosCheck = m_vOriginPos - m_pTransformCom->m_vInfo[INFO_POS];

	if (((40 <= vPosCheck.x) || (-40 >= vPosCheck.x)) || ((40 <= vPosCheck.z) || (-40 >= vPosCheck.z)) || (-40 >= ((-vPosCheck.x) + (-vPosCheck.z))))
	{
		m_bDead = true;
	}
}

CMonsterBullet_2* CMonsterBullet_2::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _Monster_Pos, _int _Dir_Count)
{
	CMonsterBullet_2* pInstance = new CMonsterBullet_2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->Set_FireInfo(_Monster_Pos, _Dir_Count);

	return pInstance;
}

void CMonsterBullet_2::Set_FireInfo(_vec3 _Monster_Pos, _int _Dir_Count)
{	
	m_pTransformCom->Set_Pos(_Monster_Pos.x, _Monster_Pos.y, _Monster_Pos.z);
	m_vOriginPos = _Monster_Pos;
	m_iDirCount = _Dir_Count;
	//m_pTransformCom->UpdatePos_OnWorld();
}

void CMonsterBullet_2::Create_Dead_Effect(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;
	CGameObject* pGameObject_2 = nullptr;

	_vec3 vTemp_Pos = m_pTransformCom->m_vInfo[INFO_POS];
	//vTemp_Pos.y += 5;

	pGameObject = CEffect_Monster_Bullet_1_Dead::Create(m_pGraphicDev, vTemp_Pos);
	if (pGameObject == nullptr)
		return;

	pGameObject_2 = CEffect_Monster_Bullet_2_Dead::Create(m_pGraphicDev, vTemp_Pos);
	if (pGameObject_2 == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
	pGameLogicLayer->Add_BulletObject(pGameObject_2);
}

void CMonsterBullet_2::Free(void)
{
	__super::Free();
}
