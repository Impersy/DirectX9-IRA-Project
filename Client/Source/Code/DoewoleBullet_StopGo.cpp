#include "stdafx.h"
#include "..\Header\DoewoleBullet_StopGo.h"
#include "Export_Function.h"
#include <Doewole.h>
#include <Effect_StandardBullet_Death.h>

CDoewoleBullet_StopGo::CDoewoleBullet_StopGo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CDoewoleBullet_StopGo::CDoewoleBullet_StopGo(const CDoewoleBullet_StopGo& rhs)
	: CBullet(rhs)
{
}

CDoewoleBullet_StopGo::~CDoewoleBullet_StopGo()
{
}


HRESULT CDoewoleBullet_StopGo::Ready_GameObject(const _vec3& vPos , ClAWDIR _e)
{
	Add_Component();

	m_fSpeed = rand() % 10 * 3.f + 10.f;
	m_fMaxSpeed = 40.f;

	m_fMaxframe = 7.f;

	m_pTransformCom->m_vScale = { 3.f , 3.f , 3.f };

	m_eClaw = _e;

	if (m_eClaw == CLAW_LEFT)
		m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x - (rand() % 30) * 0.02f , 3.f , vPos.z - ( 30 - rand() % 60 ) * 0.02f};
	else
		m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x + (rand() % 30) * 0.02f , 3.f , vPos.z - (30 - rand() % 60) * 0.02f };

	m_vDir = m_pTransformCom->m_vInfo[INFO_POS] -vPos;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	return S_OK;
}

_int CDoewoleBullet_StopGo::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead || m_bHit)
	{
		Create_DeathEffect();
		return OBJ_DEAD;
	}

	Frame_Check(fTimeDelta);

	Check_Boss_Dead();

	m_fAccTime += fTimeDelta;

	if (!m_bFirstStop)
	{
		m_fSpeed -= 0.1f;

		if (m_fSpeed < 0.f)
		{
			m_fSpeed = 0.f;
			m_bFirstStop = true;
			m_bStop = true;
		}
	}
	
	if(m_bFirstStop && !m_bStop)
	{
		m_fSpeed += 0.1f;

		if (m_fSpeed > m_fMaxSpeed)
			m_fSpeed = m_fMaxSpeed;
	}

	CDoewole* pDoewole = dynamic_cast<CDoewole*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Doewole"));
	m_bStop = !pDoewole->Get_BulletGo();

	if(!m_bFirstStop || !m_bStop)
		m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

	// Á×¿©¾ßµÊ


	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CDoewoleBullet_StopGo::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewoleBullet_StopGo::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDoewoleBullet_StopGo::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Doewole_Standard"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Doewole_Standard", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	 
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(4.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CDoewoleBullet_StopGo::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxframe * fTimeDelta;

	if (m_fMaxframe < m_fFrame)
		m_fFrame = 0.f;
}

CDoewoleBullet_StopGo* CDoewoleBullet_StopGo::Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , ClAWDIR _e)
{
	CDoewoleBullet_StopGo* pInstance = new CDoewoleBullet_StopGo(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , _e)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}


void CDoewoleBullet_StopGo::Free(void)
{
	__super::Free();
}

void CDoewoleBullet_StopGo::Create_DeathEffect()
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_StandardBullet_Death::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CDoewoleBullet_StopGo::Check_Boss_Dead()
{
	CDoewole* pDoewole = dynamic_cast<CDoewole*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Doewole"));

	if (pDoewole->Get_State() == CDoewole::BOSS_DEAD)
		m_bDead = true;
}