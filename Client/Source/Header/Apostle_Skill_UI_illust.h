#pragma once
#include "Apostle_Skill_UI.h"

class CApostle_Skill_UI_illust : public CApostle_Skill_UI
{
private:
	explicit CApostle_Skill_UI_illust(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CApostle_Skill_UI_illust(const CApostle_Skill_UI_illust& rhs);
	virtual ~CApostle_Skill_UI_illust();

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
	static CApostle_Skill_UI_illust* Create(LPDIRECT3DDEVICE9 pGraphicDev, _float size_x, _float size_y, _vec3 pos, APOSTLE_TYPE type);
private:
	virtual void Free(void) override;

private:
	_float m_Size_X = 0.f;
	_float m_Size_Y = 0.f;
	_float m_InitSize = 0.f;
	_float m_fAccTime = 0.f;
	_float dir = 3.f;
	_float m_Angle = 0.f;
	int    m_iAlpha = 0.f;
	_vec3  m_vPos;
	_float illustDir = 1.f;
	bool   m_Is_On = false;


	APOSTLE_TYPE m_Type = TIME;


};

