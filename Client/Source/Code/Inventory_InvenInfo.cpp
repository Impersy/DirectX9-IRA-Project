#include "stdafx.h"
#include "..\Header\Inventory_InvenInfo.h"
#include "Export_Function.h"

#include "Player.h"

CInventory_InvenInfo::CInventory_InvenInfo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev), m_fRadius(46.f), m_tRect {0, 0, 0, 0}, m_pReceive_PlayerInven(nullptr), m_iPicking_Index(10)
{
	//m_tINFO[0]->iMax = 5;
}

CInventory_InvenInfo::CInventory_InvenInfo(const CInventory_InvenInfo& rhs)
	: CFrontUi(rhs), m_fRadius(rhs.m_fRadius), m_tRect(rhs.m_tRect), m_pReceive_PlayerInven(rhs.m_pReceive_PlayerInven), m_iPicking_Index(rhs.m_iPicking_Index)
{
}

CInventory_InvenInfo::~CInventory_InvenInfo()
{
}

HRESULT CInventory_InvenInfo::Ready_GameObject(void)
{
	Add_Component();

	Set_Receive_PlayerInven();

	m_eRecieveInfo = m_pReceive_PlayerInven->Weapon[0];

	vTemp[0] = { 323.f, 40.f, 0.f };
	vTemp[1] = { 415.f, 40.f, 0.f };
	vTemp[2] = { 507.f, 40.f, 0.f };
	vTemp[3] = { 599.f, 40.f, 0.f };
	vTemp[4] = { 691.f, 40.f, 0.f };
	vTemp[5] = { 323.f, -52.f, 0.f };
	vTemp[6] = { 415.f, -52.f, 0.f };
	vTemp[7] = { 507.f, -52.f, 0.f };
	vTemp[8] = { 599.f, -52.f, 0.f };
	vTemp[9] = { 691.f, -52.f, 0.f };
	vTemp[10] = { -150.f, 48.f, 0.f };
	vTemp[11] = { -50.f, 118.f, 0.f };
	vTemp[12] = { 50.f, 118.f, 0.f };
	vTemp[13] = { 150.f, 48.f, 0.f };
	vTemp[14] = { -150.f, -60.f, 0.f };
	vTemp[15] = { -50.f, -130.f, 0.f };
	vTemp[16] = { 50.f, -130.f, 0.f };
	vTemp[17] = { 150.f, -60.f, 0.f };

	m_eCategory = UI_FRONT;

	return S_OK;
}

_int CInventory_InvenInfo::Update_GameObject(const _float& fTimeDelta)
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	_bool bInvenClose = pPlayer->Get_bItemUi_On();
	if (bInvenClose == false)
		return OBJ_DEAD;

	Picking_Check();
	Frame_Check(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CInventory_InvenInfo::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CInventory_InvenInfo::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	Ui_Print(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	Text_Print();
}

HRESULT CInventory_InvenInfo::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Inventory_InvenInfo"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Inventory_InvenInfo", pComponent });

	return S_OK;
}

void CInventory_InvenInfo::Change_State(void)
{

}

void CInventory_InvenInfo::Frame_Check(const _float& fTimeDelta)
{	
	if (m_iPicking_Index < 10)
	{	
		ITEM_INFO eTemp_INFO = m_pReceive_PlayerInven->Space[(m_iPicking_Index)];
		
		if ((eTemp_INFO.tType == ITEM_WEAPON) || (eTemp_INFO.tType == ITEM_ACC) )
		{
			m_eRecieveInfo = eTemp_INFO;
		}
	}
	else if ((10 <= m_iPicking_Index) && (m_iPicking_Index < 14))
	{
		ITEM_INFO eTemp_INFO = m_pReceive_PlayerInven->Weapon[(m_iPicking_Index - 10)];

		if ((eTemp_INFO.tType == ITEM_WEAPON) || (eTemp_INFO.tType == ITEM_ACC))
		{
			m_eRecieveInfo = eTemp_INFO;
		}
	}
	else if ((14 <= m_iPicking_Index) && (m_iPicking_Index < 18))
	{
		ITEM_INFO eTemp_INFO = m_pReceive_PlayerInven->Acc[(m_iPicking_Index - 14)];

		if ((eTemp_INFO.tType == ITEM_WEAPON) || (eTemp_INFO.tType == ITEM_ACC))
		{
			m_eRecieveInfo = eTemp_INFO;
		}
	}

	m_fFrame = (m_eRecieveInfo.iItemNumber - 5.f);

	if (m_fFrame < 0)
	{
		m_fFrame = 0.f;
	}
}

void CInventory_InvenInfo::Ui_Print(_int _iNum)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	matWorld._11 = 6.f;
	matWorld._22 = 6.f;
	matWorld._41 = -12.f;
	matWorld._42 = 0.f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CInventory_InvenInfo::Text_Print(void)
{
	// 아이템 인포 하단 설명부분
	std::string itemName = m_eRecieveInfo.sItemDescription_Normal;
	int bufferSize = MultiByteToWideChar(CP_ACP, 0, itemName.c_str(), -1, NULL, 0);
	wchar_t* buffer = new wchar_t[bufferSize];
	MultiByteToWideChar(CP_ACP, 0, itemName.c_str(), -1, buffer, bufferSize);
	Engine::Render_Font(L"Font_Info_1", buffer, &_vec2(147.f, 575.f), D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.9f));
	delete[] buffer;

	// 등급  (6 7 8 9 12 영웅 / 10 13 희귀) 보라 139 0 255 하늘 231 203 255
	if (((m_eRecieveInfo.iItemNumber >= 6) && (m_eRecieveInfo.iItemNumber <= 9)) || (m_eRecieveInfo.iItemNumber == 12))
	{

	}
	std::string itemGrade = m_eRecieveInfo.sItemGrade;
	int bufferSize_G = MultiByteToWideChar(CP_ACP, 0, itemGrade.c_str(), -1, NULL, 0);
	wchar_t* buffer_G = new wchar_t[bufferSize_G];
	MultiByteToWideChar(CP_ACP, 0, itemGrade.c_str(), -1, buffer_G, bufferSize_G);
	Engine::Render_Font(L"Font_Info_2", buffer_G, &_vec2(445.f, 388.5f), D3DXCOLOR(0.55f, 0.f, 1.f, 1.f));//보라
	delete[] buffer_G;
	
	// 물품 분류 (무기, 장신구) 및 강화단계
	if (((m_eRecieveInfo.iItemNumber >= 5) && (m_eRecieveInfo.iItemNumber <= 8)))
	{
		_tchar				itemCate_W[128] = L"무기";
		Engine::Render_Font(L"Font_Info_2", itemCate_W, &_vec2(320.f, 388.5f), D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.9f));
	}
	else if (((m_eRecieveInfo.iItemNumber >= 9) && (m_eRecieveInfo.iItemNumber <= 13)))
	{
		_tchar				itemCate_C[128] = L"장신구";
		Engine::Render_Font(L"Font_Info_2", itemCate_C, &_vec2(320.f, 388.5f), D3DXCOLOR(1.f, 1.f, 0.1f, 1.f));
	}
	// 무기 최소공격력
	TCHAR Buffer_AttMin[256];
	if (((m_eRecieveInfo.iItemNumber >= 5) && (m_eRecieveInfo.iItemNumber <= 8)))
	{
		_stprintf_s(Buffer_AttMin, _T("%d"), m_eRecieveInfo.iMinAtt);
		Engine::Render_Font(L"Font_Info_2", Buffer_AttMin, &_vec2(360.f, 441.f), D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.9f));
	}
	// 무기 최대 공격력
	TCHAR Buffer_AttMax[256];
	if (((m_eRecieveInfo.iItemNumber >= 5) && (m_eRecieveInfo.iItemNumber <= 8)))
	{
		_stprintf_s(Buffer_AttMax, _T("%d"), m_eRecieveInfo.iMaxAtt);
		Engine::Render_Font(L"Font_Info_2", Buffer_AttMax, &_vec2(490.f, 441.f), D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.9f));
	}

	if (((m_eRecieveInfo.iItemNumber >= 5) && (m_eRecieveInfo.iItemNumber <= 13)))
	{	
		_int iCur_1 = 0;
		_int iCur_2 = 0;
		_int iCur_3 = 0;
		_int iCur_4 = 0;

		if (m_eRecieveInfo.fPlusSpeed < 10)
			iCur_1 = 4;
		if (m_eRecieveInfo.iPlus_Att < 10)
			iCur_2 = 4;
		if (m_eRecieveInfo.iPlus_MaxHp < 10)
			iCur_3 = 4;
		if (m_eRecieveInfo.iPlus_MaxEnergy < 10)
			iCur_4 = 4;


		// 속도 추가
		TCHAR Buffer_Speed[256];
		_stprintf_s(Buffer_Speed, _T("%d"), (_int)(m_eRecieveInfo.fPlusSpeed * 10));
		Engine::Render_Font(L"Font_Info_2", Buffer_Speed, &_vec2(360.f + iCur_1, 467.5f), D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.9f));
		
		// 공격력 추가
		TCHAR Buffer_PlusAtt[256];
		_stprintf_s(Buffer_PlusAtt, _T("%d"), (_int)(m_eRecieveInfo.iPlus_Att));
		Engine::Render_Font(L"Font_Info_2", Buffer_PlusAtt, &_vec2(490.f + iCur_2, 467.5f), D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.9f));

		// 체력 추가
		TCHAR Buffer_PlusMaxHp[256];
		_stprintf_s(Buffer_PlusMaxHp, _T("%d"), (_int)(m_eRecieveInfo.iPlus_MaxHp));
		Engine::Render_Font(L"Font_Info_2", Buffer_PlusMaxHp, &_vec2(360.f + iCur_3, 494.f), D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.9f));

		// 기력 추가
		TCHAR Buffer_PlusMaxEng[256];
		_stprintf_s(Buffer_PlusMaxEng, _T("%d"), (_int)(m_eRecieveInfo.iPlus_MaxEnergy));
		Engine::Render_Font(L"Font_Info_2", Buffer_PlusMaxEng, &_vec2(490.f + iCur_4, 494.f), D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.9f));
	}
}

void CInventory_InvenInfo::Picking_Check(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	for (int i = 0; i < 18; i++)
	{
		m_tRect.left = LONG(800.f + vTemp[i].x - (50.f));
		m_tRect.top = LONG(450.f - vTemp[i].y - (50.f));
		m_tRect.right = LONG(800.f + vTemp[i].x + (50.f));
		m_tRect.bottom = LONG(450.f - vTemp[i].y + (50.f));

		if (PtInRect(&m_tRect, pt))
		{
			m_iPicking_Index = i;
		}
	}
}

void CInventory_InvenInfo::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CInventory_InvenInfo::Set_Receive_PlayerInven(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_pReceive_PlayerInven = pPlayer->Get_InvenInfo();
}

CInventory_InvenInfo* CInventory_InvenInfo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventory_InvenInfo* pInstance = new CInventory_InvenInfo(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInventory_InvenInfo::Free(void)
{
	__super::Free();
}
