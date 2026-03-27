#pragma once
#include "Bullet.h"

class CDoewoleBullet_Standard : public CBullet
{
protected:
	explicit CDoewoleBullet_Standard(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoewoleBullet_Standard(const CDoewoleBullet_Standard& rhs);
	virtual ~CDoewoleBullet_Standard();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) {};
	virtual void Change_State(void) {};
	virtual void Frame_Check(const _float & fTimeDelta) override;
	virtual void Distance_Dead_Check(void) {};

public:
	static CDoewoleBullet_Standard* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	void	Create_DeathEffect();
	void	Check_Boss_Dead();
private:
	_float		m_fMaxframe = 0.f;
	_vec3		m_vDir;
	_float		m_fAccTime = 0.f;

private:
	virtual void Free(void) override;
};

