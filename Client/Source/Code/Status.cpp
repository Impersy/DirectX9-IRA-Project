#include "stdafx.h"
#include "..\Header\Status.h"
#include "Export_Function.h"

#include "Status_Hp.h"
#include "Status_Energy.h"
#include "Status_Key.h"
#include "Status_Key_Font.h"
#include "Status_Money.h"
#include "Status_Money_Font.h"
#include "Status_Gem.h"
#include "Status_Gem_Font.h"

#include "Status_Back.h"

#include "Player.h"
#include "Effect_Ui_EnergyGage.h"



CStatus::CStatus(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f), m_bStack_Eng_Charge(false)
{
}

CStatus::CStatus(const CStatus& rhs)
	: Engine::CGameObject(rhs), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f), m_bStack_Eng_Charge(false)
{
}

CStatus::~CStatus()
{
}

HRESULT CStatus::Ready_GameObject(void)
{	
	Add_Component();

	return S_OK;
}

_int CStatus::Update_GameObject(const _float& fTimeDelta)
{	
	if (false == m_bCheck)	//서순 문제로 미리 생성시 생성 불가
	{	
		m_bCheck = true;

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		m_tRecieveInfo_Pre = pPlayer->Get_Player_Info();

		Create_StatusUi();

	}

	if (m_bCheck)
	{
		Modify_StatusUi();
	}


	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CStatus::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CStatus::Render_GameObject()
{
}

HRESULT CStatus::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CStatus::Change_State(void)
{
}

void CStatus::Frame_Check(const _float& fTimeDelta)
{
}

HRESULT CStatus::Create_StatusUi(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pBulletObject = nullptr;

	for (int i = 2; i < 5; i++)
	{
		pBulletObject = CStatus_Back::Create(m_pGraphicDev, (i + 1));
		 
		pLayer->Add_BulletObject(pBulletObject);
	}

	for (int i = 0; i < m_tRecieveInfo_Pre.Maxhp; i++)
	{
		pBulletObject = CStatus_Hp::Create(m_pGraphicDev,(i + 1));
		 
		pLayer->Add_BulletObject(pBulletObject);
	}

	for (int i = 0; i < m_tRecieveInfo_Pre.MaxEnergy; i++)
	{
		pBulletObject = CStatus_Energy::Create(m_pGraphicDev, (i + 1));
		 
		pLayer->Add_BulletObject(pBulletObject);
	}

	pBulletObject = CStatus_Key::Create(m_pGraphicDev, 1);
	 
	pLayer->Add_BulletObject(  pBulletObject);

	pBulletObject = CStatus_Money::Create(m_pGraphicDev, 1);
	 
	pLayer->Add_BulletObject(  pBulletObject);

	for (int i = 0; i < 4; i++)
	{
		pBulletObject = CStatus_Money_Font::Create(m_pGraphicDev, (i + 1));
		 
		pLayer->Add_BulletObject(pBulletObject);
	}

	for (int i = 0; i < 4; i++)
	{
		pBulletObject = CStatus_Key_Font::Create(m_pGraphicDev, (i + 1));
		 
		pLayer->Add_BulletObject(pBulletObject);
	}

	for (int i = 0; i < 4; i++)
	{
		pBulletObject = CStatus_Gem_Font::Create(m_pGraphicDev, (i + 1));
		 
		pLayer->Add_BulletObject(pBulletObject);
	}

	pBulletObject = CStatus_Gem::Create(m_pGraphicDev, 1);
	 
	pLayer->Add_BulletObject(  pBulletObject);

	return S_OK;
}

HRESULT CStatus::Modify_StatusUi(void)
{	
	PLAYER_INFO tTemp;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	tTemp = pPlayer->Get_Player_Info();

	if (m_tRecieveInfo_Pre.Maxhp < tTemp.Maxhp)
	{	

		_int iTemp_Hp = tTemp.Maxhp - m_tRecieveInfo_Pre.Maxhp;

		CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
		CGameObject* pBulletObject = nullptr;

		for (int i = 0; i < iTemp_Hp; i++)
		{
			pBulletObject = CStatus_Hp::Create(m_pGraphicDev, (m_tRecieveInfo_Pre.Maxhp + i + 1));
			 
			pLayer->Add_BulletObject(pBulletObject);
		}

		m_tRecieveInfo_Pre.Maxhp = tTemp.Maxhp;
	}

	if (m_tRecieveInfo_Pre.MaxEnergy < tTemp.MaxEnergy)
	{	
		_int iTemp_Energy = tTemp.MaxEnergy - m_tRecieveInfo_Pre.MaxEnergy;

		CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
		CGameObject* pBulletObject = nullptr;

		for (int i = 0; i < iTemp_Energy; i++)
		{
			pBulletObject = CStatus_Energy::Create(m_pGraphicDev, (m_tRecieveInfo_Pre.MaxEnergy + i + 1));
			 
			pLayer->Add_BulletObject(pBulletObject);
		}

		m_tRecieveInfo_Pre.MaxEnergy = tTemp.MaxEnergy;
	}

	if (m_tRecieveInfo_Pre.Money != tTemp.Money)
	{
		m_tRecieveInfo_Pre.Money = tTemp.Money;
	}

	if (m_tRecieveInfo_Pre.Gem != tTemp.Gem)
	{
		m_tRecieveInfo_Pre.Gem = tTemp.Gem;
	}

	if (m_tRecieveInfo_Pre.Key != tTemp.Key)
	{
		m_tRecieveInfo_Pre.Key = tTemp.Key;
	}


	if ((tTemp.NowEnergy < tTemp.MaxEnergy) && !m_bStack_Eng_Charge)
	{
		Create_ChargeEffect_Energy();
	}

	return S_OK;
}

HRESULT CStatus::Create_ChargeEffect_Energy(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pGameObject = nullptr;
	pGameObject = CEffect_Ui_EnergyGage::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);
	//if (pGameObject == nullptr)
	//	return;
	pGameLogicLayer->Add_BulletObject(pGameObject);

	m_bStack_Eng_Charge = true;

	return S_OK;
}

CStatus* CStatus::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStatus* pInstance = new CStatus(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStatus::Free(void)
{
	__super::Free();
}
