#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;

END

class CItem : public Engine::CGameObject
{
protected:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItem(const CItem& rhs);
	virtual ~CItem();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	CTransform* Get_Transform(void) { return m_pTransformCom; }

protected:
	virtual HRESULT	Add_Component(void)PURE;				
	virtual void SetUp_OnTerrain(void)PURE;					
	virtual void Change_State(void)PURE;					
	virtual void Frame_Check(const _float& fTimeDelta)PURE;

protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;
	CCollider*		m_pColliderCom;
	CCalculator*	m_pCalculatorCom;

	vector<ITEM_INFO> INFO;
	
	_float			m_fFrame;
	_float			m_Radius;	// Item가 공통적으로 가지는 수치 : 플레이어 상호작용 가능 거리
	_bool			m_bActive;	// Item가 플레이어와 상호작용중인가를 판단하는 불변수로 사용예정
	_bool			m_bCheck;
	_vec3			m_vDir;

protected:
	virtual void Free(void) override;

};

