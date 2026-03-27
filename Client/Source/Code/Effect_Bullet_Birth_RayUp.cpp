#include "stdafx.h"
#include "..\Header\Effect_Bullet_Birth_RayUp.h"
#include "Export_Function.h"
#include "Bullet.h"

CEffect_Bullet_Birth_RayUp::CEffect_Bullet_Birth_RayUp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Bullet_Birth_RayUp::CEffect_Bullet_Birth_RayUp(const CEffect_Bullet_Birth_RayUp & rhs)
	: CEffect(rhs)
{
}

CEffect_Bullet_Birth_RayUp::~CEffect_Bullet_Birth_RayUp()
{
	Free();
}

HRESULT CEffect_Bullet_Birth_RayUp::Ready_GameObject(CGameObject* pOwner, const _float& fTimeDelta)
{
	Add_Component();

	m_pOwner = pOwner;

	m_pTransformCom->m_vScale = dynamic_cast<CBullet*>(m_pOwner)->Get_Transform()->m_vScale * 2.f;
	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CBullet*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y += 2.2f;

	_vec3 vDir = *(dynamic_cast<CBullet*>(m_pOwner)->Get_Direction());

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	D3DXVec3Normalize(&vDir, &vDir);

	_float fDot = D3DXVec3Dot(&m_pTransformCom->m_vInfo[INFO_RIGHT], &vDir);

	_float fAngle = acosf(fDot);

	if (vDir.z > 0.f)
		fAngle = 2.f * D3DX_PI - fAngle;

	m_pTransformCom->Rotation(ROT_Y, fAngle);

	m_pTransformCom->m_vInfo[INFO_POS] += vDir * fTimeDelta * 100.f;

	m_fMaxFrame = 10.f;

	return S_OK;
}

_int CEffect_Bullet_Birth_RayUp::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	m_fFrame += m_fMaxFrame * fTimeDelta * 2.f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CEffect_Bullet_Birth_RayUp::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Bullet_Birth_RayUp::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Bullet_Birth_RayUp::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Birth_RayUp_Green"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Birth_RayUp_Green", pComponent });

	return S_OK;
}

CEffect_Bullet_Birth_RayUp* CEffect_Bullet_Birth_RayUp::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _float& fTimeDelta)
{
	CEffect_Bullet_Birth_RayUp* pInstance = new CEffect_Bullet_Birth_RayUp(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner, fTimeDelta)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Bullet_Birth_RayUp::Free(void)
{
	__super::Free();
}


