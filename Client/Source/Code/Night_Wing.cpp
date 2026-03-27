#include "stdafx.h"
#include "..\Header\Night_Wing.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"


CNight_Wing::CNight_Wing(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CNight_Wing::CNight_Wing(const CNight_Wing & rhs)
	: CEffect(rhs)
{
}

CNight_Wing::~CNight_Wing()
{
	Free();
}

HRESULT CNight_Wing::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 3.0f * 5.f , 3.0f * 5.f, 1.f };

	m_iAlpha = 255;
	m_pTransformCom->m_vScale = Scale * PUBLIC_SCALE;

	m_fFrame = 0.f;
	m_fMaxFrame = 20.f;
	
	m_vPos.x -= 3.f;
	m_vPos.y += 10.f - 3.f;
	m_vPos.z += 8.f;

	m_vPos.z += 2.f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;


	return S_OK;
}

_int CNight_Wing::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += m_fMaxFrame * Time * 0.8f;



	m_fAccTime += 4.f * Time * 0.2f;

	if (m_fAccTime > 4.f) {
		m_iAlpha -= 7;
	}

	if (m_iAlpha < 0) {
		return OBJ_DEAD;
	}


	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CNight_Wing::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 20.f;
	}

	


	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CNight_Wing::Render_GameObject()
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

HRESULT CNight_Wing::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Night_Wing"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Night_Wing", pComponent });



	return S_OK;
}

CNight_Wing* CNight_Wing::Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 pos)
{
	CNight_Wing* pInstance = new CNight_Wing(pGraphicDev);

	pInstance->m_vPos = pos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNight_Wing::Free(void)
{
	__super::Free();
}


