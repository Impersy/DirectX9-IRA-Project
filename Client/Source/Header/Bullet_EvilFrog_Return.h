#pragma once
#include "Bullet.h"

class CBullet_EvilFrog_Return : public CBullet
{
protected:
	explicit CBullet_EvilFrog_Return(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet_EvilFrog_Return(const CBullet_EvilFrog_Return& rhs);
	virtual ~CBullet_EvilFrog_Return();

public:
	virtual HRESULT Ready_GameObject(void) { return S_OK; };
	virtual HRESULT Ready_GameObject(CGameObject* pOwner, const _vec3& vDir, const _float& fTimeDelta);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	_float	Get_AcceleratedSpeed(void) { return m_fAcceleratedSpeed; }

protected:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) {}
	virtual void Change_State(void) {}
	virtual void Frame_Check(const _float& fTimeDelta);
	virtual void Distance_Dead_Check(void) {};

public:
	static CBullet_EvilFrog_Return* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _vec3& vDir, const _float& fTimeDelta);

private:
	void	Create_DeathEffect(_float fTimeDelta);
	void	Create_BackHitEffect(_float fTimeDelta);
	void	Create_Effect_Bullet_Birth(_float fTimeDelta);
	void	Create_Effect_Bullet_Birth_RayUp(_float fTimeDelta);

private:
	CGameObject*	m_pOwner;
	_float			m_fMaxframe = 0.f;
	_float			m_fAccumulatedTime = 0.f;
	_bool			m_bAtttack = false;
	_float			m_fAcceleratedSpeed = 300.f;
	_bool			m_bReturned = false;

private:
	virtual void Free(void) override;
};

