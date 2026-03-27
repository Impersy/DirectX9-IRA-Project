#include "stdafx.h"
#include "..\Header\Effect_Ui_TextCallByButton.h"
#include "Export_Function.h"

#include "Npc_RerollLever.h"

#include "SoundMgr.h"

CEffect_Ui_TextCallByButton::CEffect_Ui_TextCallByButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Ui_TextCallByButton::CEffect_Ui_TextCallByButton(const CEffect_Ui_TextCallByButton& rhs)
	: CEffect(rhs)
{
}

CEffect_Ui_TextCallByButton::~CEffect_Ui_TextCallByButton()
{
	Free();
}

HRESULT CEffect_Ui_TextCallByButton::Ready_GameObject(void)
{
	Add_Component();

	m_fMaxFrame = 20.f;

	return S_OK;
}

_int CEffect_Ui_TextCallByButton::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		if (m_eInter_Target == TARGET_SUPPLY_BIGCAT)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"UI_Cat_Meow.wav", CSoundMgr::UI_INTERACTION, 0.8f);
		}
		return OBJ_DEAD;
	}

	m_fFrame += (m_fMaxFrame * fTimeDelta * 0.5f);
	m_fTextPrint_Delay += (m_fTextPrint_Delay_Max * fTimeDelta);
	if (m_fTextPrint_Delay > m_fTextPrint_Delay_Max)
	{
		m_fTextPrint_Delay = m_fTextPrint_Delay_Max;
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CEffect_Ui_TextCallByButton::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{	
		m_fFrame = m_fMaxFrame;
	}

	if (m_fFrame == m_fMaxFrame)
	{
		m_bDead = true;
	}

}

void CEffect_Ui_TextCallByButton::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	m_pTextureCom->Set_Texture(0);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	if ((m_bDead == false))
	{	
		Ui_Print();
		m_pBufferCom->Render_Buffer();
		Text_Print();
	}

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);

	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEffect_Ui_TextCallByButton::Ui_Print(void)
{	
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	matWorld._11 = 10.f;
	matWorld._22 = 7.f;
	matWorld._41 = 0;
	matWorld._42 = 3.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CEffect_Ui_TextCallByButton::Text_Print(void)
{	
	if (m_eInter_Target == TARGET_ITEM_DROP)
	{	
		std::string itemName = m_eItemInfo.sItemName;
		int bufferSize = MultiByteToWideChar(CP_ACP, 0, itemName.c_str(), -1, NULL, 0);
		wchar_t* buffer = new wchar_t[bufferSize];
		MultiByteToWideChar(CP_ACP, 0, itemName.c_str(), -1, buffer, bufferSize);

		std::wstring message;

		message += L"플레이어가 아이템 [";
		message += buffer;
		message += L"] 을 획득하였습니다";

		Engine::Render_Font(L"Font_Title_3", message.c_str(), &_vec2(585.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		delete[] buffer;
	}
	else if (m_eInter_Target == TARGET_ITEM_SHOP)
	{	
		std::string itemName = m_eItemInfo.sItemName;
		int bufferSize = MultiByteToWideChar(CP_ACP, 0, itemName.c_str(), -1, NULL, 0);
		wchar_t* buffer = new wchar_t[bufferSize];
		MultiByteToWideChar(CP_ACP, 0, itemName.c_str(), -1, buffer, bufferSize);

		std::wstring message;

		message += L"플레이어가 아이템 [";
		message += buffer;
		message += L"] 을 구매하였습니다";

		Engine::Render_Font(L"Font_Title_3", message.c_str(), &_vec2(585.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		delete[] buffer;
	}
	else if (m_eInter_Target == TARGET_SUPPLYCREW)
	{
		_tchar	m_szString_Temp[128] = L"대화 - 상점 꼬마";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_BLACKSMITH)
	{
		_tchar	m_szString_Temp[128] = L"대화 - 강화 장인";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp, &_vec2(987.f, 603.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_REGEN_STONE)
	{
		_tchar	m_szString_Temp_4[128] = L"플레이어가 손을 갖다대자, 유적의 문양이 활성화되었습니다";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(560.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_APOSTLE_COURAGE)
	{
		_tchar	m_szString_Temp_3[128] = L"용기의 가호를 받아 사도의 힘을 사용할 수 있게 되었습니다";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_3, &_vec2(560.f, 150.f), D3DXCOLOR(1.f, 0.f, 0.f, 0.6f));
	}
	else if (m_eInter_Target == TARGET_APOSTLE_NIGHT)
	{
		_tchar	m_szString_Temp_4[128] = L"밤의 가호를 받아 사도의 힘을 사용할 수 있게 되었습니다";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(560.f, 150.f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.6f));
	}
	else if (m_eInter_Target == TARGET_APOSTLE_TIME)
	{
		_tchar	m_szString_Temp_4[128] = L"시간의 가호를 받아 사도의 힘을 사용할 수 있게 되었습니다";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(560.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.7f));
	}
	else if (m_eInter_Target == TARGET_REROLL_REVER)
	{
		_tchar	m_szString_Temp_4[128] = L"힘껏 레버를 돌립니다. 상점의 아이템이 새롭게 갱신됩니다.";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(560.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		if (!m_bMoney_Set)
		{
		m_bMoney_Set = true;
		CNpc_RerollLever* pLever = dynamic_cast<CNpc_RerollLever*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Npc_RerollLever"));
		pLever->Item_Delete_Reroll();
		}
	}
	else if (m_eInter_Target == TARGET_SUPPLY_BIGCAT)
	{
		_tchar	m_szString_Temp_4[128] = L"거대한 고양이를 쓰다듬습니다. 그냥 기분이 좀 좋아집니다 ";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(560.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_SUPPLY_BACKPACK)
	{
		_tchar	m_szString_Temp_4[128] = L"상인 몰래 가방을 열어봅니다. 티 안나게 동전을 획득합니다";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(560.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_RABBIT_BLUE)
	{
		_tchar	m_szString_Temp_4[128] = L"파란 토끼가 소량의 동전을 후원합니다. 따봉 토끼야 고마워";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(560.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_RABBIT_PINK)
	{
		_tchar	m_szString_Temp_4[128] = L"분홍 토끼가 소량의 동전을 후원합니다. 따봉 토끼야 고마워";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(560.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_BUY_FALSE)
	{
		_tchar	m_szString_Temp_4[128] = L"플레이어의 소지금이 부족해 아이템을 구매 할 수 없습니다. ";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(560.f, 150.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_ROOTING_FALSE)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"UI_Buy Fail.wav", CSoundMgr::UI_INTERACTION, 0.8f);
		_tchar	m_szString_Temp_4[128] = L"인벤토리에 빈 공간이 없어 아이템을 습득 할 수 없습니다. ";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(560.f, 150.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}
	else if (m_eInter_Target == TARGET_POT)
	{	
		_tchar	m_szString_Temp_4[128] = L"송민경이 항아리를 열었습니다. 안에 김장김치가 가득합니다";
		Engine::Render_Font(L"Font_Title_3", m_szString_Temp_4, &_vec2(560.f, 150.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.7f));
	}

}

void CEffect_Ui_TextCallByButton::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}


void CEffect_Ui_TextCallByButton::Face_Check(void)
{
	// 플레이어 Pos 받아 MasterPos와 구충돌
	_vec3 vPlayerPos;
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	vPlayerPos;

	_float x = vPlayerPos.x;
	_float z = vPlayerPos.z;
	_float radius1 = 3.0f;

	_float x1 = m_vMasterPos.x;
	_float z1 = m_vMasterPos.z;
	_float radius2 = 3.0f;

	_float dx = x - x1;
	_float dz = z - z1;
	_float distance = sqrt(dx * dx + dz * dz);

	if (distance < radius1 + radius2)
	{
		m_bFace = true;
	}
	else
	{
		m_bFace = false;
	}
}

CEffect_Ui_TextCallByButton* CEffect_Ui_TextCallByButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _Target, _vec3 vPos)
{
	CEffect_Ui_TextCallByButton* pInstance = new CEffect_Ui_TextCallByButton(pGraphicDev);

	pInstance->m_vMasterPos = vPos;
	pInstance->m_eInter_Target = _Target;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CEffect_Ui_TextCallByButton* CEffect_Ui_TextCallByButton::Create_And_SaveItemInfo(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _Target, _vec3 vPos, ITEM_INFO _eInfo)
{
	CEffect_Ui_TextCallByButton* pInstance = new CEffect_Ui_TextCallByButton(pGraphicDev);

	pInstance->m_eInter_Target = _Target;
	pInstance->m_eItemInfo = _eInfo;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CEffect_Ui_TextCallByButton* CEffect_Ui_TextCallByButton::Create_Buy_False(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _Target)
{
	CEffect_Ui_TextCallByButton* pInstance = new CEffect_Ui_TextCallByButton(pGraphicDev);

	pInstance->m_eInter_Target = _Target;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CEffect_Ui_TextCallByButton* CEffect_Ui_TextCallByButton::Create_Rooting_False(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _Target)
{
	CEffect_Ui_TextCallByButton* pInstance = new CEffect_Ui_TextCallByButton(pGraphicDev);

	pInstance->m_eInter_Target = _Target;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CEffect_Ui_TextCallByButton::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Ui_TextCallByButton"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Ui_TextCallByButton", pComponent });

	return S_OK;
}

void CEffect_Ui_TextCallByButton::Free(void)
{
	__super::Free();
}



