#include "stdafx.h"
#include "..\Header\DocheolBullet_SwingBullet.h"
#include "Export_Function.h"
#include <Effect_Docheol_FireExplosion.h>
#include <Docheol.h>

CDocheolBullet_SwingBullet::CDocheolBullet_SwingBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CDocheolBullet_SwingBullet::CDocheolBullet_SwingBullet(const CDocheolBullet_SwingBullet& rhs)
	: CBullet(rhs)
{
}

CDocheolBullet_SwingBullet::~CDocheolBullet_SwingBullet()
{
}


HRESULT CDocheolBullet_SwingBullet::Ready_GameObject(const _vec3& vPos, const _vec3& vDir, const _vec3& vScale, const _float& fSpeed, const _float& fAliveTime)
{
	Add_Component();

	m_fSpeed = fSpeed;

	m_fAliveTime = fAliveTime;

	m_fMaxframe = 9.f;

	m_pTransformCom->m_vScale = vScale;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90));

	m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x , 5.f , vPos.z };

	m_vDir = vDir;

	_float fThetha = 0.f;

	fThetha = acosf(m_vDir.x);

	if (m_vDir.z > 0)
		fThetha = 2 * D3DX_PI - fThetha;

	m_pTransformCom->Rotation(ROT_Y, fThetha);

	return S_OK;
}

_int CDocheolBullet_SwingBullet::Update_GameObject(const _float& fTimeDelta )
{
	if (m_bDead || m_bHit)
{
		Create_DeathEffect();
		return OBJ_DEAD;
	}

	m_fAccTime += fTimeDelta;

	Frame_Check(fTimeDelta);

	Check_Boss_Dead();

	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CDocheolBullet_SwingBullet::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDocheolBullet_SwingBullet::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDocheolBullet_SwingBullet::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Docheol_SwingBullet"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Docheol_SwingBullet", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(2.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CDocheolBullet_SwingBullet::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxframe * fTimeDelta * 1.5f;

	if (m_fMaxframe < m_fFrame)
	{
		m_fFrame = 0.f;
	}

	if (m_fAccTime > m_fAliveTime)
		m_bDead = true;
}


CDocheolBullet_SwingBullet* CDocheolBullet_SwingBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vDir, const _vec3& vScale,  const _float& fSpeed, const _float& fAliveTime)
{
	CDocheolBullet_SwingBullet* pInstance = new CDocheolBullet_SwingBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos ,vDir , vScale,fSpeed, fAliveTime)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDocheolBullet_SwingBullet::Create_DeathEffect()
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Docheol_FireExplosion::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], CEffect_Docheol_FireExplosion::BULLET);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CDocheolBullet_SwingBullet::Check_Boss_Dead()
{
	CDocheol* pDocheol = dynamic_cast<CDocheol*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Docheol"));

	if (pDocheol->Get_State() == CDocheol::BOSS_DEAD)
		m_bDead = true;
}

void CDocheolBullet_SwingBullet::Free(void)
{
	__super::Free();
}

