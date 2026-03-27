#pragma once
#include "Bullet.h"

class CBullet_Chain_Head : public CBullet
{
protected:
	explicit CBullet_Chain_Head(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet_Chain_Head(const CBullet_Chain_Head& rhs);
	virtual ~CBullet_Chain_Head();

public:
	virtual HRESULT Ready_GameObject(void) { return S_OK; };
	virtual HRESULT Ready_GameObject(CGameObject* pOwner, const _vec3& vDir);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	_vec3* Get_PrePos(void) { return &m_vPrePos; }

protected:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) override;
	virtual void Change_State(void) {}
	virtual void Frame_Check(const _float& fTimeDelta);
	virtual void Distance_Dead_Check(void) {};

public:
	static CBullet_Chain_Head* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _vec3& vDir);

private:
	void	Create_DeathEffect(_float fTimeDelta);
	void	Create_Bullet_Chain();

private:
	CGameObject*	m_pOwner;
	_float			m_fMaxframe = 0.f;
	_float			m_fAccumulatedTime = 0.f;
	_float			m_fAccumulatedTimeBullet = 0.f;
	_float			m_fAccumulatedTimeStop = 0.f;
	_float			m_fAccumulatedTimeDead = 0.f;
	_int			m_iChainCnt = 0;
	_bool			m_bStop = false;
	_bool			m_bHeadStop = false;
	_vec3			m_vPrePos;

private:
	virtual void Free(void) override;
};

