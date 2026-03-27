#pragma once

#include "Include.h"
#include "Base.h"
#include "Export_Function.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;

END

class CMainApp : public CBase
{
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	void		Set_CurScene(SCENE_TYPE _eSceneID) { m_eCurScene = _eSceneID; }

public:
	HRESULT		Ready_MainApp(void);
	int			Update_MainApp(const _float& fTimeDelta);
	void		LateUpdate_MainApp(void);
	void		Render_MainApp(void);

private:
	HRESULT			Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT			SetUp_Scene(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement);
	void			Check_SceneChange();

private:
	Engine::CGraphicDev*		m_pDeviceClass;
	Engine::CManagement*		m_pManagementClass;

	LPDIRECT3DDEVICE9			m_pGraphicDev;

	SCENE_TYPE					m_eCurScene;
	SCENE_TYPE					m_ePreScene;

public:
	static CMainApp*		Create(void);

private:
	virtual		void		Free(void);

};



