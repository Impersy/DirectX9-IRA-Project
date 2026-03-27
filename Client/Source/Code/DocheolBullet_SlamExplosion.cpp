#include "stdafx.h"
#include "..\Header\DocheolBullet_SlamExplosion.h"
#include "Export_Function.h"
#include <Effect_Docheol_FireExplosion.h>

CDocheolBullet_SlamExplosion::CDocheolBullet_SlamExplosion(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CDocheolBullet_SlamExplosion::CDocheolBullet_SlamExplosion(const CDocheolBullet_SlamExplosion& rhs)
	: CBullet(rhs)
{
}

CDocheolBullet_SlamExplosion::~CDocheolBullet_SlamExplosion()
{
}


HRESULT CDocheolBullet_SlamExplosion::Ready_GameObject(const _vec3& vPos , const _float& fDelayTime)
{
	Add_Component();

	m_fMaxframe = 23.f;

	m_pTransformCom->m_vScale = { 25.f , 25.f , 25.f };

	m_fDelayTime = fDelayTime;

	//m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90));

	m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x , 9.f, vPos.z + 3.f};

	return S_OK;
}

_int CDocheolBullet_SlamExplosion::Update_GameObject(const _float& fTimeDelta )
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAccTime += fTimeDelta;

	Frame_Check(fTimeDelta);

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CDocheolBullet_SlamExplosion::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDocheolBullet_SlamExplosion::Render_GameObject()
{
	if (m_fDelayTime > m_fAccTime)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDocheolBullet_SlamExplosion::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));

	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Docheol_SlamExplosion"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Docheol_SlamExplosion", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(9.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CDocheolBullet_SlamExplosion::Frame_Check(const _float& fTimeDelta)
{
	if(m_fDelayTime < m_fAccTime)
		m_fFrame += m_fMaxframe * fTimeDelta * 1.5f;

	if (m_fMaxframe < m_fFrame)
	{
		m_fFrame = m_fMaxframe;
		m_bDead = true;

	}
}


CDocheolBullet_SlamExplosion* CDocheolBullet_SlamExplosion::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _float& fDelayTime)
{
	CDocheolBullet_SlamExplosion* pInstance = new CDocheolBullet_SlamExplosion(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , fDelayTime)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDocheolBullet_SlamExplosion::Free(void)
{
	__super::Free();
}
