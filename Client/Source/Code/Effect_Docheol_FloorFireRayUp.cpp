#include "stdafx.h"
#include "..\Header\Effect_Docheol_FloorFireRayUp.h"
#include "Export_Function.h"

CEffect_Docheol_FloorFireRayUp::CEffect_Docheol_FloorFireRayUp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_FloorFireRayUp::CEffect_Docheol_FloorFireRayUp(const CEffect_Docheol_FloorFireRayUp & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_FloorFireRayUp::~CEffect_Docheol_FloorFireRayUp()
{
	Free();
}

HRESULT CEffect_Docheol_FloorFireRayUp::Ready_GameObject(const _vec3& vPos , const _vec3& vScale)
{
	Add_Component();

	m_pTransformCom->m_vScale = vScale;

	m_fMaxFrame = 19.f;

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;
	return S_OK;
}

_int CEffect_Docheol_FloorFireRayUp::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	m_fFrame += m_fMaxFrame * fTimeDelta * 0.8f;

	return 0;
}

void CEffect_Docheol_FloorFireRayUp::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}
}

void CEffect_Docheol_FloorFireRayUp::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_FloorFireRayUp::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));

	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_FloorFireRayUp"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_FloorFireRayUp", pComponent });

	return S_OK;
}

CEffect_Docheol_FloorFireRayUp* CEffect_Docheol_FloorFireRayUp::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale)
{
	CEffect_Docheol_FloorFireRayUp* pInstance = new CEffect_Docheol_FloorFireRayUp(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_FloorFireRayUp::Free(void)
{
	__super::Free();
}


