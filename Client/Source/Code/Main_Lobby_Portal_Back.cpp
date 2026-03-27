#include "stdafx.h"
#include "..\Header\Main_Lobby_Portal_Back.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"


CMain_Lobby_Portal_Back::CMain_Lobby_Portal_Back(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CMain_Lobby_Portal_Back::CMain_Lobby_Portal_Back(const CMain_Lobby_Portal_Back & rhs)
	: CEffect(rhs)
{
}

CMain_Lobby_Portal_Back::~CMain_Lobby_Portal_Back()
{
	Free();
}

HRESULT CMain_Lobby_Portal_Back::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 7.69f * 1.2f , 5.3f * 1.4f , 1.f };
	m_pTransformCom->m_vScale = Scale * 3.f;

	m_pTransformCom->Set_Pos(382.f, 22.f , 464.f);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));


	return S_OK;
}

_int CMain_Lobby_Portal_Back::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;



	if (m_bDead)
		return OBJ_DEAD;



	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CMain_Lobby_Portal_Back::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CMain_Lobby_Portal_Back::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);
	
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMain_Lobby_Portal_Back::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MainLobby_Portal_Back"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MainLobby_Portal_Back", pComponent });



	return S_OK;
}

CMain_Lobby_Portal_Back* CMain_Lobby_Portal_Back::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMain_Lobby_Portal_Back* pInstance = new CMain_Lobby_Portal_Back(pGraphicDev);

	
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMain_Lobby_Portal_Back::Free(void)
{
	__super::Free();
}


