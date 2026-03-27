#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CNpc_RerollLever : public CNpc
{
private:
	explicit CNpc_RerollLever(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_RerollLever(const CNpc_RerollLever& rhs);
	virtual ~CNpc_RerollLever();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;					
	virtual void	SetUp_OnTerrain(void) override;					
	virtual void	Change_State(void) override;					
	virtual void	Frame_Check(const _float& fTimeDelta) override;	

public:
	static CNpc_RerollLever* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void	Create_Active_Button_Effect(void);
	void	Item_Delete_Reroll(void);
	void	Create_Item_6(void);
	_bool	Get_ItemReset(void) { return m_bItemReset; }


private:
	NPC_NAME	m_eNpcName;
	_bool		m_bTest;

	_int		m_iItemRerollCount = 0;
	_bool		m_bLeverOn = false;

	_bool		m_bItemReset = false;
	_float		m_fDelay_for_Delete = 0;
	_float		m_m_fDelay_for_Delete_Max = 10.f;

private:
	virtual void Free(void) override;
};

