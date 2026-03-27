#pragma once
#include "Item.h"
#include "InventoryMgr.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CItem_Drop : public CItem
{
private:
	explicit CItem_Drop(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItem_Drop(const CItem_Drop& rhs);
	virtual ~CItem_Drop();

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
	static CItem_Drop* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	static CItem_Drop* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int _itemNum);


	void	Create_Drop_Effect(void);
	void	Create_Ground_Effect(void);
	void	Create_Active_Button_Effect(void);
	void	Angle_Update(void);
	void	Update_Speed(const _float& fTimeDelta);

	void	Rooting_Drop_Item();

	void	Set_Receive_PlayerInven(void);

	void	Face_Check(void);
	void	Item_Num_Check(void);

	void	RotationDrop(const _float& fTimeDelta);

	void	Create_JarDrop_Effect(void);

private:

	vector<ITEM_INFO> My_Item_INFO;

	_vec3* m_vDropPos;
	_vec3	m_vMasterPos = { 0.f, 0.f, 0.f };	// 몬스터가 죽을때 넘겨주는 위치정보
	_bool	m_bFace = false;						// 플레이어와 상호작용, 버튼 On 불변수
	_bool	m_bTest = false;					// 버튼 생성 불변수

	_bool	m_bAngleOn = false;
	_vec3	m_vVelocity = { 2.f, 3.f, 2.f };	//물체 이동속도
	_vec3	m_vAcceleration = { 0.f, 0.f, 0.f };//가속도
	_vec3	m_vGravity = { 0.f, -9.81f , 0.f };	//중력 가속도
	_vec3	m_DirectionRand = { 0.f, 0.f, 0.f };
	_float	m_fAngle = 0;
	_float	m_fSpeed = 10.f;
	_float	m_fDrag = 0.2f;	//공기저항 계수
	_float	m_fElapsedTime = 0.f;
	_float	m_fJumpTime = 1.f;
	_float	m_fMaxJumpTime = 1.f;
	_float	m_fJumpPower = 2.f;
	_float	m_fBounce = 0.7f;

	_bool	m_bDropOn = false;

	_float	m_fEffectFrame = 0.f;
	_bool	m_bDropEffect_On = false;
	_bool	m_bDropEffect_Off = false;
	_bool	m_bGroundEffect_On = false;
	_float	m_vAccel_Trace = 0.f;

	_vec3	m_SendPos = { 0.f, 0.f, 0.f };

	CInventoryMgr* m_pReceive_PlayerInven = nullptr;

	_int	m_iRecieveNumber = 0.f;

	_bool	m_bOnGround = false;




private:
	virtual void Free(void) override;
};

