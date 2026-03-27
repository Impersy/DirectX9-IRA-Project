#include "stdafx.h"
#include "..\Header\SkyBox.h"
#include "Export_Function.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

CSkyBox::CSkyBox(const CSkyBox & rhs)
	: Engine::CGameObject(rhs)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pTransformCom(rhs.m_pTransformCom)
{
}

CSkyBox::~CSkyBox()
{
}

HRESULT CSkyBox::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 40.f,40.f, 40.f };
	m_pTransformCom->m_vInfo[INFO_POS] = { 0.f , 0.f , 0.f };
	
	return S_OK;
}

_int CSkyBox::Update_GameObject(const _float& fTimeDelta)
{
	
	_matrix matCamWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, 0, &matCamWorld);

	m_pTransformCom->Set_Pos(matCamWorld._41, matCamWorld._42 - 10.f, matCamWorld._43);
	
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CSkyBox::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CSkyBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CSkyBox::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_ProtoComponent(L"Proto_CubeTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_CubeTex", pComponent });

 	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
 	 
 	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
 

	if (g_eCurScene == SCENE_MAINLOBBY) {
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_SkyBox_MainLobby"));

		m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_SkyBox_MainLobby", pComponent });
	}
	else {
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_SkyBox_Docheol"));

		m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_SkyBox_Docheol", pComponent });

	
	}
 

	return S_OK;
}

CSkyBox * CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox * pInstance = new CSkyBox(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSkyBox::Free(void)
{
	__super::Free();
}
