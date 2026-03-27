#include "stdafx.h"
#include "..\Header\Main_Lobby_Bow.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"


CMain_Lobby_Bow::CMain_Lobby_Bow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CMain_Lobby_Bow::CMain_Lobby_Bow(const CMain_Lobby_Bow & rhs)
	: CEffect(rhs)
{
}

CMain_Lobby_Bow::~CMain_Lobby_Bow()
{
	Free();
}

HRESULT CMain_Lobby_Bow::Ready_GameObject(void)
{
	Add_Component();



	m_iAlpha = 255;
	
	m_fMaxFrame = 6.f;
	m_fFrame = 1.f;

	_vec3 Scale = { 10.f * 1.1f , 10.f * 1.1f, 1.f };
	m_pTransformCom->m_vScale = Scale * 1.f;

	m_vPos.z += 1.3f;
	m_vPos.y += 7.5f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;


	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));




	return S_OK;
}

_int CMain_Lobby_Bow::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;



	if (m_bDead)
		return OBJ_DEAD;

	m_fAccTime += 2.f * Time * 0.7f * m_fDir;

	if (m_fAccTime > 2.f) {
		m_fAccTime = 2.f;
		m_fDir *= -1;
	}

	if (m_fAccTime < 0.f) {
		m_fAccTime = 0.f;
		m_fDir *= -1;
	}

	m_pTransformCom->m_vInfo[INFO_POS].y = m_fAccTime + m_vPos.y;


	
	//m_fFrame += m_fMaxFrame * Time * 0.7f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CMain_Lobby_Bow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0.f;
	}

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CMain_Lobby_Bow::Render_GameObject()
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

	
	if (m_type == 0) {
		m_pTextureCom[0]->Set_Texture((_uint)m_fFrame);
	}
	else if (m_type == 1) {
		m_pTextureCom[1]->Set_Texture((_uint)m_fFrame);
	}
	else if (m_type == 2) {
		m_pTextureCom[2]->Set_Texture((_uint)m_fFrame);
	}
	else if (m_type == 3) {
		m_pTextureCom[3]->Set_Texture((_uint)m_fFrame);
	}
	else if (m_type == 4) {
		m_pTextureCom[4]->Set_Texture((_uint)m_fFrame);
	}
	else if (m_type == 5) {
		m_pTextureCom[5]->Set_Texture((_uint)m_fFrame);
	}

	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMain_Lobby_Bow::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });



	if (m_type == 0) {
		pComponent = m_pTextureCom[0] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MainLobby_Angel_Bow"));

		m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MainLobby_Angel_Bow", pComponent });
	}
	else if (m_type == 1) {
		pComponent = m_pTextureCom[1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MainLobby_Haetae_Bow"));

		m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MainLobby_Haetae_Bow", pComponent });
	}
	else if (m_type == 2) {
		pComponent = m_pTextureCom[2] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MainLobby_Ice_Bow"));

		m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MainLobby_Ice_Bow", pComponent });
	}
	else if (m_type == 3) {
		pComponent = m_pTextureCom[3] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MainLobby_Kaff_Bow"));

		m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MainLobby_Kaff_Bow", pComponent });
	}
	else if (m_type == 4) {
		pComponent = m_pTextureCom[4] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MainLobby_Saw_Bow"));

		m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MainLobby_Saw_Bow", pComponent });
	}
	else if (m_type == 5) {
		pComponent = m_pTextureCom[5] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MainLobby_Tiff_Bow"));

		m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MainLobby_Tiff_Bow", pComponent });
	}


	return S_OK;
}

CMain_Lobby_Bow* CMain_Lobby_Bow::Create(LPDIRECT3DDEVICE9 pGraphicDev, int type, _vec3 pos)
{
	CMain_Lobby_Bow* pInstance = new CMain_Lobby_Bow(pGraphicDev);

	pInstance->m_type = type;
	pInstance->m_vPos = pos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMain_Lobby_Bow::Free(void)
{
	__super::Free();
}


