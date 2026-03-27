#pragma once
#include "FrontUi.h"

class CBoss_Bar_Text : public CFrontUi
{
private:
	explicit CBoss_Bar_Text(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoss_Bar_Text(const CBoss_Bar_Text& rhs);
	virtual ~CBoss_Bar_Text();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	Change_State(void) override;
	virtual void	Frame_Check(const _float& fTimeDelta) override;
	void			Boss_Dead_Delay(const _float& fTimeDelta);
	virtual void	Reset_Proj_Matrix(void) override;

public:
	static CBoss_Bar_Text* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber, BOSS_NAME _Name);

private:
	virtual void Free(void) override;

private:
	BOSS_NAME m_eBoss_Name = BOSS_END;
	_float	m_fDelay_Dead = 0.f;
	_float	m_fDelay_Dead_Max = 10.f;
};

