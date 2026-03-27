#include "stdafx.h"
#include "..\Header\Ghost_TimeStop.h"
#include "Export_Function.h"
#include "Player.h"


CGhost_TimeStop::CGhost_TimeStop(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGhost(pGraphicDev),m_iAngleState(0)
{
}

CGhost_TimeStop::CGhost_TimeStop(const CGhost_TimeStop& rhs)
	:CGhost(rhs), m_iAngleState(rhs.m_iAngleState)
{
}

CGhost_TimeStop::~CGhost_TimeStop()
{
	Free();
}

HRESULT CGhost_TimeStop::Ready_GameObject(void)
{
	
	__super::Ready_GameObject();

	m_pTransformCom->Set_Scale_Ratio({ 5.f * PUBLIC_SCALE, 5.f * PUBLIC_SCALE, 1.f });


	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	if (pPlayer != nullptr) {
		m_Sprite = pPlayer->Get_Run_Frame();
		m_iAngleState = pPlayer->Get_Anlge_State();
	}

	_vec3 vPos;

	if (pPlayerTransform != nullptr) {
		pPlayerTransform->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z + 0.01f);
	}

	
	return S_OK;
}

_int CGhost_TimeStop::Update_GameObject(const _float& fTimeDelta)
{

	
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;

	m_iAlpha -= 5;

	if (m_iAlpha < 0) {
		m_iAlpha = 0;
	}

	if (m_iAlpha == 0) {
		m_bDead = true;
	}



	
	__super::Update_GameObject(Time);

	
	return 0;
}

void CGhost_TimeStop::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();



	
}

void CGhost_TimeStop::Render_GameObject()
{


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 100, 216, 201));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_iAngleState == ANGLE_000) {

		m_pRunTextureCom[RUN_000]->Set_Texture((_uint)m_Sprite);
	}
	else if (m_iAngleState == ANGLE_045) {
		if (m_bReverse == true) {
			m_pTransformCom->Set_Scale_X_Ratio(-1.f);
			m_bReverse = false;
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pRunTextureCom[RUN_045]->Set_Texture((_uint)m_Sprite);
	}
	else if (m_iAngleState == ANGLE_090) {
		if (m_bReverse == true) {
			m_pTransformCom->Set_Scale_X_Ratio(-1.f);
			m_bReverse = false;
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pRunTextureCom[RUN_090]->Set_Texture((_uint)m_Sprite);
	}
	else if (m_iAngleState == ANGLE_135) {
		if (m_bReverse == true) {
			m_pTransformCom->Set_Scale_X_Ratio(-1.f);
			m_bReverse = false;
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pRunTextureCom[RUN_135]->Set_Texture((_uint)m_Sprite);
	}
	else if (m_iAngleState == ANGLE_180) {


		m_pRunTextureCom[RUN_180]->Set_Texture((_uint)m_Sprite);
	}
	else if (m_iAngleState == ANGLE_225) {
		if (m_bReverse == false) {
			m_pTransformCom->Set_Scale_X_Ratio(-1.f);
			m_bReverse = true;
		}

	
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pRunTextureCom[RUN_135]->Set_Texture((_uint)m_Sprite);
	}
	else if (m_iAngleState == ANGLE_270) {
		if (m_bReverse == false) {
			m_pTransformCom->Set_Scale_X_Ratio(-1.f);
			m_bReverse = true;
		}

		
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pRunTextureCom[RUN_090]->Set_Texture((_uint)m_Sprite);
	}
	else if (m_iAngleState == ANGLE_315) {
		if (m_bReverse == false) {
			m_pTransformCom->Set_Scale_X_Ratio(-1.f);
			m_bReverse = true;
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pRunTextureCom[RUN_045]->Set_Texture((_uint)m_Sprite);
	}
	

	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CGhost_TimeStop* CGhost_TimeStop::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGhost_TimeStop* pInstance = new CGhost_TimeStop(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGhost_TimeStop::Free(void)
{
	__super::Free();
}
