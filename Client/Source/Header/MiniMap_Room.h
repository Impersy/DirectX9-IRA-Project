#pragma once
#include "FrontUi.h"

class CMiniMap_Room : public CFrontUi
{
private:
	explicit CMiniMap_Room(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMiniMap_Room(const CMiniMap_Room& rhs);
	virtual ~CMiniMap_Room();

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
	static CMiniMap_Room* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber, _int _iRoomNumber);

private:
	_int	m_iRoomNumber;
	_bool	m_bInRoomCheck;

private:
	virtual void Free(void) override;
};

