#pragma once
#include "FrontUi.h"

class CMiniMap_Icon : public CFrontUi
{
private:
	explicit CMiniMap_Icon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMiniMap_Icon(const CMiniMap_Icon& rhs);
	virtual ~CMiniMap_Icon();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	void			Ui_MiniMap_Icon_Print(_int _LocNumber);

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	Change_State(void) override;
	virtual void	Frame_Check(const _float& fTimeDelta) override;
	virtual void	Reset_Proj_Matrix(void) override;

public:
	static CMiniMap_Icon* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iIconNumber, _int _LocNumber);

private:
	_int	m_iLocNumber = 0; 
	_int	m_iIconNumber = 0;
	_int	m_iRoomNumber;
	_bool	m_bInRoomCheck;

private:
	virtual void Free(void) override;
};

