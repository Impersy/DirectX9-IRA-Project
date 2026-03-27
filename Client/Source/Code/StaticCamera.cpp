#include "stdafx.h"
#include "..\Header\StaticCamera.h"

#include "Export_Function.h"
#include "Ending.h"
#include <SoundMgr.h>

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}


CStaticCamera::~CStaticCamera()
{
	Free();
}


HRESULT CStaticCamera::Ready_GameObject(const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const _float& fFov,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	if (m_View_Type == VIEW_BASIC) {
		m_fAngle = D3DXToRadian(300.f);
		m_fDistance = 15.f;
		m_fDisZ = 3.f;
		m_fDisY = 20.f;
		m_AtY = 5.f;
		m_AtZ = 15.f;
	}


	if (g_eCurScene == SCENE_REGEN) {
		m_fDisY = 33.f;
	}

	if (g_eCurScene == SCENE_BOSS2) {
		m_fDisY = 28.f;
	}

	if (g_eCurScene == SCENE_MAINLOBBY) {
		m_fDisY = 23.f;
	}

	m_fOriginDistance = m_fDistance;
	m_fOriginAngle = m_fAngle;
	m_fOriginHeight = m_fHeight;
	m_fOriginDisZ = m_fDisZ;
	m_fOriginDisY = m_fDisY;
	m_OriginAtY = m_AtY;
	m_OriginAtZ = m_AtZ;

	int random = rand() % 2;

	if (random == 0)
		m_Shake_Dir = -1;
	else {
		m_Shake_Dir = 1;
	}

	



	__super::Ready_GameObject();

	return S_OK;
}

_int CStaticCamera::Update_GameObject(const _float& fTimeDelta)
{

	Key_Input(fTimeDelta);

	if (m_bZoom_In) {
		Zoom_In(fTimeDelta);
	}
	else if (m_bZoom_In_Return) {
		Zoom_In_Return(fTimeDelta);
	}


	if (m_bZoom_Out) {
		Zoom_Out(fTimeDelta);
	}
	else  if (m_bZoom_Out_Return) {
		Zoom_Out_Return(fTimeDelta);
	}

	if (m_bChargeZoom_In) {
		Charge_Zoom_In(fTimeDelta);
	}
	else if (m_bChargeZoom_In_Return) {
		ChargeZoom_In_Return(fTimeDelta);
	}


	if (m_bChargeZoom_Out) {
		Charge_Zoom_Out(fTimeDelta);
	}
	else if (m_bChargeZoom_Out_Return) {
		ChargeZoom_Out_Return(fTimeDelta);
	}


	if (m_bLean_X) {
		Lean_X(fTimeDelta, 290.f);
	}
	else if(m_bLean_X_Return) {
		Lean_X_Return(fTimeDelta);
	}

	if (m_bChange_Target) {
		Change_Target(fTimeDelta);
	}
	else if (m_bChange_Target_Return) {
		Return_Target(fTimeDelta);
	}


	if (m_Pos_Update == true) {
		Target_Renewal();
	}



	if (m_bShaking) {
		Shaking(fTimeDelta, 0.3f, m_Shake_AccTime);
	}

	if (m_bEnding) {
		g_UI_Render = false;
		Ending(fTimeDelta);
	}

	_int iExit = __super::Update_GameObject(fTimeDelta);

	return iExit;
}


void CStaticCamera::LateUpdate_GameObject()
{
}


void CStaticCamera::Zoom_In(const _float& fTimeDelta)
{
	g_UI_Render = false;

	m_fDisY -= 10.f * fTimeDelta;

	if (m_fDisY < m_fOriginDisY - 10.f) {

		m_fDisY = m_fOriginDisY - 10.f;
	}

}

void CStaticCamera::Zoom_Out(const _float& fTimeDelta)
{
	m_fDisY += 10.f * fTimeDelta;

	if (m_fDisY > m_fOriginDisY + 6.f) {

		m_fDisY = m_fOriginDisY + 6.f;
	}
}

void CStaticCamera::Charge_Zoom_In(const _float& fTimeDelta)
{
	m_fDisY -= 13.f * fTimeDelta;


	if (g_eCurScene == SCENE_REGEN) {
		if (m_fDisY < m_fOriginDisY - 4.f) {

			m_fDisY = m_fOriginDisY - 4.f;
		}
	}
	else {
		if (m_fDisY < m_fOriginDisY - 2.f) {

			m_fDisY = m_fOriginDisY - 2.f;
		}
	}

	
}

void CStaticCamera::Charge_Zoom_Out(const _float& fTimeDelta)
{
	m_fDisY += 10.f * fTimeDelta;

	if (m_fDisY > m_fOriginDisY + 6.f) {

		m_fDisY = m_fOriginDisY + 6.f;
	}
}

void CStaticCamera::Zoom_In_Return(const _float& fTimeDelta)
{
	g_UI_Render = true;

	if (m_fOriginDisY - m_fDisY > 0.1f) {
		m_fDisY += 10.f * fTimeDelta;
	}
	else {
		m_fDisY = m_fOriginDisY;
		m_bZoom_In_Return = false;
	}

}

void CStaticCamera::Zoom_Out_Return(const _float& fTimeDelta)
{
	if (m_fDisY - m_fOriginDisY > 0.1f) {
		m_fDisY -= 10.f * fTimeDelta;
	}
	else {
		m_fDisY = m_fOriginDisY;
		m_bZoom_Out_Return = false;
	}

}

void CStaticCamera::ChargeZoom_In_Return(const _float& fTimeDelta)
{

	if (m_fOriginDisY - m_fDisY > 0.1f) {
		m_fDisY += 13.f * fTimeDelta;
	}
	else {
		m_fDisY = m_fOriginDisY;
		m_bChargeZoom_In_Return = false;
	}
}

void CStaticCamera::ChargeZoom_Out_Return(const _float& fTimeDelta)
{
	
	if (m_fDisY - m_fOriginDisY > 0.1f) {
		m_fDisY -= 12.f * fTimeDelta;
	}
	else {
		m_fDisY = m_fOriginDisY;
		m_bChargeZoom_Out_Return = false;
	}

}

void CStaticCamera::Shaking(const _float& fTimeDelta ,_float power, _float time)
{

	m_Shake_Time += 2.f * fTimeDelta * 0.5f;

	
	if (m_Shake_Time > time) {
		m_Shake_Time = 0.f;
		m_bShaking = false;
	}
	else {
		m_Shake_Dir *= -1;

		m_vAt.x += power * m_Shake_Dir;
		m_vAt.y += power * m_Shake_Dir;
		m_vAt.z += power * m_Shake_Dir;
	}
}

void CStaticCamera::Lean_X(const _float& fTimeDelta, _float angle)
{
	if (m_fAngle - D3DXToRadian(angle) > 0.1f) {
		m_fAngle -= D3DXToRadian(180.f) * fTimeDelta;
	}
	else {
		m_fAngle = D3DXToRadian(angle);

		m_fDisY -= 15.f * fTimeDelta;

		if (m_fDisY < 20.f) {

			m_fDisY = 20.f;
			m_bLean_X = false;
		}
	}
}

void CStaticCamera::Lean_X_Return(const _float& fTimeDelta)
{
	if (m_fOriginAngle - m_fAngle > 0.1f) {
		m_fAngle += D3DXToRadian(180.f) * fTimeDelta;
	}
	else {

		m_fAngle = m_fOriginAngle;

		if (m_fOriginDisY - m_fDisY > 0.1f) {
			m_fDisY += 12.f * fTimeDelta;
		}
		else {
			m_fDisY = m_fOriginDisY;
			m_bLean_X = false;

		}
	}

}

void CStaticCamera::Change_Target(const _float& fTimeDelta)
{

	if (g_eCurScene == SCENE_BOSS1) {
		CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole_Body", L"Proto_Transform", ID_DYNAMIC));
		m_Changed_Target_Pos = pTransform->m_vInfo[INFO_POS];
		m_Cut_Scene_Wait_Time = 1.f;
		m_bZoom_Out = true;
	}

	if (g_eCurScene == SCENE_BOSS2) {
		CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Docheol", L"Proto_Transform", ID_DYNAMIC));
		m_Changed_Target_Pos = pTransform->m_vInfo[INFO_POS];
		m_Cut_Scene_Wait_Time = 2.f;
		m_bZoom_Out = true;
	}

	// 안원 추가 튜토리얼
	if (g_eCurScene == SCENE_TUTORIAL) {
		m_Changed_Target_Pos = { 364.f , 7.5f , 347.f };
		m_Cut_Scene_Wait_Time = 2.f;
		m_bZoom_Out = true;
	}


	_vec3 dir = m_Changed_Target_Pos - m_Target_Pos;



	if (D3DXVec3Length(&dir) > 1.0f) {

	
		D3DXVec3Normalize(&dir, &dir);

	
		m_Target_Pos += 30 * dir * fTimeDelta;
	}
	else {
		m_Target_Pos = m_Changed_Target_Pos;
		m_Cut_Scene_Time += 2.f * fTimeDelta * 0.5f;
	}


	if (m_Cut_Scene_Time > m_Cut_Scene_Wait_Time) {
		m_Cut_Scene_Time = 0.f;
		m_bChange_Target = false;
		m_bChange_Target_Return = true;
	}
	


}

void CStaticCamera::Return_Target(const _float& fTimeDelta)
{
	m_bZoom_Out = false;
	m_bZoom_Out_Return = true;

	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	m_Changed_Target_Pos = pPlayerTransform->m_vInfo[INFO_POS];
	

	_vec3 dir = m_Changed_Target_Pos - m_Target_Pos;

	
	if (D3DXVec3Length(&dir) > 1.0f) {

		D3DXVec3Normalize(&dir, &dir);

		m_Target_Pos += 40 * dir * fTimeDelta;
	}
	else {
		m_Target_Pos = m_Changed_Target_Pos;
		m_bChange_Target_Return = false;

	}

}

void CStaticCamera::Ending(const _float& fTimeDelta)
{
	
	m_fDisY += 10.f * fTimeDelta;

	if (m_fDisY > m_fOriginDisY + 290.f) {

		if (m_bEnding_Logo == false) {

			CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

			CGameObject* pGameObject = nullptr;
			pGameObject = CEnding::Create(m_pGraphicDev, 1200);
			if (pGameObject == nullptr)
				return;
			pLayer->Add_GameObject(L"Ending", pGameObject);

			m_bEnding_Logo = true;
			g_Ending_Logo = true;


		}

		
	}
}



void CStaticCamera::Key_Input(const _float& fTimeDelta)
{
	if (Get_DIKeyState(DIK_UP) & 0x80)
		m_fDisY += 10.f * fTimeDelta;

	if (Get_DIKeyState(DIK_DOWN) & 0x80)
		m_fDisY -= 10.f * fTimeDelta;

	if (Get_DIKeyState(DIK_LEFT) & 0x80)
		m_fAngle += D3DXToRadian(180.f) * fTimeDelta;

	if (Get_DIKeyState(DIK_RIGHT) & 0x80)
		m_fAngle -= D3DXToRadian(180.f) * fTimeDelta;

}

void CStaticCamera::Target_Renewal(void)
{
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));


	if ((m_bChange_Target == false) && (m_bChange_Target_Return == false)) {
		
		m_Target_Pos = pPlayerTransform->m_vInfo[INFO_POS];
	}


	_vec3	vLook;
	pPlayerTransform->Get_Info(INFO_LOOK, &vLook);

	m_vEye = vLook * -1.f;
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye.y += m_fHeight;
	m_vEye.z *= m_fDisZ;
	m_vEye.y *= m_fDisY;

	_vec3	vRight;
	memcpy(vRight, &pPlayerTransform->m_matWorld.m[0][0], sizeof(_vec3));

	if (vRight.x < 0)
		vRight.x *= -1.f;

	_matrix		matRot;
	D3DXMatrixRotationAxis(&matRot, &vRight, m_fAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRot);


	if (g_eCurScene == SCENE_DUNGEONLOBBY) {

		if (m_Target_Pos.x < 280.f )
		{
			_float v = 280.f - m_Target_Pos.x;
			m_Target_Pos.x += v;
		}
		if (m_Target_Pos.x > 480.f )
		{
			_float v = m_Target_Pos.x - 480.f;
			m_Target_Pos.x -= v; 
		}
		if (m_Target_Pos.z < 256.f )
		{
			_float v = 256.f - m_Target_Pos.z;
			m_Target_Pos.z += v;
		}
		if (m_Target_Pos.z > 512.f)
		{
			_float v = m_Target_Pos.z - 512.f;
			m_Target_Pos.z -= v;
		}

	}
	else if (g_eCurScene == SCENE_REGEN) {

	}
	else if (g_eCurScene == SCENE_BOSS1) {

	}
	else if (g_eCurScene == SCENE_BOSS2) {
		
		if (!m_bLean) // 안원 추가
		{
			if (m_Target_Pos.z < 77.f)
			{
				_float v = 77.f - m_Target_Pos.z;
				m_Target_Pos.z += v;
			}
		}
		
	}
	else if (g_eCurScene == SCENE_POT) {

		if (m_Target_Pos.x > 494.65f)
		{
			_float v = 494.65f - m_Target_Pos.x;
			m_Target_Pos.x += v;
		}
	}



	m_vEye += m_Target_Pos;
	m_vEye.z += m_AtZ;

	m_vAt = m_Target_Pos;
	m_vAt.y += m_AtY;
	
}


void CStaticCamera::Mouse_Move(void)
{
	_long	dwMouseMove = 0;

	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));

		memcpy(&matRot._41, &m_vAt, sizeof(_vec3));

		_vec3	vLook = m_vEye - m_vAt;

		D3DXVec3TransformCoord(&m_vEye, &vLook, &matRot);

		//m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		_vec3		vUp = { 0.f, 1.f, 0.f };


		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));

		memcpy(&matRot._41, &m_vAt, sizeof(_vec3));

		_vec3	vLook = m_vEye - m_vAt;

		D3DXVec3TransformCoord(&m_vEye, &vLook, &matRot);

		//m_vAt = m_vEye + vLook;
	}

}



CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CStaticCamera*		pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStaticCamera::Free(void)
{
	__super::Free();
}
