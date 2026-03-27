#pragma once
#include "Effect.h"

class CEffect_Ui_LevelUp_Skill_Get : public CEffect
{
private:
	explicit CEffect_Ui_LevelUp_Skill_Get(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Ui_LevelUp_Skill_Get(const CEffect_Ui_LevelUp_Skill_Get& rhs);
	virtual ~CEffect_Ui_LevelUp_Skill_Get();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	void	Ui_Print(void);
	void	Reset_Proj_Matrix(void);
	void	Text_Print_With_Effect(void);

private:
	HRESULT			Add_Component(void);
	
public:
	static CEffect_Ui_LevelUp_Skill_Get* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_vec3			m_vPos;
	_int			m_iAlpha = 0;
	_float			m_fAlphaFrame = 0.f;
	_float			m_fAlphaFrame_Max = 10.f;
	_bool			m_bPlayer_Dialog = false;

	_bool			m_bPrintCheck = false;

private:
	virtual void Free(void) override;
};

