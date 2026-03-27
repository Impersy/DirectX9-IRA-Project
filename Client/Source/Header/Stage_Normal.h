#pragma once

#include "Scene.h"
#include "Base.h"

#include "Player.h"
#include "Npc_SupplyCrew.h"
#include "Npc_RerollLever.h"
#include "GreenEvilSlime.h"
#include "Status.h"
#include "Status_Hp.h"
#include "Apostle.h"
#include "QuickSlot.h"
#include "MiniMap.h"
#include "Boss_Bar.h"
#include "Inventory.h"
#include "BigMap.h"

#include "Terrain.h"

class CStage_Normal : public Engine::CScene
{
private:
	explicit CStage_Normal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage_Normal();

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
	static	CStage_Normal*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);

private:
	_bool				m_bTerrainInit = false;
	bool                m_Wave_1 = false;
	bool                m_Wave_2 = false;
	bool                m_Wave_3 = false;
	bool                m_Wave_4 = false;




	_bool				m_bDoorInit = false;
};

