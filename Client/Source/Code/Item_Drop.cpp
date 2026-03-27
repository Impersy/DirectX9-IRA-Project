#include "stdafx.h"
#include "..\Header\Item_Drop.h"
#include "Export_Function.h"

#include "Player.h"
#include "Interaction_Button.h"
#include "Interaction_Item_Info.h"

#include "InventoryMgr.h"
#include "KeyMgr.h"

#include "Effect_Jar_Item_Drop.h"
#include "SoundMgr.h"

CItem_Drop::CItem_Drop(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev), m_bFace(false), m_vDropPos(nullptr)
{
	My_Item_INFO.resize(1);
}

CItem_Drop::CItem_Drop(const CItem_Drop& rhs)
	: CItem(rhs), m_bFace(false), m_vDropPos(rhs.m_vDropPos)
{
}

CItem_Drop::~CItem_Drop()
{
	Free();

	if (m_vDropPos)
	{
		delete m_vDropPos;
		m_vDropPos = nullptr;
	}
}


HRESULT CItem_Drop::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->Set_Pos(m_vMasterPos.x, m_vMasterPos.y, m_vMasterPos.z);
	m_pTransformCom->UpdatePos_OnWorld();
	if (m_iRecieveNumber != 6)
	{
		m_pTransformCom->Set_Scale(3.f, 3.f, 1.f);
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));
	}
	else
	{
		m_pTransformCom->Set_Scale(5.f, 5.f, 1.f);
	}

	//_int iTemp_ItemNum = ((rand() % 9) + 4);

	//My_Item_INFO[0] = INFO[iTemp_ItemNum];

	Item_Num_Check();

	//m_vDropPos = new _vec3();
	//*m_vDropPos = m_vMasterPos;

	Set_Receive_PlayerInven();

	if (m_iRecieveNumber == 6)
		Create_JarDrop_Effect();

	return S_OK;
}

_int CItem_Drop::Update_GameObject(const _float& fTimeDelta)
{
	if (m_iRecieveNumber != 6)
	{
		if (!m_bTest)
		{
			Create_Active_Button_Effect();
			m_bTest = true;
		}
	}
	else
	{
		if (!m_bTest && m_bOnGround)
		{
			Create_Active_Button_Effect();
			m_bTest = true;
		}
	}



	Face_Check();
	Rooting_Drop_Item();

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
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_INTERACTION);
		CSoundMgr::Get_Instance()->PlaySound(L"UI_Get item.wav", CSoundMgr::UI_INTERACTION, 0.9f);
		return OBJ_DEAD;
	}

	if (6 != m_iRecieveNumber)
	{
		Angle_Update();

		if (m_vMasterPos.y >= 3.f && (m_bDropOn == false))
		{
			Update_Speed(fTimeDelta);

			if (m_fJumpTime > 0)
			{
				m_vMasterPos.x += (m_DirectionRand.x * m_fJumpPower * fTimeDelta);
				m_vMasterPos.y += m_fJumpPower * fTimeDelta;
				m_vMasterPos.z += (m_DirectionRand.z * m_fJumpPower * fTimeDelta);
				m_fJumpTime -= fTimeDelta;

			}
			else
			{
				m_vVelocity += m_vAcceleration * fTimeDelta;
				m_vMasterPos += (m_vVelocity * fTimeDelta) * 0.1;

				if (m_vMasterPos.y <= 3.f)
				{
					m_vMasterPos.y = 3.f;
					m_bDropOn = true;
				}
			}
			m_pTransformCom->Set_Pos(m_vMasterPos.x, m_vMasterPos.y, m_vMasterPos.z);
			m_pTransformCom->UpdatePos_OnWorld();
		}
	}
	else
	{
		_vec3 vTargetPos = { 403.33, 4.5f, 366.91 };
		if (D3DXVec3Length(&(vTargetPos - m_pTransformCom->m_vInfo[INFO_POS])) < 3.f)
			m_bOnGround = true;

		if (!m_bOnGround)
		{
			m_fAngle = -200.f * fTimeDelta;
			m_fSpeed = 8.f;
			m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(m_fAngle));
			m_vDir = vTargetPos - m_pTransformCom->m_vInfo[INFO_POS];
			D3DXVec3Normalize(&m_vDir, &m_vDir);
			m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;
			m_pTransformCom->UpdatePos_OnWorld();
		}
	}


	//Frame_Check(fTimeDelta);
	SetUp_OnTerrain();
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return OBJ_NOEVENT;
}

void CItem_Drop::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CItem_Drop::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CItem_Drop::Add_Component(void)
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

void CItem_Drop::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CItem_Drop::Change_State(void)
{

}

void CItem_Drop::Frame_Check(const _float& fTimeDelta)
{
}

void CItem_Drop::Face_Check(void)
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

	if (m_iRecieveNumber == 6)
	{
		x1 = m_pTransformCom->m_vInfo[INFO_POS].x;
		z1 = m_pTransformCom->m_vInfo[INFO_POS].z;
	}

	_float dx = x - x1;
	_float dz = z - z1;
	_float distance = sqrt(dx * dx + dz * dz);

	if (distance < radius1 + radius2)
		m_bFace = true;
	else
		m_bFace = false;
}

void CItem_Drop::Item_Num_Check(void)
{	

	My_Item_INFO[0] = INFO[m_iRecieveNumber];
	m_fFrame = (My_Item_INFO[0].iItemNumber - 2);
}

void CItem_Drop::RotationDrop(const _float& fTimeDelta)
{
}

void CItem_Drop::Create_JarDrop_Effect(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Jar_Item_Drop* pEffect = CEffect_Jar_Item_Drop::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

CItem_Drop* CItem_Drop::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CItem_Drop* pInstance = new CItem_Drop(pGraphicDev);

	pInstance->m_vMasterPos = vPos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CItem_Drop* CItem_Drop::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _itemNum)
{
	CItem_Drop* pInstance = new CItem_Drop(pGraphicDev);

	pInstance->m_vMasterPos = vPos;
	pInstance->m_iRecieveNumber = _itemNum;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CItem_Drop::Create_Drop_Effect(void)
{
	////TIME_STOP_VOID

	//CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	//CGameObject* pGameObject = nullptr;
	//pGameObject = CEffect_Item_Drop::Create(m_pGraphicDev, m_vDropPos);
	//if (pGameObject == nullptr)
	//	return;
	//pGameLogicLayer->Add_BulletObject(pGameObject);

	//m_bDropEffect_On = true;

}

void CItem_Drop::Create_Ground_Effect(void)
{
	//CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	//CGameObject* pGameObject = nullptr;
	//pGameObject = CEffect_Drop_Ground::Create(m_pGraphicDev, m_vDropPos);
	//if (pGameObject == nullptr)
	//	return;
	//pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CItem_Drop::Create_Active_Button_Effect(void)
{
	//TIME_STOP_VOID

	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;

	m_SendPos = m_pTransformCom->m_vInfo[INFO_POS];

	pGameObject = CInteraction_Button::Create_And_SaveItemInfo(m_pGraphicDev, TARGET_ITEM_DROP, m_SendPos, My_Item_INFO[0]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

	if (6 != m_iRecieveNumber)
		pGameObject = CInteraction_Item_Info::Create(m_pGraphicDev, TARGET_ITEM_DROP, m_SendPos, My_Item_INFO[0]);
	else
	{
		_vec3 vTargetPos = { 403.33, 14.5f, 368.91 };
		pGameObject = CInteraction_Item_Info::Create(m_pGraphicDev, TARGET_ITEM_DROP, vTargetPos, My_Item_INFO[0]);
	}

	
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

}

void CItem_Drop::Angle_Update(void)
{
	if (m_bAngleOn == false)
	{
		_float m_fAngle_X = (float)rand() / RAND_MAX * D3DX_PI * 2.0f;
		_float m_fAngle_Y = (float)rand() / RAND_MAX * D3DX_PI * 2.0f;
		m_DirectionRand = { sin(m_fAngle_X), 0.0f, cos(m_fAngle_Y) };
		m_bAngleOn = true;
	}
}

void CItem_Drop::Update_Speed(const _float& fTimeDelta)
{
	m_vVelocity += m_DirectionRand * m_fSpeed * fTimeDelta;
	m_vAcceleration += -m_vVelocity * m_fDrag + m_vGravity;
}

void CItem_Drop::Rooting_Drop_Item()
{	
	if (m_bFace == true)
	{
		_int iTemp_Space_Check = m_pReceive_PlayerInven->Find_Empty_SpaceSlot();

		if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_E))
		{
			if (iTemp_Space_Check > (-1))
			{
			m_pReceive_PlayerInven->Rooting_Item_In_Inven(My_Item_INFO[0].iItemNumber);
			m_bDead = true;
			}

		}

	}
}

void CItem_Drop::Set_Receive_PlayerInven(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_pReceive_PlayerInven = pPlayer->Get_InvenInfo();
}

void CItem_Drop::Free(void)
{
	__super::Free();
}