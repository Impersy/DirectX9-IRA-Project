#include "stdafx.h"
#include "..\Header\Docheol_Tile.h"
#include "Export_Function.h"

CDocheol_Tile::CDocheol_Tile(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandscape(pGraphicDev)
{
}

CDocheol_Tile::CDocheol_Tile(const CDocheol_Tile& rhs)
	: CLandscape(rhs)

{
}

CDocheol_Tile::~CDocheol_Tile()
{
}

HRESULT CDocheol_Tile::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 150.f , 150.f , 1.f };

	m_pTransformCom->m_vInfo[INFO_POS] = { 128.f , 0.1f  , 128.f };

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CDocheol_Tile::Update_GameObject(const _float& fTimeDelta)
{	
	__super::Update_GameObject(fTimeDelta);

	

	return 0;
}

void CDocheol_Tile::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDocheol_Tile::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CDocheol_Tile::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Object_DocheolTile"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Object_DocheolTile", pComponent });

	return S_OK;
}

CDocheol_Tile* CDocheol_Tile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDocheol_Tile* pInstance = new CDocheol_Tile(pGraphicDev);



	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDocheol_Tile::Free(void)
{
	__super::Free();
}
