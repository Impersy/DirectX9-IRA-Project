#include "stdafx.h"
#include "..\Header\Apostle_Skill_UI_illust.h"
#include "Export_Function.h"
#include "Tif_Circle.h"
#include "Tif_Ani.h"
#include "Apostle_Skill_UI_Circle.h"
#include "Night_Ani.h"
#include "Night_Skill.h"
#include "Night_Wing.h"
#include "Night_Filter.h"
#include "Night_Circle.h"
#include "Fire_Ani.h"
#include "Fire_Circle.h"
#include "Fire_Bomb.h"
#include "Fire_Filter.h"
#include "Fire_Brave.h"
#include "Fire_Wave.h"

CApostle_Skill_UI_illust::CApostle_Skill_UI_illust(LPDIRECT3DDEVICE9 pGraphicDev)
	: CApostle_Skill_UI(pGraphicDev)
{
	
}

CApostle_Skill_UI_illust::CApostle_Skill_UI_illust(const CApostle_Skill_UI_illust& rhs)
	: CApostle_Skill_UI(rhs)
{
}

CApostle_Skill_UI_illust::~CApostle_Skill_UI_illust()
{
	Free();
}

HRESULT CApostle_Skill_UI_illust::Ready_GameObject(void)
{
	Add_Component();
	return S_OK;
}

_int CApostle_Skill_UI_illust::Update_GameObject(const _float& fTimeDelta)
{	

	CTransform* pTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	_vec3 Pos = pTransCom->m_vInfo[INFO_POS];

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	Frame_Check(Time);


	m_iAlpha += illustDir * 10.f;

	
	
	if (m_iAlpha > 255) {

		m_iAlpha = 255;

		if (m_Is_On == false) {
			CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
			CLayer* pUILayer = Engine::Get_Layer(L"Layer_UI");
			CGameObject* pGameObject = nullptr;

			if (m_Type == TIME) {

				pGameObject = CTif_Circle::Create(m_pGraphicDev, Pos);

				if (pGameObject != nullptr)
					pGameLogicLayer->Add_BulletObject(pGameObject);


				pGameObject = CTif_Ani::Create(m_pGraphicDev, Pos);

				if (pGameObject != nullptr)
					pGameLogicLayer->Add_BulletObject(pGameObject);


				pGameObject = CApostle_Skill_UI_Circle::Create(m_pGraphicDev, 600.f);

				if (pGameObject != nullptr)
					pUILayer->Add_BulletObject(pGameObject);

			}
			else if (m_Type == NIGHT) {

				pGameObject = CNight_Filter::Create(m_pGraphicDev, 512.f);

				if (pGameObject != nullptr)
					pUILayer->Add_BulletObject(pGameObject);


				pGameObject = CNight_Ani::Create(m_pGraphicDev, Pos);

				if (pGameObject != nullptr)
					pGameLogicLayer->Add_BulletObject(pGameObject);


				pGameObject = CNight_Skill::Create(m_pGraphicDev,Pos);

				if (pGameObject != nullptr)
					pGameLogicLayer->Add_BulletObject(pGameObject);


				pGameObject = CNight_Wing::Create(m_pGraphicDev, Pos);

				if (pGameObject != nullptr)
					pGameLogicLayer->Add_BulletObject(pGameObject);
			}
			else if (m_Type == COURAGE) {

				pGameObject = CFire_Ani::Create(m_pGraphicDev, Pos);

				if (pGameObject != nullptr)
					pGameLogicLayer->Add_BulletObject(pGameObject);

				
				pGameObject = CFire_Filter::Create(m_pGraphicDev, 500.f);

				if (pGameObject != nullptr)
					pGameLogicLayer->Add_BulletObject(pGameObject);


			}

			m_Is_On = true;
		}


		m_fAccTime += 25.f * Time * 0.6f;

		if (m_fAccTime > 25.f) {
			illustDir = -1.f;
		}
		
	}

	if (m_iAlpha < 0) {
		m_iAlpha = 0;
		return OBJ_DEAD;
	}


	__super::Update_GameObject(Time);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CApostle_Skill_UI_illust::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CApostle_Skill_UI_illust::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	DWORD AlphaValue;
	AlphaValue = D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);




	m_pTextureCom->Set_Texture(0);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Apostle_Skill_Time_illust(m_Size_X,m_Size_Y,m_vPos);

	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CApostle_Skill_UI_illust::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });



	if (m_Type == TIME) {
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Tif_illust"));
		m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Tif_illust", pComponent });
	}
	else if (m_Type == NIGHT) {
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Night_illust"));
		m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Night_illust", pComponent });
	}
	else if(m_Type == COURAGE) {
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Courage_illust"));
		m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Courage_illust", pComponent });
	}
	else{

	}



	return S_OK;


}

void CApostle_Skill_UI_illust::Change_State(void)
{

}

void CApostle_Skill_UI_illust::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += 16.f * fTimeDelta;

	if (16.f < m_fFrame)
	{
		m_fFrame = 0.f;
		m_bCheck = false;
	}
}

void CApostle_Skill_UI_illust::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CApostle_Skill_UI_illust* CApostle_Skill_UI_illust::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float size_x , _float size_y, _vec3 pos, APOSTLE_TYPE type)
{
	CApostle_Skill_UI_illust* pInstance = new CApostle_Skill_UI_illust(pGraphicDev);

	pInstance->m_Size_X = size_x;
	pInstance->m_Size_Y = size_y;
	pInstance->m_vPos = pos;
	pInstance->m_Type = type;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

void CApostle_Skill_UI_illust::Free(void)
{
	__super::Free();
}
