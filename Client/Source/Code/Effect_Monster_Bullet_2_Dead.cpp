#include "stdafx.h"
#include "..\Header\Effect_Monster_Bullet_2_Dead.h"
#include "Export_Function.h"

CEffect_Monster_Bullet_2_Dead::CEffect_Monster_Bullet_2_Dead(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Monster_Bullet_2_Dead::CEffect_Monster_Bullet_2_Dead(const CEffect_Monster_Bullet_2_Dead& rhs)
	: CEffect(rhs)
{
}

CEffect_Monster_Bullet_2_Dead::~CEffect_Monster_Bullet_2_Dead()
{
	Free();
}

HRESULT CEffect_Monster_Bullet_2_Dead::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;
	m_pTransformCom->m_vInfo[INFO_POS].y = m_vPos.y;

	m_pTransformCom->m_vScale = { 4.f , 4.f, 1.f };
	m_fMaxFrame = 7.f;

	return S_OK;
}

_int CEffect_Monster_Bullet_2_Dead::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{	
		return OBJ_DEAD;
	}

	m_fFrame += m_fMaxFrame * fTimeDelta;

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	__super::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;
}

void CEffect_Monster_Bullet_2_Dead::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Monster_Bullet_2_Dead::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Monster_Bullet_2_Dead::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Bullet_2_Dead"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Bullet_2_Dead", pComponent });

	return S_OK;
}

CEffect_Monster_Bullet_2_Dead* CEffect_Monster_Bullet_2_Dead::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CEffect_Monster_Bullet_2_Dead* pInstance = new CEffect_Monster_Bullet_2_Dead(pGraphicDev);
	pInstance->m_vPos = vPos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Monster_Bullet_2_Dead::Free(void)
{
	__super::Free();
}

