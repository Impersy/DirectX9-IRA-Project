#include "stdafx.h"
#include "..\Header\DoewoleBullet_Thorn.h"
#include "Export_Function.h"
#include <Doewole.h>

CDoewoleBullet_Thorn::CDoewoleBullet_Thorn(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CDoewoleBullet_Thorn::CDoewoleBullet_Thorn(const CDoewoleBullet_Thorn& rhs)
	: CBullet(rhs)
{
}

CDoewoleBullet_Thorn::~CDoewoleBullet_Thorn()
{
}


HRESULT CDoewoleBullet_Thorn::Ready_GameObject(const _vec3& vPos , const _vec3& vScale)
{
	Add_Component();

	m_fMaxframe = 7.f;

	m_pTransformCom->m_vScale = vScale;
	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	return S_OK;
}

_int CDoewoleBullet_Thorn::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAccTime += fTimeDelta;

	Frame_Check(fTimeDelta);

	Check_Boss_Dead();

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CDoewoleBullet_Thorn::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewoleBullet_Thorn::Render_GameObject()
{
	if (m_bRender)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		m_pTextureCom->Set_Texture((_uint)m_fFrame);

		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

HRESULT CDoewoleBullet_Thorn::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Doewole_Thorn"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Doewole_Thorn", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	 
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(2.5f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CDoewoleBullet_Thorn::Frame_Check(const _float& fTimeDelta)
{
	if (m_fAccTime < 1.f)
		return;

	m_bRender = true;

	_float fSpeed = 2.f;

	if (m_pTransformCom->m_vScale.x < 0.f)
		fSpeed = 1.6f;

	if (!m_bReturn)
		m_fFrame += m_fMaxframe * fTimeDelta * fSpeed;
	else
		m_fFrame -= m_fMaxframe * fTimeDelta * fSpeed;

	if (m_fMaxframe < m_fFrame)
	{
		m_fFrame = m_fMaxframe;
		m_bReturn = true;
	}

	if (0.f > m_fFrame)
	{
		m_fFrame = 0.f;
		m_bReturn = false;
		m_bDead = true;
	}
}

CDoewoleBullet_Thorn* CDoewoleBullet_Thorn::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale)
{
	CDoewoleBullet_Thorn* pInstance = new CDoewoleBullet_Thorn(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewoleBullet_Thorn::Check_Boss_Dead()
{
	CDoewole* pDoewole = dynamic_cast<CDoewole*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Doewole"));

	if (pDoewole->Get_State() == CDoewole::BOSS_DEAD)
		m_bDead = true;
}

void CDoewoleBullet_Thorn::Free(void)
{
	__super::Free();
}
