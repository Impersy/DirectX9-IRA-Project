#include "stdafx.h"
#include "..\Header\Time_Hit.h"
#include "Export_Function.h"

CTime_Hit::CTime_Hit(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CTime_Hit::CTime_Hit(const CTime_Hit & rhs)
	: CEffect(rhs)
{
}

CTime_Hit::~CTime_Hit()
{
	Free();
}

HRESULT CTime_Hit::Ready_GameObject(void)
{
	Add_Component();

	if (m_Is_Charge == false)
		m_pTransformCom->m_vScale = { 1.5f * 5.f * PUBLIC_SCALE , 1.5f * 5.f * PUBLIC_SCALE, 1.f };
	else
		m_pTransformCom->m_vScale = { 3.f * 5.f * PUBLIC_SCALE , 3.f * 5.f * PUBLIC_SCALE, 1.f };

	m_fMaxFrame = 12.f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;

	return S_OK;
}

_int CTime_Hit::Update_GameObject(const _float& fTimeDelta)
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

void CTime_Hit::LateUpdate_GameObject()
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

void CTime_Hit::Render_GameObject()
{

	__super::Render_GameObject();
}

HRESULT CTime_Hit::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Time_Hit"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Time_Hit", pComponent });

	return S_OK;
}

CTime_Hit* CTime_Hit::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CTime_Hit* pInstance = new CTime_Hit(pGraphicDev);

	pInstance->m_vPos = vPos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTime_Hit::Free(void)
{
	__super::Free();
}


