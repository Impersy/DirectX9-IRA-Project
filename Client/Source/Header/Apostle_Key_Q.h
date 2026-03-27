#pragma once
#include "FrontUi.h"

class CApostle_Key_Q : public CFrontUi
{
private:
	explicit CApostle_Key_Q(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CApostle_Key_Q(const CApostle_Key_Q& rhs);
	virtual ~CApostle_Key_Q();

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
	static CApostle_Key_Q* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	virtual void Free(void) override;
};

