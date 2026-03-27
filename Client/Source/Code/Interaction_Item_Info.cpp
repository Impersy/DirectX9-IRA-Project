#include "stdafx.h"
#include "..\Header\Interaction_Item_Info.h"
#include "Export_Function.h"
#include <Player.h>

#include "KeyMgr.h"
#include "Npc_RerollLever.h"

CInteraction_Item_Info::CInteraction_Item_Info(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteraction(pGraphicDev), m_pTextureCom_Shop(nullptr)
{
}

CInteraction_Item_Info::CInteraction_Item_Info(const CInteraction_Item_Info& rhs)
	: CInteraction(rhs), m_pTextureCom_Shop(rhs.m_pTextureCom_Shop)
{
}

CInteraction_Item_Info::~CInteraction_Item_Info()
{
}

HRESULT CInteraction_Item_Info::Ready_GameObject(void)
{
	Add_Component();
	m_eInter_Type = FIELD_ITEM_INFO;

	m_pTransformCom->Set_Pos(m_vMasterPos.x, m_vMasterPos.y + 11.f, m_vMasterPos.z + 1.f);
	m_pTransformCom->UpdatePos_OnWorld();
	m_pTransformCom->Set_Scale(10.f, 10.f, 1.f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(20.f));

	m_fFrame = (m_eRecieveInfo.iItemNumber - 2);
	Set_Receive_PlayerInven();

	return S_OK;
}

_int CInteraction_Item_Info::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_eInter_Target == TARGET_ITEM_SHOP)
	{
		CNpc_RerollLever* pNpc_RerollLever = dynamic_cast<CNpc_RerollLever*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Npc_RerollLever"));
		_bool bTempReset = pNpc_RerollLever->Get_ItemReset();

		if (bTempReset == true)
		{
			m_bDead = true;
		}
	}

	Face_Check();
	Rooting_Item();

	if (m_bDead)
		return OBJ_DEAD;

	SetUp_OnTerrain();

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CInteraction_Item_Info::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CInteraction_Item_Info::Render_GameObject()
{	
	if (m_bFace)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		if (m_eInter_Target == TARGET_ITEM_DROP)
		{
			m_pTextureCom->Set_Texture(m_fFrame);
		}
		else if (m_eInter_Target == TARGET_ITEM_SHOP)
		{
			m_pTextureCom_Shop->Set_Texture(m_fFrame);
		}

		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

HRESULT CInteraction_Item_Info::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Ui_Item_Info_Frame_Drop"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Ui_Item_Info_Frame_Drop", pComponent });

	pComponent = m_pTextureCom_Shop = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Ui_Item_Info_Frame_Shop"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Ui_Item_Info_Frame_Shop", pComponent });
	

	return S_OK;
}

void CInteraction_Item_Info::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CInteraction_Item_Info::Change_State(void)
{
}

void CInteraction_Item_Info::Frame_Check(const _float& fTimeDelta)
{
}

void CInteraction_Item_Info::Face_Check(void)
{
	// 플레이어 Pos 받아 MasterPos와 구충돌
	_vec3 vPlayerPos;
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	vPlayerPos;

	_float x = vPlayerPos.x;
	_float z = vPlayerPos.z;
	_float radius1 = 4.0f;

	_float x1 = m_vMasterPos.x;
	_float z1 = m_vMasterPos.z;
	_float radius2 = 4.0f;

	_float dx = x - x1;
	_float dz = z - z1;
	_float distance = sqrt(dx * dx + dz * dz);

	if (distance < radius1 + radius2)
	{
		m_bFace = true;
		m_bItemInfo_On = true;
	}
	else
	{
		m_bFace = false;
		m_bItemInfo_On = false;
	}
}

void CInteraction_Item_Info::Text_Print(void)
{
}

void CInteraction_Item_Info::Rooting_Item()
{
	if ((m_bFace == true) && (CKeyMgr::Get_Instance()->Key_Pressing(KEY_E)))
	{	
		_int iTemp_Space_Check = m_pReceive_PlayerInven->Find_Empty_SpaceSlot();

		if (iTemp_Space_Check > (-1))
		{
			if (m_eInter_Target == TARGET_ITEM_SHOP)
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
				_int iTemp_Player_Money = pPlayer->Get_Player_Info().Money;

				if (iTemp_Player_Money > m_eRecieveInfo.iPrice)
				{
					m_bDead = true;
					pPlayer->Set_Player_Money(MONEY_DECREASE, m_eRecieveInfo.iPrice);
				}
			}
			else if (m_eInter_Target == TARGET_ITEM_DROP)
			{
				m_bDead = true;
			}
		}
	}
}

CInteraction_Item_Info* CInteraction_Item_Info::Create(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _eTarget, _vec3 vPos, ITEM_INFO _eInfo)
{
	CInteraction_Item_Info* pInstance = new CInteraction_Item_Info(pGraphicDev);

	pInstance->m_eInter_Target = _eTarget;
	pInstance->m_vMasterPos = vPos;
	pInstance->m_eRecieveInfo = _eInfo;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInteraction_Item_Info::Set_Receive_PlayerInven(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_pReceive_PlayerInven = pPlayer->Get_InvenInfo();
}


void CInteraction_Item_Info::Free(void)
{
	__super::Free();
}
