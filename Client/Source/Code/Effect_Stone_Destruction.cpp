#include "stdafx.h"
#include "..\Header\Effect_Stone_Destruction.h"
#include "Export_Function.h"

CEffect_Stone_Destruction::CEffect_Stone_Destruction(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Stone_Destruction::CEffect_Stone_Destruction(const CEffect_Stone_Destruction & rhs)
	: CEffect(rhs)
{
}

CEffect_Stone_Destruction::~CEffect_Stone_Destruction()
{
	Free();
}

HRESULT CEffect_Stone_Destruction::Ready_GameObject(const _vec3& vPos, const _vec3& vScale)
{
	Add_Component();

	m_pTransformCom->m_vScale = vScale;
	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_fMaxFrame = 7.f;

	return S_OK;
}

_int CEffect_Stone_Destruction::Update_GameObject(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	if(m_fAccTime > 0.2f)
		m_fFrame += m_fMaxFrame * fTimeDelta * 2.f;

	return 0;
}

void CEffect_Stone_Destruction::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Stone_Destruction::Render_GameObject()
{
	if (m_fAccTime > 0.2f)
	__super::Render_GameObject();
}

HRESULT CEffect_Stone_Destruction::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Stone_Destruction"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Stone_Destruction", pComponent });

	return S_OK;
}

CEffect_Stone_Destruction* CEffect_Stone_Destruction::Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos, const _vec3& vScale)
{
	CEffect_Stone_Destruction* pInstance = new CEffect_Stone_Destruction(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, vScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Stone_Destruction::Free(void)
{
	__super::Free();
}

