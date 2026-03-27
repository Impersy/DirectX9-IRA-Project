#pragma once
#include "Bullet.h"

class CDoewoleBullet_SwordShot : public CBullet
{
protected:
	explicit CDoewoleBullet_SwordShot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoewoleBullet_SwordShot(const CDoewoleBullet_SwordShot& rhs);
	virtual ~CDoewoleBullet_SwordShot();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _float& fAngle) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) override {};
	virtual void Change_State(void) override {};
	virtual void Frame_Check(const _float& fTimeDelta)override;
	virtual void Distance_Dead_Check(void) {};

public:
	static CDoewoleBullet_SwordShot* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _float& fAngle);

private:
	void	Create_DeathEffect();
	void	Check_Boss_Dead();

private:
	_float		m_fMaxframe = 0.f;
	_vec3		m_vDir;
	_float		m_fAccTime = 0.f;
	_float		m_fAngle = 0.f;
	_float		m_fAccAngle = 0.f;

private:
	virtual void Free(void) override;
};

