#include "stdafx.h"
#include "..\Header\Effect_DoubleLeaf_Bullet_Dead.h"
#include "Export_Function.h"
#include "Bullet_GreenEvilBat_DoubleLeaf.h"

CEffect_DoubleLeaf_Bullet_Dead::CEffect_DoubleLeaf_Bullet_Dead(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_DoubleLeaf_Bullet_Dead::CEffect_DoubleLeaf_Bullet_Dead(const CEffect_DoubleLeaf_Bullet_Dead & rhs)
	: CEffect(rhs)
{
}

CEffect_DoubleLeaf_Bullet_Dead::~CEffect_DoubleLeaf_Bullet_Dead()
{
	Free();
}

HRESULT CEffect_DoubleLeaf_Bullet_Dead::Ready_GameObject(CGameObject* pOwner, _float fTimeDelta)
{
	Add_Component();

	m_pOwner = pOwner;

	m_pTransformCom->m_vScale = dynamic_cast<CBullet*>(m_pOwner)->Get_Transform()->m_vScale * 1.3f;

	//CTransform* pPlayerTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic",))

	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CBullet*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS] += *(dynamic_cast<CBullet*>(m_pOwner)->Get_Direction()) * dynamic_cast<CBullet*>(m_pOwner)->Get_Speed() * 3.f * fTimeDelta;
	//m_pTransformCom->m_vInfo[INFO_POS].y += 3.f;
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(75.f));

	m_fMaxFrame = 4.f;

	return S_OK;
}

_int CEffect_DoubleLeaf_Bullet_Dead::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fFrame += m_fMaxFrame * fTimeDelta * 2.f;
	//m_fAccumulatedTime += fTimeDelta;

	//if (1.2f < m_fAccumulatedTime)
	//{
	//	m_fAccumulatedTime = 0.f;
	//	m_bDead = true;
	//}

	return 0;
}

void CEffect_DoubleLeaf_Bullet_Dead::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_DoubleLeaf_Bullet_Dead::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_DoubleLeaf_Bullet_Dead::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_CircleBullet_Death"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_CircleBullet_Death", pComponent });

	return S_OK;
}

CEffect_DoubleLeaf_Bullet_Dead* CEffect_DoubleLeaf_Bullet_Dead::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, _float fTimeDelta)
{
	CEffect_DoubleLeaf_Bullet_Dead* pInstance = new CEffect_DoubleLeaf_Bullet_Dead(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner, fTimeDelta)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_DoubleLeaf_Bullet_Dead::Free(void)
{
	__super::Free();
}


