#include "stdafx.h"
#include "..\Header\BigMap_Text.h"
#include "Export_Function.h"

#include "Player.h"
#include "BigMap_Text.h"

CBigMap_Text::CBigMap_Text(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
}

CBigMap_Text::CBigMap_Text(const CBigMap_Text& rhs)
	: CFrontUi(rhs)
{
}

CBigMap_Text::~CBigMap_Text()
{
	Free();
}

HRESULT CBigMap_Text::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;

	Recieve_Player_LVSK();

	return S_OK;
}

_int CBigMap_Text::Update_GameObject(const _float& fTimeDelta)
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	_bool bInvenClose = pPlayer->Get_bBigMapUi_On();
	if (bInvenClose == false)
		return OBJ_DEAD;

	Recieve_Player_LVSK();

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBigMap_Text::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBigMap_Text::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture((_uint)m_fFrame);

	//_matrix matCamWorld;
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	//Ui_Print(0);

	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	//Reset_Proj_Matrix();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	Text_Print();
}

HRESULT CBigMap_Text::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Ui_BigMap_Back"));
	// 
	//m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Ui_BigMap_Back", pComponent });

	return S_OK;
}

void CBigMap_Text::Change_State(void)
{

}

void CBigMap_Text::Frame_Check(const _float& fTimeDelta)
{

}

void CBigMap_Text::Ui_Print(_int _iNum)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 450.f;
	matWorld._22 = 450.f;
	matWorld._41 = 0.f;
	matWorld._42 = 25.f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

_bool CBigMap_Text::BigMap_Close(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	if (!(pPlayer->Get_bBigMapUi_On()))
	{
		return true;
	}
}

void CBigMap_Text::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBigMap_Text* CBigMap_Text::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBigMap_Text* pInstance = new CBigMap_Text(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBigMap_Text::Free(void)
{
	__super::Free();
}

void CBigMap_Text::Text_Print(void)
{	
	TCHAR Buffer_Player_Level[256];
	_stprintf_s(Buffer_Player_Level, _T("%d"), m_tRecieve_LVSK.iPlayerLevel);

	Engine::Render_Font(L"Font_Dialog_Name", Buffer_Player_Level, &_vec2(793.f, 80.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));

	// 공격력 강화 출력
	TCHAR Buffer_Player_ATTPlus[256];
	_stprintf_s(Buffer_Player_ATTPlus, _T("%d"), m_tRecieve_LVSK.iSkill_Att_Plus);
	std::wstring message_ATT;
	message_ATT += L"Lv [ ";
	message_ATT += Buffer_Player_ATTPlus;
	message_ATT += L" ]";
	Engine::Render_Font(L"Font_Info_2", message_ATT.c_str(), &_vec2(626.f, 757.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.7f));

	// 이동속도 강화 출력
	TCHAR Buffer_Player_SPEEDPlus[256];
	_stprintf_s(Buffer_Player_SPEEDPlus, _T("%d"), m_tRecieve_LVSK.iSkill_Speed_Plus);
	std::wstring message_SPD;
	message_SPD += L"Lv [ ";
	message_SPD += Buffer_Player_SPEEDPlus;
	message_SPD += L" ]";
	Engine::Render_Font(L"Font_Info_2", message_SPD.c_str(), &_vec2(725.f, 757.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.7f));

	// 코인 획득량 강화 출력
	TCHAR Buffer_Player_COINPlus[256];
	_stprintf_s(Buffer_Player_COINPlus, _T("%d"), m_tRecieve_LVSK.iSkill_Money_Plus);
	std::wstring message_Coin;
	message_Coin += L"Lv [ ";
	message_Coin += Buffer_Player_COINPlus;
	message_Coin += L" ]";
	Engine::Render_Font(L"Font_Info_2", message_Coin.c_str(), &_vec2(825.f, 757.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.7f));

	// 코인 획득량 강화 출력
	TCHAR Buffer_Player_GEMPlus[256];
	_stprintf_s(Buffer_Player_GEMPlus, _T("%d"), m_tRecieve_LVSK.iSkill_Gem_Plus);
	std::wstring message_Gem;
	message_Gem += L"Lv [ ";
	message_Gem += Buffer_Player_GEMPlus;
	message_Gem += L" ]";
	Engine::Render_Font(L"Font_Info_2", message_Gem.c_str(), &_vec2(924.f, 757.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.7f));


	// 맵상 현재위치 표시
	if ((g_eCurScene != SCENE_TITLE) && (g_eCurScene != SCENE_END))
	{
		if (g_eCurScene == SCENE_TUTORIAL)
		{
			_tchar				m_szString_eCurScene_1[128] = L"현재 위치  [ 천계 - 튜토리얼의 방   ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_1, &_vec2(970.f, 212.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_MAINLOBBY)
		{
			_tchar				m_szString_eCurScene_2[128] = L"현재 위치  [ 천계 - 사도의 공간     ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_2, &_vec2(970.f, 212.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_DUNGEONLOBBY)
		{
			_tchar				m_szString_eCurScene_3[128] = L"현재 위치  [ 던전 1층 - 시작의 방   ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_3, &_vec2(970.f, 212.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_REGEN)
		{
			_tchar				m_szString_eCurScene_4[128] = L"현재 위치  [ 던전 1층 - 몬스터의 방 ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_4, &_vec2(970.f, 212.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_SHOP)
		{
			_tchar				m_szString_eCurScene_5[128] = L"현재 위치  [ 던전 1층 - 상인의 방   ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_5, &_vec2(970.f, 212.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_BOSS1)
		{
			_tchar				m_szString_eCurScene_6[128] = L"현재 위치  [ 던전 1층 - 도올의 방   ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_6, &_vec2(970.f, 212.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_BOSS2)
		{
			_tchar				m_szString_eCurScene_7[128] = L"현재 위치  [ 던전 1층 - 도철의 방   ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_7, &_vec2(970.f, 212.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_POT)
		{
			_tchar				m_szString_eCurScene_8[128] = L"현재 위치  [ 던전 1층 - 보너스 방   ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_8, &_vec2(970.f, 212.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_NORMAL)
		{
			_tchar				m_szString_eCurScene_9[128] = L"현재 위치  [ 던전 1층 - 몬스터의 방 ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_9, &_vec2(970.f, 212.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
	}
	
	// 플레이어 좌표 출력
	TCHAR Buffer_Player_LOC_x[256];
	_stprintf_s(Buffer_Player_LOC_x, _T("%d"), (_int)(m_vRecieve_Pl_Pos.x));
	TCHAR Buffer_Player_LOC_z[256];
	_stprintf_s(Buffer_Player_LOC_z, _T("%d"), (_int)(m_vRecieve_Pl_Pos.z));

	std::wstring message_LOC;
	message_LOC += L"현재 좌표  [ X : ";
	message_LOC += Buffer_Player_LOC_x;
	message_LOC += L" , Z : ";
	message_LOC += Buffer_Player_LOC_z;
	message_LOC += L" ]";

	Engine::Render_Font(L"Font_Info_2", message_LOC.c_str(), &_vec2(970.f, 237.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));





}

void CBigMap_Text::Recieve_Player_LVSK(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_tRecieve_LVSK = pPlayer->Get_tPlayerLvSk();

	_vec3	vPlayerPos;
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_vRecieve_Pl_Pos = vPlayerPos;
}
