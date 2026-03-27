#pragma once
#include "Effect.h"
#include "Base.h"

class CMain_Lobby_Book : public CEffect
{

private:
	explicit CMain_Lobby_Book(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMain_Lobby_Book(const CMain_Lobby_Book& rhs);
	virtual ~CMain_Lobby_Book();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	void	Create_Active_Button_Effect(void);
public:
	static CMain_Lobby_Book*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;
	int                 m_iAlpha = 100;
	_bool		m_bTest;


};

