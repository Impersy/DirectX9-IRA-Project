#pragma once

#include "Base.h"
#include "Include.h"

class CThreadMgr : public CBase
{
private:
	explicit CThreadMgr(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThreadMgr();

public:
	_bool				Get_Finish(void)	{ return m_bFinish; }
	CRITICAL_SECTION	Get_Crt(void)		{ return m_Crt; }
	LOADINGID			Get_ThreadMgrID(void) { return m_eThreadMgr; }
	const _tchar*		Get_String(void) { return m_szString; }
	void				Set_String(_tchar* pString)
	{
		lstrcpy(m_szString, pString);
	}
	
public:
	static unsigned int CALLBACK Thread_Main(void* pArg);

	HRESULT			Ready_ThreadMgr(LOADINGID eID);
	_uint			ThreadMgr_ForStage_Title(void);
	_uint			ThreadMgr_ForStage_MainLobby(void);
	_uint			ThreadMgr_ForStage_Tutorial(void);
	_uint			ThreadMgr_ForStage_DungeonLobby(void);
	_uint			ThreadMgr_ForStage_Regen(void);
	_uint			ThreadMgr_ForStage_Boss1(void);
	_uint			ThreadMgr_ForStage_Boss2(void);
	_uint			ThreadMgr_ForStage_Shop(void);
	_uint			ThreadMgr_ForStage_Pot(void);
	_uint			ThreadMgr_ForStage_Normal(void);

	_uint			Load_TerrainData(void);
	_uint			Load_TerrainDocheolData(void);
	_uint			Load_TerrainRegenData(void);
	_uint			Load_TerrainTutorialData(void);
	_uint			Load_TerrainDungeonLobbyData(void);
	_uint			Load_TerrainShopData(void);
	_uint			Load_TerrainPotData(void);
	_uint			Load_TerrainNormalData(void);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	LOADINGID			m_eThreadMgr;
	bool				m_bFinish;
	_tchar				m_szString[128];
	static bool			m_bLoad[LOADING_END];


public:
	static CThreadMgr*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);

private:
	virtual void Free(void);

};
