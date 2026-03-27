#include "stdafx.h"
#include "..\Header\Effect_Summon_Wave.h"
#include "Export_Function.h"

CEffect_Summon_Wave::CEffect_Summon_Wave(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Summon_Wave::CEffect_Summon_Wave(const CEffect_Summon_Wave & rhs)
	: CEffect(rhs)
{
}

CEffect_Summon_Wave::~CEffect_Summon_Wave()
{
	Free();
}

HRESULT CEffect_Summon_Wave::Ready_GameObject(CGameObject* pOwner)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pOwner = pOwner;

	m_pTransformCom->m_vScale = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vScale * 1.3f;
	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y = 0.2f;
	m_pTransformCom->m_vInfo[INFO_POS].z -= 1.f;
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	m_fMaxFrame = 20.f;

	return S_OK;
}

_int CEffect_Summon_Wave::Update_GameObject(const _float& fTimeDelta)
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

	m_fFrame += m_fMaxFrame * fTimeDelta * 1.1f;
	m_fAccumulatedTime += fTimeDelta;

	if (0.8f < m_fAccumulatedTime)
	{
		m_fAccumulatedTime = 0.f;
		m_bDead = true;
	}

	return 0;
}

void CEffect_Summon_Wave::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0.f;
	}

	__super::Compute_ViewZ(&m_pTransformCom->m_vInfo[INFO_POS]);
}

void CEffect_Summon_Wave::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Summon_Wave::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Summon_Wave"));
	if (nullptr == m_pTextureCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Summon_Wave", pComponent });

	return S_OK;
}

CEffect_Summon_Wave* CEffect_Summon_Wave::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner)
{
	CEffect_Summon_Wave* pInstance = new CEffect_Summon_Wave(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Summon_Wave::Free(void)
{
	__super::Free();
}


