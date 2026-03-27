#pragma once
#include "Bullet.h"

class CBullet_AxeEvilSoul_Leaf : public CBullet
{
protected:
	explicit CBullet_AxeEvilSoul_Leaf(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet_AxeEvilSoul_Leaf(const CBullet_AxeEvilSoul_Leaf& rhs);
	virtual ~CBullet_AxeEvilSoul_Leaf();

public:
	virtual HRESULT Ready_GameObject(void) { return S_OK; };
	virtual HRESULT Ready_GameObject(CGameObject* pOwner, const _vec3& vDir, _float fAngleVariation);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) {}
	virtual void Change_State(void) {}
	virtual void Frame_Check(const _float& fTimeDelta);
	virtual void Distance_Dead_Check(void) {};

public:
	static CBullet_AxeEvilSoul_Leaf* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _vec3& vDir, _float fAngleVariation = 0.f);

private:
	void	Create_DeathEffect(_float fTimeDelta);
	void	Create_BackHitEffect(_float fTimeDelta);
	//void	Check_Monster_State();
private:
	CGameObject*	m_pOwner;
	_float			m_fMaxframe = 0.f;
	_float			m_fAccumulatedTime = 0.f;

private:
	virtual void Free(void) override;
};

