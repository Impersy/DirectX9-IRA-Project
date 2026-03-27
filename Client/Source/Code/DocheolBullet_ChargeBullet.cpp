#include "stdafx.h"
#include "..\Header\DocheolBullet_ChargeBullet.h"
#include "Export_Function.h"
#include "Docheol.h"
#include "Effect_Docheol_FirePulse.h"
#include "DocheolBullet_RotationBullet.h"

CDocheolBullet_ChargeBullet::CDocheolBullet_ChargeBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev)
{
}

CDocheolBullet_ChargeBullet::CDocheolBullet_ChargeBullet(const CDocheolBullet_ChargeBullet& rhs)
	: CBullet(rhs)
{
}

CDocheolBullet_ChargeBullet::~CDocheolBullet_ChargeBullet()
{
}


HRESULT CDocheolBullet_ChargeBullet::Ready_GameObject(const _vec3& vPos, const _vec3& vDir, const _vec3& vScale, const _float& fSpeed , const _bool& bMove , TYPE eType)
{
	Add_Component();

	m_fSpeed = fSpeed;

	m_bMove = bMove;

	m_fMaxframe = 23.f;

	m_pTransformCom->m_vScale = vScale;

	m_eType = eType;

	m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x , 5.f , vPos.z };

	m_vDir = vDir;

	return S_OK;
}

_int CDocheolBullet_ChargeBullet::Update_GameObject(const _float& fTimeDelta )
{
	CDocheol* pDocheol = dynamic_cast<CDocheol*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Docheol"));

	// Dead üũ
	switch (m_eType)
	{
	case CDocheolBullet_ChargeBullet::CHARGE:
		if (pDocheol->Get_ChargeBulletDead())
		{
			Create_EffectAndBullet();

			pDocheol->Add_PosList(m_pTransformCom->m_vInfo[INFO_POS]);

			return OBJ_DEAD;
		}
		break;
	case CDocheolBullet_ChargeBullet::SPECIAL:
		if (m_bDead)
			return OBJ_DEAD;
		break;
	}

	m_fAccTime += fTimeDelta;

	if (m_fAccTime > 1.f)
		m_bRender = true;

	Frame_Check(fTimeDelta);

	Check_Boss_Dead();

	switch (m_eType)
	{
	case CDocheolBullet_ChargeBullet::CHARGE:
		if (m_bMove)
		{
			if (pDocheol->Get_BulletGo())
				m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;
		}
		break;
	case CDocheolBullet_ChargeBullet::SPECIAL:
		if (m_fAccTime > 2.f)
			m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;
		break;

		if (m_fAccTime > 5.f)
			m_bDead = true;
	default:
		break;
	}


	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BULLET, this);


	return OBJ_NOEVENT;
}

void CDocheolBullet_ChargeBullet::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDocheolBullet_ChargeBullet::Render_GameObject()
{
	if (!m_bRender)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(200, 0, 0, 0));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);


	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDocheolBullet_ChargeBullet::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_FireBall"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_FireBall", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));

	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(4.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CDocheolBullet_ChargeBullet::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fMaxframe * fTimeDelta * 1.f;

	if (m_fMaxframe < m_fFrame)
	{
		m_fFrame = 0.f;
	}

}


CDocheolBullet_ChargeBullet* CDocheolBullet_ChargeBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vDir, const _vec3& vScale,  const _float& fSpeed , const _bool& bMove , TYPE eType)
{
	CDocheolBullet_ChargeBullet* pInstance = new CDocheolBullet_ChargeBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos ,vDir , vScale,fSpeed , bMove , eType)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDocheolBullet_ChargeBullet::Create_EffectAndBullet()
{
	CGameObject* pGameObject = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vScale = { 25.f , 25.f , 25.f };
	pGameObject = CEffect_Docheol_FirePulse::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], vScale);
	pLayer->Add_BulletObject(pGameObject);

	float fAngle = acosf(m_vDir.x);

	if (m_vDir.z > 0)
		fAngle = 2* D3DX_PI - fAngle;

	fAngle = D3DXToDegree(fAngle);

	vScale = { 8.f , 8.f , 8.f };

	pGameObject = CDocheolBullet_RotationBullet::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], fAngle, vScale, 40.f, 3.f);
	pLayer->Add_BulletObject(pGameObject);
	pGameObject = CDocheolBullet_RotationBullet::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], fAngle + 120.f, vScale, 40.f,3.f);
	pLayer->Add_BulletObject(pGameObject);
	pGameObject = CDocheolBullet_RotationBullet::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], fAngle + 240.f,vScale , 40.f , 3.f);
	pLayer->Add_BulletObject(pGameObject);

}

void CDocheolBullet_ChargeBullet::Check_Boss_Dead()
{
	CDocheol* pDocheol = dynamic_cast<CDocheol*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Docheol"));

	if (pDocheol->Get_State() == CDocheol::BOSS_DEAD)
		m_bDead = true;
}

void CDocheolBullet_ChargeBullet::Free(void)
{
	__super::Free();
}
