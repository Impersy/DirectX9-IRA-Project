#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CNpc_ApostleNpc_Time : public CNpc
{
	enum ApostleNpc_Time_State { TIME_STATE_IDEL, TIME_STATE_STAND, TIME_STATE_END};

private:
	explicit CNpc_ApostleNpc_Time(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_ApostleNpc_Time(const CNpc_ApostleNpc_Time& rhs);
	virtual ~CNpc_ApostleNpc_Time();

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
	static CNpc_ApostleNpc_Time* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CNpc_ApostleNpc_Time* Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 pos);

	void	Create_Active_Button_Effect(void);

private:
	CTexture* m_pTextureCom_1;	//기존 텍스쳐 컴포넌트를 제외한 추가분
	//CTexture* m_pTextureCom_2;	//기존 텍스쳐 컴포넌트를 제외한 추가분
	NPC_NAME	m_eNpcName;
	ApostleNpc_Time_State	m_TimeState;
	_bool		m_bTest;
	_vec3       m_vPos;

private:
	virtual void Free(void) override;
};

