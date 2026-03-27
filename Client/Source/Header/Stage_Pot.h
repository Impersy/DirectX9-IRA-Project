#pragma once

#include "Scene.h"
#include "Base.h"
#include "Terrain.h"

class CStage_Pot : public Engine::CScene
{
private:
	explicit CStage_Pot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage_Pot();

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
	HRESULT			Load_Object_Info(const _tchar* pPath);
	HRESULT			Load_Object_Alpha_Info(const _tchar* pPath);
	HRESULT			Load_Dynamic_Object_Info(const _tchar* pPath);

	void			Check_PlayerPos();

	void			Door_Init();

	void			Door_Activate_Check();

public:
	static	CStage_Pot*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);

private:
	_bool				m_bTerrainInit = false;
	_float				m_fAccumulatedTime = 0.f;

	_bool				m_bDoorInit = false;
};

