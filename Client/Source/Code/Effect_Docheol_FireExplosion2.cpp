#include "stdafx.h"
#include "..\Header\Effect_Docheol_FireExplosion2.h"
#include "Export_Function.h"

CEffect_Docheol_FireExplosion2::CEffect_Docheol_FireExplosion2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_FireExplosion2::CEffect_Docheol_FireExplosion2(const CEffect_Docheol_FireExplosion2 & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_FireExplosion2::~CEffect_Docheol_FireExplosion2()
{
	Free();
}

HRESULT CEffect_Docheol_FireExplosion2::Ready_GameObject(const _vec3& vPos , const _vec3& vScale)
{
	Add_Component();

	m_fMaxFrame = 21.f;
	m_pTransformCom->m_vScale = vScale;
	m_pTransformCom->m_vInfo[INFO_POS] = vPos;
	
	return S_OK;
}

_int CEffect_Docheol_FireExplosion2::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += m_fMaxFrame * fTimeDelta;

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CEffect_Docheol_FireExplosion2::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}
}

void CEffect_Docheol_FireExplosion2::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_FireExplosion2::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));

	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_FireExplosion2"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_FireExplosion2", pComponent });

	return S_OK;
}

CEffect_Docheol_FireExplosion2* CEffect_Docheol_FireExplosion2::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale)
{
	CEffect_Docheol_FireExplosion2* pInstance = new CEffect_Docheol_FireExplosion2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_FireExplosion2::Free(void)
{
	__super::Free();
}


