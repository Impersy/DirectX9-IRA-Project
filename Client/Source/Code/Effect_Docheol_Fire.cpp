#include "stdafx.h"
#include "..\Header\Effect_Docheol_Fire.h"
#include "Export_Function.h"

CEffect_Docheol_Fire::CEffect_Docheol_Fire(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_Fire::CEffect_Docheol_Fire(const CEffect_Docheol_Fire & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_Fire::~CEffect_Docheol_Fire()
{
	Free();
}

HRESULT CEffect_Docheol_Fire::Ready_GameObject(const _vec3& vPos , const _vec3& vScale, TYPE _eType , const _float& fDelayTime)
{
	Add_Component();

	m_pTransformCom->m_vScale = vScale;

	m_fDelayTime = fDelayTime;

	m_eType = _eType;
	switch (m_eType)
	{
	case CEffect_Docheol_Fire::INTRO:
		m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x , 30.f , 128.f };
		break;
	case CEffect_Docheol_Fire::SLAM:
		m_pTransformCom->m_vInfo[INFO_POS] = vPos;
		break;
	case CEffect_Docheol_Fire::RAGE_UP:
		m_pTransformCom->m_vInfo[INFO_POS] = vPos;
		break;
	case CEffect_Docheol_Fire::SPECIAL:
		m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x , 30.f , 178.f };
		break;
	default:
		break;
	}
	
	m_fMaxFrame = 15.f;
	
	return S_OK;
}

_int CEffect_Docheol_Fire::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	m_fAccTime += fTimeDelta;


	if (m_fAccTime > m_fDelayTime)
	{
		switch (m_eType)
		{
		case CEffect_Docheol_Fire::INTRO:
			m_fFrame += m_fMaxFrame * fTimeDelta * 0.8f;
			break;
		case CEffect_Docheol_Fire::SLAM:
			m_fFrame += m_fMaxFrame * fTimeDelta * 1.f;
			break;
		case CEffect_Docheol_Fire::RAGE_UP:
			m_fFrame += m_fMaxFrame * fTimeDelta * 1.f;
			break;
		case CEffect_Docheol_Fire::SPECIAL:
			m_fFrame += m_fMaxFrame * fTimeDelta * 0.8f;
			break;
		default:
			break;
		}
	}
	

	return 0;
}

void CEffect_Docheol_Fire::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}
}

void CEffect_Docheol_Fire::Render_GameObject()
{
	if (m_fAccTime < m_fDelayTime)
		return;

	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_Fire::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_Fire"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_Fire", pComponent });

	return S_OK;
}

CEffect_Docheol_Fire* CEffect_Docheol_Fire::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale, TYPE _eType , const _float& fDelayTime)
{
	CEffect_Docheol_Fire* pInstance = new CEffect_Docheol_Fire(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale , _eType , fDelayTime)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_Fire::Free(void)
{
	__super::Free();
}


