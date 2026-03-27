#pragma once
#include "Bullet.h"

class CDocheolBullet_ChargeBullet : public CBullet
{
protected:
	explicit CDocheolBullet_ChargeBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDocheolBullet_ChargeBullet(const CDocheolBullet_ChargeBullet& rhs);
	virtual ~CDocheolBullet_ChargeBullet();

public:
	enum TYPE { CHARGE, SPECIAL };


public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vDir, const _vec3& vScale, const _float& fSpeed , const _bool& bMove , TYPE eType);
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
	static CDocheolBullet_ChargeBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vDir , const _vec3& vScale ,const _float& fSpeed , const _bool& bMove , TYPE eType = CHARGE );


private:
	void			Create_EffectAndBullet();

	void			Check_Boss_Dead();


private:
	_float		m_fMaxframe = 0.f;
	_vec3		m_vDir;
	_float		m_fAccTime = 0.f;

	_float		m_fSpeed = 0.f;

	_bool		m_bRender = false;

	_bool		m_bMove = false;

	TYPE		m_eType = CHARGE;



private:
	virtual void Free(void) override;
};


