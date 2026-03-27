#pragma once
#include "Interaction.h"
#include "InventoryMgr.h"

class CInteraction_Button : public CInteraction
{
private:
	explicit CInteraction_Button(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInteraction_Button(const CInteraction_Button& rhs);
	virtual ~CInteraction_Button();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	Change_State(void) override;
	virtual void	Frame_Check(const _float& fTimeDelta) override;

	void	Ui_Print(void);
	void	Text_Check(void);
	void	Face_Check(void);
	void	Key_Press_Check(void);
	void	Create_BlackBar(void);

	void	Create_TextEffect_False(void);
	void	Create_TextEffect_Rooting_False(void);

	void	Create_TextEffect(void);
	void	Create_Balloon(void);


	void	Set_bBlackBar_False(void) { m_bBlackBar_On = false; }
	void	Set_Receive_PlayerInven(void);


public:
	static CInteraction_Button* Create(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _eTarget, _vec3 vPos);
	static CInteraction_Button* Create_And_SaveItemInfo(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _eTarget, _vec3 vPos, ITEM_INFO _eInfo);



private:
	_bool	m_bFace;
	_bool	m_bPlayer_Dialog;
	_bool	m_bBlackBar_On = false;
	_bool	m_bBallonCreate = false;

	ITEM_INFO	m_eItemInfo = { ITEM_END, 0, "", "", "", "", 0, 0, 0.f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	_vec3	m_vMasterPos;

	_bool	m_bTextEffect_On = false;
	_bool	m_bTextDeadCheck = false;

	_float	m_fDelay = 0.f;
	_float	m_fDelay_Max = 20.f;

	CInventoryMgr* m_pReceive_PlayerInven = nullptr;


private:
	virtual void Free(void) override;
};

