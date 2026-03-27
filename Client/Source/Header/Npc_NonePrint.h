#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CNpc_NonePrint : public CNpc
{
	enum StoneState { STONE_STATE_IDEL, STONE_STATE_ACT, SMITH_STATE_END};

private:
	explicit CNpc_NonePrint(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_NonePrint(const CNpc_NonePrint& rhs);
	virtual ~CNpc_NonePrint();

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
	static CNpc_NonePrint* Create(LPDIRECT3DDEVICE9 pGraphicDev, NPC_NAME _name);

	void	Create_Active_Button_Dialog(void);
	void	Create_Active_Button_Only(void);

	void	Key_Input_Check(void);

private:
	NPC_NAME			m_eNpcName;
	StoneState			m_StoneState;
	_bool				m_bTest;

private:
	virtual void Free(void) override;
};

