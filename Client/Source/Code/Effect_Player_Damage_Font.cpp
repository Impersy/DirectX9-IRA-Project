#include "stdafx.h"
#include "..\Header\Effect_Player_Damage_Font.h"
#include "Export_Function.h"

CEffect_Player_Damage_Font::CEffect_Player_Damage_Font(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Player_Damage_Font::CEffect_Player_Damage_Font(const CEffect_Player_Damage_Font & rhs)
	: CEffect(rhs)
{
}

CEffect_Player_Damage_Font::~CEffect_Player_Damage_Font()
{
	Free();
}

HRESULT CEffect_Player_Damage_Font::Ready_GameObject(void)
{
	Add_Component();

	if (m_Is_Fire == true) {

		r = 205;
		g = 80;
		b = 60;
	}

	if (m_Is_Tick_Fire == true) {

		r = 235;
		g = 80;
		b = 60;
	}


	if (g_eCurScene == SCENE_BOSS1 && m_Is_Fire == false) {
		m_vPos.y += 5.f;
	}
	else if (g_eCurScene == SCENE_BOSS2 && m_Is_Fire == false) {
		m_vPos.y += 10.f;
	
	}
	
	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;

	m_vOriginPos = m_vPos;


	

	

	return S_OK;
}

_int CEffect_Player_Damage_Font::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_Is_Fire == true) {
		r = 255;
		g = 50;
		b = 50;
	}

	if (m_Is_Tick_Fire == true) {

		r = 235;
		g = 80;
		b = 60;

		m_fAccTime_Max = 0.5f;
	}

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	
	Movement(Time);

	m_fAccTime += 2.f * Time;

	if (m_fAccTime > m_fAccTime_Max) {

		m_iAlpha -= 7;

		if (m_iAlpha < 0) {
			m_iAlpha = 0;
		}

		if (m_iAlpha == 0) {
			m_bDead = true;
		}
	}


	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	__super::Update_GameObject(Time);

	return 0;
}

void CEffect_Player_Damage_Font::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);

}

void CEffect_Player_Damage_Font::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, r, g, b));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_Is_Critical) {
		m_pTransformCom->m_vScale = { 2.f,2.f,1.f };
		m_pTransformCom->m_vScale *= m_Size;
		m_pTextureCom->Set_Texture((_uint)m_fFrame);
	}
	else {
		m_pTransformCom->m_vScale = { 1.3f,1.3f,1.3f };
		m_pTransformCom->m_vScale *= m_Size;
		m_pTextureCom_Sub->Set_Texture((_uint)m_fFrame);
	}



	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);



}

HRESULT CEffect_Player_Damage_Font::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Damage_Font_Critical"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Damage_Font_Critical", pComponent });

	pComponent = m_pTextureCom_Sub = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Damage_Font_Common"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Damage_Font_Common", pComponent });

	return S_OK;
}

void CEffect_Player_Damage_Font::Movement(const _float& fTimeDelta)
{
	
	_vec3 vDir = { 0.f,1.f,0.f };

	m_pTransformCom->m_vInfo[INFO_POS] += vDir * fTimeDelta * 7.f;

}

CEffect_Player_Damage_Font* CEffect_Player_Damage_Font::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, int num, bool Critical)
{
	CEffect_Player_Damage_Font* pInstance = new CEffect_Player_Damage_Font(pGraphicDev);

	pInstance->m_vPos = vPos;

	pInstance->m_fFrame = num;

	pInstance->m_Is_Critical = Critical;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Player_Damage_Font::Free(void)
{
	__super::Free();
}


