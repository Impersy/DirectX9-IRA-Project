#include "stdafx.h"
#include "..\Header\Effect_Docheol_FireExplosion.h"
#include "Export_Function.h"

CEffect_Docheol_FireExplosion::CEffect_Docheol_FireExplosion(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_FireExplosion::CEffect_Docheol_FireExplosion(const CEffect_Docheol_FireExplosion & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_FireExplosion::~CEffect_Docheol_FireExplosion()
{
	Free();
}

HRESULT CEffect_Docheol_FireExplosion::Ready_GameObject(const _vec3& vPos , TYPE _e)
{
	Add_Component();

	m_eType = _e;
	m_fMaxFrame = 27.f;

	switch (m_eType)
	{
	case CEffect_Docheol_FireExplosion::BOSS:
		m_pTransformCom->m_vScale = { 80.f , 80.f, 10.f };
		m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x , 40.f , 130.f };
		break;
	case CEffect_Docheol_FireExplosion::BULLET:
		m_pTransformCom->m_vScale = { 8.f , 8.f, 8.f };
		m_pTransformCom->m_vInfo[INFO_POS] = vPos;
		break;
	default:
		break;
	}

	return S_OK;
}

_int CEffect_Docheol_FireExplosion::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	switch (m_eType)
	{
	case CEffect_Docheol_FireExplosion::BOSS:
		m_fFrame += m_fMaxFrame * fTimeDelta * 0.5f;
		break;
	case CEffect_Docheol_FireExplosion::BULLET:
		m_fFrame += m_fMaxFrame * fTimeDelta * 0.6f;
		break;
	default:
		break;
	}
	

	return 0;
}

void CEffect_Docheol_FireExplosion::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}
}

void CEffect_Docheol_FireExplosion::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_FireExplosion::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_FireExplosion"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_FireExplosion", pComponent });

	return S_OK;
}

CEffect_Docheol_FireExplosion* CEffect_Docheol_FireExplosion::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , TYPE _e)
{
	CEffect_Docheol_FireExplosion* pInstance = new CEffect_Docheol_FireExplosion(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , _e)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_FireExplosion::Free(void)
{
	__super::Free();
}


