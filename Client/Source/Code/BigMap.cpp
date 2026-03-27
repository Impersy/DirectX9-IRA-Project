#include "stdafx.h"
#include "..\Header\BigMap.h"
#include "Export_Function.h"

#include "Player.h"

#include "BigMap_BackGround.h"
#include "BigMap_Skill_Icon.h"
#include "BigMap_Text.h"
#include "BigMap_Room.h"
#include "BigMap_Icon.h"

CBigMap::CBigMap(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f), m_fOpen_Delay(10.f), m_fClose_Delay(10.f)
{
}

CBigMap::CBigMap(const CBigMap& rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTextureCom(rhs.m_pTextureCom), m_pTransformCom(rhs.m_pTransformCom)
	, m_bCheck(rhs.m_bCheck), m_fFrame(rhs.m_fFrame), m_fOpen_Delay(rhs.m_fOpen_Delay), m_fClose_Delay(rhs.m_fClose_Delay)
{
}

CBigMap::~CBigMap()
{
}

HRESULT CBigMap::Ready_GameObject(void)
{	
	Add_Component();

	return S_OK;
}

_int CBigMap::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_bCheck)
	{
		Frame_Check_Close(fTimeDelta);
	}
	if (!m_bCheck)
	{
		Frame_Check_Open(fTimeDelta);
	}

	if (GetAsyncKeyState('M') & 0x8000)	//서순 문제로 미리 생성시 생성 불가
	{
		if (!m_bCheck && (m_fOpen_Delay == m_fFrame))
		{
			m_bCheck = true;

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_bBigMapUi_On(true);

			Create_BigMapUi();
			m_fFrame = 0.f;
		}
	}

	if (GetAsyncKeyState('M') & 0x8000)	//서순 문제로 미리 생성시 생성 불가
	{
		if (m_bCheck && (m_fClose_Delay == m_fFrame))
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_bBigMapUi_On(false);

			m_bCheck = false;
			m_fFrame = 0.f;
		}
	}

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CBigMap::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBigMap::Render_GameObject()
{
}

HRESULT CBigMap::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CBigMap::Change_State(void)
{
}

void CBigMap::Frame_Check(const _float& fTimeDelta)
{
}

HRESULT CBigMap::Create_BigMapUi(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pBigMapObject = nullptr;

	pBigMapObject = CBigMap_BackGround::Create(m_pGraphicDev);
	pLayer->Add_BulletObject(pBigMapObject);

	//pBulletObject = CBigMap_Gage::Create(m_pGraphicDev, 2);
	// 
	//pLayer->Add_BulletObject(pBulletObject);

	for (int i = 0; i < 4; i++)
	{
		pBigMapObject = CBigMap_Skill_Icon::Create(m_pGraphicDev, i);
		 
		pLayer->Add_BulletObject(pBigMapObject);
	}

	for (int i = 0; i < 9; i++)
	{
		pBigMapObject = CBigMap_Room::Create(m_pGraphicDev, i);
		pLayer->Add_BulletObject(pBigMapObject);
	}

	for (int i = 0; i < 12; i++)
	{
		pBigMapObject = CBigMap_Icon::Create(m_pGraphicDev, i);
		pLayer->Add_BulletObject(pBigMapObject);
	}

	pBigMapObject = CBigMap_Text::Create(m_pGraphicDev);
	pLayer->Add_BulletObject(pBigMapObject);




	return S_OK;
}

void CBigMap::Frame_Check_Close(const _float& fTimeDelta)
{
	m_fFrame += (m_fClose_Delay * fTimeDelta * 0.5);

	if (m_fFrame > m_fClose_Delay)
	{
		m_fFrame = m_fClose_Delay;
	}
}

void CBigMap::Frame_Check_Open(const _float& fTimeDelta)
{
	m_fFrame += (m_fOpen_Delay * fTimeDelta * 0.5);

	if (m_fFrame > m_fOpen_Delay)
	{
		m_fFrame = m_fOpen_Delay;
	}
}

CBigMap* CBigMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBigMap* pInstance = new CBigMap(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBigMap::Free(void)
{
	__super::Free();
}
