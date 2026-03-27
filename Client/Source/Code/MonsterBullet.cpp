#include "stdafx.h"
#include "..\Header\MonsterBullet.h"
#include "Export_Function.h"

#include "Effect_Monster_Bullet_1_Dead.h"
#include "CollisionSphere.h"
#include "CollisionMgr.h"


CMonsterBullet::CMonsterBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CMonsterBullet::CMonsterBullet(const CMonsterBullet& rhs)
	: CBullet(rhs)
{
}

CMonsterBullet::~CMonsterBullet()
{
	Free();
}

HRESULT CMonsterBullet::Ready_GameObject(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSpeed = 10.f;

	return S_OK;
}

_int CMonsterBullet::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_bDead || m_bHit)
	{
		Create_Dead_Effect();
		return OBJ_DEAD;
	}

	CGameObject::Update_GameObject(fTimeDelta);
	Frame_Check(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	if (nullptr == pPlayerTransformCom) return - 1;
	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	_vec3	vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];
	m_pTransformCom->Bullet_Move(vDir, m_fSpeed, fTimeDelta, 0);

	SetUp_OnTerrain();
	Distance_Dead_Check();

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CMonsterBullet::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CMonsterBullet::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonsterBullet::Add_Component(void)
{	
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Monster_Bullet_1"));
	if (nullptr == m_pTextureCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Monster_Bullet_1", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	if (nullptr == m_pColliderCom) { MessageBox(NULL, L"콜라이더 생성", L"System Message", MB_OK); }
	m_pColliderCom->Set_Radius(2.f);
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CMonsterBullet::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	if (nullptr == pTerrainBufferCom) return;
	m_pTransformCom->Set_Pos(vPos.x, 6.5f, vPos.z);
}

void CMonsterBullet::Change_State(void)
{
}

void CMonsterBullet::Frame_Check(const _float& fTimeDelta)
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

void CMonsterBullet::Distance_Dead_Check(void)
{
	_vec3		vPosCheck;

	vPosCheck = m_vOriginPos - m_pTransformCom->m_vInfo[INFO_POS];

	if (((40 <= vPosCheck.x) || (-40 >= vPosCheck.x)) || ((40 <= vPosCheck.z) || (-40 >= vPosCheck.z)) || (-40 >= ((-vPosCheck.x) + (-vPosCheck.z))))
	{
		m_bDead = true;
	}
}

CMonsterBullet* CMonsterBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _Monster_Pos, _bool _Target_Check)
{
	CMonsterBullet* pInstance = new CMonsterBullet(pGraphicDev);


	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->Set_FireInfo(_Monster_Pos, _Target_Check);

	return pInstance;
}

void CMonsterBullet::Set_FireInfo(_vec3 _Monster_Pos, _bool _Target_Check)
{	
	m_pTransformCom->Set_Pos(_Monster_Pos.x, _Monster_Pos.y + 1.5, _Monster_Pos.z);
	m_vOriginPos = m_pTransformCom->m_vInfo[INFO_POS];
	m_bCheck = _Target_Check;
	//m_pTransformCom->UpdatePos_OnWorld();
}

void CMonsterBullet::Create_Dead_Effect(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Monster_Bullet_1_Dead::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CMonsterBullet::Free(void)
{
	__super::Free();
}