#include "stdafx.h"
#include "..\Header\Npc_SupplyCrew.h"
#include "Export_Function.h"

#include "Interaction_Button.h"
#include "Interaction_Dialog.h"


CNpc_SupplyCrew::CNpc_SupplyCrew(LPDIRECT3DDEVICE9 pGraphicDev)
	: CNpc(pGraphicDev), m_pTextureCom_1(nullptr), m_pTextureCom_2(nullptr), m_ClewState(CLEW_STATE_IDEL), m_eNpcName(NPC_END), m_bTest(false)
{
}

CNpc_SupplyCrew::CNpc_SupplyCrew(const CNpc_SupplyCrew& rhs)
	: CNpc(rhs), m_pTextureCom_1(rhs.m_pTextureCom_1), m_pTextureCom_2(rhs.m_pTextureCom_2), m_ClewState(rhs.m_ClewState), m_eNpcName(rhs.m_eNpcName), m_bTest(rhs.m_bTest)
{
}

CNpc_SupplyCrew::~CNpc_SupplyCrew()
{
	Free();
}


HRESULT CNpc_SupplyCrew::Ready_GameObject(void)
{
	Add_Component();
	m_eNpcName = NPC_SUPPLYCREW;

	m_pTransformCom->Set_Pos(422.5f, 6.f, 413.f);
	m_pTransformCom->UpdatePos_OnWorld();
	m_pTransformCom->Set_Scale(6.f, 6.f, 1.f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	return S_OK;
}

_int CNpc_SupplyCrew::Update_GameObject(const _float& fTimeDelta)
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

void CNpc_SupplyCrew::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CNpc_SupplyCrew::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	switch (m_ClewState)
	{
	case CLEW_STATE_IDEL:
		m_pTextureCom->Set_Texture((_uint)m_fFrame);
		break;
	case CLEW_STATE_PLEASURE:
		m_pTextureCom_1->Set_Texture((_uint)m_fFrame);
		break;
	case CLEW_STATE_SALUTE:
		m_pTextureCom_2->Set_Texture((_uint)m_fFrame);
		break;
	case CLEW_STATE_END:
		break;
	default:
		break;
	}

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CNpc_SupplyCrew::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	//45 CLEW_STATE_IDEL
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Npc_SupplyCrew_Idle"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Npc_SupplyCrew_Idle", pComponent });
	//45 CLEW_STATE_PLEASURE
	pComponent = m_pTextureCom_1 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Npc_SupplyCrew_Pleasure"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Npc_SupplyCrew_Pleasure", pComponent });
	//45 CLEW_STATE_SALUTE
	pComponent = m_pTextureCom_2 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Npc_SupplyCrew_Salute"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Npc_SupplyCrew_Salute", pComponent });

	//pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	//NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	//m_pColliderCom->Set_TransformCom(m_pTransformCom);
	//m_pColliderCom->Set_Radius(5.f);
	//m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CNpc_SupplyCrew::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CNpc_SupplyCrew::Change_State(void)
{

}

void CNpc_SupplyCrew::Frame_Check(const _float& fTimeDelta)
{
	if (m_ClewState == CLEW_STATE_IDEL)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bCheck = false;
		}
	}

	if (m_ClewState == CLEW_STATE_PLEASURE)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_ClewState = CLEW_STATE_IDEL;
			m_bCheck = false;
		}
	}

	if (m_ClewState == CLEW_STATE_SALUTE)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_ClewState = CLEW_STATE_IDEL;
			m_bCheck = true;
		}
	}
}


CNpc_SupplyCrew* CNpc_SupplyCrew::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc_SupplyCrew* pInstance = new CNpc_SupplyCrew(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNpc_SupplyCrew::Create_Active_Button_Effect(void)
{
	//TIME_STOP_VOID

	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;
	pGameObject = CInteraction_Button::Create(m_pGraphicDev, TARGET_SUPPLYCREW, m_pTransformCom->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

	pGameObject = CInteraction_Dialog::Create(m_pGraphicDev, TARGET_SUPPLYCREW, m_pTransformCom->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);

}

void CNpc_SupplyCrew::Free(void)
{
	__super::Free();
}