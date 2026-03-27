#pragma once

#include "Camera.h"
#include "Player.h"

class CStaticCamera : public CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticCamera();

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;


	void Zoom_In(const _float& fTimeDelta);
	void Zoom_Out(const _float& fTimeDelta);

	void Charge_Zoom_In(const _float& fTimeDelta);
	void Charge_Zoom_Out(const _float& fTimeDelta);

	void Zoom_In_Return(const _float& fTimeDelta);
	void Zoom_Out_Return(const _float& fTimeDelta);

	void ChargeZoom_In_Return(const _float& fTimeDelta);
	void ChargeZoom_Out_Return(const _float& fTimeDelta);
	 
	void Shaking(const _float& fTimeDelta, _float power,_float time);   

	void Lean_X(const _float& fTimeDelta, _float angle);
	void Lean_X_Return(const _float& fTimeDelta);

	void Change_Target(const _float& fTimeDelta);
	void Return_Target(const _float& fTimeDelta);


	void Ending(const _float& fTimeDelta);



private:
	void		Key_Input(const _float& fTimeDelta);
	void		Target_Renewal(void);
	void		Mouse_Move(void);

private:
	_float			m_fDistance = 10.f;
	_float			m_fAngle = 0.f;
	_float          m_fHeight = 3.f;
	_float          m_fDisZ = 6.f;
	_float          m_fDisY = 6.f;
	_float          m_AtY = 3.f;
    _float          m_AtZ = 7.f;

	_float			m_fOriginDistance = 10.f;
	_float			m_fOriginAngle = 0.f;
	_float          m_fOriginHeight = 3.f;
	_float          m_fOriginDisZ = 6.f;
	_float          m_fOriginDisY = 6.f;
	_float          m_OriginAtY = 3.f;
	_float          m_OriginAtZ = 7.f;

public:
	bool            m_bZoom_In = false;
	bool            m_bZoom_Out = false;

	bool            m_bChargeZoom_In = false;
	bool            m_bChargeZoom_Out = false;

	bool            m_bZoom_In_Return = false;
	bool            m_bZoom_Out_Return = false;

	bool            m_bChargeZoom_In_Return = false;
	bool            m_bChargeZoom_Out_Return = false;

	bool            m_bLean_X = false;
	bool            m_bLean_X_Return = false;

	bool            m_bChange_Target = false;
	bool            m_bChange_Target_Return = false;
	_float          m_Cut_Scene_Wait_Time = 2.f;

	bool            m_bShaking = false;
	_float          m_Shake_Power = 0.3f;
	_float          m_Shake_AccTime = 0.5f;
	
	
	bool            m_Pos_Update = true;


	bool			m_bLean = false;		// 안원 추가 -> Lean 하는동안 영역제한 해제


	
	

public:
	static CStaticCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	bool			m_bEnding_Logo = false;
	bool			m_bEnding = false;

private:
	virtual void Free(void) override;
	VIEW_TYPE m_View_Type = VIEW_BASIC;

	_float          m_Shake_Time = 0.f;
	int             m_Shake_Dir = 1.f;

	_vec3           m_Target_Pos;
	_vec3           m_Changed_Target_Pos;
	_float          m_Cut_Scene_Time = 0.f;
	


};

