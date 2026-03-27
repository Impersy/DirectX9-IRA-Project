#pragma once
#include "Doewole.h"
#include "Base.h"

class CDoewole_Shadow : public CDoewole
{
private:
	explicit CDoewole_Shadow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDoewole_Shadow(const CDoewole_Shadow& rhs);
	virtual ~CDoewole_Shadow();

public:
	void		Set_Owner(CGameObject* pOwner) { m_pOwner = pOwner; }

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CDoewole_Shadow*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	CGameObject*					m_pOwner;
	CTexture*						m_pTextureCom;


};

