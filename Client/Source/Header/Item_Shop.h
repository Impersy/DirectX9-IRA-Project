#pragma once
#include "Item.h"
#include "InventoryMgr.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CItem_Shop : public CItem
{
private:
	explicit CItem_Shop(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItem_Shop(const CItem_Shop& rhs);
	virtual ~CItem_Shop();

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
	static CItem_Shop* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _iItem_Num);

	void	Create_Ground_Effect(void);
	void	Create_Active_Button_Effect(void);

	void	Rooting_Shop_Item();

	void	Set_Receive_PlayerInven(void);

	void	Face_Check(void);
	void	Item_Num_Check(void);
	void	Reset_Check(void);

private:
	vector<ITEM_INFO> My_Item_INFO;
	_int	m_iMyItemNum = 0;

	_vec3* m_vDropPos;
	_vec3	m_vMasterPos = { 0.f, 0.f, 0.f };	// 몬스터가 죽을때 넘겨주는 위치정보
	_bool	m_bFace = false;						// 플레이어와 상호작용, 버튼 On 불변수
	_bool	m_bTest = false;					// 버튼 생성 불변수
	_vec3	m_SendPos = { 0.f, 0.f, 0.f };

	CInventoryMgr* m_pReceive_PlayerInven = nullptr;

	_bool	m_bCreate_Rotation = false;
	_float	m_fRoatationCheck = 0.f;
	_float	m_fRoatationCheck_Max = 180.f;


private:
	virtual void Free(void) override;
};

