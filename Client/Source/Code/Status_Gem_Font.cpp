#include "stdafx.h"
#include "..\Header\Status_Gem_Font.h"
#include "Export_Function.h"
#include "Player.h"

CStatus_Gem_Font::CStatus_Gem_Font(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev), m_bFontPrint(false), m_iFontPos(0), m_iMyNumber(0)
{
}

CStatus_Gem_Font::CStatus_Gem_Font(const CStatus_Gem_Font& rhs)
	: CFrontUi(rhs), m_bFontPrint(false), m_iFontPos(0), m_iMyNumber(0)
{
}

CStatus_Gem_Font::~CStatus_Gem_Font()
{
}

HRESULT CStatus_Gem_Font::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;

	return S_OK;
}

_int CStatus_Gem_Font::Update_GameObject(const _float& fTimeDelta)
{	
	Set_Receive_PlayerInfo();
	Print_Check();

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CStatus_Gem_Font::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CStatus_Gem_Font::Render_GameObject()
{	
	if (!g_UI_Render)
		return;

	if (m_bFontPrint)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pTextureCom->Set_Texture(m_iMyNumber);

		_matrix matCamWorld;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

		Ui_Print(m_tINFO.iNum, m_iFontPos);

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
		Reset_Proj_Matrix();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

HRESULT CStatus_Gem_Font::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Number_Min"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Number_Min", pComponent });

	return S_OK;
}

void CStatus_Gem_Font::Change_State(void)
{

}

void CStatus_Gem_Font::Frame_Check(const _float& fTimeDelta)
{

}

void CStatus_Gem_Font::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CStatus_Gem_Font::Set_Receive_PlayerInfo(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	pReceiveInfo = pPlayer->Get_Player_Info();
}

HRESULT CStatus_Gem_Font::Print_Check(void)
{	
	// 1000의 자리 값이 있을때
	_int iTemp_Thousand = (int)(pReceiveInfo.Gem / 1000);
	_int iTemp_Hundred = (int)((pReceiveInfo.Gem - (1000 * iTemp_Thousand)) / 100);
	_int iTemp_Ten = (int)((pReceiveInfo.Gem - (1000 * iTemp_Thousand) - (100 * iTemp_Hundred)) / 10);
	_int iTemp_Num = (int)((pReceiveInfo.Gem - (1000 * iTemp_Thousand) - (100 * iTemp_Hundred) - (10 * iTemp_Ten)) / 1);

	if (4 == m_tINFO.iNum) // 1의 자리
	{	
		m_bFontPrint = true;
		m_iMyNumber = (iTemp_Num);

		if (1 <= iTemp_Thousand)
			m_iFontPos = 4;

		if ((1 > iTemp_Thousand))
		{
			if (1 <= iTemp_Hundred)
				m_iFontPos = 3;

			if ((1 > iTemp_Hundred) && (1 <= iTemp_Ten))
				m_iFontPos = 2;

			if ((1 > iTemp_Hundred) && (1 > iTemp_Ten))
				m_iFontPos = 1;
		}

		if (m_iMyNumber < 0)
			m_iMyNumber = 0;
	}

	if (3 == m_tINFO.iNum) // 10의 자리
	{	
		m_bFontPrint = true;
		m_iMyNumber = (iTemp_Ten);

		if (1 <= iTemp_Thousand)
			m_iFontPos = 3;

		if ((1 > iTemp_Thousand))
		{
			if (1 <= iTemp_Hundred)
				m_iFontPos = 2;

			if (1 > iTemp_Hundred)
				m_iFontPos = 1;

			if ((1 > iTemp_Hundred) && (1 > iTemp_Ten))
			{
				m_iFontPos = 1;
				m_bFontPrint = false;
			}
		}

		if (m_iMyNumber < 0)
			m_iMyNumber = 0;
	}

	if (2 == m_tINFO.iNum) // 100의 자리
	{	
		m_bFontPrint = true;
		m_iMyNumber = (iTemp_Hundred);

		if (1 <= iTemp_Thousand)
			m_iFontPos = 2;

		if (1 > iTemp_Thousand)
			m_iFontPos = 1;

		if ((1 > iTemp_Thousand) && (1 > iTemp_Hundred))
		{
			m_iFontPos = 1;
			m_bFontPrint = false;
		}

		if (m_iMyNumber < 0)
			m_iMyNumber = 0;
	}

	if (1 == m_tINFO.iNum) // 1000의 자리
	{	
		m_bFontPrint = true;
		m_iMyNumber = (iTemp_Thousand);
		m_iFontPos = 1;

		if (1 > iTemp_Thousand)
			m_bFontPrint = false;

		if (m_iMyNumber < 0)
			m_iMyNumber = 0;
	}



	return S_OK;
}

void CStatus_Gem_Font::Ui_Print(_int _iNumber, _int _iFontPos)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);
	
	switch (_iNumber)
	{
	case 1:	// 1000자리
		matWorld._11 = 0.22f;
		matWorld._22 = 0.22f;
		matWorld._41 = -17.1f;
		matWorld._42 = 5.435f;
		break;
	case 2:	// 100자리
		matWorld._11 = 0.22f;
		matWorld._22 = 0.22f;
		matWorld._41 = -16.95f + ((m_iFontPos - 1) * 0.15f);
		matWorld._42 = 5.435f;
		break;
	case 3:	// 10자리
		matWorld._11 = 0.22f;
		matWorld._22 = 0.22f;
		matWorld._41 = -16.8f + ((m_iFontPos - 1) * 0.15f);
		matWorld._42 = 5.435f;
		break;
	case 4:	// 1자리
		matWorld._11 = 0.22f;
		matWorld._22 = 0.22f;
		matWorld._41 = -16.65f + ((m_iFontPos - 1) * 0.15f);
		matWorld._42 = 5.435f;
		break;
	default:
		break;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CStatus_Gem_Font* CStatus_Gem_Font::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CStatus_Gem_Font* pInstance = new CStatus_Gem_Font(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CStatus_Gem_Font::Free(void)
{
	__super::Free();
}

