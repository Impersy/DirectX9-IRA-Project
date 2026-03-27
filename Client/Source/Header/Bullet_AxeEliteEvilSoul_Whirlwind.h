#pragma once
#include "Bullet.h"

class CBullet_AxeEvilSoul_Whirlwind : public CBullet
{
protected:
	explicit CBullet_AxeEvilSoul_Whirlwind(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet_AxeEvilSoul_Whirlwind(const CBullet_AxeEvilSoul_Whirlwind& rhs);
	virtual ~CBullet_AxeEvilSoul_Whirlwind();

public:
	virtual HRESULT Ready_GameObject(void) { return S_OK; };
	virtual HRESULT Ready_GameObject(CGameObject* pOwner);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) {}
	virtual void Change_State(void) {}
	virtual void Frame_Check(const _float& fTimeDelta) {}
	virtual void Distance_Dead_Check(void) {};

public:
	static CBullet_AxeEvilSoul_Whirlwind* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner);

private:
	//void	Create_DeathEffect();
	void	Check_Monster_State();
private:
	CGameObject*	m_pOwner;
	//_float		m_fMaxframe = 0.f;
	//_vec3		m_vDir;
	//_float		m_fAccumulatedTime = 0.f;

private:
	virtual void Free(void) override;
};

