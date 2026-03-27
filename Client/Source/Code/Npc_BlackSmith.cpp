#include "stdafx.h"
#include "..\Header\Npc_BlackSmith.h"
#include "Export_Function.h"

#include "Interaction_Button.h"
#include "Interaction_Dialog.h"


CNpc_BlackSmith::CNpc_BlackSmith(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev), m_pTextureCom_1(nullptr), m_pTextureCom_2(nullptr), m_SMITHState(SMITH_STATE_IDEL), m_eNpcName(NPC_BLACKSMITH), m_bTest(false)
{
}

CNpc_BlackSmith::CNpc_BlackSmith(const CNpc_BlackSmith& rhs)
	: CNpc(rhs), m_pTextureCom_1(rhs.m_pTextureCom_1), m_pTextureCom_2(rhs.m_pTextureCom_2), m_SMITHState(rhs.m_SMITHState), m_eNpcName(rhs.m_eNpcName), m_bTest(rhs.m_bTest)
{
}

CNpc_BlackSmith::~CNpc_BlackSmith()
{
	Free();
}


HRESULT CNpc_BlackSmith::Ready_GameObject(void)
{
	Add_Component();
	m_eNpcName = NPC_BLACKSMITH;

	m_pTransformCom->Set_Pos(338.f, 7.f, 411.f);
	m_pTransformCom->UpdatePos_OnWorld();
	m_pTransformCom->Set_Scale(8.f, 8.f, 1.f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	return S_OK;
}

_int CNpc_BlackSmith::Update_GameObject(const _float& fTimeDelta)
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

void CNpc_BlackSmith::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CNpc_BlackSmith::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	switch (m_SMITHState)
	{
	case SMITH_STATE_IDEL:
		m_pTextureCom->Set_Texture((_uint)m_fFrame);
		break;
	case SMITH_STATE_END:
		break;
	default:
		break;
	}

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CNpc_BlackSmith::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	//45 SMITH_STATE_IDEL
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Npc_BlackSmith_Idle"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Npc_BlackSmith_Idle", pComponent });

	return S_OK;
}

void CNpc_BlackSmith::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CNpc_BlackSmith::Change_State(void)
{

}

void CNpc_BlackSmith::Frame_Check(const _float& fTimeDelta)
{
	if (m_SMITHState == SMITH_STATE_IDEL)
	{
		m_fFrame += 4.f * fTimeDelta;

		if (4.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bCheck = false;
		}
	}
}


CNpc_BlackSmith* CNpc_BlackSmith::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_BlackSmith* pInstance = new CNpc_BlackSmith(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNpc_BlackSmith::Create_Active_Button_Effect(void)
{
	//TIME_STOP_VOID

	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;
	pGameObject = CInteraction_Button::Create(m_pGraphicDev, TARGET_BLACKSMITH, m_pTransformCom->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

	pGameObject = CInteraction_Dialog::Create(m_pGraphicDev, TARGET_BLACKSMITH, m_pTransformCom->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

}

void CNpc_BlackSmith::Free(void)
{
	__super::Free();
}