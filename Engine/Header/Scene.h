#pragma once

#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*	Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
	CLayer*			Get_Layer(const _tchar* pLayerTag);
	void			Decrease_MonsterCnt(void) { --m_iMonsterCnt; }
	void			Increase_MonsterCnt(void) { ++m_iMonsterCnt; }
	_int			Get_MonsterCnt(void) { return m_iMonsterCnt; }
	void			Set_MonsterCnt(_int iCnt) { m_iMonsterCnt = iCnt; }
public:
	virtual			HRESULT		Ready_Scene(void)PURE;
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(void);
	virtual			void		Render_Scene(void)PURE;

protected:
	LPDIRECT3DDEVICE9								m_pGraphicDev;
	unordered_map<const _tchar*, CLayer*>			m_uMapLayer;
	_int											m_iMonsterCnt;

protected:
	virtual void		Free(void);
};

END