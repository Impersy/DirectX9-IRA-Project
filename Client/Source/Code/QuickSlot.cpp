#include "stdafx.h"
#include "..\Header\QuickSlot.h"
#include "Export_Function.h"

#include "QuickSlot_WeaponImg_Back.h"
#include "QuickSlot_WeaponImg.h"
#include "QuickSlot_ArrowCount_Infi.h"



CQuickSlot::CQuickSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f)
{
}

CQuickSlot::CQuickSlot(const CQuickSlot& rhs)
	: Engine::CGameObject(rhs), m_pINFO(nullptr), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fFrame(0.f)
{
}

CQuickSlot::~CQuickSlot()
{
}

HRESULT CQuickSlot::Ready_GameObject(void)
{	
	Add_Component();

	return S_OK;
}

_int CQuickSlot::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	if (false == m_bCheck)	//서순 문제로 미리 생성시 생성 불가
	{	
		m_bCheck = true;
		Create_QuickSlotUi();
	}

	return 0;
}

void CQuickSlot::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CQuickSlot::Render_GameObject()
{
}

HRESULT CQuickSlot::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CQuickSlot::Change_State(void)
{
}

void CQuickSlot::Frame_Check(const _float& fTimeDelta)
{
}

HRESULT CQuickSlot::Create_QuickSlotUi(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pBulletObject = nullptr;

	pBulletObject = CQuickSlot_WeaponImg_Back::Create(m_pGraphicDev, 0);
	 
	pLayer->Add_BulletObject(  pBulletObject);

	pBulletObject = CQuickSlot_WeaponImg::Create(m_pGraphicDev, 0);
	 
	pLayer->Add_BulletObject(  pBulletObject);

	pBulletObject = CQuickSlot_ArrowCount_Infi::Create(m_pGraphicDev, 0);
	 
	pLayer->Add_BulletObject(  pBulletObject);
	return S_OK;
}

CQuickSlot* CQuickSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CQuickSlot* pInstance = new CQuickSlot(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CQuickSlot::Free(void)
{
	__super::Free();
}
