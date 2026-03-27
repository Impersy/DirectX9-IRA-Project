#include "stdafx.h"
#include "..\Header\Apostle.h"
#include "Export_Function.h"

#include "Apostle_Frame_Back.h"
#include "Apostle_Frame.h"
#include "Apostle_Key_Q.h"
#include "Apostle_Pattern.h"
#include "Apostle_Count.h"
#include "Apostle_Pattern_Glow.h"

CApostle::CApostle(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f)
{
}

CApostle::CApostle(const CApostle& rhs)
	: Engine::CGameObject(rhs), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f)
{
}

CApostle::~CApostle()
{
}

HRESULT CApostle::Ready_GameObject(void)
{	
	Add_Component();

	return S_OK;
}

_int CApostle::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	if (false == m_bCheck)	//서순 문제로 미리 생성시 생성 불가
	{	
		m_bCheck = true;
		Create_StatusUi();
	}

	return 0;
}

void CApostle::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CApostle::Render_GameObject()
{
}

HRESULT CApostle::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CApostle::Change_State(void)
{
}

void CApostle::Frame_Check(const _float& fTimeDelta)
{
}

HRESULT CApostle::Create_StatusUi(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pBulletObject = nullptr;

	//for (size_t i = 0; i < 5; i++)
	//{
	//	pBulletObject = CApostle_Back::Create(m_pGraphicDev, (i + 1));
	//	 
	//	pLayer->Add_BulletObject(  pBulletObject);
	//}


	pBulletObject = CApostle_Frame_Back::Create(m_pGraphicDev, 0);
	pLayer->Add_BulletObject(  pBulletObject);

	pBulletObject = CApostle_Pattern_Glow::Create(m_pGraphicDev, 0);
	pLayer->Add_BulletObject(  pBulletObject);

	pBulletObject = CApostle_Pattern::Create(m_pGraphicDev, 1);
	pLayer->Add_BulletObject(  pBulletObject);

	pBulletObject = CApostle_Frame::Create(m_pGraphicDev, 0);
	pLayer->Add_BulletObject(  pBulletObject);

	for (size_t i = 0; i < 3; i++)
	{
		pBulletObject = CApostle_Count::Create(m_pGraphicDev, (i + 1));
		pLayer->Add_BulletObject(  pBulletObject);
	}

	pBulletObject = CApostle_Key_Q::Create(m_pGraphicDev, 0);
	pLayer->Add_BulletObject(  pBulletObject);

	return S_OK;
}

CApostle* CApostle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CApostle* pInstance = new CApostle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CApostle::Free(void)
{
	__super::Free();
}
