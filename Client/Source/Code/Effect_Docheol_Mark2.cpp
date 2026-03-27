#include "stdafx.h"
#include "..\Header\Effect_Docheol_Mark2.h"
#include "Export_Function.h"

CEffect_Docheol_Mark2::CEffect_Docheol_Mark2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_Mark2::CEffect_Docheol_Mark2(const CEffect_Docheol_Mark2 & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_Mark2::~CEffect_Docheol_Mark2()
{
	Free();
}

HRESULT CEffect_Docheol_Mark2::Ready_GameObject(const _vec3& vPos , const _vec3& vScale, const _float& fAliveTime)
{
	Add_Component();

	m_pTransformCom->m_vScale = vScale;

	m_fAliveTime = fAliveTime;

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_fMaxFrame = 9.f;

	return S_OK;
}

_int CEffect_Docheol_Mark2::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);
	
	m_fFrame += m_fMaxFrame * fTimeDelta * m_fFrameSpeed;

	if (m_fFrame >= m_fMaxFrame)
	{
		m_fAccTime += fTimeDelta;

		m_fFrame = m_fMaxFrame;
	}

	if (!m_bFrameReturn)
	{
		if (m_fAccTime > 3.f)
		{
			m_fFrameSpeed *= -1.f;
			m_bFrameReturn = true;
		}
	}
	
	if (m_fFrame < 0.f)
	{
		m_fFrame = 0.f;
		m_bDead = true;
	}

	return 0;
}

void CEffect_Docheol_Mark2::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fAccTime > m_fAliveTime)
		m_bDead = true;
}

void CEffect_Docheol_Mark2::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_Mark2::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_Mark2"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_Mark2", pComponent });

	return S_OK;
}

CEffect_Docheol_Mark2* CEffect_Docheol_Mark2::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos,const _vec3& vScale , const _float& fAliveTime)
{
	CEffect_Docheol_Mark2* pInstance = new CEffect_Docheol_Mark2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale,fAliveTime)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_Mark2::Free(void)
{
	__super::Free();
}


