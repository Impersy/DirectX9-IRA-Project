#pragma once

#include "Scene.h"
#include "Base.h"

#include "BackGround.h"

#include "ThreadMgr.h"

class CLoading_IRA : public Engine::CScene
{
private:
	explicit CLoading_IRA(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading_IRA();

public:
	// CScene을(를) 통해 상속됨
	virtual			HRESULT		Ready_Scene(void) { return S_OK; }
	virtual			HRESULT		Ready_Scene(SCENE_TYPE eCurScene) ;
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(void);
	virtual			void		Render_Scene(void);

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag) { return S_OK; }
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag) { return S_OK; }
	HRESULT			Ready_Prototype(void);

private:
	CThreadMgr*			m_pThreadMgr;
	SCENE_TYPE			m_eCurScene = SCENE_END;
	_bool				m_bTest = false;

public:
	static	CLoading_IRA*		Create(LPDIRECT3DDEVICE9 pGraphicDev , SCENE_TYPE eCurScene);

private:
	virtual void		Free(void);
};

