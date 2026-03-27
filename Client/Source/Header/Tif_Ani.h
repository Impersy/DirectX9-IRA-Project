#pragma once
#include "Effect.h"
#include "Base.h"

class CTif_Ani : public CEffect
{
	enum TIF_STATE { TIF_START, TIF_LOOF, TIF_END, TIF_STATE_END };


private:
	explicit CTif_Ani(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTif_Ani(const CTif_Ani& rhs);
	virtual ~CTif_Ani();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	CTexture*       m_pTif_TextureCom[TIF_STATE_END];

public:
	static CTif_Ani* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_vec3               m_vPos;
	int                 m_iAlpha = 100;
	int                 m_R = 0;
	int                 m_G = 0;
	int                 m_B = 255;


	_float              m_fStartFrame = 0.f;
	_float              m_fLoofFrame = 1.f;
	_float              m_fEndFrame = 1.f;


	TIF_STATE           m_Cur_State = TIF_START;
	TIF_STATE           m_Pre_State = TIF_START;

	int                 m_Loof_Cnt = 0;

};
