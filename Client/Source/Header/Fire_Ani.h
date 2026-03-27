#pragma once
#include "Effect.h"
#include "Base.h"

class CFire_Ani : public CEffect
{
	
private:
	explicit CFire_Ani(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFire_Ani(const CFire_Ani& rhs);
	virtual ~CFire_Ani();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	CTexture*       m_pTextureCom;

public:
	static CFire_Ani* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;
	int                 m_iAlpha = 100;
	int                 m_R = 255;
	int                 m_G = 255;
	int                 m_B = 255;


	_float              m_fFrame = 0.f;
	

	int                 m_Loof_Cnt = 0;
	bool                m_Is_End = false;
	_float              m_dir = 0.5f;
	bool                m_Brave = true;
	bool                m_Bomb = true;
	bool                m_Circle = true;
	bool                m_Big = true;





};
