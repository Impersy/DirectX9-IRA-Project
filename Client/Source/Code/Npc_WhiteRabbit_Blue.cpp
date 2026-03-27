#include "stdafx.h"
#include "..\Header\Npc_WhiteRabbit_Blue.h"
#include "Export_Function.h"

#include "Interaction_Button.h"
#include "Interaction_Dialog.h"


CNpc_WhiteRabbit_Blue::CNpc_WhiteRabbit_Blue(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev),m_eNpcName(NPC_RABBIT_BLUE), m_bTest(false)
{
}

CNpc_WhiteRabbit_Blue::CNpc_WhiteRabbit_Blue(const CNpc_WhiteRabbit_Blue& rhs)
	: CNpc(rhs), m_eNpcName(rhs.m_eNpcName), m_bTest(rhs.m_bTest)
{
}

CNpc_WhiteRabbit_Blue::~CNpc_WhiteRabbit_Blue()
{
	Free();
}


HRESULT CNpc_WhiteRabbit_Blue::Ready_GameObject(void)
{
	Add_Component();
	m_eNpcName = NPC_RABBIT_BLUE;

	m_pTransformCom->Set_Pos(283.f, 7.f, 367.f);
	m_pTransformCom->UpdatePos_OnWorld();
	m_pTransformCom->Set_Scale(7.f, 7.f, 1.f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	return S_OK;
}

_int CNpc_WhiteRabbit_Blue::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bTest)
	{
		Create_Active_Button_Effect();
		m_bTest = true;
	}

	Frame_Check(fTimeDelta);
	SetUp_OnTerrain();
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return OBJ_NOEVENT;
}

void CNpc_WhiteRabbit_Blue::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CNpc_WhiteRabbit_Blue::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CNpc_WhiteRabbit_Blue::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Npc_WhiteRabbit_Blue"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Npc_WhiteRabbit_Blue", pComponent });

	return S_OK;
}

void CNpc_WhiteRabbit_Blue::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CNpc_WhiteRabbit_Blue::Change_State(void)
{

}

void CNpc_WhiteRabbit_Blue::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += 7.f * fTimeDelta;

	if (7.f < m_fFrame)
	{
		m_fFrame = 0.f;
		m_bCheck = false;
	}
}


CNpc_WhiteRabbit_Blue* CNpc_WhiteRabbit_Blue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_WhiteRabbit_Blue* pInstance = new CNpc_WhiteRabbit_Blue(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNpc_WhiteRabbit_Blue::Create_Active_Button_Effect(void)
{
	//TIME_STOP_VOID

	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;
	pGameObject = CInteraction_Button::Create(m_pGraphicDev, TARGET_RABBIT_BLUE, m_pTransformCom->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

}

void CNpc_WhiteRabbit_Blue::Free(void)
{
	__super::Free();
}