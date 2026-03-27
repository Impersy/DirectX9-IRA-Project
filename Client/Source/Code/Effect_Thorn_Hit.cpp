#include "stdafx.h"
#include "..\Header\Effect_Thorn_Hit.h"
#include "Export_Function.h"

CEffect_Thorn_Hit::CEffect_Thorn_Hit(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Thorn_Hit::CEffect_Thorn_Hit(const CEffect_Thorn_Hit & rhs)
	: CEffect(rhs)
{
}

CEffect_Thorn_Hit::~CEffect_Thorn_Hit()
{
	Free();
}

HRESULT CEffect_Thorn_Hit::Ready_GameObject(const _vec3& vPos)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 10.f , 10.f, 10.f };
	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_fMaxFrame = 5.f;

	return S_OK;
}

_int CEffect_Thorn_Hit::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fFrame += m_fMaxFrame * fTimeDelta * 2.f;

	return OBJ_NOEVENT;
}

void CEffect_Thorn_Hit::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Thorn_Hit::Render_GameObject()
{
	float f = m_fFrame;
	__super::Render_GameObject();
}

HRESULT CEffect_Thorn_Hit::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Thorn_Hit"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Thorn_Hit", pComponent });

	return S_OK;
}

CEffect_Thorn_Hit* CEffect_Thorn_Hit::Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos)
{
	CEffect_Thorn_Hit* pInstance = new CEffect_Thorn_Hit(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Thorn_Hit::Free(void)
{
	__super::Free();
}


