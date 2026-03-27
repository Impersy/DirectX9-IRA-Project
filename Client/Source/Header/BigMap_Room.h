#pragma once
#include "FrontUi.h"

class CBigMap_Room : public CFrontUi
{
private:
	explicit CBigMap_Room(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBigMap_Room(const CBigMap_Room& rhs);
	virtual ~CBigMap_Room();

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

public:
	static CBigMap_Room* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _RoomNumber);

private:
	virtual void Free(void) override;

private:
	_int	m_iRoom_Number = 0;
	_float	m_fFrame_Max = 2.f;
};

