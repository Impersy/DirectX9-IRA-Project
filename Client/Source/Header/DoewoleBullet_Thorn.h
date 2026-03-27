#pragma once
#include "Bullet.h"

class CDoewoleBullet_Thorn : public CBullet
{
protected:
	explicit CDoewoleBullet_Thorn(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoewoleBullet_Thorn(const CDoewoleBullet_Thorn& rhs);
	virtual ~CDoewoleBullet_Thorn();

public:
	_bool		Get_Render() { return m_bRender; }

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale) ;
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
	static CDoewoleBullet_Thorn* Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vScale);

private:
	_float		m_fMaxframe = 0.f;
	_bool		m_bReturn = false;

	_float		m_fAccTime = 0.f;
	_bool		m_bRender = false;

private:
	virtual void Free(void) override;
	void		Check_Boss_Dead();
};

