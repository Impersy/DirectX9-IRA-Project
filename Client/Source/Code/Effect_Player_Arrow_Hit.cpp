#include "stdafx.h"
#include "..\Header\Effect_Player_Arrow_Hit.h"
#include "Export_Function.h"

CEffect_Player_Arrow_Hit::CEffect_Player_Arrow_Hit(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Player_Arrow_Hit::CEffect_Player_Arrow_Hit(const CEffect_Player_Arrow_Hit & rhs)
	: CEffect(rhs)
{
}

CEffect_Player_Arrow_Hit::~CEffect_Player_Arrow_Hit()
{
	Free();
}

HRESULT CEffect_Player_Arrow_Hit::Ready_GameObject(void)
{
	Add_Component();

	if (m_Arrow_Type == ARROW_TYPE_SYLPH)
		m_pTransformCom->m_vScale = { 1.9f * 2.f * PUBLIC_SCALE , 1.4f * 2.f * PUBLIC_SCALE, 1.f };
	else if (m_Arrow_Type == ARROW_TYPE_CHARGE_SYLPH)
		m_pTransformCom->m_vScale = { 1.9f * 5.f * PUBLIC_SCALE , 1.4f * 5.f * PUBLIC_SCALE, 1.f };

	m_fMaxFrame = 11.f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;

	return S_OK;
}

_int CEffect_Player_Arrow_Hit::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += m_fMaxFrame * Time * 2.0f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CEffect_Player_Arrow_Hit::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CEffect_Player_Arrow_Hit::Render_GameObject()
{

	__super::Render_GameObject();
}

HRESULT CEffect_Player_Arrow_Hit::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Effect_ArrowHit_Sylph"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Effect_ArrowHit_Sylph", pComponent });

	return S_OK;
}

CEffect_Player_Arrow_Hit* CEffect_Player_Arrow_Hit::Create(LPDIRECT3DDEVICE9 pGraphicDev, ARROW_TYPE ArrowType, _vec3 vPos)
{
	CEffect_Player_Arrow_Hit* pInstance = new CEffect_Player_Arrow_Hit(pGraphicDev);

	pInstance->m_Arrow_Type = ArrowType;
	pInstance->m_vPos = vPos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Player_Arrow_Hit::Free(void)
{
	__super::Free();
}


