#pragma once
#include "Base.h"
#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement :public CBase
{
	DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement();
	virtual ~CManagement();

public:
	CComponent*			Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*		Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	CLayer*				Get_Layer(const _tchar* pLayerTag);
	CScene*				Get_Scene(void) { return m_pScene; }
	
public:
	HRESULT			Set_Scene(CScene* pScene);
	
	_int			Update_Management(const _float& fTimeDelta);
	void			LateUpdate_Management();
	void			Render_Management(LPDIRECT3DDEVICE9 pGraphicDev);
	void			Delete_Scene();

private:
	CScene*			m_pScene;

private:
	virtual void		Free(void);
};

END