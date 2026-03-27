#pragma once
#include "FrontUi.h"

class CStatus_Key : public CFrontUi
{
private:
	explicit CStatus_Key(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStatus_Key(const CStatus_Key& rhs);
	virtual ~CStatus_Key();

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
	static CStatus_Key* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	virtual void Free(void) override;
};

