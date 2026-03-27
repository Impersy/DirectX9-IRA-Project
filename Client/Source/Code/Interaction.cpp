#include "stdafx.h"
#include "..\Header\Interaction.h"
#include "Export_Function.h"

#include "Player.h"

CInteraction::CInteraction(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr), m_bCheck(false), m_fFrame(0.f), m_eInter_Type(INTER_END), m_eInter_Target(TARGET_END)
	, m_vMasterPos(0.f, 0.f, 0.f)
{
}

CInteraction::CInteraction(const CInteraction& rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTextureCom(rhs.m_pTextureCom), m_pTransformCom(rhs.m_pTransformCom), m_bCheck(rhs.m_bCheck), m_fFrame(rhs.m_fFrame)
	, m_eInter_Type(rhs.m_eInter_Type), m_eInter_Target(rhs.m_eInter_Target), m_vMasterPos(rhs.m_vMasterPos)
{
}

CInteraction::~CInteraction()
{
	Free();
}

HRESULT CInteraction::Ready_GameObject(void)
{	
	return S_OK;
}

_int CInteraction::Update_GameObject(const _float& fTimeDelta)
{	
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CInteraction::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CInteraction::Render_GameObject()
{
}

void CInteraction::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CInteraction::Free(void)
{
	__super::Free();
}
