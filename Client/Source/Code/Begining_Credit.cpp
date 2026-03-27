#include "stdafx.h"
#include "..\Header\Begining_Credit.h"
#include "Export_Function.h"
#include <KeyMgr.h>

CBegining_Credit::CBegining_Credit(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

CBegining_Credit::CBegining_Credit(const CBegining_Credit& rhs)
	: Engine::CGameObject(rhs)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTransformCom(rhs.m_pTransformCom)
{
}

CBegining_Credit::~CBegining_Credit()
{
}

HRESULT CBegining_Credit::Ready_GameObject(void)
{

	Add_Component();

	return S_OK;
}

_int CBegining_Credit::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	if (m_Type_Int == 0) {
		m_Type = ROGO_JUSIN;
	}
	else if (m_Type_Int == 1) {
		m_Type = ROGO_SUNA;
	}
	else if (m_Type_Int == 2) {
		m_Type = ROGO_TEAM;
	}


	m_fStartTime += 2.f * fTimeDelta * 0.4f;

	if (m_fStartTime > 2.f) {
		m_fStartTime = 0.f;
		m_Is_Start = true;

	}

	if (m_Is_Start) {

		m_iAlpha += 5.f * m_Dir;

		if (m_iAlpha > 255.f) {
			m_iAlpha = 255.f;
			m_Is_Alpha_Max = true;
		}

		if (m_Is_Alpha_Max) {

			m_fAccTime += 2.f * fTimeDelta * 0.2f;

			if (m_fAccTime > 2.f) {
				m_fAccTime = 0.f;
				m_Dir = -1.f;
			}
		}

		if (m_iAlpha < 0.f) {
			m_iAlpha = 0.f;

			if (m_Type_Int == 0)
				m_Type_Int = 2;
			else if (m_Type_Int == 1){
				m_Type_Int = 2;
			}
			else if (m_Type_Int == 2) {
				m_Type_Int = 3;
			}

			m_Is_Start = false;
			m_Is_Alpha_Max = false;
			m_fStartTime = 0.f;
			m_fAccTime = 0.f;
			m_Dir =  1.f;

			if (m_Type_Int > 2) {
				return 0;
			}
		}

		
	}

	Engine::Add_RenderGroup(RENDER_BLACK, this);

	return 0;
}

void CBegining_Credit::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

}
void CBegining_Credit::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((_uint)m_iAlpha, 255, 255, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	

	if (m_Type_Int == 0) {
		m_pTransformCom->Apostle_Skill_Time_illust(400, 400, { 0.f,0.f,0.f });
		m_pTextureCom[ROGO_JUSIN]->Set_Texture(0);
	}
	else if (m_Type_Int == 1) {
		m_pTransformCom->Apostle_Skill_Time_illust(750, 200, { 0.f,0.f,0.f });
		m_pTextureCom[ROGO_SUNA]->Set_Texture(0);
	}
	else if (m_Type_Int == 2) {
		m_pTransformCom->Apostle_Skill_Time_illust(350, 350, { -50.f,0.f,0.f });
		m_pTextureCom[ROGO_TEAM]->Set_Texture(0);
	}
	

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBegining_Credit::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));

	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom[ROGO_JUSIN] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Jusin"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Jusin", pComponent });

	pComponent = m_pTextureCom[ROGO_SUNA] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Suna"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Suna", pComponent });

	pComponent = m_pTextureCom[ROGO_TEAM] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Team"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Team", pComponent });

	return S_OK;
}

void CBegining_Credit::Black_Print()
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 800.f;
	matWorld._22 = 500.f;
	matWorld._41 = 0.f;
	matWorld._42 = 0.f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CBegining_Credit::Reset_Proj_Matrix()
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBegining_Credit* CBegining_Credit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBegining_Credit* pInstance = new CBegining_Credit(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBegining_Credit::Free(void)
{
	__super::Free();
}

