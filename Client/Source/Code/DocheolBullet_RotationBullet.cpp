#include "stdafx.h"
#include "..\Header\DocheolBullet_RotationBullet.h"
#include "Export_Function.h"
#include <Effect_Docheol_FireExplosion.h>
#include <Docheol.h>

CDocheolBullet_RotationBullet::CDocheolBullet_RotationBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CDocheolBullet_RotationBullet::CDocheolBullet_RotationBullet(const CDocheolBullet_RotationBullet& rhs)
	: CBullet(rhs)
{
}

CDocheolBullet_RotationBullet::~CDocheolBullet_RotationBullet()
{
}


HRESULT CDocheolBullet_RotationBullet::Ready_GameObject(const _vec3& vCenterPos, _float fAngle, const _vec3& vScale, const _float& fSpeed, const _float& fAliveTime)
{
	Add_Component();

	m_fSpeed = fSpeed;

	m_fAliveTime = fAliveTime;

	m_fFrame = 8.f;

	m_fMaxframe = 15.f;

	m_pTransformCom->m_vScale = vScale;

	m_vCenterPos = vCenterPos;

	m_fRotRadius = 50.f;

	m_fAngle = fAngle;

	return S_OK;
}

_int CDocheolBullet_RotationBullet::Update_GameObject(const _float& fTimeDelta )
{
	if (m_bDead || m_bHit)
{
		Create_DeathEffect();

		if (m_bDead)
		{
			CDocheol* pDocheol = dynamic_cast<CDocheol*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Docheol"));
			pDocheol->Set_CreateMark(true);
		}

		return OBJ_DEAD;
	}

	m_fAccTime += fTimeDelta;

	Frame_Check(fTimeDelta);

	Check_Boss_Dead();

	m_fAngle += 60.f * fTimeDelta;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vCenterPos + m_fRotRadius * _vec3{ cosf(D3DXToRadian(m_fAngle)) , 0.f , sinf(D3DXToRadian(m_fAngle)) };

	CGameObject::Update_GameObject(fTimeDelta);
	
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	m_fPreAngle = m_fAngle;

	return OBJ_NOEVENT;
}

void CDocheolBullet_RotationBullet::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDocheolBullet_RotationBullet::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDocheolBullet_RotationBullet::Add_Component(void)
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

void CDocheolBullet_RotationBullet::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxframe * fTimeDelta * 1.5f;

	if (m_fMaxframe < m_fFrame)
	{
		m_fFrame = 8.f;
	}

	if (m_fAccTime > m_fAliveTime)
		m_bDead = true;
}


CDocheolBullet_RotationBullet* CDocheolBullet_RotationBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vCenterPos, _float fAngle, const _vec3& vScale,  const _float& fSpeed, const _float& fAliveTime)
{
	CDocheolBullet_RotationBullet* pInstance = new CDocheolBullet_RotationBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vCenterPos,fAngle , vScale,fSpeed, fAliveTime)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDocheolBullet_RotationBullet::Create_DeathEffect()
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Docheol_FireExplosion::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], CEffect_Docheol_FireExplosion::BULLET);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CDocheolBullet_RotationBullet::Check_Boss_Dead()
{
	CDocheol* pDocheol = dynamic_cast<CDocheol*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Docheol"));

	if (pDocheol->Get_State() == CDocheol::BOSS_DEAD)
		m_bDead = true;
}


void CDocheolBullet_RotationBullet::Free(void)
{
	__super::Free();
}
