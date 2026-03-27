#include "stdafx.h"
#include "..\Header\Fire_Ani.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"
#include "Fire_Brave.h"
#include "Fire_Bomb.h"
#include "Fire_Circle.h"
#include "Fire_Wave.h"
#include <SoundMgr.h>


CFire_Ani::CFire_Ani(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CFire_Ani::CFire_Ani(const CFire_Ani& rhs)
	: CEffect(rhs)
{
}

CFire_Ani::~CFire_Ani()
{
	Free();
}

HRESULT CFire_Ani::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 4.f , 2.f, 1.f };
	m_pTransformCom->m_vScale = Scale * 4.0f * PUBLIC_SCALE;

	m_iAlpha = 255;

	m_vPos.x -= 3.f;
	m_vPos.y += 2.f;
	m_vPos.z += 8.f;

	m_vPos.z += 1.f;
	m_vPos.x += 1.f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;
	
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::KAFF_S);
	CSoundMgr::Get_Instance()->PlaySound(L"Kaff_Appear.wav", CSoundMgr::KAFF_S, 1.f);

	return S_OK;
}

_int CFire_Ani::Update_GameObject(const _float& fTimeDelta)
{
	CTransform* pTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	_vec3 Pos = pTransCom->m_vInfo[INFO_POS];

	CLayer* pUILayer = Engine::Get_Layer(L"Layer_UI");

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	_float Time = pGameLogicLayer->m_fTimeDelta;

	CGameObject* pGameObject = nullptr;
	
	m_fAccTime += 4.f * Time * 0.7f;


	if (m_fAccTime > 1.5f && m_Brave == true) {
		pGameObject = CFire_Brave::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject != nullptr)
			pGameLogicLayer->Add_BulletObject(pGameObject);

		


		m_Brave = false;
	}



	if (m_fAccTime > 2.5f && m_Bomb == true) {
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::KAFF_B);
		CSoundMgr::Get_Instance()->PlaySound(L"Kaff_Brave.wav", CSoundMgr::KAFF_B, 0.95f);
		pGameObject = CFire_Bomb::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject != nullptr)
			pGameLogicLayer->Add_BulletObject(pGameObject);

		m_Bomb = false;
	}

	if (m_fAccTime > 6.0f && m_Circle == true) {
		pGameObject = CFire_Circle::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject != nullptr)
			pGameLogicLayer->Add_BulletObject(pGameObject);

		m_Circle = false;
	}

	if (m_fAccTime > 8.0f && m_Big == true) {
		pGameObject = CFire_Wave::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject != nullptr)
			pGameLogicLayer->Add_BulletObject(pGameObject);

		m_Big = false;
	}


	if (m_fAccTime > 7.0f) {

		m_fFrame += 18.f * Time * m_dir;

	}


	if (m_fFrame >= 10.f && m_fAccTime <= 12.f)
	{
		m_fFrame = 10.f;
		m_dir = 0.f;
	}


	if (m_fAccTime > 12.f) {
		m_dir = 0.7f;
	}


	if (m_fFrame > 18.f) {
		m_fFrame = 18.f;
		m_dir = 0.f;
	}

	if (m_fAccTime > 14.f) {
		m_Is_End = true;
	}


	if (m_Is_End) {

		m_iAlpha -= 7;

		if (m_iAlpha < 60) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::KAFF_S);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::KAFF_L);
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::KAFF_E);
			CSoundMgr::Get_Instance()->PlaySound(L"Kaff_Disappear.wav", CSoundMgr::KAFF_E, 0.95f);
		}


		if (m_iAlpha < 0) {
			pPlayer->m_Is_Courage = false;
			return OBJ_DEAD;
		}
	}


	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	__super::Update_GameObject(Time);

	return 0;
}

void CFire_Ani::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();


	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CFire_Ani::Render_GameObject()
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

	
    m_pTextureCom->Set_Texture((_uint)m_fFrame);
	



	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFire_Ani::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Courage_Ani"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Courage_Ani", pComponent });

	

	return S_OK;
}

CFire_Ani* CFire_Ani::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pos)
{
	CFire_Ani* pInstance = new CFire_Ani(pGraphicDev);

	pInstance->m_vPos = pos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFire_Ani::Free(void)
{
	__super::Free();
}


