#include "stdafx.h"
#include "..\Header\Frame_LevelUp.h"
#include "Export_Function.h"

#include "Effect_Ui_LevelUp_Back.h"
#include "Effect_Ui_LevelUp_Lion.h"
#include "Effect_Ui_LevelUp_Text.h"


#include "Player.h"

CFrame_LevelUp::CFrame_LevelUp(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr), m_bCheck(false), m_fFrame(0.f)
{
}

CFrame_LevelUp::CFrame_LevelUp(const CFrame_LevelUp& rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTextureCom(rhs.m_pTextureCom), m_pTransformCom(rhs.m_pTransformCom), m_bCheck(rhs.m_bCheck), m_fFrame(rhs.m_fFrame)
{
}

CFrame_LevelUp::~CFrame_LevelUp()
{

}

HRESULT CFrame_LevelUp::Ready_GameObject(void)
{	
	Add_Component();

	return S_OK;
}

_int CFrame_LevelUp::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	if (false == m_bCheck)	//서순 문제로 미리 생성시 생성 불가
	{	
		Create_Frame_LevelUpUi();
		m_bCheck = true;
	}
	if (m_bCheck)
	{
		m_bDead = true;
	}

	__super::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;
}

void CFrame_LevelUp::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CFrame_LevelUp::Render_GameObject()
{
}

HRESULT CFrame_LevelUp::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CFrame_LevelUp::Change_State(void)
{
}

void CFrame_LevelUp::Frame_Check(const _float& fTimeDelta)
{
}

HRESULT CFrame_LevelUp::Create_Frame_LevelUpUi(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pBulletObject = nullptr;

	//for (int i = 2; i < 5; i++)
	//{
	//	pBulletObject = CFrame_LevelUp_Back::Create(m_pGraphicDev, (i + 1));
	//	 
	//	pLayer->Add_BulletObject(pBulletObject);
	//}

	pBulletObject = CEffect_Ui_LevelUp_Back::Create(m_pGraphicDev);
	pLayer->Add_BulletObject(pBulletObject);

	pBulletObject = CEffect_Ui_LevelUp_Lion::Create(m_pGraphicDev);
	pLayer->Add_BulletObject(pBulletObject);

	pBulletObject = CEffect_Ui_LevelUp_Text::Create(m_pGraphicDev);
	pLayer->Add_BulletObject(pBulletObject);

	return S_OK;
}

CFrame_LevelUp* CFrame_LevelUp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFrame_LevelUp* pInstance = new CFrame_LevelUp(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFrame_LevelUp::Free(void)
{
	__super::Free();
}
