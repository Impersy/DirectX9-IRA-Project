#pragma once
#include "FrontUi.h"

class CLevelUp_Select : public CFrontUi
{
private:
	explicit CLevelUp_Select(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLevelUp_Select(const CLevelUp_Select& rhs);
	virtual ~CLevelUp_Select();

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

	void			Ui_Print(void);
	_bool			LevelUp_Select_Close(void);
	void			Select_Picking_Check(void);
	void			Picking_Click_Check(void);
	void			Create_Skill_Get_Effect(void);

public:
	static CLevelUp_Select* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	RECT			m_tRect[4] = { 0, 0, 0, 0 };
	_int			m_iPicking_State = 0;
	_int			m_iClick_State = 0;

	_float			m_fFrame_Max = 5.f;

	_int			m_iAlpha = 0;
	_float			m_fAlphaFrame = 0.f;
	_float			m_fAlphaFrame_Max = 10.f;
	_bool			m_bAlphaPrintEnd = false;


	_float			m_fDeadDelay = 0.f;
	_float			m_fDeadDelay_Max = 10.f;
	_bool			m_bDeadDelay_End = false;


};

