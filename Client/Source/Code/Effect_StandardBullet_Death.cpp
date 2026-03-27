#include "stdafx.h"
#include "..\Header\Effect_StandardBullet_Death.h"
#include "Export_Function.h"

CEffect_StandardBullet_Death::CEffect_StandardBullet_Death(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_StandardBullet_Death::CEffect_StandardBullet_Death(const CEffect_StandardBullet_Death & rhs)
	: CEffect(rhs)
{
}

CEffect_StandardBullet_Death::~CEffect_StandardBullet_Death()
{
	Free();
}

HRESULT CEffect_StandardBullet_Death::Ready_GameObject(const _vec3& vPos)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 3.f , 3.f, 3.f };
	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_fMaxFrame = 13.f;

	return S_OK;
}

_int CEffect_StandardBullet_Death::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	m_fFrame += m_fMaxFrame * fTimeDelta;

	return 0;
}

void CEffect_StandardBullet_Death::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_StandardBullet_Death::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_StandardBullet_Death::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_StandardBullet_Death"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_StandardBullet_Death", pComponent });

	return S_OK;
}

CEffect_StandardBullet_Death* CEffect_StandardBullet_Death::Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos)
{
	CEffect_StandardBullet_Death* pInstance = new CEffect_StandardBullet_Death(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_StandardBullet_Death::Free(void)
{
	__super::Free();
}


