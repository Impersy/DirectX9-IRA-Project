#include "stdafx.h"
#include "../Header/Bullet_Chain_Line.h"
#include "Export_Function.h"
#include "Monster.h"  
#include "Effect_Bullet_Death.h"
#include "Bullet_Chain_Head.h"

CBullet_Chain_Line::CBullet_Chain_Line(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CBullet_Chain_Line::CBullet_Chain_Line(const CBullet_Chain_Line& rhs)
	: CBullet(rhs)
{
}

CBullet_Chain_Line::~CBullet_Chain_Line()
{
}


HRESULT CBullet_Chain_Line::Ready_GameObject(CGameObject* pOwner, const _vec3& vDir)
{
	Add_Component();

	m_pOwner = pOwner;

	//m_fSpeed = 200.f;

	m_fMaxframe = 4.f;

	m_pTransformCom->m_vScale = { 3.f , 3.f, 0.f };

	m_vDir = vDir;

	m_pTransformCom->m_vInfo[INFO_POS] = *(dynamic_cast<CBullet_Chain_Head*>(m_pOwner)->Get_PrePos());
	//m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CBullet_Chain_Head*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
		;
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

_int CBullet_Chain_Line::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead || m_bHit)
	{
		Create_DeathEffect(fTimeDelta);
		return OBJ_DEAD;
	}

	//if (dynamic_cast<CBlueEvilSoulBall*>(m_pOwner)->Get_IsStop())
	//	m_bStop = true;

	m_fAccumulatedTime += fTimeDelta;

	//if(!m_bStop)
	//	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

	CGameObject::Update_GameObject(fTimeDelta);

	Frame_Check(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);

	return OBJ_NOEVENT;
}

void CBullet_Chain_Line::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fAccumulatedTime > 4.f)
		m_bDead = true;
}

void CBullet_Chain_Line::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBullet_Chain_Line::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Chain02"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Chain02", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	 
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(2.f);
	_vec3 vOffset = { 0.f, 5.f, 0.f };
	m_pColliderCom->Set_Offset(vOffset);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CBullet_Chain_Line::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxframe * fTimeDelta;

	if (m_fFrame > m_fMaxframe)
		m_fFrame = 0.f;
}

CBullet_Chain_Line* CBullet_Chain_Line::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _vec3& vDir)
{
	CBullet_Chain_Line* pInstance = new CBullet_Chain_Line(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner, vDir)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBullet_Chain_Line::Create_DeathEffect(_float fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Bullet_Death::Create(m_pGraphicDev, this, fTimeDelta);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CBullet_Chain_Line::Free(void)
{
	__super::Free();
}
