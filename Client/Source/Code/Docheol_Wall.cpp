#include "stdafx.h"
#include "..\Header\Docheol_Wall.h"
#include "Export_Function.h"

CDocheol_Wall::CDocheol_Wall(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandscape(pGraphicDev)
{
}

CDocheol_Wall::CDocheol_Wall(const CDocheol_Wall& rhs)
	: CLandscape(rhs)

{
}

CDocheol_Wall::~CDocheol_Wall()
{
}

HRESULT CDocheol_Wall::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 150.f , 150.f, 1.f };

	m_pTransformCom->m_vInfo[INFO_POS] = { 128.f , 0.1f , 128.f };

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CDocheol_Wall::Update_GameObject(const _float& fTimeDelta)
{	
	CGameObject::Update_GameObject(fTimeDelta);

	

	return 0;
}

void CDocheol_Wall::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDocheol_Wall::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDocheol_Wall::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Object_DocheolWall"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Object_DocheolWall", pComponent });

	return S_OK;
}

CDocheol_Wall* CDocheol_Wall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDocheol_Wall* pInstance = new CDocheol_Wall(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDocheol_Wall::Free(void)
{
	__super::Free();
}
