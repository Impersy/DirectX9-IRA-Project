#pragma once
#include "GameObject.h"
#include "Base.h"
#include "CollisionMgr.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;

END

class CBullet : public Engine::CGameObject
{	
protected:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	CTransform* Get_Transform(void) { return m_pTransformCom; }
	_float		Get_Speed(void) { return m_fSpeed; }
	_vec3*		Get_Direction(void) { return &m_vDir; }

protected:
	virtual HRESULT	Add_Component(void)PURE;
	virtual void SetUp_OnTerrain(void)PURE;
	virtual void Change_State(void)PURE;
	virtual void Frame_Check(const _float& fTimeDelta)PURE;
	virtual void Distance_Dead_Check(void)PURE;

public:
	_bool				m_bHit = false;

protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;
	CCalculator*	m_pCalculatorCom;
	CCollider*		m_pColliderCom;
	_float			m_fSpeed;
	_float			m_fFrame;
	_bool			m_bCheck;

	_vec3			m_vTargetPos;
	_vec3			m_vOriginPos;
	_vec3*			m_pPos;

	_vec3			m_vDir;


	BULLET_STATE	m_eState;

protected:
	virtual void Free(void) override;

};

