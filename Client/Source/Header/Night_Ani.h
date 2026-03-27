#pragma once
#include "Effect.h"
#include "Base.h"

class CNight_Ani : public CEffect
{
	enum NIGHT_STATE { NIGHT_START, NIGHT_LOOF, NIGHT_END, NIGHT_STATE_END };


private:
	explicit CNight_Ani(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNight_Ani(const CNight_Ani& rhs);
	virtual ~CNight_Ani();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	CTexture*       m_pNight_TextureCom[NIGHT_STATE_END];

public:
	static CNight_Ani* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;
	int                 m_iAlpha = 100;
	int                 m_R = 255;
	int                 m_G = 255;
	int                 m_B = 255;


	_float              m_fStartFrame = 0.f;
	_float              m_fLoofFrame = 1.f;
	_float              m_fEndFrame = 1.f;


	NIGHT_STATE           m_Cur_State = NIGHT_START;
	NIGHT_STATE           m_Pre_State = NIGHT_START;

	int                 m_Loof_Cnt = 0;
	
};
