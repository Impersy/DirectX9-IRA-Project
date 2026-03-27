#include "stdafx.h"
#include "..\Header\Item_Shop.h"
#include "Export_Function.h"

#include "Player.h"
#include "Interaction_Button.h"
#include "Interaction_Item_Info.h"
#include "Npc_RerollLever.h"

#include "KeyMgr.h"
#include "SoundMgr.h"

CItem_Shop::CItem_Shop(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev), m_bTest(false)
{
	My_Item_INFO.resize(1);
}

CItem_Shop::CItem_Shop(const CItem_Shop& rhs)
	: CItem(rhs), m_bTest(rhs.m_bTest)
{
}

CItem_Shop::~CItem_Shop()
{
	Free();
}


HRESULT CItem_Shop::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->Set_Pos(m_vMasterPos.x, m_vMasterPos.y, m_vMasterPos.z);
	m_pTransformCom->UpdatePos_OnWorld();
	m_pTransformCom->Set_Scale(2.5f, 2.5f, 1.f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	My_Item_INFO[0] = INFO[m_iMyItemNum];

	Item_Num_Check();
	Set_Receive_PlayerInven();

	return S_OK;
}

_int CItem_Shop::Update_GameObject(const _float& fTimeDelta)
{	
	Reset_Check();

	//if (!m_bCreate_Rotation)
	//{	
	//	m_fRoatationCheck = m_fRoatationCheck_Max * fTimeDelta * 0.5f;
	//	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(m_fRoatationCheck));

	//	if (m_fRoatationCheck > m_fRoatationCheck_Max)
	//	{
	//		m_fRoatationCheck = m_fRoatationCheck_Max;
	//		m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(0.f));
	//		m_bCreate_Rotation = true;
	//	}
	//}

	if (!m_bTest)
	{
		Create_Active_Button_Effect();
		m_bTest = true;
	}

	Face_Check();
	Rooting_Shop_Item();
	//if (!m_bDropOn)
	//{
	//	Create_Drop_Effect();
	//	m_bDropOn = true;
	//}

	//*m_vDropPos = m_vMasterPos;

	//if (m_bDropEffect_Off && (!m_bGroundEffect_On))
	//{
	//	Create_Ground_Effect();
	//	m_bGroundEffect_On = true;
	//}

	if (m_bDead)
	{

		return OBJ_DEAD;
	}

	m_pTransformCom->Set_Pos(m_vMasterPos.x, m_vMasterPos.y, m_vMasterPos.z);
	m_pTransformCom->UpdatePos_OnWorld();

	//Frame_Check(fTimeDelta);
	SetUp_OnTerrain();
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return OBJ_NOEVENT;
}

void CItem_Shop::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CItem_Shop::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CItem_Shop::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Item_ItemImg"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Item_ItemImg", pComponent });

	return S_OK;
}

void CItem_Shop::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CItem_Shop::Change_State(void)
{

}

void CItem_Shop::Frame_Check(const _float& fTimeDelta)
{
}

void CItem_Shop::Face_Check(void)
{
	// 플레이어 Pos 받아 MasterPos와 구충돌
	_vec3 vPlayerPos;
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	vPlayerPos;

	_float x = vPlayerPos.x;
	_float z = vPlayerPos.z;
	_float radius1 = 3.0f;

	_float x1 = m_vMasterPos.x;
	_float z1 = m_vMasterPos.z;
	_float radius2 = 3.f;

	_float dx = x - x1;
	_float dz = z - z1;
	_float distance = sqrt(dx * dx + dz * dz);

	if (distance < radius1 + radius2)
		m_bFace = true;
	else
		m_bFace = false;
}

void CItem_Shop::Item_Num_Check(void)
{
	m_fFrame = (_float)(My_Item_INFO[0].iItemNumber - 2);
}

void CItem_Shop::Reset_Check(void)
{			
	CNpc_RerollLever* pNpc_RerollLever = dynamic_cast<CNpc_RerollLever*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Npc_RerollLever"));
	_bool bTempReset = pNpc_RerollLever->Get_ItemReset();

	if (bTempReset == true)
	{
		m_bDead = true;
	}
}

CItem_Shop* CItem_Shop::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _iItem_Num)
{
	CItem_Shop* pInstance = new CItem_Shop(pGraphicDev);

	pInstance->m_vMasterPos = vPos;

	pInstance->m_iMyItemNum = _iItem_Num;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CItem_Shop::Create_Ground_Effect(void)
{
	//CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	//CGameObject* pGameObject = nullptr;
	//pGameObject = CEffect_Drop_Ground::Create(m_pGraphicDev, m_vDropPos);
	//if (pGameObject == nullptr)
	//	return;
	//pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CItem_Shop::Create_Active_Button_Effect(void)
{
	//TIME_STOP_VOID

	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;

	m_SendPos = m_pTransformCom->m_vInfo[INFO_POS];

	pGameObject = CInteraction_Button::Create_And_SaveItemInfo(m_pGraphicDev, TARGET_ITEM_SHOP, m_SendPos, My_Item_INFO[0]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

	pGameObject = CInteraction_Item_Info::Create(m_pGraphicDev, TARGET_ITEM_SHOP, m_SendPos, My_Item_INFO[0]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

}


void CItem_Shop::Rooting_Shop_Item()
{
	if (m_bFace == true)
	{	
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		_int iTemp_Player_Money = pPlayer->Get_Player_Info().Money;

		_int iTemp_Space_Check = m_pReceive_PlayerInven->Find_Empty_SpaceSlot();

		if ((CKeyMgr::Get_Instance()->Key_Up(KEY_E)))
		{	
			if (iTemp_Space_Check > (-1))
			{
				if (iTemp_Player_Money > My_Item_INFO[0].iPrice)
				{
					if (My_Item_INFO[0].iItemNumber >= 5)
					{	
						CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_INTERACTION);
						CSoundMgr::Get_Instance()->PlaySound(L"UI_Store_Buy.wav", CSoundMgr::UI_INTERACTION, 0.9f);

						m_pReceive_PlayerInven->Rooting_Item_In_Inven(My_Item_INFO[0].iItemNumber);
						m_bDead = true;
					}
					else if (My_Item_INFO[0].iItemNumber == 2)
					{
						PLAYER_INFO tTemp = pPlayer->Get_Player_Info();
						if (tTemp.Maxhp > tTemp.Nowhp)
						{
							tTemp.Nowhp += 1;
							if (tTemp.Nowhp > tTemp.Maxhp)
								tTemp.Nowhp = tTemp.Maxhp;
							pPlayer->Set_Player_Info(tTemp);
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_INTERACTION);
							CSoundMgr::Get_Instance()->PlaySound(L"UI_Store_Buy.wav", CSoundMgr::UI_INTERACTION, 0.9f);
						}
						m_bDead = true;
					}
					else if (My_Item_INFO[0].iItemNumber == 3)
					{
						PLAYER_INFO tTemp = pPlayer->Get_Player_Info();
						if (tTemp.Maxhp == tTemp.Nowhp)
						{
							tTemp.Maxhp += 1;
							tTemp.Nowhp += 1;
							pPlayer->Set_Player_Info(tTemp);
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_INTERACTION);
							CSoundMgr::Get_Instance()->PlaySound(L"UI_Store_Buy.wav", CSoundMgr::UI_INTERACTION, 0.9f);
							m_bDead = true;
						}
					}
					else if (My_Item_INFO[0].iItemNumber == 4)
					{
						PLAYER_INFO tTemp = pPlayer->Get_Player_Info();
						tTemp.MaxEnergy += 1;

						pPlayer->Set_Player_Info(tTemp);
						CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_INTERACTION);
						CSoundMgr::Get_Instance()->PlaySound(L"UI_Store_Buy.wav", CSoundMgr::UI_INTERACTION, 0.9f);
						m_bDead = true;
					}
				}
			}
		}
	}
}

void CItem_Shop::Set_Receive_PlayerInven(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_pReceive_PlayerInven = pPlayer->Get_InvenInfo();
}

void CItem_Shop::Free(void)
{
	__super::Free();
}