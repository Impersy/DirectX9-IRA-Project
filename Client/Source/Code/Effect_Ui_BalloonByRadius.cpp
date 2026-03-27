#include "stdafx.h"
#include "..\Header\Effect_Ui_BalloonByRadius.h"
#include "Export_Function.h"

CEffect_Ui_BalloonByRadius::CEffect_Ui_BalloonByRadius(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Ui_BalloonByRadius::CEffect_Ui_BalloonByRadius(const CEffect_Ui_BalloonByRadius& rhs)
	: CEffect(rhs)
{
}

CEffect_Ui_BalloonByRadius::~CEffect_Ui_BalloonByRadius()
{
	Free();
}

HRESULT CEffect_Ui_BalloonByRadius::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->Set_Pos(m_vMasterPos.x, m_vMasterPos.y + 14.f, m_vMasterPos.z);
	m_pTransformCom->UpdatePos_OnWorld();
	m_pTransformCom->Set_Scale(6.f, 6.f, 1.f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	m_fMaxFrame = 20.f;

	return S_OK;
}

_int CEffect_Ui_BalloonByRadius::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += (m_fMaxFrame * fTimeDelta * 0.5f);

	TargetCheck_BalloonNum();

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return OBJ_NOEVENT;
}

void CEffect_Ui_BalloonByRadius::LateUpdate_GameObject()
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

void CEffect_Ui_BalloonByRadius::Render_GameObject()
{	
	if (m_eInter_Target == TARGET_APOSTLE_TIME)
		return;


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	if (m_eInter_Target != TARGET_END)
	{	
		m_pTextureCom->Set_Texture((_int)m_BalloonNum);
	}
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEffect_Ui_BalloonByRadius::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CEffect_Ui_BalloonByRadius::TargetCheck_BalloonNum(void)
{
	if (m_eInter_Target == TARGET_APOSTLE_COURAGE)
	{
		m_BalloonNum = 0;
	}
	else if (m_eInter_Target == TARGET_APOSTLE_NIGHT)
	{
		m_BalloonNum = 1;
	}
	else if (m_eInter_Target == TARGET_APOSTLE_TIME)
	{
		m_BalloonNum = 2;
	}
	else if (m_eInter_Target == TARGET_BLACKSMITH)
	{
		m_BalloonNum = 3;
	}
	else if (m_eInter_Target == TARGET_SUPPLYCREW)
	{
		m_BalloonNum = 4;
	}
	else if (m_eInter_Target == TARGET_RABBIT_BLUE)
	{
		m_BalloonNum = 5;
	}
	else if (m_eInter_Target == TARGET_RABBIT_PINK)
	{
		m_BalloonNum = 6;
	}

}

CEffect_Ui_BalloonByRadius* CEffect_Ui_BalloonByRadius::Create(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _Target, _vec3 vPos)
{
	CEffect_Ui_BalloonByRadius* pInstance = new CEffect_Ui_BalloonByRadius(pGraphicDev);

	pInstance->m_eInter_Target = _Target;
	pInstance->m_vMasterPos = vPos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

HRESULT CEffect_Ui_BalloonByRadius::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Ui_BalloonByRadius"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Ui_BalloonByRadius", pComponent });

	return S_OK;
}

void CEffect_Ui_BalloonByRadius::Free(void)
{
	__super::Free();
}



