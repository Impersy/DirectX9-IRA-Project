#pragma once

#include "Scene.h"
#include "Base.h"

#include "Player.h"
#include "Status.h"
#include "Status_Hp.h"
#include "Apostle.h"
#include "QuickSlot.h"
#include "MiniMap.h"
#include "Inventory.h"
#include "BigMap.h"
#include "Boss_Bar.h"

#include "Terrain.h"

class CStage_Boss2 : public Engine::CScene
{
private:
	explicit CStage_Boss2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage_Boss2();

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
	HRESULT			Load_Dynamic_Object_Info(const _tchar* pPath);

public:
	static	CStage_Boss2*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);

private:
	_bool				m_bTerrainInit = false;
	_bool				m_bBGM = false;
};

