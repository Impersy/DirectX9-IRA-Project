#include "stdafx.h"
#include "..\Header\Effect_Leaf_Bullet_BackHit.h"
#include "Export_Function.h"

CEffect_Leaf_Bullet_BackHit::CEffect_Leaf_Bullet_BackHit(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Leaf_Bullet_BackHit::CEffect_Leaf_Bullet_BackHit(const CEffect_Leaf_Bullet_BackHit & rhs)
	: CEffect(rhs)
{
}

CEffect_Leaf_Bullet_BackHit::~CEffect_Leaf_Bullet_BackHit()
{
	Free();
}

HRESULT CEffect_Leaf_Bullet_BackHit::Ready_GameObject(CGameObject* pOwner, _float fTimeDelta)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pOwner = pOwner;

	m_pTransformCom->m_vScale = dynamic_cast<CBullet*>(m_pOwner)->Get_Transform()->m_vScale * 2.f;

	_vec3 vDir = *(dynamic_cast<CBullet*>(m_pOwner)->Get_Direction());

	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CBullet*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS] += vDir * dynamic_cast<CBullet*>(m_pOwner)->Get_Speed() * 8.f * fTimeDelta;
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(75.f));

	D3DXVec3Normalize(&vDir, &vDir);

	_matrix matRot;
	D3DXMatrixIdentity(&matRot);

	_float fDot = D3DXVec3Dot(&m_pTransformCom->m_vInfo[INFO_RIGHT], &vDir);

	_float fAngle = acosf(fDot);

	if (vDir.z > 0.f)
		fAngle = 2.f * D3DX_PI - fAngle;

	m_pTransformCom->Rotation(ROT_Y, fAngle);

	m_fMaxFrame = 11.f;

	return S_OK;
}

_int CEffect_Leaf_Bullet_BackHit::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fFrame += m_fMaxFrame * fTimeDelta * 1.f;

	return 0;
}

void CEffect_Leaf_Bullet_BackHit::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Leaf_Bullet_BackHit::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Leaf_Bullet_BackHit::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_LeafBulletBackHit"));
	if (nullptr == m_pTextureCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_LeafBulletBackHit", pComponent });

	return S_OK;
}

CEffect_Leaf_Bullet_BackHit* CEffect_Leaf_Bullet_BackHit::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, _float fTimeDelta)
{
	CEffect_Leaf_Bullet_BackHit* pInstance = new CEffect_Leaf_Bullet_BackHit(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner, fTimeDelta)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Leaf_Bullet_BackHit::Free(void)
{
	__super::Free();
}


