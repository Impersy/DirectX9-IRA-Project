#pragma once
#include "Bullet.h"

class CMonsterBullet_2 : public CBullet
{
protected:
	explicit CMonsterBullet_2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterBullet_2(const CMonsterBullet_2& rhs);
	virtual ~CMonsterBullet_2();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void) override;
	virtual void SetUp_OnTerrain(void) override;
	virtual void Change_State(void) override;
	virtual void Frame_Check(const _float& fTimeDelta)override;
	virtual void Distance_Dead_Check(void) override;

public:
	static CMonsterBullet_2* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _Monster_Pos, _int _Dir_Count);

private:
	void Set_FireInfo(_vec3 _Monster_Pos, _int _Dir_Count);
	void Create_Dead_Effect(void);


private:
	_int	m_iDirCount;
private:
	virtual void Free(void) override;
};

