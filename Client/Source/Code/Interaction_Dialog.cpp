#include "stdafx.h"
#include "..\Header\Interaction_Dialog.h"
#include "Export_Function.h"

#include "Player.h"
#include "KeyMgr.h"
#include "Effect_Ui_BlackBar.h"
#include "KeyMgr.h"


CInteraction_Dialog::CInteraction_Dialog(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteraction(pGraphicDev), m_pTextureCom_SupplyCrew(nullptr), m_pTextureCom_BlackSmith(nullptr), m_pTextureCom_RegenStone(nullptr), m_pTextureCom_Apostle_Time(nullptr)
{
}

CInteraction_Dialog::CInteraction_Dialog(const CInteraction_Dialog& rhs)
	: CInteraction(rhs), m_pTextureCom_SupplyCrew(rhs.m_pTextureCom_SupplyCrew), m_pTextureCom_BlackSmith(rhs.m_pTextureCom_BlackSmith), m_pTextureCom_RegenStone(rhs.m_pTextureCom_RegenStone)
	, m_pTextureCom_Apostle_Time(rhs.m_pTextureCom_Apostle_Time)
{
}

CInteraction_Dialog::~CInteraction_Dialog()
{
	Free();
}

HRESULT CInteraction_Dialog::Ready_GameObject(void)
{
	Add_Component();
	m_eInter_Type = INTER_DIALOG;

	
	// 인트로인지 엔딩인지에 따라
	// 다이얼로그 카운트 변경

	if (g_Game_Clear == false) {
		m_Dialog_Num = 20;
	}
	else {
		m_Dialog_Num = 14;
	}


	return S_OK;
}

_int CInteraction_Dialog::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_bDead)
		return OBJ_DEAD;

	Alpha_Delay_Check(fTimeDelta);	// 생성간 알파값 적용 출력

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_bPlayer_Dialog = pPlayer->Get_bDialog_On();
	m_bRecieve_Player_Dialog_Target = pPlayer->Get_Player_Dialog_Target();

	if (m_bPlayer_Dialog && (m_bRecieve_Player_Dialog_Target == m_eInter_Target))
	{
		Face_Check();
		Key_Press_Check();
		Frame_Check(fTimeDelta);
		Dialog_Delay_Check(fTimeDelta);
	}

	g_Dialog_Cnt = m_Dialog_Cnt;

	__super::Update_GameObject(fTimeDelta);

	if (m_bPlayer_Dialog)
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CInteraction_Dialog::LateUpdate_GameObject()
{	

	__super::LateUpdate_GameObject();
}

void CInteraction_Dialog::Render_GameObject()
{	
	if (m_bPlayer_Dialog && (m_bRecieve_Player_Dialog_Target == m_eInter_Target))
	{
		if (m_eInter_Target == TARGET_APOSTLE_TIME) {
			
		}


		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));

		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

		TextureCom_Check();

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

	Text_Print();
}

HRESULT CInteraction_Dialog::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom_Apostle_Time = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Interaction_Dialog_Time"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Interaction_Dialog_Time", pComponent });

	pComponent = m_pTextureCom_SupplyCrew = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Interaction_Dialog_SupplyCrew"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Interaction_Dialog_SupplyCrew", pComponent });

	pComponent = m_pTextureCom_BlackSmith = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Interaction_Dialog_BlackSmith"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Interaction_Dialog_BlackSmith", pComponent });

	pComponent = m_pTextureCom_RegenStone = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Interaction_Dialog_RegenStone"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Interaction_Dialog_RegenStone", pComponent });
	
	
	/*
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Interaction_Dialog"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Interaction_Dialog", pComponent });
	
	텍스쳐 컴포넌트 추가해서 더 사용해야함
	대화상대에 따라 달라지는 텍스쳐가 필요함
	
	*/

	return S_OK;
}

void CInteraction_Dialog::Change_State(void)
{
}

void CInteraction_Dialog::Frame_Check(const _float& fTimeDelta)
{
	if (m_eInter_Target == TARGET_SUPPLYCREW)
	{
		m_fFrame_Max = 4;

		if ((m_fFrame < m_fFrame_Max) && (m_bButton_Call == false))
		{	
			m_bButton_Call = true;
			//버튼 이펙트 Next (다음 대화) // m_bButton_Call 의 불변수 만들어서 생성시 변수 온오프 추가 
		}

		if ((m_fFrame == m_fFrame_Max) && (m_bButton_Call == false))
		{	
			m_fFrame = 0.f;

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_bDialog_On(false);
		}

	}

	if (m_eInter_Target == TARGET_BLACKSMITH)
	{
		m_fFrame_Max = 3;

		if ((m_fFrame < m_fFrame_Max) && (m_bButton_Call == false))
		{
			m_bButton_Call = true;
			//버튼 이펙트 Next (다음 대화) // m_bButton_Call 의 불변수 만들어서 생성시 변수 온오프 추가 
		}

		if ((m_fFrame == m_fFrame_Max) && (m_bButton_Call == false))
		{
			m_fFrame = 0.f;

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_Player_Dialog_Target(TARGET_END);
			pPlayer->Set_bDialog_On(false);

		}
	}

	if (m_eInter_Target == TARGET_APOSTLE_TIME)
	{
		if (g_eCurScene == SCENE_MAINLOBBY) {
			m_fFrame_Max = 2;
		}
		else {
			m_fFrame_Max = 2;
		}

		if ((m_fFrame < m_fFrame_Max) && (m_bButton_Call == false))
		{
			m_bButton_Call = true;
			//버튼 이펙트 Next (다음 대화) // m_bButton_Call 의 불변수 만들어서 생성시 변수 온오프 추가 
		}

		if ((m_fFrame >= m_fFrame_Max) && (m_bButton_Call == false))
		{
			m_fFrame = 0.f;

			if (g_eCurScene == SCENE_MAINLOBBY) {

				if (m_Dialog_Cnt > m_Dialog_Num - 1) {
					m_fFrame = 3.f;

				}

				if (m_Dialog_Cnt > m_Dialog_Num) {
					CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
					pPlayer->Set_Player_Dialog_Target(TARGET_END);
					pPlayer->Set_bDialog_On(false);
					m_Dialog_Cnt = 0;
				}
			}
			else {
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
				pPlayer->Set_Player_Dialog_Target(TARGET_END);
				pPlayer->Set_bDialog_On(false);
			}

		}
	}

	if (m_eInter_Target == TARGET_REGEN_STONE)
	{
		m_fFrame_Max = 4;

		if ((m_fFrame < m_fFrame_Max) && (m_bButton_Call == false))
		{
			m_bButton_Call = true;
			//버튼 이펙트 Next (다음 대화) // m_bButton_Call 의 불변수 만들어서 생성시 변수 온오프 추가 
		}

		if ((m_fFrame == m_fFrame_Max) && (m_bButton_Call == false))
		{
			m_fFrame = 0.f;

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_bDialog_On(false);

			m_bDead = true;
		}
	}


}

void CInteraction_Dialog::TextureCom_Check(void)
{
	_float m_Frame = 0.f;

	switch (m_eInter_Target)
	{
	case TARGET_ITEM_DROP:
		break;
	case TARGET_ITEM_SHOP:
		break;
	case TARGET_APOSTLE_TIME:
		if (g_eCurScene == SCENE_MAINLOBBY) {

			if (g_Game_Clear == false) {

				if (m_Dialog_Cnt == 0)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 1)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 2)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 3)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 4)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 5)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 6)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 7)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 8)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 9)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 10)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 11)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 12)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 13)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 14)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 15)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 16)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 17)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 18)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 19)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 20)
					m_Frame = 1.f;
			}
			else {
				if (m_Dialog_Cnt == 0)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 1)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 2)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 3)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 4)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 5)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 6)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 7)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 8)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 9)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 10)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 11)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 12)
					m_Frame = 1.f;
				else if (m_Dialog_Cnt == 13)
					m_Frame = 0.f;
				else if (m_Dialog_Cnt == 14)
					m_Frame = 0.f;
			}
			m_pTextureCom_Apostle_Time->Set_Texture((_uint)m_Frame);

		}
		else {

		
			m_pTextureCom_Apostle_Time->Set_Texture((_uint)m_fFrame);
			
			
		}

		break;
	case TARGET_SUPPLYCREW:
		m_pTextureCom_SupplyCrew->Set_Texture((_uint)m_fFrame);
		break;
	case TARGET_BLACKSMITH:
		m_pTextureCom_BlackSmith->Set_Texture((_uint)m_fFrame);
		break;
	case TARGET_REGEN_STONE:
		m_pTextureCom_RegenStone->Set_Texture((_uint)m_fFrame);
		break;
	case TARGET_END:
		break;
	default:
		break;
	}
}

void CInteraction_Dialog::Ui_Print(void)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	matWorld._11 = 20.f;
	matWorld._22 = 20.f;
	matWorld._41 = 0.f;
	matWorld._42 = 0.f;

	m_eInter_Target;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CInteraction_Dialog::Face_Check(void)
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

void CInteraction_Dialog::Key_Press_Check(void)
{	
	if (!m_bDead)
	{
		if (m_bDelay && m_bFace && (CKeyMgr::Get_Instance()->Key_Down(KEY_E)))
		{
			if (m_fFrame < m_fFrame_Max)
			{
				m_fFrame += 1;

				m_Dialog_Cnt++;

			}
			else if (m_fFrame >= m_fFrame_Max)
			{

				m_fFrame = 0.f;

				CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
				pPlayer->Set_Player_Dialog_Target(TARGET_END);
				pPlayer->Set_bDialog_On(false);

				m_Dialog_Cnt = 0;

			}
			m_fDialog_Delay = 0.f;
			m_bDelay = false;
			m_bButton_Call = false;
		}
	}
}

void CInteraction_Dialog::Dialog_Delay_Check(const _float& fTimeDelta)
{
	m_fDialog_Delay += (m_fDialog_Delay_Max * fTimeDelta);

	if (m_fDialog_Delay > m_fDialog_Delay_Max)
	{
		m_fDialog_Delay = m_fDialog_Delay_Max;
		m_bDelay = true;
	}

	if (m_eInter_Target == TARGET_APOSTLE_TIME && g_eCurScene == SCENE_MAINLOBBY)
		m_bDelay = true;
}

void CInteraction_Dialog::Text_Print(void)
{
	if ((m_eInter_Target == TARGET_APOSTLE_TIME) && m_bFace)
	{
		if (g_eCurScene != SCENE_MAINLOBBY) {

			if (g_Boss1_Clear == true && g_eCurScene == SCENE_BOSS1) {

				
				if ((_int)m_fFrame == 1)
				{
					_tchar	m_szString_Temp_Name[128] = L"티프";
					Engine::Render_Font(L"Font_Dialog_Name", m_szString_Temp_Name, &_vec2(790.f, 720.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

					_tchar	m_szString_Temp_text_Line_1[128] = L"이라님..!! 벌써 혼돈의 숲의 주인인 도올을 처치하셨군요";
					_tchar	m_szString_Temp_text_Line_2[128] = L"방금 막 도철이 숨어있는 공간을 발견했으니 서둘러 가시죠!";

					Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_1, &_vec2(600.f, 790.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
					Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_2, &_vec2(600.f, 830.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));

				}

			}
			else if (g_Game_Clear == true && g_eCurScene == SCENE_BOSS2) {

				
				if ((_int)m_fFrame == 1)
				{
					
					_tchar	m_szString_Temp_Name[128] = L"티프";
					Engine::Render_Font(L"Font_Dialog_Name", m_szString_Temp_Name, &_vec2(790.f, 720.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

					_tchar	m_szString_Temp_text_Line_1[128] = L"도철이 소멸했군요..!! 다친 곳은 없으신가요? 무사하셔서 다행입니다";
					_tchar	m_szString_Temp_text_Line_2[128] = L"혼돈의 힘이 사라져 이 공간도 곧 붕괴될 겁니다. 어서 천계로 이동하죠.";

					Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_1, &_vec2(600.f, 790.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
					Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_2, &_vec2(600.f, 830.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));

				}

			}

				
		}








	}

	if ((m_eInter_Target == TARGET_SUPPLYCREW) && m_bFace)
	{
		if ((_int)m_fFrame == 0)
		{
			_tchar	m_szString_Temp_Name[128] = L"꼬마 상인";
			Engine::Render_Font(L"Font_Dialog_Name", m_szString_Temp_Name, &_vec2(790.f, 720.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			_tchar	m_szString_Temp_text_Line_1[128] = L"어서오세요! 없는 것 빼고 다 있는 아지트 떠돌이 상인입니다.";
			_tchar	m_szString_Temp_text_Line_2[128] = L"혹시 찾는 물건이 있으신가요? 울지말고 천천히 얘기해보세요.";

			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_1, &_vec2(550.f, 790.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_2, &_vec2(550.f, 830.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));

		}
		else if ((_int)m_fFrame == 1)
		{
			_tchar	m_szString_Temp_Name[128] = L"연";
			Engine::Render_Font(L"Font_Dialog_Name", m_szString_Temp_Name, &_vec2(800.f, 720.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			_tchar	m_szString_Temp_text_Line_1[128] = L"슬프고 힘들 때 용책을 찾아보라고 했어요. 혹시 용책 있나요?";
			_tchar	m_szString_Temp_text_Line_2[128] = L"         그리고 좋은 장비가 있는지 보고싶습니다.          ";

			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_1, &_vec2(550.f, 790.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_2, &_vec2(550.f, 830.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
		}
		else if ((_int)m_fFrame == 2)
		{
			_tchar	m_szString_Temp_Name[128] = L"꼬마 상인";
			Engine::Render_Font(L"Font_Dialog_Name", m_szString_Temp_Name, &_vec2(790.f, 720.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			_tchar	m_szString_Temp_text_Line_1[128] = L"     용책은 뭐하러 찾으시나요? 봐도 이해 못하실텐데..     ";
			_tchar	m_szString_Temp_text_Line_2[128] = L"아래 보이는 레버를 당기세요. 필요한 걸 찾으시길 바랍니다 !";

			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_1, &_vec2(550.f, 790.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_2, &_vec2(550.f, 830.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
		}
		else if ((_int)m_fFrame == 3)
		{
			_tchar	m_szString_Temp_Name[128] = L"연";
			Engine::Render_Font(L"Font_Dialog_Name", m_szString_Temp_Name, &_vec2(800.f, 720.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			_tchar	m_szString_Temp_text_Line_1[128] = L"눈물을 머금고 용책을 찾으러 간다. 겸사겸사 장비도 찾아본다";
			_tchar	m_szString_Temp_text_Line_2[128] = L"        DirectX 9를 이용한 3D Game 프로그래밍 입문!       ";

			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_1, &_vec2(550.f, 790.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_2, &_vec2(550.f, 830.f), D3DXCOLOR(1.f, 1.f, 0.f, 0.9f));
		}
	}

	if ((m_eInter_Target == TARGET_BLACKSMITH) && m_bFace)
	{
		if ((_int)m_fFrame == 0)
		{
			_tchar	m_szString_Temp_Name[128] = L"강화 장인";
			Engine::Render_Font(L"Font_Dialog_Name", m_szString_Temp_Name, &_vec2(790.f, 720.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			_tchar	m_szString_Temp_text_Line_1[128] = L"처음 보는 친구인데, 어디서 왔는가? 나는 쥬신에서 왔다네!  ";

			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_1, &_vec2(550.f, 790.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));

		}
		else if ((_int)m_fFrame == 1)
		{
			_tchar	m_szString_Temp_Name[128] = L"연";
			Engine::Render_Font(L"Font_Dialog_Name", m_szString_Temp_Name, &_vec2(800.f, 720.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			_tchar	m_szString_Temp_text_Line_1[128] = L"앗, 안녕하세요. 어디선가 뵌 것 같은 기분이 들긴 하지만... ";
			_tchar	m_szString_Temp_text_Line_2[128] = L"처음 들어보는 지역명이네요. 저는 이 세계의 사람이 아닙니다";

			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_1, &_vec2(550.f, 790.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_2, &_vec2(550.f, 830.f), D3DXCOLOR(1.f, 1.f, 0.f, 0.9f));

		}
		else if ((_int)m_fFrame == 2)
		{
			_tchar	m_szString_Temp_Name[128] = L"강화 장인";
			Engine::Render_Font(L"Font_Dialog_Name", m_szString_Temp_Name, &_vec2(790.f, 720.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			_tchar	m_szString_Temp_text_Line_1[128] = L"저런.. 어쩌다 여기에 왔는지는 모르겠지만, 내 가진걸 좀 줌세";
			_tchar	m_szString_Temp_text_Line_2[128] = L"자네의 원활한 쥬신 라이프에 도움이 되길 바라겠네, 아디오스!";


			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_1, &_vec2(550.f, 790.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_2, &_vec2(550.f, 830.f), D3DXCOLOR(1.f, 1.f, 0.f, 0.9f));

		}
		else if ((_int)m_fFrame == 3)
		{
			_tchar	m_szString_Temp_Name[128] = L"연";
			Engine::Render_Font(L"Font_Dialog_Name", m_szString_Temp_Name, &_vec2(800.f, 720.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

			_tchar	m_szString_Temp_text_Line_1[128] = L"대체 어떤걸 주시려고, 뭔가 주신다면 감사히 잘 써보겠습니다";

			Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_1, &_vec2(550.f, 790.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
			pPlayer->Set_Player_Money(MONEY_INCREASE, 500);

			m_bDead = true;
		}
	}

	//if ((m_eInter_Target == TARGET_REGEN_STONE) && m_bFace)
	//{
	//	if ((_int)m_fFrame == 0)
	//	{
	//		_tchar	m_szString_Temp_Name[128] = L"어두운 기운";
	//		Engine::Render_Font(L"Font_Dialog_Name", m_szString_Temp_Name, &_vec2(800.f, 720.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//		_tchar	m_szString_Temp_text_Line_1[128] = L"어서오세요! 없는 것 빼고 다 있는 아지트 떠돌이 상인입니다.";
	//		_tchar	m_szString_Temp_text_Line_2[128] = L"혹시 찾는 물건이 있으신가요? 울지말고 천천히 얘기해보세요.";

	//		Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_1, &_vec2(594.f, 790.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
	//		Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_2, &_vec2(594.f, 830.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
	//	}
	//	else if ((_int)m_fFrame == 1)
	//	{
	//		_tchar	m_szString_Temp_Name[128] = L"꼬마 상인";
	//		Engine::Render_Font(L"Font_Dialog_Name", m_szString_Temp_Name, &_vec2(800.f, 720.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	//		_tchar	m_szString_Temp_text_Line_1[128] = L"어서오세요! 없는 것 빼고 다 있는 아지트 떠돌이 상인입니다.";
	//		_tchar	m_szString_Temp_text_Line_2[128] = L"혹시 찾는 물건이 있으신가요? 울지말고 천천히 얘기해보세요.";

	//		Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_1, &_vec2(594.f, 790.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
	//		Engine::Render_Font(L"Font_Dialog_Text", m_szString_Temp_text_Line_2, &_vec2(594.f, 830.f), D3DXCOLOR(1.f, 1.f, 1.f, 0.9f));
	//	}
	//	else if ((_int)m_fFrame == 2)
	//	{
	//	}
	//	else if ((_int)m_fFrame == 3)
	//	{

	//	}
	//}
}

void CInteraction_Dialog::Alpha_Delay_Check(const _float& fTimeDelta)
{
	m_fAlphaDelay += (m_fAlphaDelay_Max * fTimeDelta * 0.5f);

	if (m_iAlpha < 255)
	{
		m_iAlpha = (_int)((255 / m_fAlphaDelay_Max) * m_fAlphaDelay);
	}
	else if (m_iAlpha > 255)
	{
		m_iAlpha = 255;
	}
}

CInteraction_Dialog* CInteraction_Dialog::Create(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _eTarget, _vec3 vPos)
{
	CInteraction_Dialog* pInstance = new CInteraction_Dialog(pGraphicDev);

	pInstance->m_eInter_Target = _eTarget;
	pInstance->m_vMasterPos = vPos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInteraction_Dialog::Free(void)
{
	
	__super::Free();
}
