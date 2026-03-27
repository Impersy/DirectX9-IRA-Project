#include "stdafx.h"
#include "..\Header\Effect_Docheol_MeteorExplosion.h"
#include "Export_Function.h"

CEffect_Docheol_MeteorExplosion::CEffect_Docheol_MeteorExplosion(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_MeteorExplosion::CEffect_Docheol_MeteorExplosion(const CEffect_Docheol_MeteorExplosion & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_MeteorExplosion::~CEffect_Docheol_MeteorExplosion()
{
	Free();
}

HRESULT CEffect_Docheol_MeteorExplosion::Ready_GameObject(const _vec3& vPos , const _vec3& vScale)
{
	Add_Component();

	m_pTransformCom->m_vScale = vScale;

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_fMaxFrame = 26.f;
	
	return S_OK;
}

_int CEffect_Docheol_MeteorExplosion::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	m_fFrame += m_fMaxFrame * fTimeDelta * 1.f;

	
	return 0;
}

void CEffect_Docheol_MeteorExplosion::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}
}

void CEffect_Docheol_MeteorExplosion::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_MeteorExplosion::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_MeteorExplosion"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_MeteorExplosion", pComponent });

	return S_OK;
}

CEffect_Docheol_MeteorExplosion* CEffect_Docheol_MeteorExplosion::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale)
{
	CEffect_Docheol_MeteorExplosion* pInstance = new CEffect_Docheol_MeteorExplosion(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_MeteorExplosion::Free(void)
{
	__super::Free();
}


