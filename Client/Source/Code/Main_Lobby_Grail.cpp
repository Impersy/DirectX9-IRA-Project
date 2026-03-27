#include "stdafx.h"
#include "..\Header\Main_Lobby_Grail.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"


CMain_Lobby_Grail::CMain_Lobby_Grail(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CMain_Lobby_Grail::CMain_Lobby_Grail(const CMain_Lobby_Grail & rhs)
	: CEffect(rhs)
{
}

CMain_Lobby_Grail::~CMain_Lobby_Grail()
{
	Free();
}

HRESULT CMain_Lobby_Grail::Ready_GameObject(void)
{
	Add_Component();



	m_iAlpha = 255;


	_vec3 Scale = { 2.f * 1.1f , 2.7f * 1.1f, 1.f };
	m_pTransformCom->m_vScale = Scale * 3.f;

	m_vPos.y += 1.3f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));


	return S_OK;
}

_int CMain_Lobby_Grail::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;



	if (m_bDead)
		return OBJ_DEAD;




	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CMain_Lobby_Grail::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CMain_Lobby_Grail::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_type);
	

	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMain_Lobby_Grail::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MainLobby_Grail"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MainLobby_Grail", pComponent });
	
	



	return S_OK;
}

CMain_Lobby_Grail* CMain_Lobby_Grail::Create(LPDIRECT3DDEVICE9 pGraphicDev, int type , _vec3 pos)
{
	CMain_Lobby_Grail* pInstance = new CMain_Lobby_Grail(pGraphicDev);

	pInstance->m_type = type;
	pInstance->m_vPos = pos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMain_Lobby_Grail::Free(void)
{
	__super::Free();
}


