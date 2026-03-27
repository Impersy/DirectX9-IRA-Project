#pragma once
#include "FrontUi.h"

class CApostle_Pattern_Glow : public CFrontUi
{
private:
	explicit CApostle_Pattern_Glow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CApostle_Pattern_Glow(const CApostle_Pattern_Glow& rhs);
	virtual ~CApostle_Pattern_Glow();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	Change_State(void) override;
	virtual void	Frame_Check(const _float& fTimeDelta) override;
	virtual void	Reset_Proj_Matrix(void) override;

public:
	static CApostle_Pattern_Glow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	virtual void Free(void) override;

private:
	APOSTLE_TYPE	m_eRecieve_Type = APOSTLE_END;
	CTexture*		m_pTextureCom_Brave = nullptr;
	CTexture*		m_pTextureCom_Night = nullptr;

	_float			m_fFrame_Max = 8.f;
};

