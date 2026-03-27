#include "stdafx.h"
#include "..\Header\Npc_NonePrint.h"
#include "Export_Function.h"

#include "Interaction_Button.h"
#include "Interaction_Dialog.h"

#include "KeyMgr.h"


CNpc_NonePrint::CNpc_NonePrint(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev), m_StoneState(STONE_STATE_IDEL), m_eNpcName(NPC_END), m_bTest(false)
{
}

CNpc_NonePrint::CNpc_NonePrint(const CNpc_NonePrint& rhs)
	: CNpc(rhs), m_StoneState(rhs.m_StoneState), m_eNpcName(rhs.m_eNpcName), m_bTest(rhs.m_bTest)
{
}

CNpc_NonePrint::~CNpc_NonePrint()
{
	//Free();
}


HRESULT CNpc_NonePrint::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->Set_Pos(388.75f, 7.9f, 387.96f);
	m_pTransformCom->UpdatePos_OnWorld();
	m_pTransformCom->Set_Scale(8.f, 8.f, 1.f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	return S_OK;
}

_int CNpc_NonePrint::Update_GameObject(const _float& fTimeDelta)
{	
	Key_Input_Check();

	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bTest)
	{	
		if (m_eNpcName == NPC_POT)
		{	
			m_pTransformCom->Set_Pos(391.59f, 6.3f, 378.15f);
			m_pTransformCom->UpdatePos_OnWorld();
			Create_Active_Button_Only();
		}
		if (m_eNpcName == NPC_REGEN_STONE)
		{
			Create_Active_Button_Dialog();
		}
		m_bTest = true;
	}

	SetUp_OnTerrain();

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return OBJ_NOEVENT;
}

void CNpc_NonePrint::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CNpc_NonePrint::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	////m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//
	////m_pTextureCom->Set_Texture((_uint)m_fFrame);
	//m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CNpc_NonePrint::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	//생성 확인용 텍스쳐
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Npc_ApostleNpc_Night_Idle"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Npc_ApostleNpc_Night_Idle", pComponent });

	return S_OK;
}

void CNpc_NonePrint::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CNpc_NonePrint::Change_State(void)
{

}

void CNpc_NonePrint::Frame_Check(const _float& fTimeDelta)
{

}


CNpc_NonePrint* CNpc_NonePrint::Create(LPDIRECT3DDEVICE9 pGraphicDev, NPC_NAME _name)
{
	CNpc_NonePrint* pInstance = new CNpc_NonePrint(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->m_eNpcName = _name;

	return pInstance;
}

void CNpc_NonePrint::Create_Active_Button_Dialog(void)
{
	TIME_STOP_VOID

	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;
	pGameObject = CInteraction_Button::Create(m_pGraphicDev, TARGET_REGEN_STONE, m_pTransformCom->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

	//pGameObject = CInteraction_Dialog::Create(m_pGraphicDev, TARGET_REGEN_STONE, m_pTransformCom->m_vInfo[INFO_POS]);
	//if (pGameObject == nullptr)
	//	return;
	//pLayer->Add_BulletObject(pGameObject);

}

void CNpc_NonePrint::Create_Active_Button_Only(void)
{
	TIME_STOP_VOID

	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;
	pGameObject = CInteraction_Button::Create(m_pGraphicDev, TARGET_POT, m_pTransformCom->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);
}

void CNpc_NonePrint::Key_Input_Check(void)
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(KEY_E))
	{
		m_bDead = true;
	}

}

void CNpc_NonePrint::Free(void)
{
	__super::Free();
}