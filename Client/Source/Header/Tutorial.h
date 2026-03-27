#pragma once

#include "Scene.h"
#include "Base.h"

#include "Player.h"
#include "Monster.h"
#include "Status.h"
#include "Status_Hp.h"
#include "Apostle.h"
#include "QuickSlot.h"
#include "MiniMap.h"
#include "Inventory.h"
#include "BigMap.h"

#include "Terrain.h"

class CTutorial : public Engine::CScene
{
private:
	explicit CTutorial(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTutorial();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene(void) override;
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(void);
	virtual			void		Render_Scene(void);

	_bool			Get_ChasePlayer() { return m_bChasePlayer; }
	void			Set_ChasePlayer() { m_bChasePlayer = true; }

	void			Set_MonsterCntDown() { --m_iMonsterCnt; }

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT			Ready_LightInfo();
	HRESULT			Load_Terrain_Info(const _tchar* pPath);

	HRESULT			Load_Static_Object_Info(const _tchar* pPath);
	HRESULT			Load_Dynamic_Object_Info(const _tchar* pPath);
	HRESULT			Load_Dynamic_Object_Info2(const _tchar* pPath);


	void			Check_PlayerPos();

public:
	static	CTutorial*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);

private:
	_bool				m_bTerrainInit = false;
	_bool				m_bMonsterMake = false;

	_bool				m_bChasePlayer = false;

	_bool				m_bTutorial = false;
	_bool				m_bSetBlack = false;

	int					m_iMonsterCnt = 7;

	_bool				m_bMake = false;

	_float				m_fAccumulatedTime = 0.f;
};

