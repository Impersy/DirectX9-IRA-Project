#pragma once

#include "Base.h"
#include "Include.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject :
	public CBase
{
public:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent*				Get_Component(const _tchar* pComponentTag, COMPONENTID eID);
	void					Set_Dead() { m_bDead = true; }
	_bool					Get_Dead() { return m_bDead; }
	_float					Get_ViewZ(void) { return m_fViewZ; }

public:
	virtual		HRESULT		Ready_GameObject(void)PURE;
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject();
	virtual		void		Render_GameObject()PURE;

protected:
	void					Compute_ViewZ(const _vec3* pPos);

private:
	CComponent*			Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	virtual void		Free(void);

protected:
	unordered_map<const _tchar*, CComponent*>		m_uMapComponent[ID_END];
	LPDIRECT3DDEVICE9								m_pGraphicDev;
	_bool											m_bDead = false;
	_bool											m_bSphereMake = false;

	_float											m_fViewZ = 0.f;

	
};

END