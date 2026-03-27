#pragma once
#include "Bullet.h"

class CDoewoleBullet_Circle : public CBullet
{
protected:
	explicit CDoewoleBullet_Circle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoewoleBullet_Circle(const CDoewoleBullet_Circle& rhs);
	virtual ~CDoewoleBullet_Circle();


public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) {};
	virtual void Change_State(void) {};
	virtual void Distance_Dead_Check(void) {};
	virtual void Frame_Check(const _float& fTimeDelta)override;

public:
	static CDoewoleBullet_Circle* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos);

private:
	virtual void Free(void) override;
	void	Create_DeathEffect();

	void	Check_Boss_Dead();

private:
	_float				m_fMaxFrame = 0.f;
	_vec3				m_vDir;
	_float				m_fAccTime = 0.f;
	_bool				m_bChangeDir = false;

	

};

