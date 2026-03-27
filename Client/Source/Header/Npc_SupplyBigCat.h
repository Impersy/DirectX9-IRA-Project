#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CNpc_SupplyBigCat : public CNpc
{
private:
	explicit CNpc_SupplyBigCat(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_SupplyBigCat(const CNpc_SupplyBigCat& rhs);
	virtual ~CNpc_SupplyBigCat();

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
	static CNpc_SupplyBigCat* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void	Create_Active_Button_Effect(void);

private:
	NPC_NAME			m_eNpcName;
	_bool				m_bTest;


private:
	virtual void Free(void) override;
};

