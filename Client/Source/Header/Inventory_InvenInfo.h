#pragma once
#include "FrontUi.h"
#include "InventoryMgr.h"

class CInventory_InvenInfo : public CFrontUi
{
private:
	explicit CInventory_InvenInfo(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventory_InvenInfo(const CInventory_InvenInfo& rhs);
	virtual ~CInventory_InvenInfo();

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

	void			Set_Receive_PlayerInven(void);

	void			Ui_Print(_int _iNum);
	void			Text_Print(void);
	void			Picking_Check(void);


public:
	static CInventory_InvenInfo* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_vec3		m_vPos_frame;
	_float		m_fRadius;
	RECT		m_tRect;
	_int		m_iPicking_Index;
	CInventoryMgr* m_pReceive_PlayerInven;

	ITEM_INFO	m_eRecieveInfo = { ITEM_END, 0, "", "", "", "", 0, 0, 0.f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	_vec3		vTemp[18];

private:
	virtual void Free(void) override;
};

