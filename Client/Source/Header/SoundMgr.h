#pragma once

#include "Include.h"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSoundMgr
{
public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
public:
	
	enum CHANNELID {
		BGM, RAIN, DOEWOLE, DOEWOLE_EFFECT, DOEWOLE_BULLET, DOEWOLE_BULLET2, DOEWOLE_BULLET3, DOEWOLE_BULLET4, DOEWOLE_BULLET5, DOEWOLE_BULLET6, DOEWOLE_BULLET7,
		TIME_STOP, PLAYER_DASH, MOVE_FOREST, MOVE_LOBBY, PLAYER_ATTACKED, PLAYER_TELEPORT, BOW_DRAW_DULL, BOW_DRAW_LIGHT, BOW_FIRE, BOW_CHARGE_FIRE, ARROW_CRI_HIT, ARROW_NORMAL_HIT
		,BOW_CHANGE, SAW_S, SAW_L, SAW_E,KAFF_S, KAFF_L, KAFF_E , KAFF_B, KAFF_SW , KAFF_SW2,FIRE_BOW_FIRE, FIRE_BOW_HIT,NIGHT_BOW_FIRE,NIGHT_BOW_HIT,TIME_BOW_FIRE,TIME_BOW_HIT,TEXT_SOUND,
		SAW_HIT,FIRE_CHARGE_FIRE,NIGHT_CHARGE_FIRE,TIME_CHARGE_FIRE, TIME_CHARGE_FIRE2,BOW_DRAW_FIRE, BOW_DRAW_NIGHT, BOW_DRAW_TIME, FIRE_BIG_HIT,NIGHT_BIG_HIT, NIGHT_BIG_HIT2, NIGHT_BIG_HIT3,
		APOSTLE_CHANGE,GAME_START,
		DOCHEOL_EFFECT , DOCHEOL_EFFECT2 , DOCHEOL_EFFECT3 , DOCHEOL_EFFECT4, DOCHEOL_BULLET , DOCHEOL_BULLET2, DOCHEOL_BULLET3 , DOCHEOL_BULLET4, DOCHEOL_BULLET5, DOCHEOL , DOCHEOL2
		, POT_BREAK, THORN_BREAK, STONE_BREAK
		, BIRD,
		MONSTER_APPEAR, MONSTER_DEATH, MONSTER_ATTACK, MONSTER_ATTACK2, MONSTER_ATTACK3, MONSTER_ATTACK4, MONSTER_ATTACK5,
		MONSTER_ATTACK6, MONSTER_ATTACK7, MONSTER_ATTACK8, MONSTER_ATTACK9, MONSTER_ATTACK10, MONSTER_HIT,
		UI_CLICK, UI_INTERACTION, ITEM_COIN,
		MAXCHANNEL
	};
private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize();

	void Release();
public:
	int  VolumeUp(CHANNELID eID, _float _vol);
	int  VolumeDown(CHANNELID eID, _float _vol);
	int  BGMVolumeUp(_float _vol);
	int  BGMVolumeDown(_float _vol);
	int  Pause(CHANNELID eID);
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, _float _vol);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;

private:
	void LoadSoundFile();

private:
	static CSoundMgr* m_pInstance;
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;
	_bool		m_bPause = false;
};

