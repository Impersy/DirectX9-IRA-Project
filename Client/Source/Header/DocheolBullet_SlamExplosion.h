#pragma once
#include "Bullet.h"

class CDocheolBullet_SlamExplosion : public CBullet
{
protected:
	explicit CDocheolBullet_SlamExplosion(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDocheolBullet_SlamExplosion(const CDocheolBullet_SlamExplosion& rhs);
	virtual ~CDocheolBullet_SlamExplosion();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _float& fDelayTime);
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
	static CDocheolBullet_SlamExplosion* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _float& fDelayTime = 0.f);

private:
	_float		m_fMaxframe = 0.f;
	_float		m_fAccTime = 0.f;

	_float		m_fDelayTime = 0.f;

private:
	virtual void Free(void) override;
};


