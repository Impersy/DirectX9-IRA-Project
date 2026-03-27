#include "stdafx.h"
#include "..\Header\Inventory.h"
#include "Export_Function.h"

#include "Inventory_BackGround.h"
#include "Inventory_FrameBack.h"
#include "Inventory_Frame.h"
#include "Inventory_ItemIcon.h"
#include "Inventory_ItemIcon_Back.h"
#include "Inventory_InvenInfo.h"

#include "Player.h"

#include "SoundMgr.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f), m_fOpen_Delay(20.f), m_fClose_Delay(20.f)
{
}

CInventory::CInventory(const CInventory& rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f), m_fOpen_Delay(20.f), m_fClose_Delay(20.f)
{
}

CInventory::~CInventory()
{
}

HRESULT CInventory::Ready_GameObject(void)
{	
	Add_Component();

	return S_OK;
}

_int CInventory::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_bCheck)
	{
		Frame_Check_Close(fTimeDelta);
	}
	if (!m_bCheck)
	{
		Frame_Check_Open(fTimeDelta);
	}

	if (GetAsyncKeyState('I') & 0x8000)	//서순 문제로 미리 생성시 생성 불가
	{
		if (!m_bCheck && (m_fOpen_Delay == m_fFrame))
		{
			m_bCheck = true;

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_bItemUi_On(true);

			Create_InventoryUi();
			m_fFrame = 0.f;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_CLICK);
			CSoundMgr::Get_Instance()->PlaySound(L"Inventory_Open_Close.wav", CSoundMgr::UI_CLICK, 0.8f);

		}
	}

	if (GetAsyncKeyState('I') & 0x8000)	//서순 문제로 미리 생성시 생성 불가
	{
		if (m_bCheck && (m_fClose_Delay == m_fFrame))
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_bItemUi_On(false);

			m_bCheck = false;
			m_fFrame = 0.f;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_CLICK);
			CSoundMgr::Get_Instance()->PlaySound(L"Inventory_Open_Close.wav", CSoundMgr::UI_CLICK, 0.8f);
		}
	}

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CInventory::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CInventory::Render_GameObject()
{
}

HRESULT CInventory::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CInventory::Change_State(void)
{
}

void CInventory::Frame_Check_Close(const _float& fTimeDelta)
{
	m_fFrame += (m_fClose_Delay * fTimeDelta * 0.5);

	if (m_fFrame > m_fClose_Delay)
	{
		m_fFrame = m_fClose_Delay;
	}
}

void CInventory::Frame_Check_Open(const _float& fTimeDelta)
{
	m_fFrame += (m_fOpen_Delay * fTimeDelta * 0.5);

	if (m_fFrame > m_fOpen_Delay)
	{
		m_fFrame = m_fOpen_Delay;
	}
}

HRESULT CInventory::Create_InventoryUi(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pInvenObject = nullptr;
	pInvenObject = CInventory_BackGround::Create(m_pGraphicDev);
	
	pLayer->Add_BulletObject(pInvenObject);

	for (size_t i = 0; i < 18; i++)
	{
		pInvenObject = CInventory_FrameBack::Create(m_pGraphicDev, (i + 1));
		pLayer->Add_BulletObject(pInvenObject);
	}

	for (size_t i = 0; i < 18; i++)
	{
		pInvenObject = CInventory_ItemIcon_Back::Create(m_pGraphicDev, (i + 1));
		pLayer->Add_BulletObject(pInvenObject);
	}

	for (size_t i = 0; i < 18; i++)
	{
		pInvenObject = CInventory_ItemIcon::Create(m_pGraphicDev, i);
		pLayer->Add_BulletObject(pInvenObject);
	}

	for (size_t i = 0; i < 18; i++)
	{
		pInvenObject = CInventory_Frame::Create(m_pGraphicDev, (i + 1));
		pLayer->Add_BulletObject(pInvenObject);
	}

	pInvenObject = CInventory_InvenInfo::Create(m_pGraphicDev);
	pLayer->Add_BulletObject(pInvenObject);

	return S_OK;
}

HRESULT CInventory::Modify_InventoryUi(void)
{	
	/*PLAYER_INFO tTemp;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	tTemp = pPlayer->Get_Player_Info();

	if (m_tRecieveInfo_Pre.Maxhp < tTemp.Maxhp)
	{	

		_int iTemp_Hp = tTemp.Maxhp - m_tRecieveInfo_Pre.Maxhp;

		CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
		CGameObject* pInvenObject = nullptr;

		for (int i = 0; i < iTemp_Hp; i++)
		{
			pInvenObject = CInventory_Hp::Create(m_pGraphicDev, (m_tRecieveInfo_Pre.Maxhp + i + 1));
			NULL_CHECK_RETURN(pInvenObject, -1);
			pLayer->Add_BulletObject(pInvenObject);
		}

		m_tRecieveInfo_Pre.Maxhp = tTemp.Maxhp;
	}

	if (m_tRecieveInfo_Pre.MaxEnergy < tTemp.MaxEnergy)
	{	
		_int iTemp_Energy = tTemp.MaxEnergy - m_tRecieveInfo_Pre.MaxEnergy;

		CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
		CGameObject* pInvenObject = nullptr;

		for (int i = 0; i < iTemp_Energy; i++)
		{
			pInvenObject = CInventory_Energy::Create(m_pGraphicDev, (m_tRecieveInfo_Pre.MaxEnergy + i + 1));
			NULL_CHECK_RETURN(pInvenObject, -1);
			pLayer->Add_BulletObject(pInvenObject);
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
	}*/

	return S_OK;
}

CInventory* CInventory::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventory* pInstance = new CInventory(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInventory::Free(void)
{
	__super::Free();
}
