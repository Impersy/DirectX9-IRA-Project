#pragma once
#include "FrontUi.h"
#include "InventoryMgr.h"

class CInventory_ItemIcon_Back : public CFrontUi
{
private:
	explicit CInventory_ItemIcon_Back(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventory_ItemIcon_Back(const CInventory_ItemIcon_Back& rhs);
	virtual ~CInventory_ItemIcon_Back();

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

	void			Slot_Check(void);
	void			Ui_Print(_int _iNum);
	void			Set_Receive_PlayerInven(void);

public:
	static CInventory_ItemIcon_Back* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	_int		m_PrintNumber;
	_int		m_iSlotNumber;

	_float		m_fRadius;
	RECT		m_tRect;

	_bool		m_bPrint;

	ITEM_INFO	m_tINFO_Now;

	CInventoryMgr* m_pReceive_PlayerInven;

private:
	virtual void Free(void) override;
};

