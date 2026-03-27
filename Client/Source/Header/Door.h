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

class CDoor : public CDynamicObject
{
private:
	explicit CDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoor(const CDoor& rhs);
	virtual ~CDoor();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:


private:
	virtual HRESULT	Add_Component(void)override;
	virtual void SetUp_OnTerrain(void)override;
	//virtual void Change_State(void)PURE;					
	//virtual void Frame_Check(const _float& fTimeDelta)PURE;

private:
	CCollider* m_pColliderCom;
	_float	m_fFrame = 0.f;

public:
	static CDoor* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

