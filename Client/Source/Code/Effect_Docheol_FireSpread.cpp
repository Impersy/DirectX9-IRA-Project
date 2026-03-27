#include "stdafx.h"
#include "..\Header\Effect_Docheol_FireSpread.h"
#include "Export_Function.h"

CEffect_Docheol_FireSpread::CEffect_Docheol_FireSpread(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_FireSpread::CEffect_Docheol_FireSpread(const CEffect_Docheol_FireSpread & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_FireSpread::~CEffect_Docheol_FireSpread()
{
	Free();
}

HRESULT CEffect_Docheol_FireSpread::Ready_GameObject(const _vec3& vPos , const _float& fDelayTime)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 70.f , 70.f, 70.f };

	m_fMaxFrame = 16.f;

	m_fDelayTime = fDelayTime;

	m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x , 40.f , 135.f };
	
	return S_OK;
}

_int CEffect_Docheol_FireSpread::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAccTime += fTimeDelta;

	__super::Update_GameObject(fTimeDelta);

	if(m_fAccTime > m_fDelayTime)
		m_fFrame += m_fMaxFrame * fTimeDelta * 0.6f;

	return 0;
}

void CEffect_Docheol_FireSpread::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}

	
}

void CEffect_Docheol_FireSpread::Render_GameObject()
{
	if (m_fAccTime < m_fDelayTime)
		return;

	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_FireSpread::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_FireSpread"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_FireSpread", pComponent });

	return S_OK;
}

CEffect_Docheol_FireSpread* CEffect_Docheol_FireSpread::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _float& fDelayTime)
{
	CEffect_Docheol_FireSpread* pInstance = new CEffect_Docheol_FireSpread(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , fDelayTime)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_FireSpread::Free(void)
{
	__super::Free();
}


