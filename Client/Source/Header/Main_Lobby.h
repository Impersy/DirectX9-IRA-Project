#pragma once

#include "Scene.h"
#include "Base.h"

#include "Player.h"
#include "Status.h"
#include "Status_Hp.h"
#include "Apostle.h"
#include "QuickSlot.h"
#include "MiniMap.h"
#include "Boss_Bar.h"

#include "Terrain.h"
#include "BigMap.h"

class CMain_Lobby : public Engine::CScene
{
private:
	explicit CMain_Lobby(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMain_Lobby();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene(void) override;
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(void);
	virtual			void		Render_Scene(void);

	const wchar_t* Get_String(void) { return m_szString.c_str(); }

	void				Set_String(wchar_t* pString) 
	{
		m_szTotalString = pString;
	}
	
	void			Check_PlayerPos();

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT			Ready_LightInfo();
	HRESULT			Load_Terrain_Info(const _tchar* pPath);
	HRESULT			Load_Dynamic_Object_Info(const _tchar* pPath);

public:
	static	CMain_Lobby*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);

private:
	_bool				m_bTerrainInit = false;
	_bool				m_bBGM = false;

	wstring				m_szString;
	wstring			    m_szTotalString;
	_float              m_SzFrame[21] = {};

	// 티프 전용
	_int   m_Dialog_Cnt = 0;
	_int   m_Dialog_Num = 20;

};

