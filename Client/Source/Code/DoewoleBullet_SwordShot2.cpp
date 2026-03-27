#include "stdafx.h"
#include "..\Header\DoewoleBullet_SwordShot2.h"
#include "Export_Function.h"
#include <Effect_SwordShot_Death.h>

CDoewoleBullet_SwordShot2::CDoewoleBullet_SwordShot2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CDoewoleBullet_SwordShot2::CDoewoleBullet_SwordShot2(const CDoewoleBullet_SwordShot2& rhs)
	: CBullet(rhs)
{
}

CDoewoleBullet_SwordShot2::~CDoewoleBullet_SwordShot2()
{
}


HRESULT CDoewoleBullet_SwordShot2::Ready_GameObject(const _vec3& vPos, const _float& fAngle)
{
	Add_Component();

	m_fSpeed = 50.f;

	m_fMaxframe = 7.f;

	m_pTransformCom->m_vScale = { -1.f , 1.f , 1.f };

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-fAngle));

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;
	m_pTransformCom->UpdatePos_OnWorld();

	CTransform* pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	 

	m_vDir = { vPos.x - pTransformCom->m_vInfo[INFO_POS].x , 0.f , vPos.z - pTransformCom->m_vInfo[INFO_POS].z };

	D3DXVec3Normalize(&m_vDir, &m_vDir);
	
	return S_OK;
}

_int CDoewoleBullet_SwordShot2::Update_GameObject(const _float& fTimeDelta )
{
	if (m_bDead || m_bHit)
{
		Create_DeathEffect();
		return OBJ_DEAD;
	}

	m_fAccTime += fTimeDelta;

	Frame_Check(fTimeDelta);

	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CDoewoleBullet_SwordShot2::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewoleBullet_SwordShot2::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDoewoleBullet_SwordShot2::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Doewole_SwordShot"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Doewole_SwordShot", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	 
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(3.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CDoewoleBullet_SwordShot2::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxframe * fTimeDelta;

	if (m_fMaxframe < m_fFrame)
	{
		m_fFrame = 0.f;
	}

	if (m_fAccTime > 5.f)
		m_bDead = true;
}


CDoewoleBullet_SwordShot2* CDoewoleBullet_SwordShot2::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _float& fAngle)
{
	CDoewoleBullet_SwordShot2* pInstance = new CDoewoleBullet_SwordShot2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos ,fAngle )))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewoleBullet_SwordShot2::Create_DeathEffect()
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_SwordShot_Death::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CDoewoleBullet_SwordShot2::Free(void)
{
	__super::Free();
}
