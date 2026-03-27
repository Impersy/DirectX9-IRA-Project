#include "stdafx.h"
#include "..\Header\Ghost.h"
#include "Export_Function.h"
#include "GhostChild.h"
#include "Ghost_TimeStop.h"

CGhost::CGhost(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pCalculatorCom(nullptr)
{
}

CGhost::CGhost(const CGhost& rhs)
	: Engine::CGameObject(rhs)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pCalculatorCom(rhs.m_pCalculatorCom)
{
}

CGhost::~CGhost()
{
	Free();
}

HRESULT CGhost::Ready_GameObject(void)
{
	Add_Component();

	

	return S_OK;
}

_int CGhost::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (Is_Dash == true) {
		m_interver += 7.f * Time;
		if (m_interver > 1.4f) {
			Show_Ghost();
			m_interver = 0.f;
		}
	}



	if (Is_Run == true) {
		m_interver += 8.4f * Time;
		if (m_interver > 1.4f) {
			Show_Ghost_TimeStop();
			m_interver = 0.f;
		}
	}


	__super::Update_GameObject(Time);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CGhost::LateUpdate_GameObject()
{

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);

	__super::LateUpdate_GameObject();
}

void CGhost::Render_GameObject()
{
	
}

void CGhost::Show_Ghost(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");


	CGameObject* pGameObject = nullptr;

	pGameObject = CGhostChild::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return;


	pGameLogicLayer->Add_BulletObject(pGameObject);
	
}

void CGhost::Show_Ghost_TimeStop(void)
{	
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic"); 
	if (pGameLogicLayer->m_fTimeStop == false)
		return;



	CGameObject* pGameObject = nullptr;

	pGameObject = CGhost_TimeStop::Create(m_pGraphicDev);

	if (pGameObject == nullptr)
		return;


	pGameLogicLayer->Add_BulletObject(pGameObject);
}

HRESULT CGhost::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pDashTextureCom[DASH_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Dash_045"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Dash_045", pComponent });

	pComponent = m_pDashTextureCom[DASH_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Dash_135"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Dash_135", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_ProtoComponent(L"Proto_Calculator"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });


	pComponent = m_pRunTextureCom[RUN_000] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_000"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_000", pComponent });

	pComponent = m_pRunTextureCom[RUN_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_045"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_045", pComponent });

	pComponent = m_pRunTextureCom[RUN_090] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_090"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_090", pComponent });

	pComponent = m_pRunTextureCom[RUN_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_135"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_135", pComponent });

	pComponent = m_pRunTextureCom[RUN_180] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_180"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_180", pComponent });



	return S_OK;
}


CGhost * CGhost::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGhost * pInstance = new CGhost(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGhost::Free(void)
{
	__super::Free();
}


