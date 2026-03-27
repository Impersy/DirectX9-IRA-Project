#include "stdafx.h"
#include "..\Header\Apostle_Pattern_Glow.h"
#include "Export_Function.h"

#include "Player.h"

CApostle_Pattern_Glow::CApostle_Pattern_Glow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CApostle_Pattern_Glow::CApostle_Pattern_Glow(const CApostle_Pattern_Glow& rhs)
	: CFrontUi(rhs), m_eRecieve_Type(rhs.m_eRecieve_Type), m_pTextureCom_Brave(rhs.m_pTextureCom_Brave), m_pTextureCom_Night(rhs.m_pTextureCom_Night)
{
}

CApostle_Pattern_Glow::~CApostle_Pattern_Glow()
{
}

HRESULT CApostle_Pattern_Glow::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CApostle_Pattern_Glow::Update_GameObject(const _float& fTimeDelta)
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	m_eRecieve_Type = pPlayer->Get_Player_ApostleType();

	Frame_Check(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CApostle_Pattern_Glow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CApostle_Pattern_Glow::Render_GameObject()
{

	if (!g_UI_Render)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	Change_State();

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_Apostle_Print(m_tINFO.iNum, 6); //서순 주의

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CApostle_Pattern_Glow::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Apostle_Pattern_Time_Glow"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Apostle_Pattern_Time_Glow", pComponent });

	pComponent = m_pTextureCom_Night = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Apostle_Pattern_Night_Glow"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Apostle_Pattern_Night_Glow", pComponent });

	pComponent = m_pTextureCom_Brave = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Apostle_Pattern_Brave_Glow"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Apostle_Pattern_Brave_Glow", pComponent });

	return S_OK;
}

void CApostle_Pattern_Glow::Change_State(void)
{
	if (m_eRecieve_Type == TIME)
	{
		m_pTextureCom->Set_Texture((_uint)m_fFrame);
	}

	if (m_eRecieve_Type == COURAGE)
	{
		m_pTextureCom_Brave->Set_Texture((_uint)m_fFrame);
	}

	if (m_eRecieve_Type == NIGHT)
	{
		m_pTextureCom_Night->Set_Texture((_uint)m_fFrame);
	}
}

void CApostle_Pattern_Glow::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += m_fFrame_Max * fTimeDelta;

	if (m_fFrame > m_fFrame_Max)
	{
		m_fFrame = 0.f;
		m_bCheck = false;
	}
}

void CApostle_Pattern_Glow::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CApostle_Pattern_Glow* CApostle_Pattern_Glow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CApostle_Pattern_Glow* pInstance = new CApostle_Pattern_Glow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CApostle_Pattern_Glow::Free(void)
{
	__super::Free();
}
