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

class CJar : public CDynamicObject
{
private:
	explicit CJar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CJar(const CJar& rhs);
	virtual ~CJar();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void	Set_IsHit(_bool bHit) { m_bHit = bHit; }
	_bool	Get_IsHit(void) { return m_bHit; }

	void	Create_Hit_Effect(void);

	void Create_Coin(void);

private:
	virtual HRESULT	Add_Component(void)override;
	virtual void SetUp_OnTerrain(void) {}

private:
	CCollider* m_pColliderCom;
	_int	m_iHitCnt = 0;
	_bool	m_bHit = false;
	_float	m_fFrame = 0.f;
	_bool			m_bSetCollider = true;

public:
	static CJar* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

