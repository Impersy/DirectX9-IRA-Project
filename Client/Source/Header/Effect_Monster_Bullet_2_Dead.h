#pragma once
#include "Effect.h"

class CEffect_Monster_Bullet_2_Dead : public CEffect	// TriÇü
{
private:
	explicit CEffect_Monster_Bullet_2_Dead(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Monster_Bullet_2_Dead(const CEffect_Monster_Bullet_2_Dead& rhs);
	virtual ~CEffect_Monster_Bullet_2_Dead();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	
public:
	static CEffect_Monster_Bullet_2_Dead* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
	_vec3	m_vPos;

private:
	virtual void Free(void) override;
};

