#pragma once
#include "Effect.h"

class CEffect_Ui_TextCallByButton : public CEffect
{
private:
	explicit CEffect_Ui_TextCallByButton(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Ui_TextCallByButton(const CEffect_Ui_TextCallByButton& rhs);
	virtual ~CEffect_Ui_TextCallByButton();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	void	Ui_Print(void);
	void	Text_Print(void);
	void	Reset_Proj_Matrix(void);
	void	Face_Check(void);

private:
	HRESULT			Add_Component(void);
	
public:
	static CEffect_Ui_TextCallByButton* Create(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _Target, _vec3 vPos);
	static CEffect_Ui_TextCallByButton* Create_And_SaveItemInfo(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _Target, _vec3 vPos, ITEM_INFO _eInfo);

	static CEffect_Ui_TextCallByButton* Create_Buy_False(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _Target);
	static CEffect_Ui_TextCallByButton* Create_Rooting_False(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _Target);


private:
	_vec3			m_vPos;
	_vec3			m_vMasterPos;
	INTERACT_TARGET m_eInter_Target = TARGET_END;
	ITEM_INFO		m_eItemInfo = { ITEM_END, 0, "", "", "", "", 0, 0, 0.f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	_bool			m_bFace = false;
	_bool			m_bMoney_Set = false;

	_float			m_fTextPrint_Delay = 0.f;
	_float			m_fTextPrint_Delay_Max = 10.f;

private:
	virtual void Free(void) override;
};

