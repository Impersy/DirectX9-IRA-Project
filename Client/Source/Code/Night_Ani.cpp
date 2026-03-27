#include "stdafx.h"
#include "..\Header\Night_Ani.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"
#include <SoundMgr.h>


CNight_Ani::CNight_Ani(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CNight_Ani::CNight_Ani(const CNight_Ani& rhs)
	: CEffect(rhs)
{
}

CNight_Ani::~CNight_Ani()
{
	Free();
}

HRESULT CNight_Ani::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 3.f , 3.f, 1.f };
	m_pTransformCom->m_vScale = Scale * 2.0f * PUBLIC_SCALE;

	m_iAlpha = 255;

	m_vPos.x -= 3.f;
	m_vPos.y += 9.f - 3.f;
	m_vPos.z += 8.f;

	m_vPos.z += 1.f;
	m_vPos.x += 1.f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;
	
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SAW_S);
	CSoundMgr::Get_Instance()->PlaySound(L"So_Appear.wav", CSoundMgr::SAW_S, 0.95f);
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SAW_L);
	CSoundMgr::Get_Instance()->PlaySound(L"So_Attack.wav", CSoundMgr::SAW_L, 0.95f);

	return S_OK;
}

_int CNight_Ani::Update_GameObject(const _float& fTimeDelta)
{

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	_float Time = pGameLogicLayer->m_fTimeDelta;


	if (m_bDead)
		return OBJ_DEAD;


	if (m_Cur_State == NIGHT_START) {

		m_fStartFrame += 5.f * Time * 2.f;

		
		if (m_fStartFrame > 5.f)
		{
			m_Cur_State = NIGHT_LOOF;
			m_fStartFrame = 0.f;
		}
	}
	
	if (m_Cur_State == NIGHT_LOOF) {
	
		m_fLoofFrame += 5.f * Time * 1.f;

		if (m_fLoofFrame > 5.f)
		{
			m_Loof_Cnt++;
			m_fLoofFrame = 1.f;
		}
	}

	if (m_Loof_Cnt > 4) {
		m_Cur_State = NIGHT_END;
	}

	if (m_Cur_State == NIGHT_END) {

		m_fEndFrame += 5.f * Time * 2.f;

	
		if (m_fEndFrame > 5.f)
		{
			m_Cur_State = NIGHT_STATE_END;
			m_fEndFrame = 5.f;
		}
	}

	if (m_Cur_State == NIGHT_STATE_END) {


		m_iAlpha -= 7;
		

		if (m_iAlpha < 0) {
			pPlayer->m_Is_Night = false;
			return OBJ_DEAD;
		}
	}


	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	__super::Update_GameObject(Time);

	return 0;
}

void CNight_Ani::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();


	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CNight_Ani::Render_GameObject()
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

	if (m_Cur_State == NIGHT_START) {
		m_pNight_TextureCom[NIGHT_START]->Set_Texture((_uint)m_fStartFrame);
	}
	else if (m_Cur_State == NIGHT_LOOF) {
		m_pNight_TextureCom[NIGHT_LOOF]->Set_Texture((_uint)m_fLoofFrame);
	}
	else {
		m_pNight_TextureCom[NIGHT_END]->Set_Texture((_uint)m_fEndFrame);
	}



	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CNight_Ani::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pNight_TextureCom[NIGHT_START] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Night_Ani_Start"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Night_Ani_Start", pComponent });

	pComponent = m_pNight_TextureCom[NIGHT_LOOF] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Night_Ani_Loop"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Night_Ani_Loof", pComponent });

	pComponent = m_pNight_TextureCom[NIGHT_END] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Night_Ani_End"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Night_Ani_End", pComponent });

	return S_OK;
}

CNight_Ani* CNight_Ani::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos)
{
	CNight_Ani* pInstance = new CNight_Ani(pGraphicDev);

	pInstance->m_vPos = pos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNight_Ani::Free(void)
{
	__super::Free();
}


