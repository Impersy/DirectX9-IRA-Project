#include "stdafx.h"
#include "..\Header\Npc_RerollLever.h"
#include "Export_Function.h"

#include "Interaction_Button.h"
#include "Interaction_Dialog.h"

#include "Player.h"
#include "Item_Shop.h"
#include "Stage_Shop.h"
#include "SoundMgr.h"


CNpc_RerollLever::CNpc_RerollLever(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev), m_eNpcName(NPC_REROLL_REVER), m_bTest(false)
{
}

CNpc_RerollLever::CNpc_RerollLever(const CNpc_RerollLever& rhs)
	: CNpc(rhs), m_eNpcName(rhs.m_eNpcName), m_bTest(rhs.m_bTest)
{
}

CNpc_RerollLever::~CNpc_RerollLever()
{
	Free();
}


HRESULT CNpc_RerollLever::Ready_GameObject(void)
{
	Add_Component();
	m_eNpcName = NPC_REROLL_REVER;

	m_pTransformCom->Set_Pos(438.f, 7.f, 383.f);
	m_pTransformCom->UpdatePos_OnWorld();
	m_pTransformCom->Set_Scale(6.f, 6.f, 1.f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	return S_OK;
}

_int CNpc_RerollLever::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bTest)
	{
		Create_Active_Button_Effect();
		m_bTest = true;
	}

	Frame_Check(fTimeDelta);
	SetUp_OnTerrain();
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return OBJ_NOEVENT;
}

void CNpc_RerollLever::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CNpc_RerollLever::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CNpc_RerollLever::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Npc_RerollLever"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Npc_RerollLever", pComponent });

	return S_OK;
}

void CNpc_RerollLever::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CNpc_RerollLever::Change_State(void)
{

}

void CNpc_RerollLever::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += 8.f * fTimeDelta;

	if (8.f < m_fFrame)
	{
		m_fFrame = 0.f;
		m_bCheck = false;
	}

	if (m_bItemReset)
	{	
		m_fDelay_for_Delete += m_m_fDelay_for_Delete_Max * fTimeDelta;
		if (m_fDelay_for_Delete > m_m_fDelay_for_Delete_Max)
		{	
			m_fDelay_for_Delete = 0.f;
			m_bItemReset = false;
			Create_Item_6();
		}
	}
}


CNpc_RerollLever* CNpc_RerollLever::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_RerollLever* pInstance = new CNpc_RerollLever(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNpc_RerollLever::Create_Active_Button_Effect(void)
{
	//TIME_STOP_VOID

	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;
	pGameObject = CInteraction_Button::Create(m_pGraphicDev, TARGET_REROLL_REVER, m_pTransformCom->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

}

void CNpc_RerollLever::Item_Delete_Reroll(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	_int iTempPlayerMoney = pPlayer->Get_Player_Info().Money;
	if (iTempPlayerMoney >= 50)
	{	
		m_bItemReset = true;
		pPlayer->Set_Player_Money(MONEY_DECREASE, 50);

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_INTERACTION);
		CSoundMgr::Get_Instance()->PlaySound(L"UI_Store_Reroll.wav", CSoundMgr::UI_INTERACTION, 1.f);


		CScene* pScene = Engine::Get_Scene();
		dynamic_cast<CStage_Shop*>(pScene)->Set_CatCry(false);
		dynamic_cast<CStage_Shop*>(pScene)->Set_CryTime(0.f);
	} 
}

void CNpc_RerollLever::Create_Item_6(void)
{	
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	_vec3	m_Item_01_Pos = { 410.f, 6.f, 383.f };
	_vec3	m_Item_02_Pos = { 420.f, 6.f, 383.f };
	_vec3	m_Item_03_Pos = { 400.f, 6.f, 383.f };

	_vec3	m_Item_04_Pos = { 389.f, 6.f, 421.f };
	_vec3	m_Item_05_Pos = { 389.f, 6.f, 409.f };
	_vec3	m_Item_06_Pos = { 389.f, 6.f, 397.f };

	_int iTemp_ItemNum = ((rand() % 9) + 4);
	if ((iTemp_ItemNum == 6) || (iTemp_ItemNum == 7))
	{
		iTemp_ItemNum = 5;
	}
	pGameObject = CItem_Shop::Create(m_pGraphicDev, m_Item_01_Pos, iTemp_ItemNum);
	if (pGameObject == nullptr)
		return;
	pGameLogicLayer->Add_BulletObject(pGameObject);

	iTemp_ItemNum = ((rand() % 9) + 4);
	if ((iTemp_ItemNum == 6) || (iTemp_ItemNum == 7))
	{
		iTemp_ItemNum = 4;
	}
	pGameObject = CItem_Shop::Create(m_pGraphicDev, m_Item_02_Pos, iTemp_ItemNum);
	if (pGameObject == nullptr)
		return;
	pGameLogicLayer->Add_BulletObject(pGameObject);

	iTemp_ItemNum = ((rand() % 9) + 4);
	if ((iTemp_ItemNum == 6) || (iTemp_ItemNum == 7))
	{
		iTemp_ItemNum = 5;
	}
	pGameObject = CItem_Shop::Create(m_pGraphicDev, m_Item_03_Pos, iTemp_ItemNum);
	if (pGameObject == nullptr)
		return;
	pGameLogicLayer->Add_BulletObject(pGameObject);

	iTemp_ItemNum = ((rand() % 3) + 1);
	pGameObject = CItem_Shop::Create(m_pGraphicDev, m_Item_04_Pos, iTemp_ItemNum);
	if (pGameObject == nullptr)
		return;
	pGameLogicLayer->Add_BulletObject(pGameObject);

	iTemp_ItemNum = ((rand() % 3) + 1);
	pGameObject = CItem_Shop::Create(m_pGraphicDev, m_Item_05_Pos, iTemp_ItemNum);
	if (pGameObject == nullptr)
		return;
	pGameLogicLayer->Add_BulletObject(pGameObject);

	iTemp_ItemNum = ((rand() % 3) + 1);
	pGameObject = CItem_Shop::Create(m_pGraphicDev, m_Item_06_Pos, iTemp_ItemNum);
	if (pGameObject == nullptr)
		return;
	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CNpc_RerollLever::Free(void)
{
	__super::Free();
}