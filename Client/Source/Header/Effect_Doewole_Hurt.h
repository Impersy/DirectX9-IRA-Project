#pragma once
#include "Bullet.h"
#include "Base.h"

class CEffect_Doewole_Hurt : public CBullet
{
private:
	explicit CEffect_Doewole_Hurt(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Doewole_Hurt(const CEffect_Doewole_Hurt& rhs);
	virtual ~CEffect_Doewole_Hurt();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual void Distance_Dead_Check(void) {};
	virtual void SetUp_OnTerrain(void) {};
	virtual void Change_State(void) {};
	virtual void Frame_Check(const _float& fTimeDelta) {  };

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Doewole_Hurt*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;

};

