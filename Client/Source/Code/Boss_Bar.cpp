#include "stdafx.h"
#include "..\Header\Boss_Bar.h"
#include "Export_Function.h"

#include "Boss_Bar_Hp_Back.h"
#include "Boss_Bar_Hp.h"
#include "Boss_Bar_Hp_Delay.h"
#include "Boss_Bar_Icon.h"


CBoss_Bar::CBoss_Bar(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f)
{
}

CBoss_Bar::CBoss_Bar(const CBoss_Bar& rhs)
	: Engine::CGameObject(rhs), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f)
{
}

CBoss_Bar::~CBoss_Bar()
{
}

HRESULT CBoss_Bar::Ready_GameObject(void)
{	
	Add_Component();
	return S_OK;
}

_int CBoss_Bar::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	if (false == m_bCheck)	//서순 문제로 미리 생성시 생성 불가
	{	
		m_bCheck = true;
		Create_StatusUi();
	}
	return 0;
}

void CBoss_Bar::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBoss_Bar::Render_GameObject()
{
}

HRESULT CBoss_Bar::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CBoss_Bar::Change_State(void)
{
}

void CBoss_Bar::Frame_Check(const _float& fTimeDelta)
{
}

HRESULT CBoss_Bar::Create_StatusUi(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pBulletObject = nullptr;

	pBulletObject = CBoss_Bar_Hp_Back::Create(m_pGraphicDev, 0, m_eBoss_Name);
	pLayer->Add_BulletObject(pBulletObject);

	pBulletObject = CBoss_Bar_Hp_Delay::Create(m_pGraphicDev, 0, m_eBoss_Name);
	pLayer->Add_BulletObject(pBulletObject);

	pBulletObject = CBoss_Bar_Hp::Create(m_pGraphicDev, 0, m_eBoss_Name);
	pLayer->Add_BulletObject(pBulletObject);

	pBulletObject = CBoss_Bar_Icon::Create(m_pGraphicDev, 0, m_eBoss_Name);
	pLayer->Add_BulletObject(pBulletObject);

	return S_OK;
}

CBoss_Bar* CBoss_Bar::Create(LPDIRECT3DDEVICE9 pGraphicDev, BOSS_NAME _Name)
{
	CBoss_Bar* pInstance = new CBoss_Bar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->m_eBoss_Name = _Name;

	return pInstance;
}

void CBoss_Bar::Free(void)
{
	__super::Free();
}
