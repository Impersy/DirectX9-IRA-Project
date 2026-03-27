#include "stdafx.h"
#include "../Header/Bullet_EvilFrog_Return.h"
#include "Export_Function.h"
#include "Monster.h"
#include "EvilFrog.h"
#include "Effect_Bullet_Death.h"
#include "Effect_Bullet_BackHit.h"
#include "Effect_Bullet_Birth.h"
#include "Effect_Bullet_Birth_RayUp.h"

CBullet_EvilFrog_Return::CBullet_EvilFrog_Return(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CBullet_EvilFrog_Return::CBullet_EvilFrog_Return(const CBullet_EvilFrog_Return& rhs)
	: CBullet(rhs)
{
}

CBullet_EvilFrog_Return::~CBullet_EvilFrog_Return()
{
}


HRESULT CBullet_EvilFrog_Return::Ready_GameObject(CGameObject* pOwner, const _vec3& vDir, const _float& fTimeDelta)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pOwner = pOwner;

	m_fSpeed = 45.f;

	m_fMaxframe = 5.f;

	m_pTransformCom->m_vScale = { 4.f , 4.f, 0.f };

	m_vDir = vDir;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	_float fDot = D3DXVec3Dot(&m_pTransformCom->m_vInfo[INFO_RIGHT], &m_vDir);

	_float fAngle = acosf(fDot);

	if (m_vDir.z > 0.f)
		fAngle = 2.f * D3DX_PI - fAngle;

	m_pTransformCom->Rotation(ROT_Y, fAngle);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y = 5.f;

	Create_Effect_Bullet_Birth(fTimeDelta);
	Create_Effect_Bullet_Birth_RayUp(fTimeDelta);

	return S_OK;
}

_int CBullet_EvilFrog_Return::Update_GameObject(const _float& fTimeDelta)
{
	if (-290.f >= m_fAcceleratedSpeed)
		m_bReturned = true;

	if (m_pOwner->Get_Dead())
		m_bDead = true;

	if (m_bDead || m_bHit)
	{
		Create_DeathEffect(fTimeDelta);
		Create_BackHitEffect(fTimeDelta);
		return OBJ_DEAD;
	}

	if (m_bReturned)
		return OBJ_DEAD;

	m_fAccumulatedTime += fTimeDelta;

	m_fAcceleratedSpeed -= 450.f * fTimeDelta;
	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fAcceleratedSpeed * fTimeDelta;

	CGameObject::Update_GameObject(fTimeDelta);

	Frame_Check(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CBullet_EvilFrog_Return::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBullet_EvilFrog_Return::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBullet_EvilFrog_Return::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Return"));
	if (nullptr == m_pTextureCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Return", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	if (nullptr == m_pColliderCom) return E_FAIL;
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(2.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CBullet_EvilFrog_Return::Frame_Check(const _float& fTimeDelta)
{
	if (0 <= m_fAcceleratedSpeed)
		m_fFrame += m_fMaxframe * 2.f * fTimeDelta;
	else
		m_fFrame -= m_fMaxframe * 2.f * fTimeDelta;
	
	if (m_fFrame < 0.f)
		m_fFrame = 0.f;

	if (m_fFrame > m_fMaxframe)
		m_fFrame = m_fMaxframe;
}

CBullet_EvilFrog_Return* CBullet_EvilFrog_Return::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _vec3& vDir, const _float& fTimeDelta)
{
	CBullet_EvilFrog_Return* pInstance = new CBullet_EvilFrog_Return(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner, vDir, fTimeDelta)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBullet_EvilFrog_Return::Create_Effect_Bullet_Birth(_float fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Bullet_Birth* pEffect = CEffect_Bullet_Birth::Create(m_pGraphicDev, this, fTimeDelta);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

void CBullet_EvilFrog_Return::Create_Effect_Bullet_Birth_RayUp(_float fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Bullet_Birth_RayUp* pEffect = CEffect_Bullet_Birth_RayUp::Create(m_pGraphicDev, this, fTimeDelta);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

void CBullet_EvilFrog_Return::Create_DeathEffect(_float fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Bullet_Death::Create(m_pGraphicDev, this, fTimeDelta);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CBullet_EvilFrog_Return::Create_BackHitEffect(_float fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Bullet_BackHit::Create(m_pGraphicDev, this, fTimeDelta);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CBullet_EvilFrog_Return::Free(void)
{
	__super::Free();
}
