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
#include "Inventory.h"
#include "BigMap.h"

#include "Terrain.h"

class CStage_Shop : public Engine::CScene
{
private:
	explicit CStage_Shop(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage_Shop();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene(void) override;
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(void);
	virtual			void		Render_Scene(void);

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT			Ready_LightInfo();
	HRESULT			Load_Terrain_Info(const _tchar* pPath);

	HRESULT			Load_Static_Object_Info(const _tchar* pPath);
	HRESULT			Load_Dynamic_Object_Info(const _tchar* pPath);

	void			Check_PlayerPos();

	void			Door_Init();

	void			Door_Activate_Check();

public:
	void			Set_CatCry(_bool bCry) { m_bMeow = bCry; }
	void			Set_CryTime(_float fTime) { m_fCatAccumulatedTime = fTime; }

public:
	static	CStage_Shop*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);

private:
	_bool				m_bTerrainInit = false;
	_bool				m_bMeow = false;
	_float				m_fCatAccumulatedTime = 0.f;

	_bool				m_bDoorInit = false;
};

