#include "stdafx.h"
#include "..\Header\DocheolBullet_DoorBullet.h"
#include "Export_Function.h"
#include <Effect_Docheol_FireExplosion.h>

CDocheolBullet_DoorBullet::CDocheolBullet_DoorBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CDocheolBullet_DoorBullet::CDocheolBullet_DoorBullet(const CDocheolBullet_DoorBullet& rhs)
	: CBullet(rhs)
{
}

CDocheolBullet_DoorBullet::~CDocheolBullet_DoorBullet()
{
}


HRESULT CDocheolBullet_DoorBullet::Ready_GameObject(const _vec3& vPos)
{
	Add_Component();

	m_fSpeed = 20.f;

	m_fMaxframe = 39.f;

	m_fFrame = (rand() % 400) / 10.f;

	m_pTransformCom->m_vScale = { 10.f , 10.f , 10.f };

	m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x , 5.f , vPos.z };

	return S_OK;
}

_int CDocheolBullet_DoorBullet::Update_GameObject(const _float& fTimeDelta )
{
	if (m_bDead || m_bHit)
	{
		//Create_DeathEffect();
		return OBJ_DEAD;
	}

	m_fAccTime += fTimeDelta;

	Frame_Check(fTimeDelta);

	Color_Check();

	Direction_Setting();

	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CDocheolBullet_DoorBullet::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDocheolBullet_DoorBullet::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDocheolBullet_DoorBullet::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Docheol_DoorBullet"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Docheol_DoorBullet", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(2.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CDocheolBullet_DoorBullet::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxframe * fTimeDelta * 0.1f;

	if (m_fMaxframe < m_fFrame)
	{
		m_fFrame = 0.f;
	}
}

void CDocheolBullet_DoorBullet::Direction_Setting()
{
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	

	_vec3 vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];
	_vec3 vBulletPos = m_pTransformCom->m_vInfo[INFO_POS];

	m_vDir = { vPlayerPos.x - vBulletPos.x , 0.f , vPlayerPos.z - vBulletPos.z };
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}

void CDocheolBullet_DoorBullet::Color_Check()
{
	if (m_fFrame >= 0 && m_fFrame < 8.f)
		m_eColor = RED;
	else if (m_fFrame >= 8.f && m_fFrame < 16.f)
		m_eColor = BLUE;
	else if (m_fFrame >= 16.f && m_fFrame < 24.f)
		m_eColor = GREEN;
	else if (m_fFrame >= 24.f && m_fFrame < 32.f)
		m_eColor = WHITE;
	else if (m_fFrame >= 32.f && m_fFrame < 40.f)
		m_eColor = BLACK;
}


CDocheolBullet_DoorBullet* CDocheolBullet_DoorBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos)
{
	CDocheolBullet_DoorBullet* pInstance = new CDocheolBullet_DoorBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDocheolBullet_DoorBullet::Create_DeathEffect()
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Docheol_FireExplosion::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], CEffect_Docheol_FireExplosion::BULLET);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CDocheolBullet_DoorBullet::Free(void)
{
	__super::Free();
}
