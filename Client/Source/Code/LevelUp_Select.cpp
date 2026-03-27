#include "stdafx.h"
#include "..\Header\LevelUp_Select.h"
#include "Export_Function.h"

#include "Player.h"
#include "KeyMgr.h"
#include "Effect_Ui_LevelUp_Skill_Get.h"

#include "StaticCamera.h"

CLevelUp_Select::CLevelUp_Select(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
}

CLevelUp_Select::CLevelUp_Select(const CLevelUp_Select& rhs)
	: CFrontUi(rhs)
{
}

CLevelUp_Select::~CLevelUp_Select()
{
	Free();
}

HRESULT CLevelUp_Select::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;

	m_tRect[0] = { 280, 265, 520, 740 };
	m_tRect[1] = { 545, 265, 785, 740 };
	m_tRect[2] = { 810, 265, 1050, 740 };
	m_tRect[3] = { 1075, 265, 1315, 740 };

	CAMERA_ZOOM_IN

	return S_OK;
}

_int CLevelUp_Select::Update_GameObject(const _float& fTimeDelta)
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	_bool bLevelClose = pPlayer->Get_bLevelUpUi_On();

	Frame_Check(fTimeDelta);

	if (m_iClick_State == 0)
		Picking_Click_Check();

	if (m_bDead)
	{	
		CAMERA_ZOOM_IN_RETURN;
		pPlayer->Set_tPlayerLvSk(m_iClick_State);
		pPlayer->Set_bLevelUpUi_On(false);
		Create_Skill_Get_Effect();
		return OBJ_DEAD;
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CLevelUp_Select::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CLevelUp_Select::Render_GameObject()
{	
	if (m_iClick_State == 0)
	{
		if (!m_bAlphaPrintEnd)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

			m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));

			m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

			m_pTextureCom->Set_Texture((_uint)0);

			_matrix matCamWorld;
			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

			Ui_Print();
			m_pBufferCom->Render_Buffer();

			Reset_Proj_Matrix();
			m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
			m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
			m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}

		if (m_bAlphaPrintEnd)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			m_pTextureCom->Set_Texture((_uint)m_fFrame);

			_matrix matCamWorld;
			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

			Ui_Print();

			m_pBufferCom->Render_Buffer();

			m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
			Reset_Proj_Matrix();
			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		}
	}

	if (m_iClick_State)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));

		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

		m_pTextureCom->Set_Texture((_uint)m_iClick_State);

		_matrix matCamWorld;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

		Ui_Print();
		m_pBufferCom->Render_Buffer();

		Reset_Proj_Matrix();
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	
}

HRESULT CLevelUp_Select::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Ui_LevelUp_Select"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Ui_LevelUp_Select", pComponent });

	return S_OK;
}

void CLevelUp_Select::Change_State(void)
{
	m_iPicking_State;
}

void CLevelUp_Select::Frame_Check(const _float& fTimeDelta)
{
	if ((!m_bAlphaPrintEnd) && (m_iClick_State == 0))
	{
		m_fAlphaFrame += m_fAlphaFrame_Max * fTimeDelta;
		m_iAlpha = (_int)((255 / 10) * m_fAlphaFrame);

		if (m_fAlphaFrame > m_fAlphaFrame_Max)
		{
			m_fAlphaFrame = m_fAlphaFrame_Max;
			m_iAlpha = 255;
			m_bAlphaPrintEnd = true;
		}
	}

	if ((m_bAlphaPrintEnd == true) && (m_iClick_State == 0))
	{
		Select_Picking_Check();
	}

	if (m_iClick_State > 0)
	{	
		if (!m_bDeadDelay_End)
		{
			m_fDeadDelay += m_fDeadDelay_Max * fTimeDelta;
			if (m_fDeadDelay > m_fDeadDelay_Max)
			{
				m_fDeadDelay = m_fDeadDelay_Max;
				m_bDeadDelay_End = true;
			}
		}

		if (m_bDeadDelay_End)
		{
			m_fAlphaFrame += m_fAlphaFrame_Max * fTimeDelta;
			m_iAlpha = (_int)(255 - ((255 / 10) * m_fAlphaFrame));
			if (m_fAlphaFrame > m_fAlphaFrame_Max)
			{
				m_fAlphaFrame = m_fAlphaFrame_Max;
				m_iAlpha = 255;
				m_bAlphaPrintEnd = true;
				m_bDead = true;
			}
		}

	}
}

void CLevelUp_Select::Ui_Print(void)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 700.f;
	matWorld._22 = 700.f;
	matWorld._41 = 0.f;
	matWorld._42 = 50.f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

_bool CLevelUp_Select::LevelUp_Select_Close(void)
{
	//CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	//if (!(pPlayer->Get_bBigMapUi_On()))
	//{
	//	return true;
	//}
	return true;
}

void CLevelUp_Select::Select_Picking_Check(void)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);	// 전체 스크린 영역에서 클라이언트 좌표로 변환해주는 함수

	(float)ptMouse.x;
	(float)ptMouse.y;

	m_iPicking_State = 0;

	for (int i = 0; i < 4; i++)
	{	
		if (PtInRect(&m_tRect[i], ptMouse))
			m_iPicking_State = i + 1;
	}

	m_fFrame = m_iPicking_State;
}

void CLevelUp_Select::Picking_Click_Check(void)
{	
	if ((m_iPicking_State > 0) && (m_iClick_State == 0))
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_LBUTTON))
		{
			m_iClick_State = m_iPicking_State;
			m_fAlphaFrame = 0.f;
			m_iAlpha = 255;
		}
	}
}

void CLevelUp_Select::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CLevelUp_Select::Create_Skill_Get_Effect(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");
	CGameObject* pBulletObject = nullptr;
	pBulletObject = CEffect_Ui_LevelUp_Skill_Get::Create(m_pGraphicDev);
	pLayer->Add_BulletObject(pBulletObject);

}

CLevelUp_Select* CLevelUp_Select::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLevelUp_Select* pInstance = new CLevelUp_Select(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLevelUp_Select::Free(void)
{
	__super::Free();
}
