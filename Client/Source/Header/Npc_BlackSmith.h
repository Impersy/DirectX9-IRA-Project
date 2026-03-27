#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CNpc_BlackSmith : public CNpc
{
	enum BlackSmithState { SMITH_STATE_IDEL, SMITH_STATE_END};

private:
	explicit CNpc_BlackSmith(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_BlackSmith(const CNpc_BlackSmith& rhs);
	virtual ~CNpc_BlackSmith();

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
	static CNpc_BlackSmith* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void	Create_Active_Button_Effect(void);

private:
	CTexture* m_pTextureCom_1;	//기존 텍스쳐 컴포넌트를 제외한 추가분
	CTexture* m_pTextureCom_2;	//기존 텍스쳐 컴포넌트를 제외한 추가분

	NPC_NAME			m_eNpcName;
	BlackSmithState		m_SMITHState;
	_bool	m_bTest;


private:
	virtual void Free(void) override;
};

