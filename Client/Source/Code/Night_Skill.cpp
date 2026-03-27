#include "stdafx.h"
#include "..\Header\Night_Skill.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"
#include "Night_Circle.h"


CNight_Skill::CNight_Skill(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CNight_Skill::CNight_Skill(const CNight_Skill & rhs)
	: CEffect(rhs)
{
}

CNight_Skill::~CNight_Skill()
{
	Free();
}

HRESULT CNight_Skill::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 3.2f * 5.f , 2.4f * 5.f, 1.f };

	m_iAlpha = 255;
	m_pTransformCom->m_vScale = Scale * PUBLIC_SCALE;

	m_fFrame = 0.f;
	m_fMaxFrame = 20.f;

	m_vPos.x -= 3.f;
	m_vPos.y += 10.f - 3.f;
	m_vPos.z += 8.f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;

	if (g_eCurScene == SCENE_BOSS2 || g_eCurScene == SCENE_BOSS1)
		m_Is_Boss2_Apostle = true;


	return S_OK;
}

_int CNight_Skill::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += m_fMaxFrame * Time * 0.8f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CNight_Skill::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = 20.f;

		CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

		
		CGameObject* pGameObject = nullptr;

		if (m_Is_Boss2_Apostle == true)
			pGameObject = CNight_Circle::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], true);
		else{
			pGameObject = CNight_Circle::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], false);
		}


		if (pGameObject != nullptr)
			pGameLogicLayer->Add_BulletObject(pGameObject);


	}

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CNight_Skill::Render_GameObject()
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

HRESULT CNight_Skill::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Night_Skill"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Night_Skill", pComponent });



	return S_OK;
}

CNight_Skill* CNight_Skill::Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 pos)
{
	CNight_Skill* pInstance = new CNight_Skill(pGraphicDev);

	pInstance->m_vPos = pos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNight_Skill::Free(void)
{
	__super::Free();
}


