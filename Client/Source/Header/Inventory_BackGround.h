#pragma once
#include "FrontUi.h"

class CInventory_BackGround : public CFrontUi
{
private:
	explicit CInventory_BackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventory_BackGround(const CInventory_BackGround& rhs);
	virtual ~CInventory_BackGround();

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
	_bool			Inventory_Close(void);

public:
	static CInventory_BackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

