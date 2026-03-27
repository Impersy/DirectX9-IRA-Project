#pragma once
#include "FrontUi.h"

class CBigMap_Text : public CFrontUi
{
private:
	explicit CBigMap_Text(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBigMap_Text(const CBigMap_Text& rhs);
	virtual ~CBigMap_Text();

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

	void			Ui_Print(_int _iNum);
	void			Text_Print(void);
	_bool			BigMap_Close(void);

	void			Recieve_Player_LVSK(void);

public:
	static CBigMap_Text* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	PLAYER_LVSK		m_tRecieve_LVSK = { 0, 0, 0, 0, 0 };
	_vec3			m_vRecieve_Pl_Pos = { 0.f, 0.f, 0.f };
};

