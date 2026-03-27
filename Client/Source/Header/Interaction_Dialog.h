#pragma once
#include "Interaction.h"

class CInteraction_Dialog : public CInteraction
{
private:
	explicit CInteraction_Dialog(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInteraction_Dialog(const CInteraction_Dialog& rhs);
	virtual ~CInteraction_Dialog();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	Change_State(void) override;
	virtual void	Frame_Check(const _float& fTimeDelta) override;

	void TextureCom_Check(void);
	void Ui_Print(void);
	void Face_Check(void);
	void Key_Press_Check(void);
	void Dialog_Delay_Check(const _float& fTimeDelta);
	void Text_Print(void);
	void Alpha_Delay_Check(const _float& fTimeDelta);

public:
	static CInteraction_Dialog* Create(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _eTarget, _vec3 vPos);

private:
	CTexture* m_pTextureCom_SupplyCrew;
	CTexture* m_pTextureCom_BlackSmith;
	CTexture* m_pTextureCom_Apostle_Time;
	CTexture* m_pTextureCom_RegenStone;

	INTERACT_TARGET m_bRecieve_Player_Dialog_Target = TARGET_END;

	_bool	m_bFace = false;
	_bool	m_bPlayer_Dialog = false;
	_bool	m_bFadeEffect_On = false;
	_float	m_fDialog_Delay = 0.f;
	_float	m_fDialog_Delay_Max = 5.f;

	_bool	m_bDelay = false;
	_bool	m_bButton_Call = false;

public:
	// 티프 전용
	_int   m_Dialog_Cnt = 0;
	_int   m_Dialog_Num = 20;

	
	_int	m_iAlpha = 0;
	_float	m_fAlphaDelay = 0.f;
	_float	m_fAlphaDelay_Max = 10.f;

private:
	virtual void Free(void) override;
};

