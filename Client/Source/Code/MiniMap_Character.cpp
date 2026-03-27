#include "stdafx.h"
#include "..\Header\MiniMap_Character.h"
#include "Export_Function.h"

CMiniMap_Character::CMiniMap_Character(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CMiniMap_Character::CMiniMap_Character(const CMiniMap_Character& rhs)
	: CFrontUi(rhs)
{
}

CMiniMap_Character::~CMiniMap_Character()
{
}

HRESULT CMiniMap_Character::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CMiniMap_Character::Update_GameObject(const _float& fTimeDelta)
{	
	_vec3 vPlayerPos;
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	m_vPlayerPos = vPlayerPos;

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CMiniMap_Character::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMiniMap_Character::Render_GameObject()
{
	if (!g_UI_Render)
		return;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_MiniMap_Print(m_tINFO.iNum, g_eCurScene, 0, &m_vPlayerPos); //서순 주의

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	Text_Print();
}

HRESULT CMiniMap_Character::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MiniMap_Character"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MiniMap_Character", pComponent });

	return S_OK;
}

void CMiniMap_Character::Change_State(void)
{

}

void CMiniMap_Character::Frame_Check(const _float& fTimeDelta)
{

}

void CMiniMap_Character::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CMiniMap_Character::Text_Print(void)
{	
	if ((g_eCurScene != SCENE_TITLE) && (g_eCurScene != SCENE_END))
	{
		if (g_eCurScene == SCENE_TUTORIAL)
		{
			_tchar				m_szString_eCurScene_1[128] = L"[  천계 - 튜토리얼의 방  ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_1, &_vec2(1412.f, 230.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_MAINLOBBY)
		{
			_tchar				m_szString_eCurScene_2[128] = L"[   천계 - 사도의 공간   ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_2, &_vec2(1412.f, 230.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_DUNGEONLOBBY)
		{
			_tchar				m_szString_eCurScene_3[128] = L"[  던전 1층 - 시작의 방  ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_3, &_vec2(1412.f, 230.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_REGEN)
		{
			_tchar				m_szString_eCurScene_4[128] = L"[ 던전 1층 - 몬스터의 방 ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_4, &_vec2(1412.f, 230.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_SHOP)
		{
			_tchar				m_szString_eCurScene_5[128] = L"[  던전 1층 - 상인의 방  ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_5, &_vec2(1412.f, 230.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_BOSS1)
		{
			_tchar				m_szString_eCurScene_6[128] = L"[  던전 1층 - 도올의 방  ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_6, &_vec2(1412.f, 230.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_BOSS2)
		{
			_tchar				m_szString_eCurScene_7[128] = L"[  던전 1층 - 도철의 방  ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_7, &_vec2(1412.f, 230.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_POT)
		{
			_tchar				m_szString_eCurScene_8[128] = L"[  던전 1층 - 보너스 방  ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_8, &_vec2(1412.f, 230.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
		if (g_eCurScene == SCENE_NORMAL)
		{
			_tchar				m_szString_eCurScene_9[128] = L"[ 던전 1층 - 몬스터의 방 ]";
			Engine::Render_Font(L"Font_Info_2", m_szString_eCurScene_9, &_vec2(1412.f, 230.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));
		}
	}

	// 플레이어 좌표 출력
	TCHAR Buffer_Player_LOC_x[256];
	_stprintf_s(Buffer_Player_LOC_x, _T("%d"), (_int)(m_vPlayerPos.x));
	TCHAR Buffer_Player_LOC_z[256];
	_stprintf_s(Buffer_Player_LOC_z, _T("%d"), (_int)(m_vPlayerPos.z));

	std::wstring message_LOC;
	message_LOC += L"[  좌표 x : ";
	message_LOC += Buffer_Player_LOC_x;
	message_LOC += L"  z : ";
	message_LOC += Buffer_Player_LOC_z;
	message_LOC += L"   ]";

	Engine::Render_Font(L"Font_Info_2", message_LOC.c_str(), &_vec2(1412.f, 209.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.8f));



}

CMiniMap_Character* CMiniMap_Character::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CMiniMap_Character* pInstance = new CMiniMap_Character(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CMiniMap_Character::Free(void)
{
	__super::Free();
}
