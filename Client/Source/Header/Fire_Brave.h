#pragma once
#include "Effect.h"
#include "Base.h"

class CFire_Brave : public CEffect
{

private:
	explicit CFire_Brave(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFire_Brave(const CFire_Brave& rhs);
	virtual ~CFire_Brave();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CFire_Brave*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	int                 m_iAlpha = 255;
	_float              m_fFrameSpeed = 0.7f;
public:
	_vec3               m_vPos;
	bool                m_Is_Small = false;
	bool                m_Is_Big_Hit = false;
	bool                m_Is_Target_Dead = false;
	bool                m_bRender = true;


	

};

