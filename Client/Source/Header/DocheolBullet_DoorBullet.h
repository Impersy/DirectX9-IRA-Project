#pragma once
#include "Bullet.h"

class CDocheolBullet_DoorBullet : public CBullet
{
public:
	enum COLOR { RED = 0, BLUE, GREEN, WHITE , BLACK};

protected:
	explicit CDocheolBullet_DoorBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDocheolBullet_DoorBullet(const CDocheolBullet_DoorBullet& rhs);
	virtual ~CDocheolBullet_DoorBullet();

public:
	COLOR			Get_Color() { return m_eColor; }

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) {};
	virtual void Change_State(void) {};
	virtual void Frame_Check(const _float& fTimeDelta) override;
	virtual void Distance_Dead_Check(void) {};

	void		Direction_Setting();
	void		Color_Check();

public:
	static CDocheolBullet_DoorBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos);

private:
	_float		m_fMaxframe = 0.f;
	_vec3		m_vDir;
	_float		m_fAccTime = 0.f;

	_float		m_fSpeed = 0.f;
	_float		m_fAliveTime = 0.f;
	COLOR		m_eColor;

private:
	void	Create_DeathEffect();

private:
	virtual void Free(void) override;
};


