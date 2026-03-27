#include "stdafx.h"
#include "..\Header\Main_Lobby_Tile.h"
#include "Export_Function.h"

CMain_Lobby_Tile::CMain_Lobby_Tile(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandscape(pGraphicDev)
{
}

CMain_Lobby_Tile::CMain_Lobby_Tile(const CMain_Lobby_Tile& rhs)
	: CLandscape(rhs)

{
}

CMain_Lobby_Tile::~CMain_Lobby_Tile()
{
	Free();
}

HRESULT CMain_Lobby_Tile::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 150.f * m_Scale, 100.f * m_Scale, 1.f };

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;

	
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	

	return S_OK;
}

_int CMain_Lobby_Tile::Update_GameObject(const _float& fTimeDelta)
{	
	__super::Update_GameObject(fTimeDelta);

	
	return 0;
}

void CMain_Lobby_Tile::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMain_Lobby_Tile::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CMain_Lobby_Tile::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MainLobby_Tile"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MainLobby_Tile", pComponent });

	return S_OK;
}

CMain_Lobby_Tile*CMain_Lobby_Tile::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos , _float Scale)
{
	CMain_Lobby_Tile* pInstance = new CMain_Lobby_Tile(pGraphicDev);
	
	pInstance->m_vPos = pos;
	pInstance->m_Scale = Scale;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMain_Lobby_Tile::Free(void)
{
	__super::Free();
}
