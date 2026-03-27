#include "stdafx.h"
#include "..\Header\MiniMap.h"
#include "Export_Function.h"

#include "MiniMap_Back.h"
#include "MiniMap_Gage.h"
#include "MiniMap_Character.h"
#include "MiniMap_Room.h"
#include "MiniMap_Icon.h"

//#include "MiniMap_Pattern.h"
//#include "MiniMap_Count.h"
//#include "MiniMap_Pattern_Glow.h"

CMiniMap::CMiniMap(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f)
{
}

CMiniMap::CMiniMap(const CMiniMap& rhs)
	: Engine::CGameObject(rhs), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f)
{
}

CMiniMap::~CMiniMap()
{
}

HRESULT CMiniMap::Ready_GameObject(void)
{	
	Add_Component();

	return S_OK;
}

_int CMiniMap::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	if (false == m_bCheck)	//서순 문제로 미리 생성시 생성 불가
	{	
		m_bCheck = true;
		Create_StatusUi();
	}

	return 0;
}

void CMiniMap::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMiniMap::Render_GameObject()
{
}

HRESULT CMiniMap::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CMiniMap::Change_State(void)
{
}

void CMiniMap::Frame_Check(const _float& fTimeDelta)
{
}

HRESULT CMiniMap::Create_StatusUi(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pBulletObject = nullptr;

	pBulletObject = CMiniMap_Back::Create(m_pGraphicDev, 1);
	 
	pLayer->Add_BulletObject(pBulletObject);

	pBulletObject = CMiniMap_Gage::Create(m_pGraphicDev, 2);
	 
	pLayer->Add_BulletObject(pBulletObject);

	switch (g_eCurScene)
	{
	case SCENE_TITLE:
		break;
	case SCENE_TUTORIAL:
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 5);
		pLayer->Add_BulletObject(pBulletObject);

		break;
	case SCENE_MAINLOBBY:
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 5);
		pLayer->Add_BulletObject(pBulletObject);

		break;
	case SCENE_DUNGEONLOBBY:
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 2);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 4);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 5);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 6);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 9);
		pLayer->Add_BulletObject(pBulletObject);

		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 8, 2);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 2, 4);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 5, 6);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 6, 9);
		pLayer->Add_BulletObject(pBulletObject);

		break;
	case SCENE_REGEN:
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 1);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 4);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 5);
		pLayer->Add_BulletObject(pBulletObject);

		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 5, 1);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 6, 4);
		pLayer->Add_BulletObject(pBulletObject);

		break;
	case SCENE_SHOP:
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 3);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 5);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 6);
		pLayer->Add_BulletObject(pBulletObject);

		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 8, 3);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 4, 6);
		pLayer->Add_BulletObject(pBulletObject);

		break;
	case SCENE_BOSS1:
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 2);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 5);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 7);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 8);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 9);
		pLayer->Add_BulletObject(pBulletObject);

		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 9, 2);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 2, 7);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 4, 8);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 5, 9);
		pLayer->Add_BulletObject(pBulletObject);
		break;
	case SCENE_BOSS2:
		//pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 5, 7);
		//pLayer->Add_BulletObject(pBulletObject);
		break;

	case SCENE_POT:
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 1);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 2);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 5);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 6);
		pLayer->Add_BulletObject(pBulletObject);

		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 4, 1);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 5, 2);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 7, 6);
		pLayer->Add_BulletObject(pBulletObject);


		break;
	case SCENE_NORMAL:
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 1);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 4);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 5);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 8);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Room::Create(m_pGraphicDev, 4, 9);
		pLayer->Add_BulletObject(pBulletObject);

		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 8, 1);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 4, 4);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 6, 8);
		pLayer->Add_BulletObject(pBulletObject);
		pBulletObject = CMiniMap_Icon::Create(m_pGraphicDev, 7, 9);
		pLayer->Add_BulletObject(pBulletObject);
		break;
	case SCENE_END:
		break;
	default:
		break;
	}

	pBulletObject = CMiniMap_Character::Create(m_pGraphicDev, 3);
	 
	pLayer->Add_BulletObject(pBulletObject);

	return S_OK;
}

CMiniMap* CMiniMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniMap* pInstance = new CMiniMap(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniMap::Free(void)
{
	__super::Free();
}
