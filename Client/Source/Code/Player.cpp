#include "stdafx.h"
#include "..\Header\Player.h"
#include "Export_Function.h"
#include "Ghost.h"
#include "SylphArrow.h"
#include "SylphChargeArrow.h"
#include "SylphBow.h"
#include "SylphBowPair.h"
#include "Effect_Player_Bow_Pulse.h"
#include "Effect_Player_Bow_Charge.h"
#include "Effect_Player_Charge_Gauge.h"
#include "Effect_Player_Foot.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "Tif_Circle.h"
#include "Tif_Ani.h"
#include "Apostle_Skill_UI_Circle.h"
#include "Apostle_Skill_UI_illust.h"
#include "Terrain.h"
#include "TP_Bot.h"
#include "TP_Top.h"
#include "Fire_Pulse.h"
#include "Night_Pulse.h"
#include "Night_Charge.h"
#include "Fire_Charge.h"
#include "Time_Pulse.h"
#include "Time_Charge.h"
#include "Time_Big.h"
#include "StaticCamera.h"
#include <SoundMgr.h>

#include "Frame_LevelUp.h"



CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pDeathTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_pSphereBufferCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_pInventory(nullptr)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: Engine::CGameObject(rhs)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pDeathTextureCom(rhs.m_pDeathTextureCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pCalculatorCom(rhs.m_pCalculatorCom)
	, m_pColliderCom(rhs.m_pColliderCom)
	, m_pSphereBufferCom(rhs.m_pSphereBufferCom)
	, m_pInventory(rhs.m_pInventory)
{
}

CPlayer::~CPlayer()
{	

	Save_Player_InvenInfo();

	if (m_pInventory)
	{
		delete m_pInventory;
		m_pInventory = nullptr;
	}
}

HRESULT CPlayer::Ready_GameObject(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSpeed = 25.f * PUBLIC_SCALE;
	m_fDashSpeed = 0.8f * PUBLIC_SCALE;

	m_pTransformCom->Set_Scale_Ratio({ 2.4f * 1.66f * PUBLIC_SCALE, 3.01f * 1.66f * PUBLIC_SCALE, 1.f });


	m_pInventory = new CInventoryMgr;
	Load_Player_InvenInfo();
	m_bItemUi_On = false;
	m_bDialog_On = false;

	m_pColliderCom->Set_Radius(2.f);

	m_tInfo = g_pInfo;
	m_Bow_Type = g_Bow_Type;
	m_Arrow_Type = g_Arrow_Type;
	m_Apostle_Type = g_Apostle_Type;
	m_Apostle_Proof = g_Apostle_Proof;

	m_tPlayerLvSk = g_pLvsk;


	if (g_eCurScene == SCENE_MAINLOBBY) {
		m_pTransformCom->m_vInfo[INFO_POS] = { 383.f,5.f * PUBLIC_SCALE,300.f };
	}
	else if (g_eCurScene == SCENE_TUTORIAL) {
		m_pTransformCom->m_vInfo[INFO_POS] = TUTORIAL_TELEPORT;
	}
	else if (g_eCurScene == SCENE_BOSS2) {
		m_pTransformCom->m_vInfo[INFO_POS] = { 76.f,5.f * PUBLIC_SCALE,60.f };
	}
	else if (g_eCurScene == SCENE_BOSS2) {
		m_pTransformCom->m_vInfo[INFO_POS] = DUNGEONLOBBY_TELEPORT;
	}
	else
		m_pTransformCom->m_vInfo[INFO_POS] = g_vNextPos;


	if (g_eCurScene == SCENE_TUTORIAL || g_eCurScene == SCENE_MAINLOBBY || g_eCurScene == SCENE_DUNGEONLOBBY) {
		m_Is_Render = false;
	}




	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	m_pTransformCom->UpdatePos_OnWorld();
	
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_PLAYER, this);

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	// Time Stop 예외 처리용 실제 TimeDelta 참조
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	// 레이어에 저장된 실제 TimeDelta를 사용해 플레이어 업데이트
	_float Time = pGameLogicLayer->Get_TimeDelta();
	
	



	if (m_Bow_Type == BOW_SYLPH) {
	
		m_Fire_Speed = 2.f;
	}
	else if (m_Bow_Type == BOW_FIRE) {
		
		m_Fire_Speed = 3.f;
	}
	else if (m_Bow_Type == BOW_NIGHT) {
		
		m_Fire_Speed = 3.5f;
	}
	else if (m_Bow_Type == BOW_TIME) {
		
		m_Fire_Speed = 6.f;
	}



	if (m_fLanding_Cnt == 0)
		m_Scene_Time += 2.f * Time * 0.6f;

	if (m_Scene_Time > 2.f) {
		m_Scene_Time = 0;
		m_Scene_Landing = true;

	}

	if (g_eCurScene == SCENE_TUTORIAL || g_eCurScene == SCENE_MAINLOBBY || g_eCurScene == SCENE_DUNGEONLOBBY) {

		if (m_Scene_Landing) {
			Teleport();
			m_fLanding_Cnt++;
			m_Scene_Landing = false;
		}
	}
	

	if (m_tInfo.Nowhp == 0) {
		m_iState = DEATH;
		m_Is_Fire_Arrow = false;
		m_Is_Effect_Charge_Arrow = false;
		m_Is_Charge_Arrow = false;
		m_bImmuned = true;
		m_Is_Run = false;
		m_bHit = false;

		if (m_Bow_Type == BOW_SYLPH) {
			CEffect_Player_Bow_Charge* pObject = dynamic_cast<CEffect_Player_Bow_Charge*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Bow_Charge_Effect"));
			if (pObject != nullptr) {
				pObject->Set_Dead();
				m_Charge_Effect_Cnt = 0;
			}

			m_Fire_Speed = 2.f;
		}
		else if (m_Bow_Type == BOW_FIRE) {
			CFire_Charge* pObject = dynamic_cast<CFire_Charge*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Bow_Charge_Effect"));
			if (pObject != nullptr) {
				pObject->Set_Dead();
				m_Charge_Effect_Cnt = 0;
			}
			m_Fire_Speed = 3.f;
		}
		else if (m_Bow_Type == BOW_NIGHT) {
			CNight_Charge* pObject = dynamic_cast<CNight_Charge*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Bow_Charge_Effect"));
			if (pObject != nullptr) {
				pObject->Set_Dead();
				m_Charge_Effect_Cnt = 0;
			}
			m_Fire_Speed = 3.5f;
		}
		else if (m_Bow_Type == BOW_TIME) {
			CTime_Charge* pObject = dynamic_cast<CTime_Charge*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Bow_Charge_Effect"));
			if (pObject != nullptr) {
				pObject->Set_Dead();
				m_Charge_Effect_Cnt = 0;
			}
			m_Fire_Speed = 6.f;
		}
	}


	/*if (m_bHit == true && m_bImmuned == false) {
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MOVE_LOBBY);
		CSoundMgr::Get_Instance()->PlaySound(L"Player_Attacked.wav", CSoundMgr::MOVE_LOBBY, 0.65f);
		Hp_Down();
		m_HitBlend = true;
		m_bImmuned = true;
	}*/


	if (m_iState == RUN) {
		if (g_eCurScene == SCENE_MAINLOBBY || g_eCurScene == SCENE_BOSS2)
			CSoundMgr::Get_Instance()->PlaySound(L"Move_Lobby.wav", CSoundMgr::MOVE_LOBBY, 0.55f);
		else {
			CSoundMgr::Get_Instance()->PlaySound(L"Move_Forest.wav", CSoundMgr::MOVE_FOREST, 0.55f);
		}
	}


	Frame_Manage(Time);


	m_pTransformCom->m_vInfo[INFO_POS].y = 5.f * PUBLIC_SCALE + jump * PUBLIC_SCALE;


	

	if (((false == m_bItemUi_On) && (false == m_bBigMapUi_On)) && (false == m_bLevelUpUi_On))
	{
		Key_Input(Time);
	}

	if ((GetAsyncKeyState('R') & 0x8000) && (m_bLevelUpUi_On == false))
	{	
		m_bLevelUpUi_On = true;
		Player_Level_Up();
	}

	Update_State();

	if (m_Is_Dash == true) {
		m_bImmuned = true;
		Dash(Time);
	}

	if (m_Is_Fire_Arrow == true && m_Is_Effect_Charge_Arrow == false) {
		m_Fire_Frame += m_Fire_Init * Time * m_Fire_Speed;
		if (m_Fire_Frame > m_Fire_Init){
			Fire_Arrow();
			m_Fire_Frame = 0.f;
		}
	}

	if (m_Is_Effect_Charge_Arrow == true) {
		m_fSpeed = 10.f * PUBLIC_SCALE;
		Effect_Charge_Arrow();
	}
	
	
	if (m_Is_Run == true) {
		m_FootFrame += 1.f * Time * 2.f;
		if (m_FootFrame > 1.f) {
			m_FootFrame = 0.f;
			Effect_Foot_Step_Smoke();
		}
	}
	

	
	__super::Update_GameObject(Time);

	if (m_HitBlendRender == false)
		Engine::Add_RenderGroup(RENDER_ALPHATEST, this);
	
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_PLAYER, this);
	
	

	return 0;
}

void CPlayer::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	
	if (m_HitBlendRender == true || m_Is_Teleport == true) {
		_vec3	vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		Compute_ViewZ(&vPos);

		Engine::Add_RenderGroup(RENDER_ALPHA, this);
	}
		
}

void CPlayer::Render_GameObject()
{	
	Set_Player_Money_Print(); 
	Set_Player_Gem_Print();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	if (m_Is_Render == false)
		return;

	if (m_HitBlendRender) {
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(AlphaValue, R, G, B));
	
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		
	}


	if (m_iState == STAND) {

		if (m_iAngleState == ANGLE_000) {
			m_pStandTextureCom[STAND_000]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_045) {
			m_pStandTextureCom[STAND_045]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_090) {
			m_pStandTextureCom[STAND_090]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_135) {
			m_pStandTextureCom[STAND_135]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_180) {
			m_pStandTextureCom[STAND_180]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_225) {
			m_pStandTextureCom[STAND_135]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_270) {
			m_pStandTextureCom[STAND_090]->Set_Texture((_uint)m_fStandFrame);
		}
		else if (m_iAngleState == ANGLE_315) {
			m_pStandTextureCom[STAND_045]->Set_Texture((_uint)m_fStandFrame);
		}
	}
	else if (m_iState == RUN) {

		if (m_iAngleState == ANGLE_000)
			m_pRunTextureCom[RUN_000]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_045)
			m_pRunTextureCom[RUN_045]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_090)
			m_pRunTextureCom[RUN_090]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_135)
			m_pRunTextureCom[RUN_135]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_180)
			m_pRunTextureCom[RUN_180]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_225)
			m_pRunTextureCom[RUN_135]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_270)
			m_pRunTextureCom[RUN_090]->Set_Texture((_uint)m_fRunFrame);

		else if (m_iAngleState == ANGLE_315)
			m_pRunTextureCom[RUN_045]->Set_Texture((_uint)m_fRunFrame);

	}
	else if (m_iState == MOVE_ATTACK) {
		if (m_iAttackAngleState == ATTACK_ANGLE_045) {
			m_pMoveAttackTextureCom[MOVE_ATTACK_045]->Set_Texture((_uint)m_fAttackFrame);
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_135) {
			m_pMoveAttackTextureCom[MOVE_ATTACK_135]->Set_Texture((_uint)m_fAttackFrame);
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_225) {
			m_pMoveAttackTextureCom[MOVE_ATTACK_135]->Set_Texture((_uint)m_fAttackFrame);
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_315) {
			m_pMoveAttackTextureCom[MOVE_ATTACK_045]->Set_Texture((_uint)m_fAttackFrame);
		}
	}
	else if (m_iState == STAND_ATTACK) {
		if (m_iAttackAngleState == ATTACK_ANGLE_045) {
			m_pStandAttackTextureCom[STAND_ATTACK_045]->Set_Texture((_uint)m_fAttackFrame);
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_135) {
			m_pStandAttackTextureCom[STAND_ATTACK_135]->Set_Texture((_uint)m_fAttackFrame);
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_225) {
			m_pStandAttackTextureCom[STAND_ATTACK_135]->Set_Texture((_uint)m_fAttackFrame);
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_315) {
			m_pStandAttackTextureCom[STAND_ATTACK_045]->Set_Texture((_uint)m_fAttackFrame);
		}

	}
	else if (m_iState == DASH) {

		if (m_iAngleState == ANGLE_000) {
			m_pDashTextureCom[DASH_045]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_045) {
			m_pDashTextureCom[DASH_045]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_090) {
			m_pDashTextureCom[DASH_045]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_135) {
			m_pDashTextureCom[DASH_045]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_180) {
			m_pDashTextureCom[DASH_135]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_225) {
			m_pDashTextureCom[DASH_135]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_270) {
			m_pDashTextureCom[DASH_135]->Set_Texture((_uint)m_fDashFrame);
		}
		else if (m_iAngleState == ANGLE_315) {
			m_pDashTextureCom[DASH_135]->Set_Texture((_uint)m_fDashFrame);
		}


	}
	else if (m_iState == DEATH) {
		m_pDeathTextureCom->Set_Texture((_uint)m_fDeathFrame);
	}
	else if (m_iState == TELEPORT) {
		m_pTeleportTextureCom->Set_Texture((_uint)m_fTeleportFrame);
	}
	else if (m_iState == LANDING) {
		m_pLandingTextureCom->Set_Texture((_uint)m_fLandingFrame);
	}

	m_pBufferCom->Render_Buffer();
	m_pSphereBufferCom->Render_Buffer();

	
	if (m_HitBlendRender) {
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	}


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}



void CPlayer::Teleport(void)
{
	m_iState = TELEPORT;
	m_Is_Teleport = true;
}

void CPlayer::Landing(_vec3 pos)
{
	m_iState = LANDING;

 	m_Is_Render = true;
	
	m_pTransformCom->m_vInfo[INFO_POS] = pos;

	m_pTransformCom->UpdatePos_OnWorld();

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CGameObject* pGameObject = nullptr;


	_vec3 Pos = m_pTransformCom->m_vInfo[INFO_POS];

	Pos.y += 2.1f;

	pGameObject = CTP_Bot::Create(m_pGraphicDev, Pos);

	if (pGameObject != nullptr)
		pGameLogicLayer->Add_BulletObject(pGameObject);

	pGameObject = CTP_Top::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject != nullptr)
		pGameLogicLayer->Add_BulletObject(pGameObject);

}

void CPlayer::Set_tPlayerLvSk(_int _Number)
{	
	m_tPlayerLvSk.iPlayerLevel += 1;

	if (_Number == 1)
		m_tPlayerLvSk.iSkill_Att_Plus += 1;
	if (_Number == 2)
		m_tPlayerLvSk.iSkill_Speed_Plus += 1;
	if (_Number == 3)
		m_tPlayerLvSk.iSkill_Money_Plus += 1;
	if (_Number == 4)
		m_tPlayerLvSk.iSkill_Gem_Plus += 1;
}

void CPlayer::Save_Player_InvenInfo(void)
{
	CCollisionMgr::GetInstance()->Get_ItemMgr_Info(m_pInventory);
}

void CPlayer::Load_Player_InvenInfo(void)
{
	for (int i = 0; i < 4; i++)
	{
		m_pInventory->Set_Weapon(i, CCollisionMgr::GetInstance()->Set_ItemMgr_Info(1, i));
	}
	for (int i = 0; i < 4; i++)
	{
		m_pInventory->Set_Acc(i, CCollisionMgr::GetInstance()->Set_ItemMgr_Info(2, i));
	}
	for (int i = 0; i < 10; i++)
	{
		m_pInventory->Set_Space(i, CCollisionMgr::GetInstance()->Set_ItemMgr_Info(3, i));
	}
}

void CPlayer::Set_Player_Money(MONEY_VARIATION _Money_Vari, _int _MoneyValue)
{	
	if (_Money_Vari == MONEY_INCREASE)
	{	
		m_tInfo.Money += ((m_tPlayerLvSk.iSkill_Money_Plus + 1) * _MoneyValue);
	}
	else if (_Money_Vari == MONEY_DECREASE)
	{
		m_tInfo.Money -= _MoneyValue;
	}

	m_tRecieve_Vari = _Money_Vari;

	if (_Money_Vari == MONEY_INCREASE)
	{
		m_iRecieve_Money_Value = ((m_tPlayerLvSk.iSkill_Money_Plus + 1) * _MoneyValue);

	}
	else if (_Money_Vari == MONEY_DECREASE)
	{
		m_iRecieve_Money_Value = _MoneyValue;
	}

	m_fMoney_Vari = true;
}

void CPlayer::Set_Player_Money_Print(void)
{	
	if (m_tRecieve_Vari == MONEY_INCREASE)
	{
		TCHAR Buffer_Upg[256];
		_stprintf_s(Buffer_Upg, _T("%d"), (_int)(m_iRecieve_Money_Value));

		std::wstring message;
		message += L"+ ";
		message += Buffer_Upg;

		Engine::Render_Font(L"Font_Money_02", message.c_str(), &_vec2(174.f, 176.5f), D3DXCOLOR(0.8f, 0.8f, 0.8f, (1.f - (0.025f * m_fMoney_Vari_Frame))));
	}
	else if (m_tRecieve_Vari == MONEY_DECREASE)
	{
		TCHAR Buffer_Upg[256];
		_stprintf_s(Buffer_Upg, _T("%d"), (_int)(m_iRecieve_Money_Value));

		std::wstring message;
		message += L"- ";
		message += Buffer_Upg;

		Engine::Render_Font(L"Font_Money_02", message.c_str(), &_vec2(174.f, 176.5f), D3DXCOLOR(0.8f, 0.8f, 0.8f, (1.f - (0.025f * m_fMoney_Vari_Frame))));
	}
}

void CPlayer::Set_Player_Gem(GEM_VARIATION _Gem_Vari, _int _GemValue)
{
	if (_Gem_Vari == GEM_INCREASE)
	{
		m_tInfo.Gem += ((m_tPlayerLvSk.iSkill_Gem_Plus + 1) * _GemValue);
	}
	else if (_Gem_Vari == GEM_DECREASE)
	{
		m_tInfo.Gem -= _GemValue;
	}

	m_tRecieve_Vari_GEM = _Gem_Vari;

	if (_Gem_Vari == GEM_INCREASE)
	{
		m_iRecieve_Gem_Value = ((m_tPlayerLvSk.iSkill_Gem_Plus + 1) * _GemValue);

	}
	else if (_Gem_Vari == GEM_DECREASE)
	{
		m_iRecieve_Gem_Value = _GemValue;
	}

	m_fGem_Vari = true;
}

void CPlayer::Set_Player_Gem_Print(void)
{
	if (m_tRecieve_Vari_GEM == GEM_INCREASE)
	{
		TCHAR Buffer_Upg[256];
		_stprintf_s(Buffer_Upg, _T("%d"), (_int)(m_iRecieve_Gem_Value));

		std::wstring message;
		message += L"+ ";
		message += Buffer_Upg;

		Engine::Render_Font(L"Font_Money_02", message.c_str(), &_vec2(174.f, 224.5f), D3DXCOLOR(0.8f, 0.8f, 0.8f, (1.f - (0.025f * m_fGem_Vari_Frame))));
	}
	else if (m_tRecieve_Vari_GEM == GEM_DECREASE)
	{
		TCHAR Buffer_Upg[256];
		_stprintf_s(Buffer_Upg, _T("%d"), (_int)(m_iRecieve_Gem_Value));

		std::wstring message;
		message += L"- ";
		message += Buffer_Upg;

		Engine::Render_Font(L"Font_Money_02", message.c_str(), &_vec2(174.f, 224.5f), D3DXCOLOR(0.8f, 0.8f, 0.8f, (1.f - (0.025f * m_fGem_Vari_Frame))));
	}
}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

#pragma region Player_Texture

	pComponent = m_pStandTextureCom[STAND_000] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_000"));
	if (nullptr == m_pStandTextureCom[STAND_000]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_000", pComponent });

	pComponent = m_pStandTextureCom[STAND_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_045"));
	if (nullptr == m_pStandTextureCom[STAND_045]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_045", pComponent });

	pComponent = m_pStandTextureCom[STAND_090] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_090"));
	if (nullptr == m_pStandTextureCom[STAND_090]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_090", pComponent });

	pComponent = m_pStandTextureCom[STAND_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_135"));
	if (nullptr == m_pStandTextureCom[STAND_135]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_135", pComponent });

	pComponent = m_pStandTextureCom[STAND_180] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_180"));
	if (nullptr == m_pStandTextureCom[STAND_180]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_180", pComponent });


	pComponent = m_pRunTextureCom[RUN_000] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_000"));
	if (nullptr == m_pRunTextureCom[RUN_000]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_000", pComponent });

	pComponent = m_pRunTextureCom[RUN_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_045"));
	if (nullptr == m_pRunTextureCom[RUN_045]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_045", pComponent });

	pComponent = m_pRunTextureCom[RUN_090] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_090"));
	if (nullptr == m_pRunTextureCom[RUN_090]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_090", pComponent });

	pComponent = m_pRunTextureCom[RUN_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_135"));
	if (nullptr == m_pRunTextureCom[RUN_135]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_135", pComponent });

	pComponent = m_pRunTextureCom[RUN_180] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Run_180"));
	if (nullptr == m_pRunTextureCom[RUN_180]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Run_180", pComponent });


	pComponent = m_pMoveAttackTextureCom[MOVE_ATTACK_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Move_Attack_045"));
	if (nullptr == m_pMoveAttackTextureCom[MOVE_ATTACK_045]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Move_Attack_045", pComponent });


	pComponent = m_pMoveAttackTextureCom[MOVE_ATTACK_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Move_Attack_135"));
	if (nullptr == m_pMoveAttackTextureCom[MOVE_ATTACK_135]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Move_Attack_135", pComponent });

	pComponent = m_pStandAttackTextureCom[STAND_ATTACK_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_Attack_045"));
	if (nullptr == m_pStandAttackTextureCom[STAND_ATTACK_045]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_Attack_045", pComponent });

	pComponent = m_pStandAttackTextureCom[STAND_ATTACK_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Stand_Attack_135"));
	if (nullptr == m_pStandAttackTextureCom[STAND_ATTACK_135]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Stand_Attack_135", pComponent });


	pComponent = m_pDashTextureCom[DASH_045] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Dash_045"));
	if (nullptr == m_pDashTextureCom[DASH_045]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Dash_045", pComponent });

	pComponent = m_pDashTextureCom[DASH_135] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Dash_135"));
	if (nullptr == m_pDashTextureCom[DASH_135]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Dash_135", pComponent });

	pComponent = m_pDeathTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Death"));
	if (nullptr == m_pDeathTextureCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Death", pComponent });

	pComponent = m_pTeleportTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Teleport"));
	if (nullptr == m_pTeleportTextureCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Teleport", pComponent });

	pComponent = m_pLandingTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Landing"));
	if (nullptr == m_pLandingTextureCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Landing", pComponent });


#pragma endregion


	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_ProtoComponent(L"Proto_Calculator"));
	if (nullptr == m_pLandingTextureCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	if (nullptr == m_pColliderCom) return E_FAIL;
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	pComponent = m_pSphereBufferCom = dynamic_cast<CSphereTex*>(Engine::Clone_ProtoComponent(L"Proto_SphereTex"));
	if (nullptr == m_pSphereBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_SphereTex", pComponent });

	return S_OK;
}


void CPlayer::Key_Input(const _float & fTimeDelta)
{
	if (m_iState == DEATH) {
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_L)) {
			m_Is_Fire_Arrow = false;
			m_Fire_Frame = m_Fire_Init;

			m_iState = STAND;
			m_tInfo.Nowhp = 5;
			m_bImmuned = false;
			m_fDeathFrame = 0.f;
		}
		else {
			return;
		}

	}

	if (CKeyMgr::Get_Instance()->Key_Down(KEY_1)) {
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_CHANGE);
		CSoundMgr::Get_Instance()->PlaySound(L"UI_Bow_Change.wav", CSoundMgr::BOW_CHANGE, 0.75f);
		m_Bow_Type = BOW_SYLPH;

		m_Arrow_Type = ARROW_TYPE_SYLPH;

	}
	else if (CKeyMgr::Get_Instance()->Key_Down(KEY_2)) {
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_CHANGE);
		CSoundMgr::Get_Instance()->PlaySound(L"UI_Bow_Change.wav", CSoundMgr::BOW_CHANGE, 0.75f);
		m_Bow_Type = BOW_FIRE;

		m_Arrow_Type = ARROW_TYPE_FIRE;

	}
	else if (CKeyMgr::Get_Instance()->Key_Down(KEY_3)) {
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_CHANGE);
		CSoundMgr::Get_Instance()->PlaySound(L"UI_Bow_Change.wav", CSoundMgr::BOW_CHANGE, 0.75f);
		m_Bow_Type = BOW_NIGHT;

		m_Arrow_Type = ARROW_TYPE_NIGHT;

	}
	else if (CKeyMgr::Get_Instance()->Key_Down(KEY_4)) {
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_CHANGE);
		CSoundMgr::Get_Instance()->PlaySound(L"UI_Bow_Change.wav", CSoundMgr::BOW_CHANGE, 0.75f);
		m_Bow_Type = BOW_TIME;

		m_Arrow_Type = ARROW_TYPE_TIME;

	}


	Apostle_Manage();

	


	if (CKeyMgr::Get_Instance()->Key_Down(KEY_T)) {
		Teleport();
		CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
		CGameObject* pGameObject = nullptr;

		pGameObject = CTP_Bot::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject != nullptr)
			pGameLogicLayer->Add_BulletObject(pGameObject);


		return;
	}


	if (CKeyMgr::Get_Instance()->Key_Down(KEY_L) && g_eCurScene == SCENE_MAINLOBBY && g_Game_Clear == true) {
		Teleport();
		CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
		CGameObject* pGameObject = nullptr;

		pGameObject = CTP_Bot::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject != nullptr)
			pGameLogicLayer->Add_BulletObject(pGameObject);


		return;
	}


	if (m_Is_Dash == true)
		return;

	if (m_Is_Teleport == true)
		return;

	if (m_Is_Landing == true)
		return;


	CGhost* pGhost = dynamic_cast<CGhost*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Ghost"));

	_vec3	vDir;
	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3	vRight;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);


	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	
	if (vRight.x < 0)
		vRight.x *= -1.f;

	_vec3	vLU = vDir - vRight;
	D3DXVec3Normalize(&vLU, &vLU);

	_vec3	vRU = vDir + vRight;
	D3DXVec3Normalize(&vRU, &vRU);

	_vec3	vLD = -vDir - vRight;
	D3DXVec3Normalize(&vLD, &vLD);

	_vec3	vRD = -vDir + vRight;
	D3DXVec3Normalize(&vRD, &vRD);


	
	if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_LBUTTON)){
		m_Is_Fire_Arrow = true;

		CSylphBow* pObject1 = dynamic_cast<CSylphBow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBow"));
		CSylphBowPair* pObject2 = dynamic_cast<CSylphBowPair*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBowPair"));

		if (m_Is_Effect_Charge_Arrow == false) {
			pObject1->m_bCharge = false;
			pObject2->m_bCharge = false;
		}
		

		POINT ptCursor;

		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);


		if (ptCursor.x >= WINCX*0.5 && ptCursor.y <= WINCY*0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_225;
			m_iAngleState = ANGLE_225;
		}
		else if (ptCursor.x < WINCX * 0.5 && ptCursor.y <= WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_135;
			m_iAngleState = ANGLE_135;
		}
		else if (ptCursor.x < WINCX * 0.5 && ptCursor.y > WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_045;
			m_iAngleState = ANGLE_045;
		}
		else if (ptCursor.x >= WINCX * 0.5 && ptCursor.y > WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_315;
			m_iAngleState = ANGLE_315;
		}

		if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_W)) {
			

			if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vLU * fTimeDelta * m_fSpeed));
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vRU * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * m_fSpeed));
			}
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_S)) {
			

			if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vLD * fTimeDelta * m_fSpeed));
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vRD * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * -m_fSpeed));
			}

		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
			
			m_iState = MOVE_ATTACK;
			m_pTransformCom->Move_Pos(&(-vRight * fTimeDelta * m_fSpeed));

		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
			
			m_iState = MOVE_ATTACK;
			m_pTransformCom->Move_Pos(&(vRight * fTimeDelta * m_fSpeed));

		}
		else {
			m_iState = STAND_ATTACK;
		}

	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_RBUTTON)) {

		if (!m_Is_Charge_Sound) {

			// 차지 드로우
		
			if (m_Bow_Type == BOW_SYLPH) {
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_DRAW_DULL);
				CSoundMgr::Get_Instance()->PlaySound(L"Bow_Draw_Dull.wav", CSoundMgr::BOW_DRAW_DULL, 0.75f);
			
			}
			else if (m_Bow_Type == BOW_FIRE) {
				
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_DRAW_FIRE);
				CSoundMgr::Get_Instance()->PlaySound(L"Kaff_Appear.wav", CSoundMgr::BOW_DRAW_FIRE, 0.75f);
			}
			else if (m_Bow_Type == BOW_NIGHT) {
				
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_DRAW_NIGHT);
				CSoundMgr::Get_Instance()->PlaySound(L"Light_Charge.wav", CSoundMgr::BOW_DRAW_NIGHT, 0.95f);
			}
			else if (m_Bow_Type == BOW_TIME) {
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_DRAW_TIME);
				CSoundMgr::Get_Instance()->PlaySound(L"Time_Charge.wav", CSoundMgr::BOW_DRAW_TIME, 0.1f);
			}


			m_Is_Charge_Sound = true;
		}
			

		CAMERA_CHARGE_ZOOM_IN

		CSylphBow* pObject1 = dynamic_cast<CSylphBow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBow"));
		CSylphBowPair* pObject2 = dynamic_cast<CSylphBowPair*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBowPair"));

		pObject1->m_bCharge = true;
		pObject2->m_bCharge = true;

		m_Is_Effect_Charge_Arrow = true;

		m_fChargeFrame += 1.f * fTimeDelta * 0.8f * 1.5f;
		if (m_fChargeFrame > 1.f) {
			m_Is_Charge_Arrow = true;
		}
		
		POINT ptCursor;

		GetCursorPos(&ptCursor);
		ScreenToClient(g_hWnd, &ptCursor);


		if (ptCursor.x >= WINCX * 0.5 && ptCursor.y <= WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_225;
			m_iAngleState = ANGLE_225;
		}
		else if (ptCursor.x < WINCX * 0.5 && ptCursor.y <= WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_135;
			m_iAngleState = ANGLE_135;
		}
		else if (ptCursor.x < WINCX * 0.5 && ptCursor.y > WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_045;
			m_iAngleState = ANGLE_045;
		}
		else if (ptCursor.x >= WINCX * 0.5 && ptCursor.y > WINCY * 0.5) {
			m_iAttackAngleState = ATTACK_ANGLE_315;
			m_iAngleState = ANGLE_315;
		}

		
		if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_W)) {
			
			if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vLU * fTimeDelta * m_fSpeed));
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vRU * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * m_fSpeed));
			}
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_S)) {
			
			if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vLD * fTimeDelta * m_fSpeed));
			}
			else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vRD * fTimeDelta * m_fSpeed));
			}
			else {
				m_iState = MOVE_ATTACK;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * -m_fSpeed));
			}

		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
			
			m_iState = MOVE_ATTACK;
			m_pTransformCom->Move_Pos(&(-vRight * fTimeDelta * m_fSpeed));

		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
			
			m_iState = MOVE_ATTACK;
			m_pTransformCom->Move_Pos(&(vRight * fTimeDelta * m_fSpeed));

		}
		else {
			m_iState = STAND_ATTACK;
		}

	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_W)) {
	    
		m_Is_Fire_Arrow = false;
		m_Fire_Frame = m_Fire_Init;

		if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
			if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {

			
				if (m_tInfo.NowEnergy > 0) {
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_DASH);
					CSoundMgr::Get_Instance()->PlaySound(L"Player_Dash.wav", CSoundMgr::PLAYER_DASH, 0.75f);
					pGhost->Is_Dash = true;
					m_Is_Dash = true;
					m_iState = DASH;
					m_iAngleState = ANGLE_135;
					m_pTransformCom->Move_Pos(&(vLU * fTimeDelta * m_fSpeed));
				}
			}
			else {
				m_iState = RUN;
				m_iAngleState = ANGLE_135;
				m_pTransformCom->Move_Pos(&(vLU * fTimeDelta * m_fSpeed));
			}
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
			

			if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
				
				if (m_tInfo.NowEnergy > 0) {
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_DASH);
					CSoundMgr::Get_Instance()->PlaySound(L"Player_Dash.wav", CSoundMgr::PLAYER_DASH, 0.75f);
					pGhost->Is_Dash = true;
					m_Is_Dash = true;
					m_iState = DASH;
					m_iAngleState = ANGLE_225;
					m_pTransformCom->Move_Pos(&(vRU * fTimeDelta * m_fSpeed));
				}
			}
			else {
				m_iState = RUN;
				m_iAngleState = ANGLE_225;
				m_pTransformCom->Move_Pos(&(vRU * fTimeDelta * m_fSpeed));
			}
		}
		else {
			if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
			
				if (m_tInfo.NowEnergy > 0) {
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_DASH);
					CSoundMgr::Get_Instance()->PlaySound(L"Player_Dash.wav", CSoundMgr::PLAYER_DASH, 0.75f);
					pGhost->Is_Dash = true;
					m_Is_Dash = true;
					m_iState = DASH;
					m_iAngleState = ANGLE_180;
					m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * m_fSpeed));
				}
			}
			else {
				m_iState = RUN;
				m_iAngleState = ANGLE_180;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * m_fSpeed));
			}
		}
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_S)) {
	    m_Is_Fire_Arrow = false;
		m_Fire_Frame = m_Fire_Init;

		if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
			if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
			
				if (m_tInfo.NowEnergy > 0) {
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_DASH);
					CSoundMgr::Get_Instance()->PlaySound(L"Player_Dash.wav", CSoundMgr::PLAYER_DASH, 0.75f);
					pGhost->Is_Dash = true;
					m_Is_Dash = true;
					m_iState = DASH;
					m_iAngleState = ANGLE_045;
					m_pTransformCom->Move_Pos(&(vLD * fTimeDelta * m_fSpeed));
				}
			}
			else {
				m_iState = RUN;
				m_iAngleState = ANGLE_045;
				m_pTransformCom->Move_Pos(&(vLD * fTimeDelta * m_fSpeed));
			}
		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
			if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
				
				if (m_tInfo.NowEnergy > 0) {
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_DASH);
					CSoundMgr::Get_Instance()->PlaySound(L"Player_Dash.wav", CSoundMgr::PLAYER_DASH, 0.75f);
					pGhost->Is_Dash = true;
					m_Is_Dash = true;
					m_iState = DASH;
					m_iAngleState = ANGLE_315;
					m_pTransformCom->Move_Pos(&(vRD * fTimeDelta * m_fSpeed));
				}
			}
			else {
				m_iState = RUN;
				m_iAngleState = ANGLE_315;
				m_pTransformCom->Move_Pos(&(vRD * fTimeDelta * m_fSpeed));
			}
		}
		else {
			if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
				
				if (m_tInfo.NowEnergy > 0) {
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_DASH);
					CSoundMgr::Get_Instance()->PlaySound(L"Player_Dash.wav", CSoundMgr::PLAYER_DASH, 0.75f);
					pGhost->Is_Dash = true;
					m_Is_Dash = true;
					m_iState = DASH;
					m_iAngleState = ANGLE_000;
					m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * -m_fSpeed));
				}
			}
			else {

				m_iState = RUN;
				m_iAngleState = ANGLE_000;
				m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * -m_fSpeed));
			}
		}

	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_A)) {
	    m_Is_Fire_Arrow = false; 
		m_Fire_Frame = m_Fire_Init;
	    
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
			
			if (m_tInfo.NowEnergy > 0) {
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_DASH);
				CSoundMgr::Get_Instance()->PlaySound(L"Player_Dash.wav", CSoundMgr::PLAYER_DASH, 0.75f);
				pGhost->Is_Dash = true;
				m_Is_Dash = true;
				m_iState = DASH;
				m_iAngleState = ANGLE_090;
				m_pTransformCom->Move_Pos(&(-vRight * fTimeDelta * m_fSpeed));
			}
		}
		else {
			m_iState = RUN;
			m_iAngleState = ANGLE_090;
			m_pTransformCom->Move_Pos(&(-vRight * fTimeDelta * m_fSpeed));
		}

	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_D)) {
	    m_Is_Fire_Arrow = false;
		m_Fire_Frame = m_Fire_Init;

	    if (CKeyMgr::Get_Instance()->Key_Down(KEY_SPACE)) {
		
			if (m_tInfo.NowEnergy > 0) {
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_DASH);
				CSoundMgr::Get_Instance()->PlaySound(L"Player_Dash.wav", CSoundMgr::PLAYER_DASH, 0.75f);
				pGhost->Is_Dash = true;
				m_Is_Dash = true;
				m_iState = DASH;
				m_iAngleState = ANGLE_270;
				m_pTransformCom->Move_Pos(&(vRight * fTimeDelta * m_fSpeed));
			}
		}
		else {
			m_iState = RUN;
			m_iAngleState = ANGLE_270;
			m_pTransformCom->Move_Pos(&(vRight * fTimeDelta * m_fSpeed));
		}
	}
	else {
		m_Is_Fire_Arrow = false;
		m_Fire_Frame = m_Fire_Init;
		m_iState = STAND;
	}


	if (CKeyMgr::Get_Instance()->Key_Up(KEY_RBUTTON)) {

		// 당기는 소리 멈춤
		
		if (m_Bow_Type == BOW_SYLPH) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_DRAW_DULL);
		}
		else if (m_Bow_Type == BOW_FIRE) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_DRAW_FIRE);
		}
		else if (m_Bow_Type == BOW_NIGHT) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_DRAW_NIGHT);
		}
		else if (m_Bow_Type == BOW_TIME) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_DRAW_TIME);
		}

		m_Is_Charge_Sound = false;

		CAMERA_CHARGE_ZOOM_IN_RETURN

		if (m_Bow_Type == BOW_SYLPH) {
			CEffect_Player_Bow_Charge* pObject = dynamic_cast<CEffect_Player_Bow_Charge*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Bow_Charge_Effect"));
			if (pObject != nullptr) {
				pObject->Set_Dead();
				m_Charge_Effect_Cnt = 0;
			}
		}
		else if (m_Bow_Type == BOW_FIRE) {
			CFire_Charge* pObject = dynamic_cast<CFire_Charge*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Bow_Charge_Effect"));
			if (pObject != nullptr) {
				pObject->Set_Dead();
				m_Charge_Effect_Cnt = 0;
			}

		}
		else if (m_Bow_Type == BOW_NIGHT) {
			CNight_Charge* pObject = dynamic_cast<CNight_Charge*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Bow_Charge_Effect"));
			if (pObject != nullptr) {
				pObject->Set_Dead();
				m_Charge_Effect_Cnt = 0;
			}
			
		}
		else if (m_Bow_Type == BOW_TIME) {
			CTime_Charge* pObject = dynamic_cast<CTime_Charge*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Bow_Charge_Effect"));
			if (pObject != nullptr) {
				pObject->Set_Dead();
				m_Charge_Effect_Cnt = 0;
			}

		}
		
		
		m_Is_Fire_Arrow = false;
		m_Fire_Frame = m_Fire_Init;

		if (m_Is_Charge_Arrow == true) {
			// 차지 Fire	
			if (m_Bow_Type == BOW_SYLPH) {
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_CHARGE_FIRE);
				CSoundMgr::Get_Instance()->PlaySound(L"Bow_Charge_Fire.wav", CSoundMgr::BOW_CHARGE_FIRE, 0.85f);
			}
			else if (m_Bow_Type == BOW_FIRE) {
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::FIRE_CHARGE_FIRE);
				CSoundMgr::Get_Instance()->PlaySound(L"Kaff_Brave.wav", CSoundMgr::FIRE_CHARGE_FIRE, 0.75f);
			}
			else if (m_Bow_Type == BOW_NIGHT) {
			
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::NIGHT_CHARGE_FIRE);
				CSoundMgr::Get_Instance()->PlaySound(L"Light_Charge_Fire.wav", CSoundMgr::NIGHT_CHARGE_FIRE, 0.85f);
			}
			else if (m_Bow_Type == BOW_TIME) {
				
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TIME_CHARGE_FIRE);
				CSoundMgr::Get_Instance()->PlaySound(L"Time_Fire.wav", CSoundMgr::TIME_CHARGE_FIRE, 1.f);
			}

			Fire_Charge_Arrow();
		}

		m_Is_Effect_Charge_Arrow = false;
		m_fChargeFrame = 0.f;
		m_Is_Charge_Arrow = false;
		
	}




}



void CPlayer::Fire_Arrow(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	_vec3 Axis = { -1.f,0.f,0.f };

	_vec3 Dir = { (float)(ptCursor.x - WINCX * 0.5),float(ptCursor.y - WINCY * 0.5),0 };
	D3DXVec3Normalize(&Dir, &Dir);


	// Angle 계산
	m_fAngle = acos(D3DXVec3Dot(&Axis, &Dir));

	if (WINCY * 0.5 < ptCursor.y)
		m_fAngle = 2.f * D3DX_PI - m_fAngle;

	
	m_Mouse_Dir = { Dir.x,0.f,-Dir.y };


	CSylphBow* pSylphBow = dynamic_cast<CSylphBow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBow"));


	_vec3 vPos = pSylphBow->m_vPos;
	
	pGameObject = CSylphArrow::Create(m_pGraphicDev, vPos, m_Mouse_Dir, m_fAngle);

	if (pGameObject == nullptr)
		return;


	pGameLogicLayer->Add_BulletObject(pGameObject);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_ARROW, pGameObject);


	if (m_Bow_Type == BOW_SYLPH) {
		pGameObject = CEffect_Player_Bow_Pulse::Create(m_pGraphicDev);

		if (pGameObject == nullptr)
			return;

		pGameLogicLayer->Add_BulletObject(pGameObject);
	}
	else if (m_Bow_Type == BOW_FIRE) {
		pGameObject = CFire_Pulse::Create(m_pGraphicDev);

		if (pGameObject == nullptr)
			return;

		pGameLogicLayer->Add_BulletObject(pGameObject);
	}
	else if (m_Bow_Type == BOW_NIGHT) {
		pGameObject = CNight_Pulse::Create(m_pGraphicDev);

		if (pGameObject == nullptr)
			return;

		pGameLogicLayer->Add_BulletObject(pGameObject);


	}
	else if (m_Bow_Type == BOW_TIME) {
		
		pGameObject = CTime_Pulse::Create(m_pGraphicDev);

		if (pGameObject == nullptr)
			return;

		pGameLogicLayer->Add_BulletObject(pGameObject);

	}

	



}

void CPlayer::Fire_Charge_Arrow(void)
{

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	_vec3 Axis = { -1.f,0.f,0.f };
	D3DXVec3Normalize(&Axis, &Axis);

	_vec3 Dir = { (float)(ptCursor.x - WINCX * 0.5),float(ptCursor.y - WINCY * 0.5),0 };
	D3DXVec3Normalize(&Dir, &Dir);


	// Angle 계산
	m_fAngle = acos(D3DXVec3Dot(&Axis, &Dir));

	if (WINCY * 0.5 < ptCursor.y)
		m_fAngle = 2.f * D3DX_PI - m_fAngle;


	m_Mouse_Dir = { Dir.x,0.f,-Dir.y };


	CSylphBow* pSylphBow = dynamic_cast<CSylphBow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBow"));


	_vec3 vPos = pSylphBow->m_vPos;




	pGameObject = CSylphChargeArrow::Create(m_pGraphicDev, vPos, m_Mouse_Dir, m_fAngle);

	if (pGameObject == nullptr)
		return;


	pGameLogicLayer->Add_BulletObject(pGameObject);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_ARROW, pGameObject);


}

void CPlayer::Effect_Charge_Arrow(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;


	if (m_Charge_Effect_Cnt == 0) {

		if (m_Bow_Type == BOW_SYLPH) {
			pGameObject = CEffect_Player_Bow_Charge::Create(m_pGraphicDev);

			if (pGameObject == nullptr)
				return;

			pGameLogicLayer->Add_GameObject(L"Bow_Charge_Effect", pGameObject);

			m_Charge_Effect_Cnt++;
		}
		else if (m_Bow_Type == BOW_FIRE) {
			pGameObject = CFire_Charge::Create(m_pGraphicDev);

			if (pGameObject == nullptr)
				return;

			pGameLogicLayer->Add_GameObject(L"Bow_Charge_Effect", pGameObject);

			m_Charge_Effect_Cnt++;

		}
		else if (m_Bow_Type == BOW_NIGHT) {

			pGameObject = CNight_Charge::Create(m_pGraphicDev);

			if (pGameObject == nullptr)
				return;

			pGameLogicLayer->Add_GameObject(L"Bow_Charge_Effect", pGameObject);

			m_Charge_Effect_Cnt++;

		}
		else if (m_Bow_Type == BOW_TIME) {
			pGameObject = CTime_Charge::Create(m_pGraphicDev);

			if (pGameObject == nullptr)
				return;

			pGameLogicLayer->Add_GameObject(L"Bow_Charge_Effect", pGameObject);

			m_Charge_Effect_Cnt++;

		}



		
	}


}

void CPlayer::Effect_Foot_Step_Smoke(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	if (m_bReverse)
		m_FootRev = false;
	else
		m_FootRev = true;
	
	pGameObject = CEffect_Player_Foot::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS],FOOT_TYPE_SMOKE, m_FootRev);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);


}

void CPlayer::Effect_Foot_Step_Water(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	if (m_bReverse)
		m_FootRev = false;
	else
		m_FootRev = true;

	pGameObject = CEffect_Player_Foot::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], FOOT_TYPE_WATER, m_FootRev);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);

}

void CPlayer::Frame_Manage(const _float& fTimeDelta)
{
	if (m_Is_Time_Stop == true) {
		m_Time_Stop_Frame += 4.f * fTimeDelta * 0.2f;
		if (m_Time_Stop_Frame > 4.f) {
			m_Time_Stop_Frame = 0.f;
			CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
			pGameLogicLayer->m_fTimeStop = false;
			m_Is_Time_Stop = false;
		}
	}

	if (m_iState == TELEPORT) {
		m_fTeleportFrame += 17.f * fTimeDelta;

		if (5.f < m_fTeleportFrame) {
			CSoundMgr::Get_Instance()->PlaySound(L"Player_Teleport.wav", CSoundMgr::PLAYER_TELEPORT, 0.95f);
		}
		
		if (17.f < m_fTeleportFrame) {
			m_fTeleportFrame = 0.f;
			m_Is_Teleport = false;
			m_Is_Render = false;
			m_Is_Landing = true;

			if (g_eCurScene == SCENE_TUTORIAL) {
				Landing(TUTORIAL_TELEPORT);
			}
			else if(g_eCurScene == SCENE_BOSS1) {
				Landing(BOSS_1_TELEPORT);
			}
			else if (g_eCurScene == SCENE_MAINLOBBY) {
				Landing(MAINLOBBY_TELEPORT);
			}
			else if (g_eCurScene == SCENE_DUNGEONLOBBY) {
				Landing(DUNGEONLOBBY_TELEPORT);
			}
			else {
				Landing(BOSS_1_TELEPORT);
			}
			
		}
	}

	if (m_iState == LANDING) {
		m_fLandingFrame += 9.f * fTimeDelta;
		if (9.f < m_fLandingFrame) {
			m_fLandingFrame = 0.f;
			m_iState = STAND;
			m_iAngleState = ANGLE_000;
			m_Is_Landing = false;
		}
			
	}

	if (m_iState == STAND) {
		m_fStandFrame += 7.f * fTimeDelta;
		if (7.f < m_fStandFrame)
			m_fStandFrame = 0.f;
	}

	if (m_iState == RUN) {

		
		m_fRunFrame += 7.f * fTimeDelta * 1.5f;
		if (7.f < m_fRunFrame)
			m_fRunFrame = 0.f;
	}

	if (m_iState == MOVE_ATTACK || m_iState == STAND_ATTACK) {

		m_fAttackFrame += 9.f * fTimeDelta;
		if (9.f < m_fAttackFrame)
			m_fAttackFrame = 0.f;
	}

	if (m_iState == DASH) {

		m_fDashFrame += 5.f * fTimeDelta * 2.0f;
		if (5.f < m_fDashFrame) {
			m_fDashFrame = 0.f;
		}
	}

	if (m_iState == DEATH) {
		m_fDeathFrame += 11.f * fTimeDelta * 0.5f;
		if (11.f < m_fDeathFrame) {
			m_fDeathFrame = 11.f;
		}
	}

	if (m_HitBlend) {
		m_HitBlendFrame += m_HitMaxFrame * fTimeDelta * 8.f;
		if (m_HitBlendFrame > m_HitMaxFrame) {
			m_HitBlendFrame = 0.f;
			m_HitBlendCnt++;
			if (m_HitBlendRender)
				m_HitBlendRender = false;
			else {
				m_HitBlendRender = true;
			}
		}
	}

	if (m_HitBlendCnt > m_HitBlendMaxCnt) {
		m_HitBlend = false;
		m_HitBlendRender = false;
		m_bImmuned = false;
		m_HitBlendCnt = 0;
	}

	if (m_fMoney_Vari == true)
	{
		m_fMoney_Vari_Frame += m_fMoney_Vari_Frame_Max * fTimeDelta * 0.5f;

		if (m_fMoney_Vari_Frame > m_fMoney_Vari_Frame_Max)
		{
			m_fMoney_Vari_Frame = 0.f;
			m_iRecieve_Money_Value = 0;
			m_tRecieve_Vari = MONEY_END;
			m_fMoney_Vari = false;
		}
	}

	if (m_fGem_Vari == true)
	{
		m_fGem_Vari_Frame += m_fGem_Vari_Frame_Max * fTimeDelta * 0.5f;

		if (m_fGem_Vari_Frame > m_fGem_Vari_Frame_Max)
		{
			m_fGem_Vari_Frame = 0.f;
			m_iRecieve_Gem_Value = 0;
			m_tRecieve_Vari_GEM = GEM_END;
			m_fGem_Vari = false;
		}
	}
}

void CPlayer::Apostle_Skill_Effect_Time(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CLayer* pUILayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;


	pGameObject = CApostle_Skill_UI_illust::Create(m_pGraphicDev, 354.f * 0.7f, 150.f * 0.7f, { -560.f,-100.f,0.f }, TIME);

	if (pGameObject == nullptr)
		return;

	pUILayer->Add_BulletObject(pGameObject);




}



void CPlayer::Apostle_Skill_Effect_Courage(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CLayer* pUILayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;


	pGameObject = CApostle_Skill_UI_illust::Create(m_pGraphicDev, 354.f * 0.7f, 150.f * 0.7f, { -560.f,-100.f,0.f }, COURAGE);

	if (pGameObject == nullptr)
		return;

	pUILayer->Add_BulletObject(pGameObject);
}

void CPlayer::Apostle_Skill_Effect_Night(void)
{


	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CLayer* pUILayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;



	pGameObject = CApostle_Skill_UI_illust::Create(m_pGraphicDev, 354.f * 0.7f, 150.f * 0.7f, { -560.f,-100.f,0.f }, NIGHT);

	if (pGameObject == nullptr)
		return;

	pUILayer->Add_BulletObject(pGameObject);

}


void CPlayer::Apostle_Manage(void)
{
	if (m_Apostle_Type == TIME) {
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_Q) && m_Is_Time_Stop == false) {

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TIME_STOP);
			CSoundMgr::Get_Instance()->PlaySound(L"Time_Stop.wav", CSoundMgr::TIME_STOP, 0.95f);

			m_Apostle_Proof--;

			if (m_Apostle_Proof < 0) {
				m_Apostle_Proof = 0;
			}

			CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
			pGameLogicLayer->m_fTimeStop = true;
			m_Is_Time_Stop = true;

			Apostle_Skill_Effect_Time();

			m_Apostle_Count -= 1;
		}
	}
	else if (m_Apostle_Type == COURAGE) {
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_Q) && m_Is_Courage == false /* && m_Apostle_Proof > 0*/) {

			m_Apostle_Proof--;

			if (m_Apostle_Proof < 0) {
				m_Apostle_Proof = 0;
			}

			m_Is_Courage = true;

			Apostle_Skill_Effect_Courage();

			m_Apostle_Count -= 1;
		}
	}
	else if (m_Apostle_Type == NIGHT) {
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_Q) && m_Is_Night == false/* && m_Apostle_Proof > 0*/) {

			m_Apostle_Proof--;

			if (m_Apostle_Proof < 0) {
				m_Apostle_Proof = 0;
			}

			m_Is_Night = true;

			Apostle_Skill_Effect_Night();

			m_Apostle_Count -= 1;
		}
	}
}

void CPlayer::Player_Level_Up(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CGameObject* pLevelObject = nullptr;

	pLevelObject = CFrame_LevelUp::Create(m_pGraphicDev);
	pLayer->Add_BulletObject(pLevelObject);

	//m_bLevelUpUi_On = false;
}


void CPlayer::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	_tchar szBuf[MAX_STR];
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Environment", L"Terrain"));
	pTerrain->Get_TerrainTexKey(szBuf);
	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", szBuf, ID_STATIC));
	if (nullptr == pTerrainBufferCom) return;

	_float fHeight = 0.f;
	
	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), pTerrainBufferCom->Get_VtxCntX(), pTerrainBufferCom->Get_VtxCntZ());
	

	m_pTransformCom->Set_Pos(vPos.x, fHeight + 5.f * PUBLIC_SCALE + jump * PUBLIC_SCALE, vPos.z);
}

_vec3 CPlayer::Picking_OnTerrain(void)
{
	_tchar szBuf[MAX_STR];
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Environment", L"Terrain"));
	pTerrain->Get_TerrainTexKey(szBuf);
	CTerrainTex * pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", szBuf, ID_STATIC));
	if (nullptr == pTerrainBufferCom) return _vec3();

	CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_Transform", ID_DYNAMIC));
	if (nullptr == pTerrainBufferCom) return _vec3();

	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

void CPlayer::Update_State()
{
	if (m_iState == m_iPreState && m_iAngleState == m_iPreAngleState && m_iAttackAngleState == m_iPreAttackAngleState)
		return;

	CSylphBow* pObject1 = dynamic_cast<CSylphBow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBow"));
	CSylphBowPair* pObject2 = dynamic_cast<CSylphBowPair*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBowPair"));

	CGhost* pGhost = dynamic_cast<CGhost*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Ghost"));

	pGhost->m_bReverse = m_bReverse;

	if (m_iState == STAND) {

		CGhost* pGhost = dynamic_cast<CGhost*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Ghost"));
		pGhost->Is_Run = false;

		m_Is_Run = false;
		m_fSpeed = 25.f * PUBLIC_SCALE;
		if (pObject1 != nullptr && pObject2 != nullptr) {
			pObject1->Set_Render(false);
			pObject2->Set_Render(false);
		}
			
		if (m_iAngleState == ANGLE_000) {
			
		}
		else if (m_iAngleState == ANGLE_045) {
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_090) {
			
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_135) {
			
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_180) {
			
		}
		else if (m_iAngleState == ANGLE_225) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAngleState == ANGLE_270) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAngleState == ANGLE_315) {
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
	}
	else if (m_iState == RUN) {
	
		CGhost* pGhost = dynamic_cast<CGhost*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Ghost"));
		pGhost->Is_Run = true;

		m_fSpeed = 25.f * PUBLIC_SCALE;
		m_Is_Run = true;

		if (pObject1 != nullptr && pObject2 != nullptr) {
			pObject1->Set_Render(false);
			pObject2->Set_Render(false);
		}

		if (m_iAngleState == ANGLE_000) {
			
		}
		else if (m_iAngleState == ANGLE_045) {
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_090) {
			
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_135) {
			
			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAngleState == ANGLE_180) {
			
		}
		else if (m_iAngleState == ANGLE_225) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAngleState == ANGLE_270) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAngleState == ANGLE_315) {
			
			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
	}
	else if (m_iState == MOVE_ATTACK) {

		CGhost* pGhost = dynamic_cast<CGhost*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Ghost"));
		pGhost->Is_Run = false;

		m_fSpeed = 20.f * PUBLIC_SCALE;
		m_Is_Run = false;

		if (pObject1 != nullptr && pObject2 != nullptr) {
			pObject1->Set_Render(true);
			pObject2->Set_Render(true);
		}
		if (m_iAttackAngleState == ATTACK_ANGLE_045) {

			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_135) {

			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_225) {

			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_315) {

			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
	}
	else if (m_iState == STAND_ATTACK) {
		CGhost* pGhost = dynamic_cast<CGhost*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Ghost"));
		pGhost->Is_Run = false;
		m_Is_Run = false;
		if (pObject1 != nullptr && pObject2 != nullptr) {
			pObject1->Set_Render(true);
			pObject2->Set_Render(true);
		}


		if (m_iAttackAngleState == ATTACK_ANGLE_045) {

			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_135) {

			if (m_bReverse == true) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = false;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_225) {

			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
		else if (m_iAttackAngleState == ATTACK_ANGLE_315) {

			if (m_bReverse == false) {
				m_pTransformCom->Set_Scale_X_Ratio(-1.f);
				m_bReverse = true;
			}
		}
	}
	else if (m_iState == DASH) {

		CGhost* pGhost = dynamic_cast<CGhost*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Ghost"));
		pGhost->Is_Run = false;


		m_fSpeed = 25.f * PUBLIC_SCALE;
		m_Is_Run = false;
		if (pObject1 != nullptr && pObject2 != nullptr) {
			pObject1->Set_Render(false);
			pObject2->Set_Render(false);
		}
	}

	m_iPreState = m_iState;
	m_iPreAngleState = m_iAngleState;
	m_iPreAttackAngleState = m_iAttackAngleState;

}

_vec3 CPlayer::Get_World_Mouse()
{
	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec3	vMousePos;

	// 윈도우 -> 투영
	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영 -> 뷰 스페이스
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matView);
	
	

	return vMousePos;
}

void CPlayer::Dash(const _float& fTimeDelta)
{

	if (m_Dash_Time == 0.f) {
		m_tInfo.NowEnergy--;
		if (m_tInfo.NowEnergy < 0) {
			m_tInfo.NowEnergy = 0;
		}
	}

	m_Dash_Time += 5.f * fTimeDelta * 2.0f;


	jump += 2.f * fTimeDelta * jumpDir * 4.0f;

	if (jump >= 2.f) {
		jump = 2.f;
		jumpDir = -1.f;
	}

	if (jump <= 0.f) {
		jump = 0.f;
		jumpDir = 1.f;
	}



	if (m_Dash_Time > 5.f) {
		m_Dash_Time = 0.f;
		m_Is_Dash = false;
		m_bImmuned = false;
		CGhost* pGhost = dynamic_cast<CGhost*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Ghost"));
		pGhost->Is_Dash = false;
	}


	_vec3	vDir;
	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3	vRight;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vRight, &vRight);

	_vec3	vLU = vDir - vRight;
	D3DXVec3Normalize(&vLU, &vLU);

	_vec3	vRU = vDir - vRight;
	D3DXVec3Normalize(&vRU, &vRU);

	_vec3	vLD = -vDir - vRight;
	D3DXVec3Normalize(&vLD, &vLD);

	_vec3	vRD = -vDir - vRight;
	D3DXVec3Normalize(&vRD, &vRD);


	if (m_iAngleState == ANGLE_000) {
		m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * -m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_045) {
		m_pTransformCom->Move_Pos(&(vLD * fTimeDelta * m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_090) {
		m_pTransformCom->Move_Pos(&(vRight * fTimeDelta * -m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_135) {
		m_pTransformCom->Move_Pos(&(vLU * fTimeDelta * m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_180) {
		m_pTransformCom->Move_Pos(&(vDir * fTimeDelta * m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_225) {
		m_pTransformCom->Move_Pos(&(vRU * fTimeDelta * m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_270) {
		m_pTransformCom->Move_Pos(&(-vRight * fTimeDelta * m_fSpeed * m_fDashSpeed));
	}
	else if (m_iAngleState == ANGLE_315) {
		m_pTransformCom->Move_Pos(&(vRD * fTimeDelta * m_fSpeed * m_fDashSpeed));
	}

}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer * pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Free(void)
{
	
	g_pInfo = m_tInfo;
	g_Bow_Type = m_Bow_Type;
	g_Arrow_Type = m_Arrow_Type;
	g_Apostle_Type = m_Apostle_Type;
	g_Apostle_Proof = m_Apostle_Proof;

	g_pLvsk = m_tPlayerLvSk;


	__super::Free();
}


