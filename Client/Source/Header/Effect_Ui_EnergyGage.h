#pragma once
#include "Effect.h"

class CEffect_Ui_EnergyGage : public CEffect
{
private:
	explicit CEffect_Ui_EnergyGage(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Ui_EnergyGage(const CEffect_Ui_EnergyGage& rhs);
	virtual ~CEffect_Ui_EnergyGage();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	void	Set_Receive_PlayerInfo(void);
	void	Ui_Print(_int _iNowEng);
	void	Reset_Proj_Matrix(void);
	void	Create_Perk_And_Stack(void);

private:
	HRESULT			Add_Component(void);
	
public:
	static CEffect_Ui_EnergyGage* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
	_vec3			m_vPos;
	PLAYER_INFO		pReceiveInfo;

private:
	virtual void Free(void) override;
};

