#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CNpc_None_RegenStone : public CNpc
{
	enum StoneState { STONE_STATE_IDEL, STONE_STATE_ACT, SMITH_STATE_END};

private:
	explicit CNpc_None_RegenStone(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_None_RegenStone(const CNpc_None_RegenStone& rhs);
	virtual ~CNpc_None_RegenStone();

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
	static CNpc_None_RegenStone* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void	Create_Active_Button_Effect(void);
	void	Key_Input_Check(void);

private:
	NPC_NAME			m_eNpcName;
	StoneState			m_StoneState;
	_bool				m_bTest;

private:
	virtual void Free(void) override;
};

