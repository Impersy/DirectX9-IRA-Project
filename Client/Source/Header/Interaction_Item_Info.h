#pragma once
#include "Interaction.h"
#include "InventoryMgr.h"

class CInteraction_Item_Info : public CInteraction
{
private:
	explicit CInteraction_Item_Info(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInteraction_Item_Info(const CInteraction_Item_Info& rhs);
	virtual ~CInteraction_Item_Info();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	Change_State(void) override;
	virtual void	Frame_Check(const _float& fTimeDelta) override;

	void			SetUp_OnTerrain(void);
	void			Face_Check(void);
	void			Text_Print(void);
	void			Rooting_Item();
	void			Set_Receive_PlayerInven(void);


public:
	static CInteraction_Item_Info* Create(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _eTarget, _vec3 vPos, ITEM_INFO _eInfo);


private:
	virtual void Free(void) override;

private:

	CTexture* m_pTextureCom_Shop;

	ITEM_INFO	m_eRecieveInfo = { ITEM_END, 0, "", "", "", "", 0, 0, 0.f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	_float		m_fRadius = 0.f;
	_vec3		m_vMasterPos = { 0.f, 0.f, 0.f };

	_bool		m_bFace = false;
	_bool		m_bItemInfo_On = false;

	CInventoryMgr* m_pReceive_PlayerInven = nullptr;

};

