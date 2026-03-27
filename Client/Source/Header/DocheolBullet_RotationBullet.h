#pragma once
#include "Bullet.h"

class CDocheolBullet_RotationBullet : public CBullet
{
protected:
	explicit CDocheolBullet_RotationBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDocheolBullet_RotationBullet(const CDocheolBullet_RotationBullet& rhs);
	virtual ~CDocheolBullet_RotationBullet();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vCenterPos , _float fAngle, const _vec3& vScale, const _float& fSpeed, const _float& fAliveTime);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) {};
	virtual void Change_State(void) {};
	virtual void Frame_Check(const _float& fTimeDelta) override;
	virtual void Distance_Dead_Check(void) {};

public:
	static CDocheolBullet_RotationBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vCenterPos , _float fAngle , const _vec3& vScale ,const _float& fSpeed , const _float& fAliveTime);

private:
	_float		m_fMaxframe = 0.f;
	_vec3		m_vDir;
	_float		m_fAccTime = 0.f;

	_float		m_fSpeed = 0.f;
	_float		m_fAliveTime = 0.f;

	_vec3		m_vCenterPos;
	_float		m_fRotRadius = 0.f;

	_float		m_fAngle = 0.f;

	_float		m_fPreAngle = 0.f;

private:
	void	Create_DeathEffect();
	void	Check_Boss_Dead();

private:
	virtual void Free(void) override;
};


