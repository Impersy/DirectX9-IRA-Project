#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
END

class CLandscape : public Engine::CGameObject
{
protected:
	explicit CLandscape(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLandscape(const CLandscape& rhs);
	virtual ~CLandscape();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	_byte	Get_DrawID(void) { return m_byDrawID; }
	void	Set_DrawID(_byte id) { m_byDrawID = id; }
	_byte	Get_DrawOption(void) { return m_byDrawOption; }
	void	Set_DrawOption(_byte option) { m_byDrawOption = option; }
	void	Set_ObjKey(_tchar* pKey, _ulong strLength);
	_tchar* Get_ObjKey(void) { return m_pObjKey; }
	_vec3	Get_ColliderPos(void) { return m_vColliderPos; }
	void	Set_ColliderPos(_vec3* pPos) { m_vColliderPos = *pPos; }
	_float	Get_ColliderRadius(void) { return m_fColliderRadius; }
	void	Set_ColliderRadius(_float fradius) { m_fColliderRadius = fradius; }

protected:
	virtual HRESULT	Add_Component(void)PURE;				
	virtual void SetUp_OnTerrain(void)PURE;					

protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CCalculator*	m_pCalculatorCom;

	_byte			m_byDrawID;
	_byte			m_byDrawOption;

	_tchar*			m_pObjKey;

	_vec3			m_vColliderPos;
	_float			m_fColliderRadius;

protected:
	virtual void Free(void) override;

};

