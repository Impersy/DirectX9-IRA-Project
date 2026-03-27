#pragma once
#include "Effect.h"

class CEffect_Ui_ImgCallByButton : public CEffect
{
private:
	explicit CEffect_Ui_ImgCallByButton(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Ui_ImgCallByButton(const CEffect_Ui_ImgCallByButton& rhs);
	virtual ~CEffect_Ui_ImgCallByButton();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	void	Ui_Print(void);
	void	Reset_Proj_Matrix(void);

private:
	HRESULT			Add_Component(void);
	
public:
	static CEffect_Ui_ImgCallByButton* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	_vec3			m_vPos;
	INTERACT_TARGET m_eInter_Target = TARGET_END;

	_int	m_iItemNum = 0;

private:
	virtual void Free(void) override;
};

