#pragma once
#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CItem_Coin : public CItem
{
private:
	explicit CItem_Coin(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItem_Coin(const CItem_Coin& rhs);
	virtual ~CItem_Coin();

public:
	virtual HRESULT Ready_GameObject(void) { return S_OK; }
	virtual HRESULT Ready_GameObject(_bool bIsEffectOn);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;					
	virtual void	SetUp_OnTerrain(void) override;					
	virtual void	Change_State(void) override;					
	virtual void	Frame_Check(const _float& fTimeDelta) override;

	void			Trace_Check(void);
	void			Trace_Player(const _float& fTimeDelta);


public:
	static CItem_Coin* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _bool bIsEffectOn = true);

	void	Create_Drop_Effect(void);
	void	Create_Ground_Effect(void);
	void	Angle_Update(void);
	void	Update_Speed(const _float& fTimeDelta);

private:
	_vec3*  m_vCoinPos;
	_vec3	m_vMasterPos = { 0.f, 0.f, 0.f };	// 몬스터가 죽을때 넘겨주는 위치정보
	_bool	m_bTrace = false;						// 플레이어를 추격하여 획득되기 위한 불변수

	_bool	m_bAngleOn = false;
	_vec3	m_vVelocity = { 2.f, 3.f, 2.f };			//물체 이동속도
	_vec3	m_vAcceleration = { 0.f, 0.f, 0.f };	//중력 가속도
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

	_float m_fBounceCount = 0.f;
	_bool m_bGrounded = false;

	_float	m_fEffectFrame = 0.f;
	_bool	m_bDropEffect_On = false;
	_bool	m_bDropEffect_Off = false;
	_bool	m_bGroundEffect_On = false;
	_float	m_vAccel_Trace = 0.f;

	// 항아리맵에서 동전이펙트 생성할지 말지 결정하는 변수
	_bool	m_bIsEffectOn = true;





private:
	virtual void Free(void) override;
};

