#pragma once
#include "GameObject.h"
#include "Base.h"
#include "Fire_Brave.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;
class CSphereTex;

END

class CBoss : public Engine::CGameObject
{
protected:
	explicit CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoss(const CBoss& rhs);
	virtual ~CBoss();

public:
	virtual HRESULT Ready_GameObject(void) PURE;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() {}

	_int Get_MaxHp() { return m_iBossMaxHP; }
	_int Get_CurHp() { return m_iBossCurHP; }

	void		Set_Render(_bool _b) { m_bRender = _b; }
	_bool		Get_Render() { return m_bRender; }


	CTransform* Get_TransCom(void) {
		return m_pTransformCom;
	}



public:
	_bool					m_bHit = false;
	list<int>				m_Damage_List;
	CGameObject*			m_pArrow = nullptr;
	_bool					m_bRender = true;

protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CCalculator*	m_pCalculatorCom;
	CCollider*		m_pColliderCom;

protected:
	_float			m_fFrame = 0.f;
	_float			m_fMaxFrame = 0.f;
	_int			m_iBossMaxHP = 0;
	_int			m_iBossCurHP = 0;

	_float			m_fDamageTimeDelta = 0.f;
	

public:
	_float          m_fTick = 2.f;
	int             m_iTick_Cnt = 0;
	_bool           m_bTick_Damaged = false;

protected:
	virtual void Free(void) override;

};

