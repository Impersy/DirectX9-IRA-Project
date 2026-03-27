#pragma once
#include "FrontUi.h"

class CBigMap_Icon : public CFrontUi
{
private:
	explicit CBigMap_Icon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBigMap_Icon(const CBigMap_Icon& rhs);
	virtual ~CBigMap_Icon();

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
	_bool			BigMap_Close(void);
	void			Player_Loc_Check(SCENE_TYPE _Scene, _vec3* _pPlayerPos);

public:
	static CBigMap_Icon* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _IconNumber);

private:
	virtual void Free(void) override;

private:
	_int	m_iIcon_Number = 0;
	_float	m_fFrame_Max = 12.f;

	SCENE_TYPE	m_ePlayer_Loc = SCENE_END;
	_vec3		m_vPlayerPos = { 0.f, 0.f, 0.f };

};

