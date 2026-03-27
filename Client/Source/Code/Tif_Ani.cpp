#include "stdafx.h"
#include "..\Header\Tif_Ani.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"


CTif_Ani::CTif_Ani(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CTif_Ani::CTif_Ani(const CTif_Ani& rhs)
	: CEffect(rhs)
{
}

CTif_Ani::~CTif_Ani()
{
	Free();
}

HRESULT CTif_Ani::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 3.f , 3.f, 1.f };
	m_pTransformCom->m_vScale = Scale * 2.0f * PUBLIC_SCALE;

	m_iAlpha = 255;

	m_vPos.y += 9.f;
	m_vPos.z += 20.f;
	m_vPos.z += 3.f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;
	


	return S_OK;
}

_int CTif_Ani::Update_GameObject(const _float& fTimeDelta)
{

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;


	if (m_bDead)
		return OBJ_DEAD;

	if (m_Cur_State == TIF_START) {

		m_R = 0;
		m_G = 150;
		m_B = 180;

		m_fStartFrame += 11.f * Time * 3.f;

		
		if (m_fStartFrame > 11.f)
		{
			m_Cur_State = TIF_LOOF;
			m_fStartFrame = 0.f;
		}
	}
	
	if (m_Cur_State == TIF_LOOF) {
		
		m_R = 255;
		m_G = 255;
		m_B = 255;

		m_fLoofFrame += 10.f * Time * 1.f;

		if (m_fLoofFrame > 10.f)
		{
			m_Loof_Cnt++;
			m_fLoofFrame = 1.f;
		}
	}

	if (m_Loof_Cnt > 4) {
		m_Cur_State = TIF_END;
	}

	if (m_Cur_State == TIF_END) {

		m_fEndFrame += 11.f * Time * 7.f;

	
		if (m_fEndFrame > 11.f)
		{
			m_Cur_State = TIF_STATE_END;
			m_fEndFrame = 11.f;
		}
	}


	if (m_Cur_State == TIF_STATE_END) {


		m_iAlpha -= 7;


		if (m_iAlpha < 0) {
			return OBJ_DEAD;
		}
	}




	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	__super::Update_GameObject(Time);

	return 0;
}

void CTif_Ani::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();




	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CTif_Ani::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, m_R, m_G, m_B));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_Cur_State == TIF_START) {
		m_pTif_TextureCom[TIF_START]->Set_Texture((_uint)m_fStartFrame);
	}
	else if (m_Cur_State == TIF_LOOF) {
		m_pTif_TextureCom[TIF_LOOF]->Set_Texture((_uint)m_fLoofFrame);
	}
	else {
		m_pTif_TextureCom[TIF_END]->Set_Texture((_uint)m_fEndFrame);
	}



	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTif_Ani::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTif_TextureCom[TIF_START] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Tif_Ani_Start"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Tif_Ani_Start", pComponent });

	pComponent = m_pTif_TextureCom[TIF_LOOF] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Tif_Ani_Loof"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Tif_Ani_Loof", pComponent });

	pComponent = m_pTif_TextureCom[TIF_END] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Tif_Ani_End"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Tif_Ani_End", pComponent });

	return S_OK;
}

CTif_Ani* CTif_Ani::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos)
{
	CTif_Ani* pInstance = new CTif_Ani(pGraphicDev);

	pInstance->m_vPos = pos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTif_Ani::Free(void)
{
	__super::Free();
}


