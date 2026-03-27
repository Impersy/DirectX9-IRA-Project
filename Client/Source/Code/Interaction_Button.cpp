#include "stdafx.h"
#include "..\Header\Interaction_Button.h"
#include "Export_Function.h"
#include "Player.h"
#include "Effect_Ui_BlackBar.h"

#include "Effect_Ui_TextCallByButton.h"
#include "Effect_Ui_ImgCallByButton.h"
#include "Effect_Ui_BalloonByRadius.h"

#include "Npc_RerollLever.h"
#include "KeyMgr.h"

#include "OpenJar.h"
#include "MovingObj.h"
#include "SpriteObj.h"
#include "SoundMgr.h"
#include "Stage_Regen.h"

CInteraction_Button::CInteraction_Button(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteraction(pGraphicDev), m_bFace(false), m_bPlayer_Dialog(false)
{
}

CInteraction_Button::CInteraction_Button(const CInteraction_Button& rhs)
	: CInteraction(rhs), m_bFace(rhs.m_bFace), m_bPlayer_Dialog(rhs.m_bPlayer_Dialog)
{
}

CInteraction_Button::~CInteraction_Button()
{
	Free();
}

HRESULT CInteraction_Button::Ready_GameObject(void)
{
	Add_Component();
	m_eInter_Type = INTER_BUTTON;

	Set_Receive_PlayerInven();

	return S_OK;
}

_int CInteraction_Button::Update_GameObject(const _float& fTimeDelta)
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

	m_fDelay += m_fDelay_Max * fTimeDelta;
	if (m_fDelay > m_fDelay_Max)
	{
		m_fDelay = m_fDelay_Max;
	}

	if (m_bFace && m_bTextDeadCheck && ((m_eInter_Target == TARGET_ITEM_DROP || m_eInter_Target == TARGET_ITEM_SHOP)))
	{
		m_bDead = true;
	}

	if (m_bFace && m_bTextDeadCheck && ((m_eInter_Target == TARGET_POT || m_eInter_Target == TARGET_REGEN_STONE)))
	{
		m_bDead = true;
	}

	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_bPlayer_Dialog = pPlayer->Get_bDialog_On();

	if (false == m_bPlayer_Dialog)
	{
		Face_Check();
	}

	if (m_bFace)
	{
		Key_Press_Check();
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CInteraction_Button::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CInteraction_Button::Render_GameObject()
{	
	if (m_bFace && !m_bPlayer_Dialog)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pTextureCom->Set_Texture((_uint)m_fFrame);

		_matrix matCamWorld;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

		Ui_Print();

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
		Reset_Proj_Matrix();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		Text_Check();
	}
	


}

HRESULT CInteraction_Button::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Interaction_Button"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Interaction_Button", pComponent });

	return S_OK;
}

void CInteraction_Button::Change_State(void)
{
}

void CInteraction_Button::Frame_Check(const _float& fTimeDelta)
{

}

void CInteraction_Button::Ui_Print(void)
{	
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	matWorld._11 = 2.7f;
	matWorld._22 = 2.7f;
	matWorld._41 = 5.8f;
	matWorld._42 = -4.1f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CInteraction_Button::Text_Check(void)
{	
	if (m_eInter_Target == TARGET_ITEM_DROP)
	{
		_tchar	m_szString_Temp_3[128] = L"아이템 획득";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_3, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_ITEM_SHOP)
	{
		_tchar	m_szString_Temp_2[128] = L"아이템 구매";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_2, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_SUPPLYCREW)
	{
		_tchar	m_szString_Temp[128] = L"대화 - 상점 꼬마";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_BLACKSMITH)
	{
		_tchar	m_szString_Temp[128] = L"대화 - 강화 장인";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_REGEN_STONE)
	{
		_tchar	m_szString_Temp_4[128] = L"유적 문양 활성화";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_APOSTLE_COURAGE)
	{
		_tchar	m_szString_Temp_4[128] = L"용기의 가호 받기";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_APOSTLE_NIGHT)
	{
		_tchar	m_szString_Temp_4[128] = L"밤의 가호 받기";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		g_bPot_To_Regen = true;
	}
	else if (m_eInter_Target == TARGET_APOSTLE_TIME)
	{
		if (g_eCurScene == SCENE_MAINLOBBY) {
			_tchar	m_szString_Temp_4[128] = L"대화하기";
			Engine::Render_Font(L"Font_Title", m_szString_Temp_4, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
		else {
			_tchar	m_szString_Temp_4[128] = L"대화하기";
			Engine::Render_Font(L"Font_Title", m_szString_Temp_4, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
	}
	else if (m_eInter_Target == TARGET_RABBIT_BLUE)
	{
		_tchar	m_szString_Temp_4[128] = L"대화 - 파란 토끼";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_RABBIT_PINK)
	{
		_tchar	m_szString_Temp_4[128] = L"대화 - 분홍 토끼";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_SUPPLY_BIGCAT)
	{
		_tchar	m_szString_Temp_4[128] = L"고양이 쓰다듬기";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_SUPPLY_BACKPACK)
	{
		_tchar	m_szString_Temp_4[128] = L"몰래 가방 열기";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_REROLL_REVER)
	{
		_tchar	m_szString_Temp_4[128] = L"상점 레버 작동";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_POT)
	{
		_tchar	m_szString_Temp_4[128] = L"항아리 열어보기";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
}

void CInteraction_Button::Face_Check(void)
{	
	// 플레이어 Pos 받아 MasterPos와 구충돌
	_vec3 vPlayerPos;
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	_float x = vPlayerPos.x;
	_float z = vPlayerPos.z;
	_float radius1 = 3.0f;

	vPlayerPos;

	_float x1 = m_vMasterPos.x;
	_float z1 = m_vMasterPos.z;
	_float radius2 = 3.0f;

	if ((m_eInter_Target == TARGET_POT) || (m_eInter_Target == TARGET_REGEN_STONE))
	{
		radius2 = 10.f;
	}

	_float dx = x - x1;
	_float dz = z - z1;
	_float distance = sqrt(dx * dx + dz * dz);

	if (distance < radius1 + radius2)
	{
		m_bFace = true;
		if (m_bBallonCreate == false)
		{
			if ((m_eInter_Target == TARGET_BLACKSMITH) || (m_eInter_Target == TARGET_SUPPLYCREW) || (m_eInter_Target == TARGET_APOSTLE_TIME) 
				|| (m_eInter_Target == TARGET_APOSTLE_COURAGE) || (m_eInter_Target == TARGET_APOSTLE_NIGHT) || (m_eInter_Target == TARGET_RABBIT_BLUE) || (m_eInter_Target == TARGET_RABBIT_PINK))
			{
				Create_Balloon();
				m_bBallonCreate = true;
			}
		}
	}
	else
	{
		m_bFace = false;
		m_bTextEffect_On = false;
	}
}

void CInteraction_Button::Key_Press_Check(void)
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_bPlayer_Dialog = pPlayer->Get_bDialog_On();

	if (m_bPlayer_Dialog == false)
	{
		m_bBlackBar_On = false;
	}

	if ((m_bPlayer_Dialog == false) && m_bFace && (CKeyMgr::Get_Instance()->Key_Down(KEY_E)))
	{
		if ((m_eInter_Target == TARGET_BLACKSMITH) || (m_eInter_Target == TARGET_SUPPLYCREW) || (m_eInter_Target == TARGET_APOSTLE_TIME))
		{
			pPlayer->Set_Player_Dialog_Target(m_eInter_Target);
			pPlayer->Set_bDialog_On(true);

			if (m_bBlackBar_On == false)
			{
				m_bBlackBar_On = true;
			}
			if ((m_eInter_Target == TARGET_APOSTLE_TIME) && (m_bTextEffect_On == false))
			{	
				Create_BlackBar();
				m_bTextEffect_On = true;
				Create_TextEffect();
			}
		}
		else if ((m_eInter_Target == TARGET_REROLL_REVER) && (m_bTextEffect_On == false))
		{	
			//CNpc_RerollLever* pNpc_RerollLever = dynamic_cast<CNpc_RerollLever*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Npc_RerollLever"));
			//pNpc_RerollLever->Create_Item_6();

			m_bTextEffect_On = true;
			Create_TextEffect();
		}
		else if (m_eInter_Target == TARGET_POT)
		{
			m_bTextEffect_On = true;
			COpenJar* pOpenJar = dynamic_cast<COpenJar*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Spr_TreasureJar_Rare_Head_0"));
			pOpenJar->Set_IsOpened(true);

			Create_TextEffect();
			m_bDead = true;
		}
		else if (m_eInter_Target == TARGET_REGEN_STONE)
		{
			m_bTextEffect_On = true;
			CScene* pScene = Engine::Get_Scene();
			dynamic_cast<CStage_Regen*>(pScene)->StartWave();
			//CMovingObj* pTemple = dynamic_cast<CMovingObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Temple_0"));
			//pTemple->Set_IsActivated(true);
			//pTemple = dynamic_cast<CMovingObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Temple_0|Clone0"));
			//pTemple->Set_IsActivated(true);
			//pTemple = dynamic_cast<CMovingObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Temple_1"));
			//pTemple->Set_IsActivated(true);
			//pTemple = dynamic_cast<CMovingObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Temple_0|Clone0"));
			//pTemple->Set_IsActivated(true);

			//CSpriteObj* pTempleHead = dynamic_cast<CSpriteObj*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Spr_Temple_Head_0"));
			//pTemple->Set_IsActivated(true);
			Create_TextEffect();
			m_bDead = true;
		}
		else if((m_eInter_Target != TARGET_ITEM_DROP) && (m_eInter_Target != TARGET_ITEM_SHOP))
		{
			if (m_bTextEffect_On == false)
			{	
				m_bTextEffect_On = true;
				Create_TextEffect();
			}
		}

	}

	if ((m_eInter_Target == TARGET_ITEM_DROP) && (m_bTextEffect_On == false))
	{	
		_int iTemp_Space_Check = m_pReceive_PlayerInven->Find_Empty_SpaceSlot();

		if (m_bFace && (CKeyMgr::Get_Instance()->Key_Pressing(KEY_E)))
		{
			if (iTemp_Space_Check > (-1))
			{
				m_bTextEffect_On = true;
				Create_TextEffect();
			}
			else if (iTemp_Space_Check == (-1))
			{	
				m_bTextEffect_On = false;
				Create_TextEffect_Rooting_False();
			}

		}
	}

	if ((m_eInter_Target == TARGET_ITEM_SHOP) && (m_bTextEffect_On == false))
	{	
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		_int iTemp_Player_Money = pPlayer->Get_Player_Info().Money;
		_int iTemp_Space_Check = m_pReceive_PlayerInven->Find_Empty_SpaceSlot();

		if (m_bFace && (CKeyMgr::Get_Instance()->Key_Pressing(KEY_E)))		// 이쪽 키다운으로 바꿔야할것으로 생각됨 
		{	
			if (iTemp_Space_Check > (-1))
			{
				if (iTemp_Player_Money > m_eItemInfo.iPrice)
				{
					m_bTextEffect_On = true;
					Create_TextEffect();
				}
				else if (iTemp_Player_Money < m_eItemInfo.iPrice)
				{
					m_bTextEffect_On = false;
					Create_TextEffect_False();
				}
			}
			else if (iTemp_Space_Check == (-1))
			{	
				m_bTextEffect_On = false;
				Create_TextEffect_Rooting_False();
			}
		}
	}

	if ((m_eInter_Target == TARGET_RABBIT_BLUE) || (m_eInter_Target == TARGET_RABBIT_PINK) || (m_eInter_Target == TARGET_SUPPLY_BIGCAT) || (m_eInter_Target == TARGET_SUPPLY_BACKPACK))
	{
		if (m_bFace && (CKeyMgr::Get_Instance()->Key_Pressing(KEY_E)))
		{	
			if (m_eInter_Target == TARGET_SUPPLY_BACKPACK)
			{
				pPlayer->Set_Player_Money(MONEY_INCREASE, 100);
				m_bDead = true;

			}
			else if ((m_eInter_Target == TARGET_RABBIT_PINK) || (m_eInter_Target == TARGET_RABBIT_BLUE))
			{
				pPlayer->Set_Player_Money(MONEY_INCREASE, 100);
				m_bDead = true;

			}
			else
			{
				m_bDead = true;
			}
		}
	}
}

void CInteraction_Button::Create_BlackBar(void)
{
	CLayer* pUILayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Ui_BlackBar::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pUILayer->Add_BulletObject(pGameObject);
}

void CInteraction_Button::Create_TextEffect_False(void)
{
	CLayer* pUILayer = Engine::Get_Layer(L"Layer_GameLogic");

	CSoundMgr::Get_Instance()->PlaySound(L"UI_Buy Fail.wav", CSoundMgr::UI_INTERACTION, 0.8f);

	CGameObject* pGameObject;
	pGameObject = CEffect_Ui_TextCallByButton::Create_Buy_False(m_pGraphicDev, TARGET_BUY_FALSE);
	if (pGameObject == nullptr)
		return;
	pUILayer->Add_BulletObject(pGameObject);
}

void CInteraction_Button::Create_TextEffect_Rooting_False(void)
{
	CLayer* pUILayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;
	pGameObject = CEffect_Ui_TextCallByButton::Create_Rooting_False(m_pGraphicDev, TARGET_ROOTING_FALSE);
	if (pGameObject == nullptr)
		return;
	pUILayer->Add_BulletObject(pGameObject);
}


void CInteraction_Button::Create_TextEffect(void)
{
	CLayer* pUILayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	if ((m_eInter_Target == TARGET_ITEM_DROP) || (m_eInter_Target == TARGET_ITEM_SHOP))
	{
		pGameObject = CEffect_Ui_TextCallByButton::Create_And_SaveItemInfo(m_pGraphicDev, m_eInter_Target, m_pTransformCom->m_vInfo[INFO_POS], m_eItemInfo);
	}
	else
	{
		if (m_eInter_Target != TARGET_APOSTLE_TIME)
			pGameObject = CEffect_Ui_TextCallByButton::Create(m_pGraphicDev, m_eInter_Target, m_pTransformCom->m_vInfo[INFO_POS]);
		else {
			pGameObject = nullptr;
		}
	}

	if (pGameObject == nullptr)
		return;

	pUILayer->Add_BulletObject(pGameObject);

	if (((m_eInter_Target == TARGET_ITEM_DROP) || (m_eInter_Target == TARGET_ITEM_SHOP)) && (m_eItemInfo.iItemNumber > 2))
	{
		pGameObject = CEffect_Ui_ImgCallByButton::Create(m_pGraphicDev, m_eItemInfo.iItemNumber);
		if (pGameObject == nullptr)
			return;
		pUILayer->Add_BulletObject(pGameObject);
	}

	//if (m_eInter_Target == TARGET_REROLL_REVER)
	//{
	//	pGameObject = CEffect_Ui_TextCallByButton::Create(m_pGraphicDev, m_eInter_Target, m_pTransformCom->m_vInfo[INFO_POS]);
	//}

	m_bTextDeadCheck = true;
}

void CInteraction_Button::Create_Balloon(void)
{
	if (g_eCurScene == SCENE_MAINLOBBY)
		return;

	CLayer* pUILayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Ui_BalloonByRadius::Create(m_pGraphicDev, m_eInter_Target, m_vMasterPos);

	if (pGameObject == nullptr)
		return;

	pUILayer->Add_BulletObject(pGameObject);
}

CInteraction_Button* CInteraction_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _eTarget, _vec3 vPos)
{
	CInteraction_Button* pInstance = new CInteraction_Button(pGraphicDev);

	pInstance->m_eInter_Target = _eTarget;
	pInstance->m_vMasterPos = vPos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CInteraction_Button* CInteraction_Button::Create_And_SaveItemInfo(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _eTarget, _vec3 vPos, ITEM_INFO _eInfo)
{	
	CInteraction_Button* pInstance = new CInteraction_Button(pGraphicDev);

	pInstance->m_eInter_Target = _eTarget;
	pInstance->m_vMasterPos = vPos;
	pInstance->m_eItemInfo = _eInfo;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CInteraction_Button::Set_Receive_PlayerInven(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_pReceive_PlayerInven = pPlayer->Get_InvenInfo();
}

void CInteraction_Button::Free(void)
{
	__super::Free();
}
