#include "stdafx.h"
#include "..\Header\Npc_ApostleNpc_Time.h"
#include "Export_Function.h"

#include "Interaction_Button.h"
#include "Interaction_Dialog.h"
#include <KeyMgr.h>
#include <Black.h>

CNpc_ApostleNpc_Time::CNpc_ApostleNpc_Time(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev), m_pTextureCom_1(nullptr), m_eNpcName(NPC_APOSTLE_TIME), m_TimeState(TIME_STATE_IDEL), m_bTest(false)
{
}

CNpc_ApostleNpc_Time::CNpc_ApostleNpc_Time(const CNpc_ApostleNpc_Time& rhs)
	: CNpc(rhs), m_pTextureCom_1(rhs.m_pTextureCom_1), m_eNpcName(rhs.m_eNpcName), m_TimeState(rhs.m_TimeState), m_bTest(rhs.m_bTest)
{
}

CNpc_ApostleNpc_Time::~CNpc_ApostleNpc_Time()
{
	Free();
}


HRESULT CNpc_ApostleNpc_Time::Ready_GameObject(void)
{
	Add_Component();
	m_eNpcName = NPC_APOSTLE_TIME;

	if (g_eCurScene == SCENE_MAINLOBBY) {
		m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;
		m_TimeState = TIME_STATE_STAND;
	}
	else {
		m_pTransformCom->Set_Pos(412.f, 8.f, 352.f);
	}

	if (g_Boss1_Clear == true && g_eCurScene == SCENE_BOSS1) {
		m_pTransformCom->Set_Pos(384.f, 40.f, 384.f);
	}
	else if (g_Game_Clear == true && g_eCurScene == SCENE_BOSS2) {
		m_pTransformCom->Set_Pos(128.f, 40.f, 128.f);
	}


	
	m_pTransformCom->UpdatePos_OnWorld();
	m_pTransformCom->Set_Scale(8.f, 8.f, 1.f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	

	return S_OK;
}

_int CNpc_ApostleNpc_Time::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;


	m_pTransformCom->m_vInfo[INFO_POS].y -= 4.5f * fTimeDelta * 2.f;

	m_pTransformCom->UpdatePos_OnWorld();


	if (m_pTransformCom->m_vInfo[INFO_POS].y < 8.f) {
		m_pTransformCom->m_vInfo[INFO_POS].y = 8.f;
		m_pTransformCom->UpdatePos_OnWorld();
	}

	if (g_Boss1_Clear == true && g_eCurScene == SCENE_BOSS1) {
		
		// 도철
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_H))
		{
			CBlack* pBlack = dynamic_cast<CBlack*>(Engine::Get_GameObject(L"Layer_UI", L"Black"));
			
			pBlack->Scene_Change(SCENE_BOSS2, { 76.f,5.f * PUBLIC_SCALE,60.f });

		}

	}
	else if (g_Game_Clear == true && g_eCurScene == SCENE_BOSS2) {
		
		// 메인로비
		if (CKeyMgr::Get_Instance()->Key_Down(KEY_H))
		{
			CBlack* pBlack = dynamic_cast<CBlack*>(Engine::Get_GameObject(L"Layer_UI", L"Black"));
			
			pBlack->Scene_Change(SCENE_MAINLOBBY, MAINLOBBY_TELEPORT);

		}



	}


	if (!m_bTest)
	{
		
		Create_Active_Button_Effect();

		m_bTest = true;
	}

	Frame_Check(fTimeDelta);
	SetUp_OnTerrain();
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return OBJ_NOEVENT;
}

void CNpc_ApostleNpc_Time::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CNpc_ApostleNpc_Time::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	switch (m_TimeState)
	{
	case CNpc_ApostleNpc_Time::TIME_STATE_IDEL:
		m_pTextureCom->Set_Texture((_uint)m_fFrame);
		break;
	case CNpc_ApostleNpc_Time::TIME_STATE_STAND:
		m_pTextureCom_1->Set_Texture((_uint)m_fFrame);
		break;
	case CNpc_ApostleNpc_Time::TIME_STATE_END:
		break;
	default:
		break;
	}

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CNpc_ApostleNpc_Time::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	//Spr_Npc_ApostleOfTime_Undressing_Idle_00
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Npc_ApostleOfTime_Undressing_Idle"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Npc_ApostleOfTime_Undressing_Idle", pComponent });
	//Spr_Npc_ApostleOfTime_Stand_Idle_00
	pComponent = m_pTextureCom_1 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Npc_ApostleOfTime_Stand_Idle"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Npc_ApostleOfTime_Stand_Idle", pComponent });
	//
	return S_OK;
}

void CNpc_ApostleNpc_Time::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CNpc_ApostleNpc_Time::Change_State(void)
{

}

void CNpc_ApostleNpc_Time::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += 7.f * fTimeDelta;

	if (7.f < m_fFrame)
	{
		m_fFrame = 0.f;
		m_bCheck = false;
	}
}


CNpc_ApostleNpc_Time* CNpc_ApostleNpc_Time::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_ApostleNpc_Time* pInstance = new CNpc_ApostleNpc_Time(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CNpc_ApostleNpc_Time* CNpc_ApostleNpc_Time::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos)
{
	CNpc_ApostleNpc_Time* pInstance = new CNpc_ApostleNpc_Time(pGraphicDev);

	pInstance->m_vPos = pos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNpc_ApostleNpc_Time::Create_Active_Button_Effect(void)
{
	//TIME_STOP_VOID

	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;
	pGameObject = CInteraction_Button::Create(m_pGraphicDev, TARGET_APOSTLE_TIME, m_pTransformCom->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

	pGameObject = CInteraction_Dialog::Create(m_pGraphicDev, TARGET_APOSTLE_TIME, m_pTransformCom->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

}

void CNpc_ApostleNpc_Time::Free(void)
{
	__super::Free();
}