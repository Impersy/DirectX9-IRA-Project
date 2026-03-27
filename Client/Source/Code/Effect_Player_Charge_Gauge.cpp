#include "stdafx.h"
#include "..\Header\Effect_Player_Charge_Gauge.h"
#include "Export_Function.h"
#include "Player.h"
#include "SylphBow.h"

CEffect_Player_Charge_Gauge::CEffect_Player_Charge_Gauge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Player_Charge_Gauge::CEffect_Player_Charge_Gauge(const CEffect_Player_Charge_Gauge& rhs)
	: CEffect(rhs)
{
}

CEffect_Player_Charge_Gauge::~CEffect_Player_Charge_Gauge()
{
	Free();
}

HRESULT CEffect_Player_Charge_Gauge::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 3.f , 3.f , 1.f };

	m_pTransformCom->m_vScale = Scale * PUBLIC_SCALE * 0.38f * m_fSize;

	m_fFrame = 0.f;
	m_fMaxFrame = 0.f;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CEffect_Player_Charge_Gauge::Update_GameObject(const _float& fTimeDelta)
{


	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;



	CTransform* pPlayerCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = { pPlayerCom->m_vInfo[INFO_POS].x , 0.012f, pPlayerCom->m_vInfo[INFO_POS].z + 0.5f };




	m_iAlpha += 1.f;

	if (m_iAlpha > 255.f)
	{
		m_iAlpha = 255;
	}


	m_fFrame += m_fMaxFrame * Time * 1.5f;
	m_fSize += Time * 5.f;

	if (m_fFrame > m_fMaxFrame)
	{
		m_iAlpha = 255;
		m_fFrame = 0.f;
	}

	if (m_fSize > 8.f) {
		m_fSize = 8.f;
	}

	

	_vec3 Scale = { 1.f , 1.f , 1.f };
	m_pTransformCom->m_vScale = Scale * PUBLIC_SCALE * 0.38f * m_fSize;
	


	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CEffect_Player_Charge_Gauge::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CEffect_Player_Charge_Gauge::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((int)0, 255, 255, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CEffect_Player_Charge_Gauge::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Effect_BowCharge_Gauge_Sylph"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Effect_BowCharge_Gauge_Sylph", pComponent });

	return S_OK;
}

CEffect_Player_Charge_Gauge* CEffect_Player_Charge_Gauge::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Player_Charge_Gauge* pInstance = new CEffect_Player_Charge_Gauge(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Player_Charge_Gauge::Free(void)
{
	__super::Free();
}


