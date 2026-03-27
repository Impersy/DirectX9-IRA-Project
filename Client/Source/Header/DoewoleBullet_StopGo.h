#pragma once
#include "Bullet.h"

class CDoewoleBullet_StopGo : public CBullet
{
protected:
	explicit CDoewoleBullet_StopGo(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoewoleBullet_StopGo(const CDoewoleBullet_StopGo& rhs);
	virtual ~CDoewoleBullet_StopGo();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , ClAWDIR _e) ;
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
	static CDoewoleBullet_StopGo* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , ClAWDIR _e);

private:
	_float		m_fMaxframe = 0.f;
	_vec3		m_vDir;
	_float		m_fAccTime = 0.f;
	ClAWDIR		m_eClaw;

	_bool		m_bStop = false;
	_bool		m_bFirstStop = false;

	_float		m_fMaxSpeed = 0.f;

private:
	virtual void Free(void) override;
	void	Create_DeathEffect();

	void	Check_Boss_Dead();
};

