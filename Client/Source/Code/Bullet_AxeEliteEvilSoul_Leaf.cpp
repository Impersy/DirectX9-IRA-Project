#include "stdafx.h"
#include "..\Header\Bullet_AxeEliteEvilSoul_Leaf.h"
#include "Export_Function.h"
#include "Monster.h"
#include "AxeEliteEvilSoul.h"
#include "Effect_Leaf_Bullet_Dead.h"
#include "Effect_Leaf_Bullet_BackHit.h"

CBullet_AxeEvilSoul_Leaf::CBullet_AxeEvilSoul_Leaf(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CBullet_AxeEvilSoul_Leaf::CBullet_AxeEvilSoul_Leaf(const CBullet_AxeEvilSoul_Leaf& rhs)
	: CBullet(rhs)
{
}

CBullet_AxeEvilSoul_Leaf::~CBullet_AxeEvilSoul_Leaf()
{
}


HRESULT CBullet_AxeEvilSoul_Leaf::Ready_GameObject(CGameObject* pOwner, const _vec3& vDir, _float fAngleVariation)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pOwner = pOwner;

	m_fSpeed = 80.f;

	m_fMaxframe = 4.f;

	m_pTransformCom->m_vScale = { 5.f , 5.f, 0.f };

	m_vDir = vDir;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	_matrix matRot;
	D3DXMatrixIdentity(&matRot);

	D3DXMatrixRotationY(&matRot, fAngleVariation);
	D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRot);

	_float fDot = D3DXVec3Dot(&m_pTransformCom->m_vInfo[INFO_RIGHT], &m_vDir);

	_float fAngle = acosf(fDot);

	if (m_vDir.z > 0.f)
		fAngle = 2.f * D3DX_PI - fAngle;

	m_pTransformCom->Rotation(ROT_Y, fAngle);


	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y = 5.f;

	_vec3* pTargetPos = dynamic_cast<CAxeEliteEvilSoul*>(m_pOwner)->Get_TargetPos();
	pTargetPos->y = 5.f;

	//_vec3 vTemp = *pTargetPos - m_pTransformCom->m_vInfo[INFO_POS];
	//if (vTemp < _vec3(0.f, 0.f, 0.f))
	//	m_bDead = true;

	return S_OK;
}

_int CBullet_AxeEvilSoul_Leaf::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pOwner->Get_Dead())
		return OBJ_DEAD;

	//if (m_bDead || m_bHit)
	if (m_bDead)
	{
		Create_DeathEffect(fTimeDelta);
		Create_BackHitEffect(fTimeDelta);
		return OBJ_DEAD;
	}
	if (m_bHit)
	{
		return OBJ_DEAD;
	}

	Frame_Check(fTimeDelta);

	//Check_Boss_Dead();

	m_fAccumulatedTime += fTimeDelta;

	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CBullet_AxeEvilSoul_Leaf::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBullet_AxeEvilSoul_Leaf::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBullet_AxeEvilSoul_Leaf::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Leaf"));
	if (nullptr == m_pTextureCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Leaf", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	if (nullptr == m_pColliderCom) return E_FAIL;
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(2.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CBullet_AxeEvilSoul_Leaf::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxframe * fTimeDelta;

	if (m_fMaxframe < m_fFrame)
		m_fFrame = 0.f;

	if (m_fAccumulatedTime > 10.f)
		m_bDead = true;
}

CBullet_AxeEvilSoul_Leaf* CBullet_AxeEvilSoul_Leaf::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _vec3& vDir, _float fAngleVariation)
{
	CBullet_AxeEvilSoul_Leaf* pInstance = new CBullet_AxeEvilSoul_Leaf(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner, vDir, fAngleVariation)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBullet_AxeEvilSoul_Leaf::Create_DeathEffect(_float fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Leaf_Bullet_Dead::Create(m_pGraphicDev, this, fTimeDelta);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CBullet_AxeEvilSoul_Leaf::Create_BackHitEffect(_float fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Leaf_Bullet_BackHit::Create(m_pGraphicDev, this, fTimeDelta);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}



void CBullet_AxeEvilSoul_Leaf::Free(void)
{
	__super::Free();
}
