#pragma once
#include "Bullet.h"

class CMonsterBullet : public CBullet
{	
protected:
	explicit CMonsterBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterBullet(const CMonsterBullet& rhs);
	virtual ~CMonsterBullet();

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
	static CMonsterBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 _Monster_Pos, _bool _Target_Check);

private:
	void Set_FireInfo(_vec3 _Monster_Pos, _bool _Target_Check);
	void Create_Dead_Effect(void);

private:
	virtual void Free(void) override;

};

