#include "stdafx.h"
#include "..\Header\Effect_Birth_LandMark.h"
#include "Export_Function.h"

CEffect_Birth_LandMark::CEffect_Birth_LandMark(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Birth_LandMark::CEffect_Birth_LandMark(const CEffect_Birth_LandMark & rhs)
	: CEffect(rhs)
{
}

CEffect_Birth_LandMark::~CEffect_Birth_LandMark()
{
	Free();
}

HRESULT CEffect_Birth_LandMark::Ready_GameObject(CGameObject* pOwner)
{
	Add_Component();

	m_pOwner = pOwner;

	m_pTransformCom->m_vScale = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vScale * 1.5f;
	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y = 0.2f;
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	m_fMaxFrame = 41.f;

	return S_OK;
}

_int CEffect_Birth_LandMark::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pOwner->Get_Dead())
		return OBJ_DEAD;

	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	//_vec3 vPos = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	//_vec3 vDir = vPos - m_pTransformCom->m_vInfo[INFO_POS];
	//if( 3.f < D3DXVec3Length(&vDir))
	//	m_pTransformCom->Move_Pos(&vPos, 50.f, fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fFrame += m_fMaxFrame * fTimeDelta * .9f;
	m_fAccumulatedTime += fTimeDelta;

	if (1.f < m_fAccumulatedTime)
	{
		m_fAccumulatedTime = 0.f;
		m_bDead = true;
	}

	return 0;
}

void CEffect_Birth_LandMark::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0.f;
	}
}

void CEffect_Birth_LandMark::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Birth_LandMark::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Birth_LandMark"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Birth_LandMark", pComponent });

	return S_OK;
}

CEffect_Birth_LandMark* CEffect_Birth_LandMark::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner)
{
	CEffect_Birth_LandMark* pInstance = new CEffect_Birth_LandMark(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Birth_LandMark::Free(void)
{
	__super::Free();
}


