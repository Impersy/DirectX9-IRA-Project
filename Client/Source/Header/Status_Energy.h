#pragma once
#include "FrontUi.h"

class CStatus_Energy : public CFrontUi
{
private:
	explicit CStatus_Energy(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStatus_Energy(const CStatus_Energy& rhs);
	virtual ~CStatus_Energy();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	void	Set_Receive_PlayerInfo(void);


private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	Change_State(void) override;
	virtual void	Frame_Check(const _float& fTimeDelta) override;
	virtual void	Reset_Proj_Matrix(void) override;

public:
	static CStatus_Energy* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	_bool	m_bFillCheck;
	PLAYER_INFO pReceiveInfo;

private:
	virtual void Free(void) override;
};

