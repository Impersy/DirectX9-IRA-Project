#pragma once
#include "GameObject.h"
#include "Monster.h"
#include "Base.h"

class CMonster_Shadow : public CGameObject
{
private:
	explicit CMonster_Shadow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster_Shadow(const CMonster_Shadow& rhs);
	virtual ~CMonster_Shadow();

public:
	void		Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }
	void		Set_Dead(void) { m_bDead = true; }

public:
	virtual HRESULT Ready_GameObject(void) { return S_OK; }
	virtual HRESULT Ready_GameObject(CGameObject* pOwner, const _vec3& vScale);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

private:
	HRESULT			Add_Component(void);

public:
	static CMonster_Shadow*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _vec3& vScale);

private:
	virtual void Free(void) override;

private:
	CGameObject*	m_pOwner;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;
	CRcTex*			m_pBufferCom;
	_vec3			m_vScale;
};

