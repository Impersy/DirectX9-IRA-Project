#include "stdafx.h"
#include "../Header/Bullet_Chain_Head.h"
#include "Export_Function.h"
#include "Monster.h"  
#include "Effect_Bullet_Death.h"
#include "BlueEvilSoulBall.h"
#include "Bullet_Chain_Line.h"
#include "Bullet_Chain_Circle.h"
#include "Terrain.h"

CBullet_Chain_Head::CBullet_Chain_Head(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CBullet_Chain_Head::CBullet_Chain_Head(const CBullet_Chain_Head& rhs)
	: CBullet(rhs)
{
}

CBullet_Chain_Head::~CBullet_Chain_Head()
{
}


HRESULT CBullet_Chain_Head::Ready_GameObject(CGameObject* pOwner, const _vec3& vDir)
{
	Add_Component();

	m_pOwner = pOwner;

	m_fSpeed = 70.f;

	m_fMaxframe = 4.f;

	m_pTransformCom->m_vScale = { 3.f , 3.f, 0.f };

	m_vDir = vDir;

	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y = 2.0f;
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	D3DXVec3Normalize(&m_vDir, &m_vDir);

	_float fDot = D3DXVec3Dot(&m_pTransformCom->m_vInfo[INFO_RIGHT], &m_vDir);

	_float fAngle = acosf(fDot);

	if (m_vDir.z > 0.f)
		fAngle = 2.f * D3DX_PI - fAngle;

	m_pTransformCom->Rotation(ROT_Y, fAngle);

	return S_OK;
}

_int CBullet_Chain_Head::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pOwner->Get_Dead())
		m_bDead = true;

	if (m_bDead || m_bHit)
	{
		Create_DeathEffect(fTimeDelta);
		return OBJ_DEAD;
	}

	if (dynamic_cast<CBlueEvilSoulBall*>(m_pOwner)->Get_IsStop())
		m_bStop = true;


	if (!m_bStop)
	{
		m_fAccumulatedTime += fTimeDelta;
		m_fAccumulatedTimeBullet += fTimeDelta;

		m_vPrePos = m_pTransformCom->m_vInfo[INFO_POS];
		m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

		if (.1f < m_fAccumulatedTimeBullet)
		{
			Create_Bullet_Chain();
			m_fAccumulatedTimeBullet = 0.f;
		}
	}

	if (m_bStop && !m_bHeadStop)
	{
		m_fAccumulatedTimeStop += fTimeDelta;
		m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

		if (.03f < m_fAccumulatedTimeStop)
		{
			m_bHeadStop = true;
		}
	}

	if (m_bStop)
	{
		m_fAccumulatedTimeDead += fTimeDelta;
	}


	CGameObject::Update_GameObject(fTimeDelta);

	SetUp_OnTerrain();

	Frame_Check(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CBullet_Chain_Head::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fAccumulatedTimeDead > 4.1f)
		m_bDead = true;
}

void CBullet_Chain_Head::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBullet_Chain_Head::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_ChainHead"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_ChainHead", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(2.f);
	_vec3 vOffset = { 0.f, 5.f, 0.f };
	m_pColliderCom->Set_Offset(vOffset);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CBullet_Chain_Head::SetUp_OnTerrain(void)
{
	_vec3		vPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos;

	_tchar szBuf[MAX_STR];
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Environment", L"Terrain"));
	pTerrain->Get_TerrainTexKey(szBuf);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", szBuf, ID_STATIC));

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), pTerrainBufferCom->Get_VtxCntX(), pTerrainBufferCom->Get_VtxCntZ());

	m_pTransformCom->Set_Pos(vPos.x, fHeight + 1.0f, vPos.z);
}

void CBullet_Chain_Head::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxframe * fTimeDelta;

	if (m_fFrame > m_fMaxframe)
		m_fFrame = 0.f;
}

CBullet_Chain_Head* CBullet_Chain_Head::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _vec3& vDir)
{
	CBullet_Chain_Head* pInstance = new CBullet_Chain_Head(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner, vDir)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBullet_Chain_Head::Create_DeathEffect(_float fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Bullet_Death::Create(m_pGraphicDev, this, fTimeDelta);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CBullet_Chain_Head::Create_Bullet_Chain()
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CBullet* pBullet;

	if (0 == m_iChainCnt % 2)
	{
		pBullet = CBullet_Chain_Line::Create(m_pGraphicDev, this, m_vDir);
		pGameLogicLayer->Add_BulletObject(pBullet);
		++m_iChainCnt;
	}
	else
	{
		pBullet = CBullet_Chain_Circle::Create(m_pGraphicDev, this, m_vDir);
		pGameLogicLayer->Add_BulletObject(pBullet);
		++m_iChainCnt;
	}
}

void CBullet_Chain_Head::Free(void)
{
	__super::Free();
}
