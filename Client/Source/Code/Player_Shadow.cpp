#include "stdafx.h"
#include "..\Header\Player_Shadow.h"
#include "Export_Function.h"
#include "GameObject.h"
#include "Player.h"

CPlayer_Shadow::CPlayer_Shadow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)

{
}

CPlayer_Shadow::CPlayer_Shadow(const CPlayer_Shadow & rhs)
	: CGameObject(rhs.m_pGraphicDev)
{
}

CPlayer_Shadow::~CPlayer_Shadow()
{
	Free();
}

HRESULT CPlayer_Shadow::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 3.5f * PUBLIC_SCALE  , 3.5f * PUBLIC_SCALE  , 1.f };

	_vec3	vRight;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CPlayer_Shadow::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;


	CTransform* pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = { pTransformCom->m_vInfo[INFO_POS].x + 0.1f, 0.0065f, pTransformCom->m_vInfo[INFO_POS].z - 1.f };


	Engine::Add_RenderGroup(RENDER_ALPHA, this);


	__super::Update_GameObject(Time);

	return 0;
}

void CPlayer_Shadow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CPlayer_Shadow::Render_GameObject()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	if (pPlayer->m_Is_Render == false)
		return;


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer_Shadow::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Shadow"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Shadow", pComponent });

	return S_OK;
}

CPlayer_Shadow * CPlayer_Shadow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_Shadow * pInstance = new CPlayer_Shadow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Shadow::Free(void)
{
	__super::Free();
}


