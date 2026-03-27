#include "stdafx.h"
#include "..\Header\Effect_Monster_Dead_2.h"
#include "Export_Function.h"

CEffect_Monster_Dead_2::CEffect_Monster_Dead_2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Monster_Dead_2::CEffect_Monster_Dead_2(const CEffect_Monster_Dead_2& rhs)
	: CEffect(rhs)
{
}

CEffect_Monster_Dead_2::~CEffect_Monster_Dead_2()
{
	Free();
}

HRESULT CEffect_Monster_Dead_2::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;
	m_pTransformCom->m_vInfo[INFO_POS].y = m_vPos.x - 5.f;
	m_pTransformCom->m_vInfo[INFO_POS].y = m_vPos.y + 1.f;

	m_pTransformCom->m_vScale = { 7.f , 7.f, 1.f };

	m_fMaxFrame = 23.f;

	return S_OK;
}

_int CEffect_Monster_Dead_2::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += m_fMaxFrame * fTimeDelta;

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	__super::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;
}

void CEffect_Monster_Dead_2::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Monster_Dead_2::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Monster_Dead_2::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Monster_Dead_B"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Monster_Dead_B", pComponent });

	return S_OK;
}

CEffect_Monster_Dead_2* CEffect_Monster_Dead_2::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CEffect_Monster_Dead_2* pInstance = new CEffect_Monster_Dead_2(pGraphicDev);
	pInstance->m_vPos = vPos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Monster_Dead_2::Free(void)
{
	__super::Free();
}


