#include "stdafx.h"
#include "..\Header\ThreadMgr.h"
#include "Export_Function.h"
#include <SoundMgr.h>

bool  CThreadMgr::m_bLoad[LOADING_END] = {};

CThreadMgr::CThreadMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	ZeroMemory(m_szString, sizeof(_tchar) * 128);
	m_pGraphicDev->AddRef();
}


CThreadMgr::~CThreadMgr()
{
}

unsigned int CThreadMgr::Thread_Main(void * pArg)
{
	CThreadMgr*	pThreadMgr = reinterpret_cast<CThreadMgr*>(pArg);

	_uint iFlag = 0;

	EnterCriticalSection(&(pThreadMgr->Get_Crt()));

	switch (pThreadMgr->Get_ThreadMgrID())
	{
	case LOADING_TITLE:
		iFlag = pThreadMgr->ThreadMgr_ForStage_Title();
		break;
	case LOADING_TUTORIAL:
		iFlag = pThreadMgr->ThreadMgr_ForStage_Tutorial();
		break;
	case LOADING_MAINLOBBY:
		iFlag = pThreadMgr->ThreadMgr_ForStage_MainLobby();
		break;
	case LOADING_DUNGEONLOBBY:
		iFlag = pThreadMgr->ThreadMgr_ForStage_DungeonLobby();
		break;
	case LOADING_REGEN:
		iFlag = pThreadMgr->ThreadMgr_ForStage_Regen();
		break;
	case LOADING_BOSS1:
		iFlag = pThreadMgr->ThreadMgr_ForStage_Boss1();
		break;
	case LOADING_BOSS2:
		iFlag = pThreadMgr->ThreadMgr_ForStage_Boss2();
		break;
	case LOADING_SHOP:
		iFlag = pThreadMgr->ThreadMgr_ForStage_Shop();
		break;
	case LOADING_POT:
		iFlag = pThreadMgr->ThreadMgr_ForStage_Pot();
		break;
	case LOADING_NORMAL:
		iFlag = pThreadMgr->ThreadMgr_ForStage_Normal();
		break;
	}

	LeaveCriticalSection(&(pThreadMgr->Get_Crt()));

	// _endthreadex(0);

	return iFlag;
}

HRESULT CThreadMgr::Ready_ThreadMgr(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, NULL);

	m_eThreadMgr = eID;

	return S_OK;
}

_uint CThreadMgr::ThreadMgr_ForStage_Title(void)
{
	if (!m_bLoad[LOADING_TITLE])
	{
		Set_String(L"Map Texture Loading.....");
#pragma region Map_Texture
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Tile/Tile%d.png", 45))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"MainLobby", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/MainLobby/MainLobby%d.png", 32))))
			return E_FAIL;
		
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Thorn_Destruction", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Object/spr_Thorns_DstnEffect/Spr_InfectionThorns_Effect_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Thorn_Hit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Object/spr_Thorns_HitEffect/Spr_InfectionThorns_HitEffect_%d.png", 6))))
			return E_FAIL;

		// 돌 파괴 이펙트 - 보스 도철 비석
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Stone_Destruction", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Object/spr_Stone_DstnEffect/03_Stone_Destruction_%d.png", 8))))
			return E_FAIL;

		// Object Effect
		// 일반맵 돌 파괴 이펙트
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_StoneDestruction", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Destruction/Effect/StoneBox_Destruction_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Debris_Stone", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Destruction/Debris/Stone_Debris_%d.png", 9))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_Jar_Debris_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/Debris/jar/Spr_Jar_Debris_%d.png", 16))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"StoneBox_HitEffect_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Object/StoneBox_HitEffect/StoneBox_HitEffect_%d.png", 5))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"TreeBox_HitEffect_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Object/TreeBox_HitEffect/TreeBox_HitEffect_%d.png", 5))))
			return E_FAIL;

		// BossDowoleMap
		if (FAILED(Engine::Ready_ProtoComponent(L"BossDowoleObj", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/2dTexture/BossDowoleObj%d.png", 43))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"spr_bush_01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_bush_01/spr_bush_01_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"spr_bush_02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_bush_02/spr_bush_02_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"spr_bush_03_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_bush_03/spr_bush_03_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"spr_bush_04_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_bush_04/spr_bush_04_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"spr_bush_05_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_bush_05/spr_bush_05_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_Deco_BushFlower01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_bush_flower_01/Spr_Deco_BushFlower01_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_Deco_BushFlower02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_bush_flower_02/Spr_Deco_BushFlower02_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_thorn_01/Spr_DestructibleObject_Infectionthorn01_%d.png", 4))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_thorn_02/Spr_DestructibleObject_Infectionthorn02_%d.png", 4))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn03_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_thorn_03/Spr_DestructibleObject_Infectionthorn03_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn04_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_thorn_04/Spr_DestructibleObject_Infectionthorn04_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn05_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_thorn_05/Spr_DestructibleObject_Infectionthorn05_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_DestructibleObject_Infectionthorn06_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDowole/spr_thorn_06/Spr_DestructibleObject_Infectionthorn06_%d.png", 3))))
			return E_FAIL;
#pragma endregion

		// 마우스 로딩
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Mouse_Cursor_Img", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MyMouse/Spr_Ui_MousePointer_2.png"))))
			return E_FAIL;

		// 보스 UI 프레임
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_BossBar_Back", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_BossBar_Back.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_BossBar_Hp", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_BossBar_Hp.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_BossBar_Icon_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_BossBar_Icon_3_0%d.png", 2))))
			return E_FAIL;

		// 플레이어 레벨업 출력
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Ui_LevelUp_Select", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Level/Ui_LevelUp_Select_0%d.png", 5))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Ui_LevelUp_Back", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Level/Effect_Ui_LevelUp_Back_0%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Ui_LevelUp_Lion", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Level/Effect_Ui_LevelUp_Lion_0%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Ui_Skill_Get", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Level/Spr_Effect_Skill_Get_0%d.png", 15))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Ui_LevelUp_Text", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Level/Effect_Ui_LevelUp_Text.png"))))
			return E_FAIL;
		
#pragma endregion

#pragma region Player_Texture
		Set_String(L"Player Texture Loading.....");
		// Stand
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_000", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand/Spr_Yeon_Stand_000_0%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_045", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand/Spr_Yeon_Stand_045_0%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_090", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand/Spr_Yeon_Stand_090_0%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand/Spr_Yeon_Stand_135_0%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_180", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand/Spr_Yeon_Stand_180_0%d.png", 8))))
			return E_FAIL;

		// Run
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Run_000", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Run/Spr_Yeon_Run_000_0%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Run_045", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Run/Spr_Yeon_Run_045_0%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Run_090", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Run/Spr_Yeon_Run_090_0%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Run_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Run/Spr_Yeon_Run_135_0%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Run_180", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Run/Spr_Yeon_Run_180_0%d.png", 8))))
			return E_FAIL;

		// MoveAttack
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Move_Attack_045", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Move_Attack/Spr_Yeon_ChargeAttack_045_%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Move_Attack_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Move_Attack/Spr_Yeon_ChargeAttack_135_%d.png", 10))))
			return E_FAIL;

		// StandAttack
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_Attack_045", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand_Attack/Spr_Yeon_ChargeStandingAttack_045_%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Stand_Attack_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Stand_Attack/Spr_Yeon_ChargeStandingAttack_135_%d.png", 10))))
			return E_FAIL;

		// Dash
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Dash_045", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Dash/Spr_Yeon_Dash_045_0%d.png", 6))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Dash_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Dash/Spr_Yeon_Dash_135_0%d.png", 6))))
			return E_FAIL;

		// Dead
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Death", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Death/Spr_Yeon_Death_%d.png", 12))))
			return E_FAIL;

		// Teleport
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Landing", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Landing/Spr_Yeon_Landing_%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Teleport", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Teleport_Motion/Spr_Haetae_Teleport_%d.png", 18))))
			return E_FAIL;

		// 데미지 폰트
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Damage_Font_Critical", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Damage_Font/New_Critical/Cri0%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Damage_Font_Common", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Damage_Font/New_Common/Com0%d.png", 10))))
			return E_FAIL;

		// 발자국
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Water_Wave", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Water_Wave/Spr_Effect_WaterWave_%d.png", 17))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Foot_Step", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Foot_Step/0%d.png", 14))))
			return E_FAIL;

		// 텔레포트
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_TP_Bot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Teleport/Bottom/B00%d.png", 21))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_TP_Top", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Teleport/Top/tp00%d.png", 61))))
			return E_FAIL;

		// Rain Test
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Rain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Rain/Rain%d.png", 40))))
			return E_FAIL;


		// [사도 스킬]
		// [티프]
		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Tif_Circle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Time/Tif_Circle/T%d.png", 126))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Tif_UI_Circle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Time/Tif_UI_Circle/KK.png"))))
			return E_FAIL;
		
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Tif_Ani_End", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Time/Tif_Ani/Spr_Npc_IraOfTime_OpenAbility_End_0%d.png", 12))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Tif_Ani_Loof", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Time/Tif_Ani/Spr_Npc_IraOfTime_OpenAbility_Loof_0%d.png", 11))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Tif_Ani_Start", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Time/Tif_Ani/Spr_Npc_IraOfTime_OpenAbility_Start_0%d.png", 12))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Tif_illust", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Time/Illust/Tif_DialoguePortraitV2_Normal.png"))))
			return E_FAIL;


		// [나이트]
		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Night_Ani_End", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Night/Night_Ani/Spr_Npc_ApostleOfNight_ActiveSkill_End_0%d.png", 6))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Night_Ani_Loop", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Night/Night_Ani/Spr_Npc_ApostleOfNight_ActiveSkill_Loop_0%d.png", 6))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Night_Ani_Start", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Night/Night_Ani/Spr_Npc_ApostleOfNight_ActiveSkill_Start_0%d.png", 11))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Night_Skill", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Night/Skill/Night_00%d.png", 21))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Night_Wing", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Night/Wing/Wing_00%d.png", 21))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Night_Filter", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Night/Filter/Filter.png"))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Night_Circle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Night/Circle/Bottom00%d.png",151))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Night_illust", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Night/Illust/illust.png"))))
			return E_FAIL;

		// [브레이브]
		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Courage_Ani", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Courage/Motion/%d.png", 19))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Courage_Filter", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Courage/Filter/Filter.png"))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Courage_illust", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Courage/Illust/illust.png"))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Courage_Circle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Courage/Circle/Spr_Effect_ApostleOfCourage_SlashFloorFire01_%d.png",22))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Courage_Brave", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Courage/Brave/fire_brave00%d.png", 61))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Courage_Bomb", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Courage/Bomb/Fire_bomb00%d.png", 50))))
			return E_FAIL;

		if(FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Courage_Wave", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Courage/Wave/big000%d.png", 9))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Courage_Hit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Apostle_Skill/Courage/Hit/Spr_Effect_021_FireArrowHitEffect_%d.png", 25))))
			return E_FAIL;

#pragma endregion


#pragma region Weapon_Texture
		Set_String(L"Weapon Texture Loading.....");

		// Basic
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Arrow_Sylph_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Arrow/Sylph/Spr_Weapon_No039_Sylph'sArrow_%d.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Bow_Sylph", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Bow/Sylph/Spr_Weapon_No039_Sylph'sBow_0%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Bow_Sylph_Pair", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Bow/Sylph/Spr_Weapon_No039_Sylph'sBow_Pair_0%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Effect_BowPulse_Sylph", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Bow_Pulse/Sylph/Spr_Effect_No039_Sylph'sBow_Pulse_%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Effect_BowCharge_Sylph", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Bow_Charge/Sylph/Spr_Effect_WindSpiritChargeEffect_%d.png", 6))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Effect_ArrowHit_Sylph", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Arrow_Hit/Sylph/Spr_Effect_No039_Sylph'sBow_HitEffect_%d.png", 12))))
			return E_FAIL;

		// Fire 
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Bow_Fire", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Bow/Fire/FB_%d.png", 12))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Arrow_Fire", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Arrow/Fire/Fire_Arrow.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Bow_Fire_Pulse", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Bow_Pulse/Fire/Spr_Effect_No021_FireBowPulse_%d.png", 22))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Bow_Fire_Charge", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Bow_Charge/Fire/Spr_Effect_No064_RoyalFireSpiritChargeEffect_0%d.png", 9))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Fire_Big", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Arrow_Hit/Fire_Big/Spr_Effect_No064_RoyalFireSpiritCharge_Explosion_%d.png", 35))))
			return E_FAIL;

		// Night 
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Bow_Night", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Bow/Night/NB_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Arrow_Night", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Arrow/Night/Night_Arrow.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Bow_Night_Pulse", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Bow_Pulse/Night/Spr_Effect_No108_ThunderBowPulse_%d.png", 18))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Bow_Night_Charge", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Bow_Charge/Night/Spr_Effect_No110_ThunderBringerMarble_%d.png", 21))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Night_Hit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Arrow_Hit/Night/Spr_Effect_No108_ThunderArrowEffect01_%d.png", 16))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Night_Big", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Arrow_Hit/Night_Big/Awakened_Thunder_%d.png", 26))))
			return E_FAIL;
		
		// Time 
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Bow_Time", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Bow/Time/TB_%d.png",11))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Arrow_Time", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Arrow/Time/Time_Arrow.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Bow_Time_Pulse", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Bow_Pulse/Time/Spr_Effect_No031_Heaven'sEmperorHitEffect_%d.png", 18))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Bow_Time_Pulse_Origin", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Bow_Pulse/Time_Origin/Spr_Effect_No031_Heaven'sEmperorHitEffect_%d.png", 18))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Bow_Time_Charge", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Bow_Charge/Time/Spr_Ui_Effect_DeathResult_0%d.png", 15))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Time_Hit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Effect/Arrow_Hit/Time/Spr_Effect_No031_Heaven'sEmperorBackHitEffect_0%d.png", 13))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Player_Time_Skill_Arrow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Arrow/Time_Skill/Spr_Weapon_No65_RoyalIceSpiritChargeArrow_0%d.png", 7))))
			return E_FAIL;

#pragma endregion
		
#pragma region UI_Texture
		Set_String(L"UI Texture Loading.....");
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Energy", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_DashStock_Idle_05_0%d.png", 2))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_FieldHp", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_FieldHp4_0%d.png", 2))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Key", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Key.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Coin", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Coin.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Gem", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Gem.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Back", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_HpStock_Back.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Apostle_Frame_Back", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_UI_Apostle_Back.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Apostle_Frame", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_UI_Apostle_Frame.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Apostle_Key_Q", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Key_Q.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Apostle_Pattern_Time", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Apostle_Pattern_Time.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Apostle_Pattern_Night", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Apostle_Pattern_Night.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Apostle_Pattern_Brave", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Apostle_Pattern_Brave.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Apostle_Count", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Apostle_Count.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Apostle_Pattern_Time_Glow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Apostle_Pattern_Time_Glow_0%d.png", 9))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Apostle_Pattern_Night_Glow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Apostle_Pattern_Night_Glow_0%d.png", 9))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Apostle_Pattern_Brave_Glow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Apostle_Pattern_Brave_Glow_0%d.png", 9))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_QuickSlot_WeaponImg_Back", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Weapon_Back_Last.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_QuickSlot_ArrowCount_Infi", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Weapon_ArrowCount_Back.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_QuickSlot_WeaponImg_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Weapon_0%d.png", 4))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_QuickSlot_Infi", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_Infi_0%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_MiniMap_Back", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_UI_MiniMap_Back_2.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_MiniMap_Gage", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_UI_MiniMap_Gage.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_MiniMap_Character", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_MiniMap_Character.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_MiniMap_Room", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_UI_MiniMap_Room_0%d.png", 3))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Number_Min", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Font/Number_Min_0%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Ui_EnergyGage", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Ui_Dash/Spr_Ui_DashStock_Charge_0%d.png", 22))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Inventory_BackGround", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Inventory/Spr_Ui_Inventory_Background.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Inventory_Frame_Back", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Inventory/Spr_Ui_Inventory_Frame_Back.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Inventory_Frame", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Inventory/Spr_Ui_Inventory_Frame_0%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Inventory_Icon_Back", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Inventory/Spr_Ui_Inventory_Icon_Back_0%d.png", 2))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Inventory_Icon", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Inventory/Spr_Ui_Inventory_Icon_0%d.png", 14))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Interaction_Button", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Interaction/Spr_Ui_Interaction_Button.png"))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Ui_BlackBar", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Ui_BlackBar/Spr_Ui_Interaction_Black_Bar.png"))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Interaction_Dialog_SupplyCrew", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Interaction/Spr_Ui_Interaction_Dialog_SupplyCrew_0%d.png", 5))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Interaction_Dialog_BlackSmith", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Interaction/Spr_Ui_Interaction_Dialog_BlackSmith_0%d.png", 5))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Interaction_Dialog_RegenStone", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Interaction/Spr_Ui_Interaction_Dialog_RegenStone_0%d.png", 5))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Interaction_Dialog_Time", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Interaction/Spr_Ui_Interaction_Dialog_Time_0%d.png", 5))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Ui_TextCallByButton", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Ui_TextCallByButton/TextCallByButton_Back.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Ui_ImgCallByButton", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Ui_ImgCallByButton/Spr_Ui_Get_ItemImg_0%d.png", 11))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Ui_BalloonByRadius", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Ui_BalloonByRadius/Spr_Ui_BalloonByRadius_0%d.png", 7))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Inventory_InvenInfo", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Inventory/InvenInfo/InvenInfo_Back_0%d.png", 9))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Ui_Item_Info_Frame_Drop", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Interaction/Spr_Item_Info_Frame_Drop_0%d.png", 12))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Ui_Item_Info_Frame_Shop", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Interaction/Spr_Item_Info_Frame_Shop_0%d.png", 12))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Ui_BigMap_Back", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_BigMap/Spr_Ui_BigMap_MapBackground.png"))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Ui_BigMap_Room", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_BigMap/Spr_UI_BigMap_Room_0%d.png", 6))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Ui_BigMap_Icon", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_BigMap/Spr_Ui_BigMap_Skill_0%d.png", 4))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Ui_BigMap_Min_Icon", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_BigMap/Spr_Ui_BigMap_Icon_0%d.png", 12))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Ui_MiniMap_Icon", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_BigMap_Icon_0%d.png", 12))))
			return E_FAIL;

#pragma endregion
		
		
		// 코인, 드랍, 숍 아이템
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Item_Coin", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/Coin/Spr_Coin_0%d.png", 5))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Item_ItemImg", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/ItemImg/Spr_Ui_Inventory_Icon_0%d.png", 12))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Coin_Drop", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Item/Coin_Drop/Spr_Effect_Coin_Drop_0%d.png", 9))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Coin_Ground", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Item/Coin_Ground/Spr_Effect_Coin_Ground_0%d.png", 14))))
			return E_FAIL;

		Set_String(L"Buffer Loading.....");
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_TriCol", CTriCol::Create(m_pGraphicDev))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_RcCol", CRcCol::Create(m_pGraphicDev))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_CubeTex", CCubeTex::Create(m_pGraphicDev))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_SphereTex", CSphereTex::Create(m_pGraphicDev))))
			return E_FAIL;
	
		Set_String(L"ETC Loading.....");
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Collider", CCollider::Create(m_pGraphicDev))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Shadow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Shadow/spr_ShadowV2.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_SkyBox", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_SkyBox_Docheol", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/SkyBox_Docheol.dds"))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_CircleBullet_Death", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Bullet/DoewoleBullet/CircleBullet/Spr_Bullet_Doewole_CircleBullet_Death_%d.png", 14))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Monster_Dead_A", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Normal_Monster/Spr_Effect_Monster_Dead_A_0%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Monster_Dead_B", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Normal_Monster/Spr_Effect_Monster_Dead_B_%d.png", 25))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Summon", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Named_Monster/Summon/Spr_Effect_MonsterSummons01_%d.png", 20))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Birth_Charge", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Named_Monster/Birth_Charge/Spr_Bullet_Standard_Charge_%d.png", 19))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Birth_LandMark", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Named_Monster/Birth_LandMark/Spr_Effect_Harmony'sIreha_ChaosForm_Circle_%d.png", 42))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Summon_Wave", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Named_Monster/Summon_Wave/Spr_Effect_MonsterSummons02_%d.png", 21))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_AlertCircle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/AlertCircle/AlertCircle.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_AlertCircle2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/AlertCircle/AlertCircle2.png"))))
			return E_FAIL;

		// Fade In Out용
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Black", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Etc/Black.png"))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"door_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/Door/door_%d.png", 46))))return E_FAIL;

		// Door Effect
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Door_Fire", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Door_Fire/Door_Fire_%d.png", 21))))return E_FAIL;


		// 지형 데이터 읽어들여서 생성
		Load_TerrainData();
		m_bLoad[LOADING_TITLE] = true;
	}

	m_bFinish = true;
	

	return 0;
}

_uint CThreadMgr::ThreadMgr_ForStage_MainLobby(void)
{
	if (!m_bLoad[LOADING_MAINLOBBY])
	{
		CSoundMgr::Get_Instance()->PlaySound(L"Game_start.wav", CSoundMgr::GAME_START, 1.f);


		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_ApostleNpc_Courage_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/ApostleNpc/Spr_Npc_ApostleOfCourage_Idle_0%d.png", 4))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_ApostleNpc_Night_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/ApostleNpc/Spr_Npc_ApostleOfNight_Idle_0%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_ApostleOfTime_Undressing_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/ApostleNpc/Spr_Npc_ApostleOfTime_Undressing_Idle_0%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_ApostleOfTime_Stand_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/ApostleNpc/Spr_Npc_ApostleOfTime_Stand_Idle_0%d.png", 7))))
			return E_FAIL;

		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Tile", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Spr_Object_Goddess'Floor.png"));

		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Portal", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Portal.png"));

		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Portal_Back", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Portal_Back.png"));

		Engine::Ready_ProtoComponent(L"Proto_Texture_SkyBox_MainLobby", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/MainLobby.dds"));

		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Column", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Column_Big.png"));

		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Portal_Effect", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Portal_Effect/GE_%d.png",25));
		
		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Grail", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Grail/Grail_%d.png",6));
		
		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Angel_Bow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Bow/Angel/Angel_Arrow%d.png",7));

		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Kaff_Bow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Bow/Kaff/Kaff_Arrow%d.png",7));

		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Haetae_Bow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Bow/Haetae/Haetae_Arrow%d.png",7));

		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Ice_Bow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Bow/Ice/Ice_Arrow%d.png",7));

		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Saw_Bow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Bow/Saw/Saw_Arrow%d.png",7));

		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Tiff_Bow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Bow/Tiff/Tiff_Arrow%d.png",7));

		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Fire", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Fire/Fire_%d.png",8));

		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Book", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Book/book_%d.png",3));


		Engine::Ready_ProtoComponent(L"Proto_Texture_MainLobby_Chalice", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/chalice.png"));

		Engine::Ready_ProtoComponent(L"Proto_Texture_Story_Book", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MainLobby/Story/storybook_%d.png", 6));

		Engine::Ready_ProtoComponent(L"Proto_Texture_Ending", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Spr_Tilte_logo.png"));






		//Load_TerrainDocheolData();

		m_bLoad[LOADING_MAINLOBBY] = true;
	}
	m_bFinish = true;
	//Set_String(L"Loading Complete");

	return 0;

	
}

_uint CThreadMgr::ThreadMgr_ForStage_Tutorial(void)
{
	if (!m_bLoad[LOADING_TUTORIAL])
	{
		Set_String(L"Obj Texture Loading.....");

		// Dynamic Obj
		if (FAILED(Engine::Ready_ProtoComponent(L"Tuto_spr_bush_01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Tutorial/Dynamic/spr_bush_01/Tuto_spr_bush_01_%d.png", 8))))
			return E_FAIL;						 																														
		if (FAILED(Engine::Ready_ProtoComponent(L"Tuto_spr_bush_02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Tutorial/Dynamic/spr_bush_02/Tuto_spr_bush_02_%d.png", 8))))
			return E_FAIL;						  																														  
		if (FAILED(Engine::Ready_ProtoComponent(L"Tuto_spr_bush_03_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Tutorial/Dynamic/spr_bush_03/Tuto_spr_bush_03_%d.png", 8))))
			return E_FAIL;						  																														  
		if (FAILED(Engine::Ready_ProtoComponent(L"Tuto_spr_bush_04_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Tutorial/Dynamic/spr_bush_04/Tuto_spr_bush_04_%d.png", 8))))
			return E_FAIL;						  																														
		if (FAILED(Engine::Ready_ProtoComponent(L"Tuto_spr_bush_05_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Tutorial/Dynamic/spr_bush_05/Tuto_spr_bush_05_%d.png", 8))))
			return E_FAIL;						 
		if (FAILED(Engine::Ready_ProtoComponent(L"Tuto_Spr_Deco_BushFlower01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Tutorial/Dynamic/spr_bush_flower_01/Tuto_Spr_Deco_BushFlower01_%d.png", 8))))
			return E_FAIL;						  																																		  
		if (FAILED(Engine::Ready_ProtoComponent(L"Tuto_Spr_Deco_BushFlower02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Tutorial/Dynamic/spr_bush_flower_02/Tuto_Spr_Deco_BushFlower02_%d.png", 8))))
			return E_FAIL;

		// Static Obj
		if (FAILED(Engine::Ready_ProtoComponent(L"TutorialObj_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Tutorial/Static/TutorialObj_%d.png", 11))))
			return E_FAIL;

		//// 슬라임_일반 충돌형_1
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_GreenEvilSlime_Move", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_GreenEvilSlime_Move_0%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_GreenEvilSlime_Attack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_GreenEvilSlime_Attack_0%d.png", 10))))
			return E_FAIL;
		// 이블소울_일반 원거리형_1
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_45_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_Lv2_EvilSoul_Stand_045_0%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_45_Move", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_Lv2_EvilSoul_Move_045_0%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_135_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_Lv2_EvilSoul_Stand_135_0%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_135_Move", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_Lv2_EvilSoul_Move_135_0%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Monster_Bullet_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MonsterBullet/Spr_Bullet_Deva_Circle_White_0%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Monster_Bullet_2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MonsterBullet/Spr_Bullet_SwordShot_White_0%d.png", 7))))return E_FAIL;
		// 뮤턴트 이블소울_변형 원거리형_1
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_45_All", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_MutationEvilSoul_Stand_045_0%d.png", 7))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilSoul_135_All", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_MutationEvilSoul_Stand_135_0%d.png", 7))))return E_FAIL;
#pragma endregion

#pragma region Effect_Junseok
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Bullet_1_Dead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Normal_Monster/Spr_Effect_Deva_Hit_0%d.png", 12))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Bullet_2_Dead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Normal_Monster/Spr_Bullet_SwordShot_Hit_0%d.png", 8))))return E_FAIL;

		// Obj
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Tutorial_Arrow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Tutorial/Arrow/Arrow%d.png", 6))))return E_FAIL;

		// UI
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Tutorial_UI", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Tutorial/UI_Tutorial/TutoUI%d.png", 6))))return E_FAIL;


		Load_TerrainTutorialData();

		m_bLoad[LOADING_TUTORIAL] = true;
	}
	m_bFinish = true;

	return 0;
}

_uint CThreadMgr::ThreadMgr_ForStage_DungeonLobby(void)
{
	if (!m_bLoad[LOADING_DUNGEONLOBBY])
	{	
		Set_String(L"Monster Texture Loading.....");

#pragma region Npc
		Set_String(L"NPC Texture Loading.....");
	


		Set_String(L"Obj Texture Loading.....");

		// Static Obj
		if (FAILED(Engine::Ready_ProtoComponent(L"DungeonLobbyObj_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Static/DungeonLobbyObj_%d.png", 15))))
			return E_FAIL;

		// Dynamic Obj
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_spr_bush_01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/spr_bush_01/DLobby_spr_bush_01_%d.png", 8))))
			return E_FAIL;						 																																
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_spr_bush_02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/spr_bush_02/DLobby_spr_bush_02_%d.png", 8))))
			return E_FAIL;						 																																 
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_spr_bush_03_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/spr_bush_03/DLobby_spr_bush_03_%d.png", 8))))
			return E_FAIL;						 																																 
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_spr_bush_04_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/spr_bush_04/DLobby_spr_bush_04_%d.png", 8))))
			return E_FAIL;						 																																 
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_spr_bush_05_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/spr_bush_05/DLobby_spr_bush_05_%d.png", 8))))
			return E_FAIL;						 
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_Spr_Deco_BushFlower01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/spr_bush_flower_01/DLobby_Spr_Deco_BushFlower01_%d.png", 8))))
			return E_FAIL;						 
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_Spr_Deco_BushFlower02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/spr_bush_flower_02/DLobby_Spr_Deco_BushFlower02_%d.png", 8))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_BossDoor_Head_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Door/BossDoor_Head/DLobby_BossDoor_Head_%d.png", 11))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_BossDoor_Pillar_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Door/BossDoor_Pillar/DLobby_BossDoor_Pillar_%d.png", 11))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_BossDoor_Pillar2_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Door/BossDoor_Pillar2/DLobby_BossDoor_Pillar2_%d.png", 11))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_BossDoor_Fire_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Door/BossDoor_Fire/DLobby_BossDoor_Fire_%d.png", 7))))return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_SideDoor_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Door/SideDoor/DLobby_SideDoor_%d.png", 11))))return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_NormalDoor_Head_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Door/NormalDoor_Head/DLobby_NormalDoor_Head_%d.png", 11))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_NormalDoor_Pillar_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Door/NormalDoor_Pillar/DLobby_NormalDoor_Pillar_%d.png", 11))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_NormalDoor_Pillar2_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Door/NormalDoor_Pillar2/DLobby_NormalDoor_Pillar2_%d.png", 11))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_NormalDoor_Fire_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Door/NormalDoor_Fire/DLobby_NormalDoor_Fire_%d.png", 7))))return E_FAIL;
		

		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_door_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Door/DLobby_door_%d.png", 46)))) return E_FAIL;


		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_InfectionWall2_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Destructive/InfectionWall2/DLobby_InfectionWall2_%d.png", 3)))) return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_Pillar02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Destructive/Pillar2/DLobby_Pillar02_%d.png", 3)))) return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_Spr_Infectionthorn01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Destructive/spr_thorn_01/DLobby_Spr_Infectionthorn01_%d.png", 4)))) return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_Spr_Infectionthorn02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Destructive/spr_thorn_02/DLobby_Spr_Infectionthorn02_%d.png", 4)))) return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_Spr_Infectionthorn03_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Destructive/spr_thorn_03/DLobby_Spr_Infectionthorn03_%d.png", 3)))) return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_Spr_Infectionthorn04_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Destructive/spr_thorn_04/DLobby_Spr_Infectionthorn04_%d.png", 3)))) return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_Spr_Infectionthorn05_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Destructive/spr_thorn_05/DLobby_Spr_Infectionthorn05_%d.png", 3)))) return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_Spr_Infectionthorn06_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Destructive/spr_thorn_06/DLobby_Spr_Infectionthorn06_%d.png", 3)))) return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"DLobby_StoneWall02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/DungeonLobby/Dynamic/Destructive/StoneWall2/DLobby_StoneWall02_%d.png", 3)))) return E_FAIL;


		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_WhiteRabbit_Blue", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/WhiteRabbit/Spr_Npc_WhiteRabbit_Blue_0%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_WhiteRabbit_Pink", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/WhiteRabbit/Spr_Npc_WhiteRabbit_Pink_0%d.png", 7))))
			return E_FAIL;
		

		Load_TerrainDungeonLobbyData();

#pragma endregion
		m_bLoad[LOADING_DUNGEONLOBBY] = true;
	}
	m_bFinish = true;
	//Set_String(L"Loading Complete");

	return 0;
}

_uint CThreadMgr::ThreadMgr_ForStage_Regen(void)
{
	if (!m_bLoad[LOADING_REGEN])
	{
		Set_String(L"Map Texture Loading.....");
#pragma region Map_Texture
		// dynamic Object
		if (FAILED(Engine::Ready_ProtoComponent(L"Temple_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Moving/Temple_%d.png", 15))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_Temple_Body_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Moving/Spr_Temple_Body_%d.png", 11))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_Temple_Head_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Moving/Spr_Temple_Head_%d.png", 11))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"StoneWall01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Dynamic/StoneWall/StoneWall01_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"StoneWall02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Dynamic/StoneWall2/StoneWall02_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"InfectionWall2_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Dynamic/InfectionWall2/InfectionWall2_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Pillar01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Dynamic/Pillar1/Pillar01_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Pillar02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Dynamic/Pillar2/Pillar02_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Pillar04_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Dynamic/Pillar4/Pillar04_%d.png", 4))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Regen_door_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Dynamic/Door/Regen_door_%d.png", 13))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Regen_SideDoor_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Dynamic/Door/SideDoor/Regen_SideDoor_%d.png", 11))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Regen_NormalDoor_Pillar2_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Dynamic/Door/RegenDoor_Pillar2/Regen_NormalDoor_Pillar2_%d.png", 11))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Regen_NormalDoor_Fire_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Dynamic/Door/RegenDoor_Fire/Regen_NormalDoor_Fire_%d.png", 7))))
			return E_FAIL;

		// static Object
		if (FAILED(Engine::Ready_ProtoComponent(L"RegenObj_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Regen/Static/RegenObj_%d.png", 105))))
			return E_FAIL;
#pragma endregion

		Set_String(L"Monster Texture Loading.....");
#pragma region Normal_Monster_Texture
		// GreenEvilBat
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_GreenEvilBat_45", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_GreenEvilBat_045_%d.png", 6))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_GreenEvilBat_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Spr_Monster_GreenEvilBat_135_%d.png", 6))))
			return E_FAIL;
		// EvilFrog
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilFrog_Attack_45", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/EvilFrog/Spr_Monster_EvilFrog_Attack_045_%d.png", 5))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilFrog_Attack_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/EvilFrog/Spr_Monster_EvilFrog_Attack_135_%d.png", 5))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilFrog_Jump_45", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/EvilFrog/Spr_Monster_EvilFrog_Jump_045_%d.png", 6))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilFrog_Jump_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/EvilFrog/Spr_Monster_EvilFrog_Jump_135_%d.png", 6))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilFrog_Stand_45", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/EvilFrog/Spr_Monster_EvilFrog_Stand_045_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_EvilFrog_Stand_135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/EvilFrog/Spr_Monster_EvilFrog_Stand_135_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_BlueEvilSoulBall_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/BlueEvilSoulBall/Spr_Monster_BlueEvilSoulBall_Idle_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_BlueEvilSoulBall_Death", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/BlueEvilSoulBall/Spr_Monster_BlueEvilSoulBall_Death_%d.png", 14))))
			return E_FAIL;
#pragma endregion


#pragma region Named_Monster_Texture
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_AxeEliteEvilSoul_Stand045", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Named/AxeEliteEvilSoul/Stand/Down/Spr_Monster_AxeEliteEvilSoul_Stand_045_%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_AxeEliteEvilSoul_Stand135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Named/AxeEliteEvilSoul/Stand/Up/Spr_Monster_AxeEliteEvilSoul_Stand_135_%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_AxeEliteEvilSoul_Attack045", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Named/AxeEliteEvilSoul/Attack/Down/Spr_Monster_AxeEliteEvilSoul_Attack_045_%d.png", 9))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_AxeEliteEvilSoul_Attack135", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Named/AxeEliteEvilSoul/Attack/Up/Spr_Monster_AxeEliteEvilSoul_Attack_135_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_AxeEliteEvilSoul_AttackRot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Named/AxeEliteEvilSoul/AttackRot/Spr_Monster_AxeEliteEvilSoul_AttackLoof_%d.png", 5))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_AxeEliteEvilSoul_Death", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Monster/Named/AxeEliteEvilSoul/Death/Spr_Monster_AxeEliteEvilSoul_Death_045_%d.png", 11))))
			return E_FAIL;
#pragma endregion
		
#pragma region Named_Monster_Effect
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Whirlwind", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Named_Monster/AxeEliteEvilSoul/Whirlwind/Spr_Effect_Whirlwind_%d.png", 6))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_BleedingDeath", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Named_Monster/BleedingDeath/Spr_Effect_No098_BloodDestroyerBuffHitEffect01_%d.png", 15))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_LeafBulletDeath", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Named_Monster/Leaf_Bullet_Death/Spr_Effect_Leaf_Bullet_Dead_%d.png", 5))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_LeafBulletBackHit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Named_Monster/Leaf_Bullet_BackHit/Spr_Effect_No009_FairyArrow_BackHitEffect_%d.png", 12))))
			return E_FAIL;
#pragma endregion

#pragma region Normal_Monster_Effect
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Birth_Green", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Normal_Monster/Bullet_Birth/Green/Spr_Bullet_Standard_Birth_Green_%d.png", 20))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Birth_RayUp_Green", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Normal_Monster/Bullet_Birth_RayUp/Green/Spr_Bullet_Standard_BirthRayUp_Green_%d.png", 11))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Death_Red", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Normal_Monster/Bullet_Death/Red/Spr_Bullet_Standard_Death_Red_%d.png", 12))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Bullet_BackHit", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Normal_Monster/Bullet_BackHit/Spr_Bullet_SwordShot_Hit_%d.png", 9))))
			return E_FAIL;
#pragma endregion

#pragma region Named_Monster_Bullet
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Leaf", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MonsterBullet/Leaf/Spr_Bullet_LaulaStandardBullet_%d.png", 6))))
			return E_FAIL;
#pragma endregion

#pragma region Normal_Monster_Bullet
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_DoubleLeaf", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MonsterBullet/DoubleLeaf/Spr_Bullet_Cheonlog_DoubleFireworkLv1_%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Return", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MonsterBullet/Return_Bullet/Spr_Bullet_FrogBullet_%d.png", 6))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Chain01", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MonsterBullet/Chain_Bullet/Spr_Bullet_Chain01_%d.png", 5))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Chain02", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MonsterBullet/Chain_Bullet/Spr_Bullet_Chain02_%d.png", 5))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_ChainHead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/MonsterBullet/Chain_Bullet/Spr_Bullet_ChainHead_%d.png", 8))))
			return E_FAIL;
#pragma endregion

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_KeyNamed", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Named_Monster/key_%d.png"))))
			return E_FAIL;

		Load_TerrainRegenData();

		m_bLoad[LOADING_REGEN] = true;
	}

	m_bFinish = true;
	//Set_String(L"Loading Complete");

	return 0;
}

_uint CThreadMgr::ThreadMgr_ForStage_Boss1(void)
{
	if (!m_bLoad[LOADING_BOSS1])
	{
		// 보스 도올
		Set_String(L"Boss Texture Loading.....");
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Body_Stand", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleBody/Stand/Spr_Boss_DoewoleBody_Stand_%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Body_Stand_FaceOn", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleBody/Stand_FaceOn/Spr_Boss_DoewoleBody_Stand_FaceOn_%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Body_PowerSlam", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleBody/PowerSlam/Spr_Boss_DoewoleBody_PowerSlam_%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Body_PowerSlamAfter", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleBody/PowerSlamAfter/Spr_Boss_DoewoleBody_PowerSlamAfter_%d.png", 9))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Body_PowerSlamFaceOff", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleBody/PowerSlamFaceoff/Spr_Boss_DoewoleBody_PowerSlamFaceoff_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Body_Scratch", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleBody/Scratch/Spr_Boss_DoewoleBody_Scratch_%d.png", 6))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Body_Dipping", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleBody/Dipping/Spr_Boss_DoewoleBody_Dipping_%d.png", 11))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Body_Death", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleBody/Death/Spr_Boss_DoewoleBody_Death_%d.png", 18))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Claw_Stand", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleClaw/Stand/Spr_Boss_DoewoleClaw_Stand_%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Claw_StandardAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleClaw/StandardAttack/Spr_Boss_DoewoleClaw_StandardAttack%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Claw_OutStretchAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleClaw/OutStretchAttack/Spr_Boss_DoewoleClaw_SpellAction_%d.png", 9))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Claw_Up", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleClaw/Up/Spr_Boss_DoewoleClaw_Up_%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Claw_Smash", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleClaw/Smash/Spr_Boss_DoewoleClaw_DippingEffect_%d.png", 14))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Claw_Scratch", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleClaw/CrossScratch/Spr_Boss_DoewoleClaw_ScratchEffect_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Claw_CrossScratch", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleClaw/CrossScratch/Spr_Boss_DoewoleClaw_CrossScratch_%d.png", 14))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Doewole_Claw_Death", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Doewole/Boss_DoewoleClaw/Death/Spr_Boss_DoewoleClaw_Death_%d.png", 13))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Doewole_Vanish", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Doewole_Vanish/Spr_Effect_No051_Vanish_%d.png", 6))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Doewole_StandardAttack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Doewole_StandardAttack/Spr_Effect_Doewole_StandardAttack_Pulse02_%d.png", 17))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Doewole_ChargeExplosion", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Doewole_OutStretchAttack/Spr_Effect_Doewole_ChargeExplosion_%d.png", 16))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Doewole_Loof", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Doewole_OutStretchAttack/Spr_Effect_Doewole_RadialLinearLoof_%d.png", 12))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Doewole_ChargeCircle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Doewole_ChargeCircle/Spr_Effect_Doewole_ChargeCircle_%d.png", 13))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Doewole_Slam", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Doewole_Slam/Spr_Effect_DoewoleClaw_SlamEffect_%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_AlertRect", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/AlertRect/Spr_DiagonalWarning.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Doewole_Hurt", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Doewole_Hurt/Spr_Effect_Buff_BleedingHurt.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_StandardBullet_Death", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Bullet/DoewoleBullet/Standard/Spr_Bullet_Doewole_Death_%d.png", 14))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_SwordShot_Death", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Bullet/DoewoleBullet/SwordShot/Spr_Bullet_SwordShot_Death_%d.png", 12))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Doewole_SwordShot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Bullet/DoewoleBullet/SwordShot/Spr_Bullet_Doewole_SwordShot_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Doewole_Circle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Bullet/DoewoleBullet/CircleBullet/Spr_Bullet_Doewole_CircleBullet_Black_%d.png", 5))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Doewole_Standard", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Bullet/DoewoleBullet/Standard/Spr_Bullet_Doewole_Standard_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Doewole_Thorn", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Bullet/DoewoleBullet/Thorn/Spr_InfectionDoor_Thorns01_%d.png", 8))))
			return E_FAIL;

		//// 보스 UI
		//if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_BossBar_Back", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_BossBar_Back.png"))))
		//	return E_FAIL;
		//if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_BossBar_Hp", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_BossBar_Hp.png"))))
		//	return E_FAIL;
		//if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_BossBar_Icon_1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ui_Front/Spr_Ui_BossBar_Icon_3.png"))))
		//	return E_FAIL;
		//
		m_bLoad[LOADING_BOSS1] = true;
	}
	m_bFinish = true;
	//Set_String(L"Loading Complete");

	return 0;
}

_uint CThreadMgr::ThreadMgr_ForStage_Boss2(void)
{
	if (!m_bLoad[LOADING_BOSS2])
	{
		// 보스 도철
		Set_String(L"Boss Texture Loading.....");
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_Appear", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/Appear/Spr_Boss_Docheol_Appear_%d.png", 25))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_HandUpAppear", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/HandUpAppear/Spr_Boss_Docheol_HandUpAppear_%d.png", 14))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_Stand", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/Stand/Spr_Boss_Docheol_Stand_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_Swing", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/Swing/Spr_Boss_Docheol_RightSwing_%d.png", 12))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_TwoHandSlam", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/TwoHandSlam/Spr_Boss_Docheol_TwoHandSlam_%d.png", 11))))
			return E_FAIL;
		
		Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_FullSwing", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/FullSwing/Spr_Boss_Docheol_FullSwing_%d.png", 12));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_RageUp", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/Rage_Up/Spr_Boss_Docheol_RageUp_%d.png", 33));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_RageStand", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/Rage_Stand/Spr_Boss_Docheol_Rage_Stand_%d.png", 8));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_RageSwing", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/Rage_RightSwing/Spr_Boss_Docheol_Rage_RightSwing_%d.png", 11));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_RageTwoHandSlam", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/Rage_TwoHandSlam/Spr_Boss_Docheol_Rage_TwoHandSlam_%d.png", 11));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_BossDead", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/Death/Spr_Boss_Docheol_Death_%d.png", 27));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_RageCharge", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/Rage_Charge/Spr_Boss_Docheol_Rage_Charge_%d.png", 8));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Docheol_TwoHandSlamShake", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Boss_Docheol/Rage_TwoHandSlamShake/Spr_Boss_Docheol_Rage_TwoHandSlamShake_%d.png", 1));

		Set_String(L"Effect Texture Loading.....");
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_FloorFire", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/FloorFire/Spr_Effect_Docheol_FloorFire_%d.png", 24));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_Tile", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/Tile/TileEffect%d.png", 4));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_AppearMarkFire", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/AppearMarkFire/Spr_Effect_DocheolAppearMarkFire_%d.png", 17));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_FloorFireRayUp", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/FloorFireRayUp/Spr_Effect_Docheol_FloorFireRayUp_%d.png", 20));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_FireExplosion", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/FireExplosion/%d.png", 28));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_SlamExplosion", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/SlamExplosion/c%d.png", 24));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_FireSpread", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/FireSpread/%d.png", 17));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_Fire", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/Fire/%d.png", 16));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_FireCircle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/FireCircle/%d.png", 19));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_SwingEffect", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/SwingEffect/%d.png", 14));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_Mark", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/Fire2/Mark.png"));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_Fire2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/Fire2/%d.png", 28));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_Meteor", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/Meteor/%d.png", 15));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_MeteorExplosion", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/MeteorExplosion/%d.png", 27));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_FireExplosion2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/FireExplosion2/%d.png", 22));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_Electronic", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/Electronic/%d.png", 6));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_Mark2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/Mark2/Mark%d.png", 10));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_FireBall", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/FireBall/Spr_Effect_DocheolFireBall_%d.png", 24));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_FirePulse", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/FirePulse/Spr_Effect_DocheolFirePulse_%d.png", 24));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_FireExplosion3", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/FireExplosion3/%d.png", 15));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_Fire3", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/Fire3/%d.png", 34));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_Circle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/Circle/Boss_circle_%d.png", 21));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_Docheol_Electronic2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Effect/Docheol/Electronic2/Spr_Effect_ChaosEye_Electricity01_%d.png", 12));

		Set_String(L"Bullet Texture Loading.....");
		Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Docheol_SwingBullet", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Bullet/DocheolBullet/SwingBullet/%d.png", 10));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Docheol_SlamExplosion", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Bullet/DocheolBullet/SlamExplosion/%d.png", 24));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Bullet_Docheol_DoorBullet", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss/Bullet/DocheolBullet/DoorBullet/DoorBullet%d.png", 40));

		Set_String(L"Object Texture Loading.....");
		Engine::Ready_ProtoComponent(L"Proto_Texture_Object_DocheolTile", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDocheol/DocheolTile.png"));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Object_DocheolWall", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDocheol/DocheolWall.png"));
		Engine::Ready_ProtoComponent(L"Proto_Texture_Object_DocheolSpecialDoor", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDocheol/SpecialDoor%d.png", 4));
		Engine::Ready_ProtoComponent(L"DocheolObj", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/BossDocheol/FloatingObj/DocheolObj%d.png", 24));

		Load_TerrainDocheolData();


		m_bLoad[LOADING_BOSS2] = true;
	}
	m_bFinish = true;
	//Set_String(L"Loading Complete");

	return 0;
}

_uint CThreadMgr::ThreadMgr_ForStage_Pot(void)
{
	if (!m_bLoad[LOADING_POT])
	{
		Set_String(L"Map Texture Loading.....");
#pragma region Map_Texture
		// dynamic Object
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_spr_bush_01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_bush_01/Jar_spr_bush_01_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_spr_bush_02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_bush_02/Jar_spr_bush_02_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_spr_bush_03_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_bush_03/Jar_spr_bush_03_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_spr_bush_04_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_bush_04/Jar_spr_bush_04_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_spr_bush_05_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_bush_05/Jar_spr_bush_05_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_Spr_Deco_BushFlower01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_bush_flower_01/Jar_Spr_Deco_BushFlower01_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_Spr_Deco_BushFlower02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_bush_flower_02/Jar_Spr_Deco_BushFlower02_%d.png", 8))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_Spr_Infectionthorn01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_thorn_01/Jar_Spr_Infectionthorn01_%d.png", 4))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_Spr_Infectionthorn02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_thorn_02/Jar_Spr_Infectionthorn02_%d.png", 4))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_Spr_Infectionthorn03_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_thorn_03/Jar_Spr_Infectionthorn03_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_Spr_Infectionthorn04_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_thorn_04/Jar_Spr_Infectionthorn04_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_Spr_Infectionthorn05_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_thorn_05/Jar_Spr_Infectionthorn05_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_Spr_Infectionthorn06_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/spr_thorn_06/Jar_Spr_Infectionthorn06_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_InfectionWall2_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/InfectionWall2/Jar_InfectionWall2_%d.png", 3))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"HistoricSitesObj1_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/HistoricSitesObj1_/HistoricSitesObj1_%d.png", 11))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"HistoricSitesObj2_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/HistoricSitesObj2_/HistoricSitesObj2_%d.png", 11))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"HistoricSitesObj3_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/HistoricSitesObj3_/HistoricSitesObj3_%d.png", 11))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"HistoricSitesObj4_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/HistoricSitesObj3_/HistoricSitesObj4_%d.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_HistoricSites_DecoObject_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/Jar/Spr_HistoricSites_DecoObject_%d.png", 9)))) return E_FAIL;

		
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_TreasureJar_Rare_Center_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/OpenJar/Rare/Spr_TreasureJar_Rare_Center_%d.png", 11)))) 
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_TreasureJar_Rare_Body_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/OpenJar/Rare/Spr_TreasureJar_Rare_Body_%d.png")))) 
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Spr_TreasureJar_Rare_Head_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/OpenJar/Rare/Spr_TreasureJar_Rare_Head_%d.png"))))
			return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_door_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/Door/Jar_door_%d.png", 13))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_SideDoor_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/Door/SideDoor/Jar_SideDoor_%d.png", 11))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_NormalDoor_Pillar2_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/Door/JarDoor_Pillar2/Jar_NormalDoor_Pillar2_%d.png", 11))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_NormalDoor_Fire_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/Door/JarDoor_Fire/Jar_NormalDoor_Fire_%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_NormalDoor_Head_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/Door/JarDoor_Head/Jar_NormalDoor_Head_%d.png", 11))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Jar_NormalDoor_Pillar_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Dynamic/Door/JarDoor_Pillar/Jar_NormalDoor_Pillar_%d.png", 11))))
			return E_FAIL;

		// static Object
		if (FAILED(Engine::Ready_ProtoComponent(L"JarMapObj_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Jar/Static/JarMapObj_%d.png", 24))))
			return E_FAIL;

		// Object Effect
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_JarOpen", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/OpenJar/Spr_Ui_Effect_DashStockBirthEffectV2_%d.png", 20))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Effect_JarItemDrop", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Object/OpenJar/Spr_Ui_Effect_DeathResult_%d.png", 14))))
			return E_FAIL;

#pragma endregion

		Load_TerrainPotData();

		m_bLoad[LOADING_POT] = true;
	}

	m_bFinish = true;
	//Set_String(L"Loading Complete");

	return 0;
}

_uint CThreadMgr::ThreadMgr_ForStage_Normal(void)
{
	if (!m_bLoad[LOADING_NORMAL])
	{
		// Static Obj
		if (FAILED(Engine::Ready_ProtoComponent(L"NormalObj_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Normal/Static/NormalObj_%d.png", 1))))return E_FAIL;

		// Dynamic Obj
		Set_String(L"Object Texture Loading.....");
		if (FAILED(Engine::Ready_ProtoComponent(L"Normal_door_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Normal/Dynamic/Door/Normal_door_%d.png", 46)))) return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Normal_SideDoor_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Normal/Dynamic/Door/SideDoor/Normal_SideDoor_%d.png", 11))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Normal_NormalDoor_Pillar_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Normal/Dynamic/Door/NormalDoor_Pillar/Normal_NormalDoor_Pillar_%d.png", 11))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Normal_NormalDoor_Pillar2_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Normal/Dynamic/Door/NormalDoor_Pillar2/Normal_NormalDoor_Pillar2_%d.png", 11))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Normal_NormalDoor_Fire_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Normal/Dynamic/Door/NormalDoor_Fire/Normal_NormalDoor_Fire_%d.png", 7))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Normal_NormalDoor_Head_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Normal/Dynamic/Door/NormalDoor_Head/Normal_NormalDoor_Head_%d.png", 11))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Normal_spr_bush_01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Normal/Dynamic/spr_bush_01/Normal_spr_bush_01_%d.png", 8))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Normal_spr_bush_02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Normal/Dynamic/spr_bush_02/Normal_spr_bush_02_%d.png", 8))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Normal_spr_bush_03_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Normal/Dynamic/spr_bush_03/Normal_spr_bush_03_%d.png", 8))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Normal_Spr_Deco_BushFlower01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Normal/Dynamic/spr_bush_flower_01/Normal_Spr_Deco_BushFlower01_%d.png", 8))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Normal_Spr_Deco_BushFlower02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Normal/Dynamic/spr_bush_flower_02/Normal_Spr_Deco_BushFlower02_%d.png", 8))))return E_FAIL;

		m_bLoad[LOADING_NORMAL] = true;
		
		Load_TerrainNormalData();

	}
	m_bFinish = true;

	return 0;
	
}

_uint CThreadMgr::ThreadMgr_ForStage_Shop(void)
{
	if (!m_bLoad[LOADING_SHOP])
	{
		

#pragma region Npc
		Set_String(L"NPC Texture Loading.....");
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_SupplyCrew_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/SupplyCrew/Spr_Npc_SupplyCrew_Idle_0%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_SupplyCrew_Pleasure", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/SupplyCrew/Spr_Npc_SupplyCrew_Pleasure_0%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_SupplyCrew_Salute", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/SupplyCrew/Spr_Npc_SupplyCrew_Salute_0%d.png", 7))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_BlackSmith_Idle", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/BlackSmith/Spr_npc_blacksmith_0%d.png", 4))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_SupplyBigCat", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/SupplyCrew/Spr_Npc_SupplyBigCat_Stand_0%d.png", 10))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_SupplyBackPack", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/SupplyCrew/Spr_Npc_SupplyCrewBackpack_320.png"))))
			return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Proto_Texture_Npc_RerollLever", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Npc/RerollLever/Spr_Npc_RerollLever_0%d.png", 8))))
			return E_FAIL;

		Set_String(L"Obj Texture Loading.....");

		// Static Obj
		if (FAILED(Engine::Ready_ProtoComponent(L"ShopObj_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Shop/Static/ShopObj_%d.png", 14))))return E_FAIL;

		// Dynamic Obj
		if (FAILED(Engine::Ready_ProtoComponent(L"Shop_door_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Shop/Dynamic/Door/Shop_door_%d.png", 46)))) return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Shop_SideDoor_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Shop/Dynamic/Door/SideDoor/Shop_SideDoor_%d.png", 11))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Shop_NormalDoor_Pillar2_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Shop/Dynamic/Door/NormalDoor_Pillar2/Shop_NormalDoor_Pillar2_%d.png", 11))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Shop_NormalDoor_Fire_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Shop/Dynamic/Door/NormalDoor_Fire/Shop_NormalDoor_Fire_%d.png", 7))))return E_FAIL;

		if (FAILED(Engine::Ready_ProtoComponent(L"Shop_spr_bush_01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Shop/Dynamic/spr_bush_01/Shop_spr_bush_01_%d.png", 8))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Shop_spr_bush_02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Shop/Dynamic/spr_bush_02/Shop_spr_bush_02_%d.png", 8))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Shop_spr_bush_03_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Shop/Dynamic/spr_bush_03/Shop_spr_bush_03_%d.png", 8))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Shop_spr_bush_04_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Shop/Dynamic/spr_bush_04/Shop_spr_bush_04_%d.png", 8))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Shop_spr_bush_05_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Shop/Dynamic/spr_bush_05/Shop_spr_bush_05_%d.png", 8))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Shop_Spr_Deco_BushFlower01_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Shop/Dynamic/spr_bush_flower_01/Shop_Spr_Deco_BushFlower01_%d.png", 8))))return E_FAIL;
		if (FAILED(Engine::Ready_ProtoComponent(L"Shop_Spr_Deco_BushFlower02_", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Object/Shop/Dynamic/spr_bush_flower_02/Shop_Spr_Deco_BushFlower02_%d.png", 8))))return E_FAIL;
		

		Load_TerrainShopData();


		m_bLoad[LOADING_SHOP] = true;
	}
	m_bFinish = true;

	return 0;
}



_uint CThreadMgr::Load_TerrainData(void)
{
	//HANDLE hFile = CreateFile(L"../../Data/Terrain/TestMap.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	HANDLE hFile = CreateFile(L"../../Data/Terrain/BossDowoleMap.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	TERRAINDATA tTerrainData;
	ZeroMemory(&tTerrainData, sizeof(TERRAINDATA));
	TERRAINDATA_P tTerrainDataPointer;
	ZeroMemory(&tTerrainDataPointer, sizeof(TERRAINDATA_P));

	ReadFile(hFile, &tTerrainData, sizeof(TERRAINDATA), &dwByte, nullptr);

	ReadFile(hFile, &(tTerrainDataPointer.dwVtxCnt), sizeof(_ulong), &dwByte, nullptr);

	tTerrainDataPointer.pPos = new _vec3[tTerrainDataPointer.dwVtxCnt];

	for (int i = 0; i < tTerrainDataPointer.dwVtxCnt; ++i)
		ReadFile(hFile, &(tTerrainDataPointer.pPos[i]), sizeof(_vec3), &dwByte, nullptr);

	//Safe_Delete_Array(tTerrainDataPointer.pPos);

	ReadFile(hFile, &dwStrByte, sizeof(_ulong), &dwByte, nullptr);

	TCHAR* pHeightmapFilePath = nullptr;
	pHeightmapFilePath = new TCHAR[dwStrByte];

	ReadFile(hFile, pHeightmapFilePath, dwStrByte, &dwByte, nullptr);

	//wstring strFilePath;
	//wstring strHeightmapFilePath;
	//strFilePath = pHeightmapFilePath;
	//strHeightmapFilePath = strFilePath.substr(0, 3);
	//strHeightmapFilePath += strFilePath.substr(10);

	//_tchar* pHeightmapPath = nullptr;
	//pHeightmapPath = new TCHAR[dwStrByte];
	//lstrcpy(pHeightmapPath, strHeightmapFilePath.c_str());


	//FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, pHeightmapPath));
	if (FAILED(Engine::Ready_ProtoComponent(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, tTerrainDataPointer.pPos, tTerrainData.fDetailLevel))))
		return E_FAIL;

	Safe_Delete_Array(pHeightmapFilePath);
	//Safe_Delete_Array(pHeightmapPath);

	CloseHandle(hFile);

	return 0;
}

_uint CThreadMgr::Load_TerrainDocheolData(void)
{
	HANDLE hFile = CreateFile(L"../../Data/Terrain/BossDocheolMap.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	TERRAINDATA tTerrainData;
	ZeroMemory(&tTerrainData, sizeof(TERRAINDATA));
	TERRAINDATA_P tTerrainDataPointer;
	ZeroMemory(&tTerrainDataPointer, sizeof(TERRAINDATA_P));

	ReadFile(hFile, &tTerrainData, sizeof(TERRAINDATA), &dwByte, nullptr);

	ReadFile(hFile, &(tTerrainDataPointer.dwVtxCnt), sizeof(_ulong), &dwByte, nullptr);

	tTerrainDataPointer.pPos = new _vec3[tTerrainDataPointer.dwVtxCnt];

	for (int i = 0; i < tTerrainDataPointer.dwVtxCnt; ++i)
		ReadFile(hFile, &(tTerrainDataPointer.pPos[i]), sizeof(_vec3), &dwByte, nullptr);

	ReadFile(hFile, &dwStrByte, sizeof(_ulong), &dwByte, nullptr);

	TCHAR* pHeightmapFilePath = nullptr;
	pHeightmapFilePath = new TCHAR[dwStrByte];

	ReadFile(hFile, pHeightmapFilePath, dwStrByte, &dwByte, nullptr);

	if (FAILED(Engine::Ready_ProtoComponent(L"Proto_TerrainTex_Docheol", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, tTerrainDataPointer.pPos, tTerrainData.fDetailLevel))))
		return E_FAIL;


	Safe_Delete_Array(pHeightmapFilePath);

	CloseHandle(hFile);

	return 0;
}

_uint CThreadMgr::Load_TerrainRegenData(void)
{
	HANDLE hFile = CreateFile(L"../../Data/Terrain/RegenMap.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	TERRAINDATA tTerrainData;
	ZeroMemory(&tTerrainData, sizeof(TERRAINDATA));
	TERRAINDATA_P tTerrainDataPointer;
	ZeroMemory(&tTerrainDataPointer, sizeof(TERRAINDATA_P));

	ReadFile(hFile, &tTerrainData, sizeof(TERRAINDATA), &dwByte, nullptr);

	ReadFile(hFile, &(tTerrainDataPointer.dwVtxCnt), sizeof(_ulong), &dwByte, nullptr);

	tTerrainDataPointer.pPos = new _vec3[tTerrainDataPointer.dwVtxCnt];

	for (int i = 0; i < tTerrainDataPointer.dwVtxCnt; ++i)
		ReadFile(hFile, &(tTerrainDataPointer.pPos[i]), sizeof(_vec3), &dwByte, nullptr);

	//Safe_Delete_Array(tTerrainDataPointer.pPos);

	ReadFile(hFile, &dwStrByte, sizeof(_ulong), &dwByte, nullptr);

	TCHAR* pHeightmapFilePath = nullptr;
	pHeightmapFilePath = new TCHAR[dwStrByte];

	ReadFile(hFile, pHeightmapFilePath, dwStrByte, &dwByte, nullptr);

	//FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, pHeightmapPath));
	if (FAILED(Engine::Ready_ProtoComponent(L"Proto_TerrainTex_Regen", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, tTerrainDataPointer.pPos, tTerrainData.fDetailLevel))))
		return E_FAIL;

	Safe_Delete_Array(pHeightmapFilePath);
	//Safe_Delete_Array(pHeightmapPath);

	CloseHandle(hFile);

	return 0;
}

_uint CThreadMgr::Load_TerrainTutorialData(void)
{
	HANDLE hFile = CreateFile(L"../../Data/Terrain/TutoriaLMap.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	TERRAINDATA tTerrainData;
	ZeroMemory(&tTerrainData, sizeof(TERRAINDATA));
	TERRAINDATA_P tTerrainDataPointer;
	ZeroMemory(&tTerrainDataPointer, sizeof(TERRAINDATA_P));

	ReadFile(hFile, &tTerrainData, sizeof(TERRAINDATA), &dwByte, nullptr);

	ReadFile(hFile, &(tTerrainDataPointer.dwVtxCnt), sizeof(_ulong), &dwByte, nullptr);

	tTerrainDataPointer.pPos = new _vec3[tTerrainDataPointer.dwVtxCnt];

	for (int i = 0; i < tTerrainDataPointer.dwVtxCnt; ++i)
		ReadFile(hFile, &(tTerrainDataPointer.pPos[i]), sizeof(_vec3), &dwByte, nullptr);

	//Safe_Delete_Array(tTerrainDataPointer.pPos);

	ReadFile(hFile, &dwStrByte, sizeof(_ulong), &dwByte, nullptr);

	TCHAR* pHeightmapFilePath = nullptr;
	pHeightmapFilePath = new TCHAR[dwStrByte];

	ReadFile(hFile, pHeightmapFilePath, dwStrByte, &dwByte, nullptr);

	//FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, pHeightmapPath));
	if (FAILED(Engine::Ready_ProtoComponent(L"Proto_TerrainTex_Tutorial", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, tTerrainDataPointer.pPos, tTerrainData.fDetailLevel))))
		return E_FAIL;

	Safe_Delete_Array(pHeightmapFilePath);
	//Safe_Delete_Array(pHeightmapPath);

	CloseHandle(hFile);

	return 0;
}

_uint CThreadMgr::Load_TerrainDungeonLobbyData(void)
{
	HANDLE hFile = CreateFile(L"../../Data/Terrain/DungeonLobbyMap.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	TERRAINDATA tTerrainData;
	ZeroMemory(&tTerrainData, sizeof(TERRAINDATA));
	TERRAINDATA_P tTerrainDataPointer;
	ZeroMemory(&tTerrainDataPointer, sizeof(TERRAINDATA_P));

	ReadFile(hFile, &tTerrainData, sizeof(TERRAINDATA), &dwByte, nullptr);

	ReadFile(hFile, &(tTerrainDataPointer.dwVtxCnt), sizeof(_ulong), &dwByte, nullptr);

	tTerrainDataPointer.pPos = new _vec3[tTerrainDataPointer.dwVtxCnt];

	for (int i = 0; i < tTerrainDataPointer.dwVtxCnt; ++i)
		ReadFile(hFile, &(tTerrainDataPointer.pPos[i]), sizeof(_vec3), &dwByte, nullptr);

	//Safe_Delete_Array(tTerrainDataPointer.pPos);

	ReadFile(hFile, &dwStrByte, sizeof(_ulong), &dwByte, nullptr);

	TCHAR* pHeightmapFilePath = nullptr;
	pHeightmapFilePath = new TCHAR[dwStrByte];

	ReadFile(hFile, pHeightmapFilePath, dwStrByte, &dwByte, nullptr);

	//FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, pHeightmapPath));
	if (FAILED(Engine::Ready_ProtoComponent(L"Proto_TerrainTex_DungeonLobby", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, tTerrainDataPointer.pPos, tTerrainData.fDetailLevel))))
		return E_FAIL;

	Safe_Delete_Array(pHeightmapFilePath);
	//Safe_Delete_Array(pHeightmapPath);

	CloseHandle(hFile);

	return 0;
}

_uint CThreadMgr::Load_TerrainShopData(void)
{
	HANDLE hFile = CreateFile(L"../../Data/Terrain/Stage_ShopMap.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	TERRAINDATA tTerrainData;
	ZeroMemory(&tTerrainData, sizeof(TERRAINDATA));
	TERRAINDATA_P tTerrainDataPointer;
	ZeroMemory(&tTerrainDataPointer, sizeof(TERRAINDATA_P));

	ReadFile(hFile, &tTerrainData, sizeof(TERRAINDATA), &dwByte, nullptr);

	ReadFile(hFile, &(tTerrainDataPointer.dwVtxCnt), sizeof(_ulong), &dwByte, nullptr);

	tTerrainDataPointer.pPos = new _vec3[tTerrainDataPointer.dwVtxCnt];

	for (int i = 0; i < tTerrainDataPointer.dwVtxCnt; ++i)
		ReadFile(hFile, &(tTerrainDataPointer.pPos[i]), sizeof(_vec3), &dwByte, nullptr);


	ReadFile(hFile, &dwStrByte, sizeof(_ulong), &dwByte, nullptr);

	TCHAR* pHeightmapFilePath = nullptr;
	pHeightmapFilePath = new TCHAR[dwStrByte];

	ReadFile(hFile, pHeightmapFilePath, dwStrByte, &dwByte, nullptr);

	if (FAILED(Engine::Ready_ProtoComponent(L"Proto_TerrainTex_Shop", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, tTerrainDataPointer.pPos, tTerrainData.fDetailLevel))))
		return E_FAIL;

	Safe_Delete_Array(pHeightmapFilePath);

	CloseHandle(hFile);

	return 0;
}

_uint CThreadMgr::Load_TerrainPotData(void)
{
	HANDLE hFile = CreateFile(L"../../Data/Terrain/JarMap.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	TERRAINDATA tTerrainData;
	ZeroMemory(&tTerrainData, sizeof(TERRAINDATA));
	TERRAINDATA_P tTerrainDataPointer;
	ZeroMemory(&tTerrainDataPointer, sizeof(TERRAINDATA_P));

	ReadFile(hFile, &tTerrainData, sizeof(TERRAINDATA), &dwByte, nullptr);

	ReadFile(hFile, &(tTerrainDataPointer.dwVtxCnt), sizeof(_ulong), &dwByte, nullptr);

	tTerrainDataPointer.pPos = new _vec3[tTerrainDataPointer.dwVtxCnt];

	for (int i = 0; i < tTerrainDataPointer.dwVtxCnt; ++i)
		ReadFile(hFile, &(tTerrainDataPointer.pPos[i]), sizeof(_vec3), &dwByte, nullptr);

	//Safe_Delete_Array(tTerrainDataPointer.pPos);

	ReadFile(hFile, &dwStrByte, sizeof(_ulong), &dwByte, nullptr);

	TCHAR* pHeightmapFilePath = nullptr;
	pHeightmapFilePath = new TCHAR[dwStrByte];

	ReadFile(hFile, pHeightmapFilePath, dwStrByte, &dwByte, nullptr);

	//FAILED_CHECK_RETURN(Engine::Ready_ProtoComponent(L"Proto_TerrainTex", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, pHeightmapPath));
	if (FAILED(Engine::Ready_ProtoComponent(L"Proto_TerrainTex_Pot", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, tTerrainDataPointer.pPos, tTerrainData.fDetailLevel))))
		return E_FAIL;

	Safe_Delete_Array(pHeightmapFilePath);
	//Safe_Delete_Array(pHeightmapPath);

	CloseHandle(hFile);

	return 0;
}

_uint CThreadMgr::Load_TerrainNormalData(void)
{
	HANDLE hFile = CreateFile(L"../../Data/Terrain/Stage_NormalMap.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	DWORD dwStrByte = 0;

	TERRAINDATA tTerrainData;
	ZeroMemory(&tTerrainData, sizeof(TERRAINDATA));
	TERRAINDATA_P tTerrainDataPointer;
	ZeroMemory(&tTerrainDataPointer, sizeof(TERRAINDATA_P));

	ReadFile(hFile, &tTerrainData, sizeof(TERRAINDATA), &dwByte, nullptr);

	ReadFile(hFile, &(tTerrainDataPointer.dwVtxCnt), sizeof(_ulong), &dwByte, nullptr);

	tTerrainDataPointer.pPos = new _vec3[tTerrainDataPointer.dwVtxCnt];

	for (int i = 0; i < tTerrainDataPointer.dwVtxCnt; ++i)
		ReadFile(hFile, &(tTerrainDataPointer.pPos[i]), sizeof(_vec3), &dwByte, nullptr);


	ReadFile(hFile, &dwStrByte, sizeof(_ulong), &dwByte, nullptr);

	TCHAR* pHeightmapFilePath = nullptr;
	pHeightmapFilePath = new TCHAR[dwStrByte];

	ReadFile(hFile, pHeightmapFilePath, dwStrByte, &dwByte, nullptr);

	if (FAILED(Engine::Ready_ProtoComponent(L"Proto_TerrainTex_Normal", CTerrainTex::Create(m_pGraphicDev, tTerrainData.dwVtxCntX, tTerrainData.dwVtxCntZ, tTerrainData.dwVtxItv, tTerrainDataPointer.pPos, tTerrainData.fDetailLevel))))
		return E_FAIL;

	Safe_Delete_Array(pHeightmapFilePath);

	CloseHandle(hFile);

	return 0;
}

CThreadMgr * CThreadMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CThreadMgr *	pInstance = new CThreadMgr(pGraphicDev);

	if (FAILED(pInstance->Ready_ThreadMgr(eID)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CThreadMgr::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
