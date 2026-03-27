#pragma once
#include "Effect.h"

class CEffect_Ui_LevelUp_Text : public CEffect
{
private:
	explicit CEffect_Ui_LevelUp_Text(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Ui_LevelUp_Text(const CEffect_Ui_LevelUp_Text& rhs);
	virtual ~CEffect_Ui_LevelUp_Text();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	void	Ui_Print(void);
	void	Reset_Proj_Matrix(void);

private:
	HRESULT			Add_Component(void);

	void Create_LevelUp_Select(void);
	
public:
	static CEffect_Ui_LevelUp_Text* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_vec3			m_vPos;
	_int			m_iAlpha = 0;
	_bool			m_bPlayer_Dialog = false;

	_bool			m_bPrintCheck = false;

private:
	virtual void Free(void) override;
};

