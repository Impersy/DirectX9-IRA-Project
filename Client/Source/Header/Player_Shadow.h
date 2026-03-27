#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;
class CSphereTex;

END
class CPlayer_Shadow : public CGameObject
{
private:
	explicit CPlayer_Shadow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer_Shadow(const CPlayer_Shadow& rhs);
	virtual ~CPlayer_Shadow();

public:
	void		Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

	CRcTex* m_pBufferCom;
	CTransform* m_pTransformCom;
	

public:
	static CPlayer_Shadow*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	CGameObject*					m_pOwner;
	CTexture*						m_pTextureCom;

};

