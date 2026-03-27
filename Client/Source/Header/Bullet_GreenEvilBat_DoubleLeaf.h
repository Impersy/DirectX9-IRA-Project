#pragma once
#include "Bullet.h"

class CBullet_GreenEvilBat_DoubleLeaf : public CBullet
{
protected:
	explicit CBullet_GreenEvilBat_DoubleLeaf(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet_GreenEvilBat_DoubleLeaf(const CBullet_GreenEvilBat_DoubleLeaf& rhs);
	virtual ~CBullet_GreenEvilBat_DoubleLeaf();

public:
	virtual HRESULT Ready_GameObject(void) { return S_OK; };
	virtual HRESULT Ready_GameObject(CGameObject* pOwner, const _vec3& vDir);
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
	static CBullet_GreenEvilBat_DoubleLeaf* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _vec3& vDir);

private:
	void	Create_DeathEffect(_float fTimeDelta);
	void	Create_BackHitEffect(_float fTimeDelta);

private:
	CGameObject*	m_pOwner;
	_float			m_fMaxframe = 0.f;
	_float			m_fAccumulatedTime = 0.f;
	_bool			m_bAtttack = false;

private:
	virtual void Free(void) override;
};

