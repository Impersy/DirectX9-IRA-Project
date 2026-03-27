#pragma once
#include "FrontUi.h"
#include "InventoryMgr.h"

#include "KeyMgr.h"

class CInventory_ItemIcon : public CFrontUi
{
private:
	explicit CInventory_ItemIcon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventory_ItemIcon(const CInventory_ItemIcon& rhs);
	virtual ~CInventory_ItemIcon();

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


	void			Slot_Frame_Check(void);
	void			Ui_Print(_int _iNum);
	void			Ui_Print_Drag(void);
	void			Set_Receive_PlayerInven(void);
	void			Compare_In(_int _iTemp, _int _iSlotNum);
	_bool			Inventory_Close(void);

public:
	static CInventory_ItemIcon* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	CInventoryMgr* m_pReceive_PlayerInven;

	_int		m_PrintNumber;
	_int		m_iSlotNumber;

	_vec3		m_vPos_frame;
	_vec3		m_vPos_Origin;
	_float		m_fRadius;
	RECT		m_tRect;

	_bool		m_bPrint;
	_bool		m_bDown;
	_bool		m_bDrag;
	_bool		m_bDrop;

	POINT		m_Down_pt;
	POINT		m_Drop_pt;
	_vec3		vTemp[18];

	_int		m_iDownSpace;
	_int		m_iDropSpace;

	ITEM_INFO	m_tINFO_Now;


private:
	virtual void Free(void) override;
};

