#pragma once
#include "Effect.h"

class CEffect_Ui_BlackBar : public CEffect
{
private:
	explicit CEffect_Ui_BlackBar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Ui_BlackBar(const CEffect_Ui_BlackBar& rhs);
	virtual ~CEffect_Ui_BlackBar();

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
	static CEffect_Ui_BlackBar* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
	_vec3			m_vPos;
	_int			m_iAlpha = 0;
	_bool			m_bPlayer_Dialog = false;

private:
	virtual void Free(void) override;
};

