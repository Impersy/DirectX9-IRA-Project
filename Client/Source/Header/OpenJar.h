#pragma once
#include "DynamicObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;

END

class COpenJar : public CDynamicObject
{
private:
	explicit COpenJar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit COpenJar(const COpenJar& rhs);
	virtual ~COpenJar();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void	Set_IsHit(_bool bHit) { m_bHit = bHit; }
	_bool	Get_IsHit(void) { return m_bHit; }
	void	Set_IsOpened(_bool bIsOpened);

private:
	virtual HRESULT	Add_Component(void)override;
	virtual void SetUp_OnTerrain(void) {}
	void	Open_Jar(const _float& fTimeDelta);
	void	Create_Open_Effect(void);

private:
	CCollider* m_pColliderCom;
	_bool		m_bHit = false;
	_float		m_fSpeed = 0.f;
	_bool		m_bJarOpen = false;
	_bool		m_bDropItem = false;
	_bool		m_bCreateEffect = false;

public:
	static COpenJar* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

