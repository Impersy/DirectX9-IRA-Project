#include "stdafx.h"
#include "..\Header\Fire_Brave.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"
#include "Night_Circle.h"
#include "Monster.h"
#include <SoundMgr.h>


CFire_Brave::CFire_Brave(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CFire_Brave::CFire_Brave(const CFire_Brave & rhs)
	: CEffect(rhs)
{
}

CFire_Brave::~CFire_Brave()
{
	Free();
}

HRESULT CFire_Brave::Ready_GameObject(void)
{
	Add_Component();

	_vec3 Scale = { 6.4f * 3.f , 4.66f * 3.f, 1.f };

	if (m_Is_Small)
		Scale *= 0.5;


	m_iAlpha = 255;
	m_pTransformCom->m_vScale = Scale * PUBLIC_SCALE;

	m_fFrame = 0.f;
	m_fMaxFrame = 60.f;

	if (m_Is_Big_Hit) {
		m_vPos.z += -1.f;
		m_vPos.y += 2.f;
	}
	else{
		m_vPos.x -= 1.5f;
		m_vPos.y += 4.f;
		m_vPos.z += -2.f;
	}

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;

	

	return S_OK;
}

_int CFire_Brave::Update_GameObject(const _float& fTimeDelta)
{
	
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (pGameLogicLayer->m_fTimeStop == true)
		Time = 0.f;

	
	if (m_Is_Big_Hit) {

		_vec3 Scale = { 6.4f * 3.f , 4.66f * 3.f, 1.f };

		if (m_Is_Small)
			Scale *= 0.5;

		m_pTransformCom->m_vScale = Scale * PUBLIC_SCALE;

		
		m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;


	}


	if (m_bDead)
		return OBJ_DEAD;


	m_fFrame += m_fMaxFrame * Time * m_fFrameSpeed;


	if (m_fFrame > 20) {
		//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::KAFF_SW);
		CSoundMgr::Get_Instance()->PlaySound(L"Kaff_Sword.wav", CSoundMgr::KAFF_SW, 0.75f);

	}

	if (m_fFrame > 30) {
		//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::KAFF_SW2);
		CSoundMgr::Get_Instance()->PlaySound(L"Kaff_Sword.wav", CSoundMgr::KAFF_SW2, 0.75f);
	}

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CFire_Brave::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	

	if (m_fFrame > m_fMaxFrame)
	{

		if (m_Is_Big_Hit) {
			m_fFrame = 0.f;
		}
		else {
			m_bDead = true;
			m_fFrame = 60.f;
		}
		
	}

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CFire_Brave::Render_GameObject()
{
	if (!m_bRender)
		return;

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

HRESULT CFire_Brave::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Courage_Brave"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Courage_Brave", pComponent });



	return S_OK;
}

CFire_Brave* CFire_Brave::Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 pos)
{
	CFire_Brave* pInstance = new CFire_Brave(pGraphicDev);

	pInstance->m_vPos = pos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFire_Brave::Free(void)
{
	
	__super::Free();
}


