#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <process.h>
#include <math.h>

#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Function.h"
#include "Typedef.h"

#pragma warning(disable : 4251)


typedef	struct tagPlayerInfo
{
	int Maxhp;
	int Nowhp;
	int MaxEnergy;
	int NowEnergy;
	int Key;
	int Money;
	int Gem;

}PLAYER_INFO;

typedef	struct tagPlayerLvSkill
{
	int iPlayerLevel;
	int iSkill_Att_Plus;
	int iSkill_Speed_Plus;
	int iSkill_Money_Plus;
	int iSkill_Gem_Plus;

}PLAYER_LVSK;





extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;
extern _bool		g_bSphereMake;
extern _bool		g_bSphereRender;
extern SCENE_TYPE	g_eCurScene;
extern SCENE_TYPE	g_ePreScene;
extern _bool		g_bTest;
extern PLAYER_INFO  g_pInfo;
extern PLAYER_LVSK  g_pLvsk;

extern BOW_TYPE        g_Bow_Type;
extern ARROW_TYPE      g_Arrow_Type;
extern APOSTLE_TYPE    g_Apostle_Type;
extern _int            g_Apostle_Proof;
extern _bool           g_UI_Render;
extern _bool           g_Story_Book;
extern _bool           g_Ending_Logo;
extern _bool           g_Game_Clear;
extern _bool           g_Boss1_Clear;
extern _int            g_Dialog_Cnt;

extern _vec3		g_vNextPos;


extern _bool           g_bDLobby_To_Boss;
extern _bool           g_bNormal_To_Pot;
extern _bool           g_bPot_To_Regen;
extern _bool           g_bRegen_To_Pot;



using namespace std;
using namespace Engine;
