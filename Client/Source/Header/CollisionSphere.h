#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CSphereTex;
class CTransform;

END

class CCollisionSphere : public Engine::CGameObject
{
private:
	explicit CCollisionSphere(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollisionSphere(const CCollisionSphere& rhs);
	virtual ~CCollisionSphere();

public:
	virtual HRESULT Ready_GameObject() { return S_OK; }
	virtual HRESULT Ready_GameObject(CGameObject* pOwner , COLLIDER_TYPE _e);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT					Add_Component(void);

public:
	static CCollisionSphere*	Create(LPDIRECT3DDEVICE9 pGraphicDev ,CGameObject* pOwner , COLLIDER_TYPE _e);

private:
	CSphereTex*			m_pSphereBufferCom;
	CTransform*			m_pTransformCom;
	CGameObject*		m_pOwner;
	
	COLLIDER_TYPE		m_eCollideType;
private:
	virtual void Free(void) override;

};

