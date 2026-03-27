#include "stdafx.h"
#include "..\Header\DoewoleBullet_SwordShot.h"
#include "Export_Function.h"
#include <Effect_SwordShot_Death.h>
#include <Doewole.h>

CDoewoleBullet_SwordShot::CDoewoleBullet_SwordShot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CDoewoleBullet_SwordShot::CDoewoleBullet_SwordShot(const CDoewoleBullet_SwordShot& rhs)
	: CBullet(rhs)
{
}

CDoewoleBullet_SwordShot::~CDoewoleBullet_SwordShot()
{
}


HRESULT CDoewoleBullet_SwordShot::Ready_GameObject(const _vec3& vPos , const _float& fAngle)
{
	Add_Component();

	m_fSpeed = 40.f;

	m_fMaxframe = 7.f;

	m_pTransformCom->m_vScale = { -3.f , 3.f , 3.f };

	m_fAngle = fAngle;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(-m_fAngle));

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;
	
	CTransform* pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	 

	m_vDir = { vPos.x - pTransformCom->m_vInfo[INFO_POS].x , 0.f , vPos.z - pTransformCom->m_vInfo[INFO_POS].z };

	D3DXVec3Normalize(&m_vDir, &m_vDir);

	return S_OK;
}

_int CDoewoleBullet_SwordShot::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead || m_bHit)
	{
		Create_DeathEffect();
		return OBJ_DEAD;
	}

	m_fAccTime += fTimeDelta;

	m_fAccAngle += 0.02f * fTimeDelta * 50.f;
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(m_fAccAngle));

	_float RotAngle = 0.02f;

	D3DXMATRIX rotation_matrix;
	D3DXMatrixRotationAxis(&rotation_matrix, &_vec3(0.f , 0.1f, 0.f), RotAngle);

	D3DXVec3TransformCoord(&m_vDir, &m_vDir, &rotation_matrix);

	Check_Boss_Dead();

	Frame_Check(fTimeDelta);
	
	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;
 
	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CDoewoleBullet_SwordShot::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewoleBullet_SwordShot::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDoewoleBullet_SwordShot::Add_Component(void)
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
	m_pColliderCom->Set_Radius(4.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CDoewoleBullet_SwordShot::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += 7.f * fTimeDelta;

	if (7.f < m_fFrame)
	{
		m_fFrame = 0.f;
	}

	if (m_fAccTime > 5.f)
		m_bDead = true;
}


CDoewoleBullet_SwordShot* CDoewoleBullet_SwordShot::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _float& fAngle)
{
	CDoewoleBullet_SwordShot* pInstance = new CDoewoleBullet_SwordShot(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , fAngle)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewoleBullet_SwordShot::Create_DeathEffect()
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_SwordShot_Death::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CDoewoleBullet_SwordShot::Check_Boss_Dead()
{
	CDoewole* pDoewole = dynamic_cast<CDoewole*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Doewole"));

	if (pDoewole->Get_State() == CDoewole::BOSS_DEAD)
		m_bDead = true;
}


void CDoewoleBullet_SwordShot::Free(void)
{
	__super::Free();
}
