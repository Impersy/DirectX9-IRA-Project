#pragma once
#include "FrontUi.h"

class CApostle_Count : public CFrontUi
{
private:
	explicit CApostle_Count(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CApostle_Count(const CApostle_Count& rhs);
	virtual ~CApostle_Count();

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
	static CApostle_Count* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	virtual void Free(void) override;

private:
	_bool	m_bNonePrint = false;
};

