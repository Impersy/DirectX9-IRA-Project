#pragma once
#include "FrontUi.h"

class CInventory_Frame : public CFrontUi
{
private:
	explicit CInventory_Frame(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventory_Frame(const CInventory_Frame& rhs);
	virtual ~CInventory_Frame();

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
	void			Picking_Check(void);

public:
	static CInventory_Frame* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	_vec3		m_vPos_frame;
	_float		m_fRadius;
	RECT		m_tRect;

private:
	virtual void Free(void) override;
};

