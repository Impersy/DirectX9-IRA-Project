#pragma once
#include "FrontUi.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CStatus_Hp : public CFrontUi
{
private:
	explicit CStatus_Hp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStatus_Hp(const CStatus_Hp& rhs);
	virtual ~CStatus_Hp();

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

	void	Set_Receive_PlayerInfo(void);

public:
	static CStatus_Hp* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	_bool	m_bFillCheck;
	PLAYER_INFO pReceiveInfo;

	//_int	m_iReceiveMaxHp;
	

private:
	virtual void Free(void) override;
};

