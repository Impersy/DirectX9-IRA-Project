#include "stdafx.h"
#include "..\Header\Effect_Bullet_Death.h"
#include "Export_Function.h"
#include "Bullet.h"

CEffect_Bullet_Death::CEffect_Bullet_Death(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Bullet_Death::CEffect_Bullet_Death(const CEffect_Bullet_Death & rhs)
	: CEffect(rhs)
{
}

CEffect_Bullet_Death::~CEffect_Bullet_Death()
{
	Free();
}

HRESULT CEffect_Bullet_Death::Ready_GameObject(CGameObject* pOwner, const _float& fTimeDelta)
{
	Add_Component();

	m_pOwner = pOwner;
	m_pTransformCom->m_vScale = dynamic_cast<CBullet*>(m_pOwner)->Get_Transform()->m_vScale * 1.2f;

	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CBullet*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS] += *(dynamic_cast<CBullet*>(m_pOwner)->Get_Direction()) * dynamic_cast<CBullet*>(m_pOwner)->Get_Speed() * 3.f * fTimeDelta;
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(75.f));

	m_fMaxFrame = 11.f;

	return S_OK;
}

_int CEffect_Bullet_Death::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fFrame += m_fMaxFrame * fTimeDelta * 2.5f;

	return 0;
}

void CEffect_Bullet_Death::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Bullet_Death::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Bullet_Death::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Bullet_Death_Red"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Bullet_Death_Red", pComponent });

	return S_OK;
}

CEffect_Bullet_Death* CEffect_Bullet_Death::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _float& fTimeDelta)
{
	CEffect_Bullet_Death* pInstance = new CEffect_Bullet_Death(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner, fTimeDelta)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Bullet_Death::Free(void)
{
	__super::Free();
}


