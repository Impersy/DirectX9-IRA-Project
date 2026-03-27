#include "stdafx.h"
#include "..\Header\Effect_Named_Monster_Dead.h"
#include "Export_Function.h"

CEffect_Named_Monster_Dead::CEffect_Named_Monster_Dead(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Named_Monster_Dead::CEffect_Named_Monster_Dead(const CEffect_Named_Monster_Dead & rhs)
	: CEffect(rhs)
{
}

CEffect_Named_Monster_Dead::~CEffect_Named_Monster_Dead()
{
	Free();
}

HRESULT CEffect_Named_Monster_Dead::Ready_GameObject(CGameObject* pOwner)
{
	Add_Component();

	m_pOwner = pOwner;

	m_pTransformCom->m_vScale = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vScale * 1.3f;
	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y -= 4.f;
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(75.f));

	m_fMaxFrame = 14.f;

	return S_OK;
}

_int CEffect_Named_Monster_Dead::Update_GameObject(const _float& fTimeDelta)
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

void CEffect_Named_Monster_Dead::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Named_Monster_Dead::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Named_Monster_Dead::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_BleedingDeath"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_BleedingDeath", pComponent });

	return S_OK;
}

CEffect_Named_Monster_Dead* CEffect_Named_Monster_Dead::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner)
{
	CEffect_Named_Monster_Dead* pInstance = new CEffect_Named_Monster_Dead(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Named_Monster_Dead::Free(void)
{
	__super::Free();
}


