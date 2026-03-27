#pragma once
#include "FrontUi.h"

class CStatus_Gem_Font : public CFrontUi
{
private:
	explicit CStatus_Gem_Font(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStatus_Gem_Font(const CStatus_Gem_Font& rhs);
	virtual ~CStatus_Gem_Font();

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

	void			Set_Receive_PlayerInfo(void);
	HRESULT			Print_Check(void);

	void Ui_Print(_int _iNumber, _int _iNowEng);


public:
	static CStatus_Gem_Font* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	PLAYER_INFO pReceiveInfo;
	_bool		m_bFontPrint;
	_int		m_iFontPos;

	_int		m_iMyNumber;

private:
	virtual void Free(void) override;
};

