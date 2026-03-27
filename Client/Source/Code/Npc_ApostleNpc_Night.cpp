#include "stdafx.h"
#include "..\Header\Npc_ApostleNpc_Night.h"
#include "Export_Function.h"

#include "Interaction_Button.h"

CNpc_ApostleNpc_Night::CNpc_ApostleNpc_Night(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev), m_pTextureCom_1(nullptr), m_pTextureCom_2(nullptr), m_eNpcName(NPC_APOSTLE_NIGHT), m_bTest(false)
{
}

CNpc_ApostleNpc_Night::CNpc_ApostleNpc_Night(const CNpc_ApostleNpc_Night& rhs)
	: CNpc(rhs), m_pTextureCom_1(rhs.m_pTextureCom_1), m_pTextureCom_2(rhs.m_pTextureCom_2), m_eNpcName(rhs.m_eNpcName), m_bTest(rhs.m_bTest)
{
}

CNpc_ApostleNpc_Night::~CNpc_ApostleNpc_Night()
{
	Free();
}


HRESULT CNpc_ApostleNpc_Night::Ready_GameObject(void)
{
	Add_Component();
	m_eNpcName = NPC_APOSTLE_NIGHT;

	m_pTransformCom->Set_Pos(496.f, 8.f, 310.f);
	m_pTransformCom->UpdatePos_OnWorld();
	m_pTransformCom->Set_Scale(8.f, 8.f, 1.f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	return S_OK;
}

_int CNpc_ApostleNpc_Night::Update_GameObject(const _float& fTimeDelta)
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

void CNpc_ApostleNpc_Night::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CNpc_ApostleNpc_Night::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CNpc_ApostleNpc_Night::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	//45 SMITH_STATE_IDEL
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Npc_ApostleNpc_Night_Idle"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Npc_ApostleNpc_Night_Idle", pComponent });

	return S_OK;
}

void CNpc_ApostleNpc_Night::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CNpc_ApostleNpc_Night::Change_State(void)
{

}

void CNpc_ApostleNpc_Night::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += 7.f * fTimeDelta;

	if (7.f < m_fFrame)
	{
		m_fFrame = 0.f;
		m_bCheck = false;
	}
}


CNpc_ApostleNpc_Night* CNpc_ApostleNpc_Night::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_ApostleNpc_Night* pInstance = new CNpc_ApostleNpc_Night(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNpc_ApostleNpc_Night::Create_Active_Button_Effect(void)
{
	//TIME_STOP_VOID

	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;
	pGameObject = CInteraction_Button::Create(m_pGraphicDev, TARGET_APOSTLE_NIGHT, m_pTransformCom->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

}

void CNpc_ApostleNpc_Night::Free(void)
{
	__super::Free();
}