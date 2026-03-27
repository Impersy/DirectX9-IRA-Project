#include "stdafx.h"
#include "..\Header\Effect_TestEffect.h"
#include "Export_Function.h"

#include "Player.h"
#include "Status.h"

CEffect_TestEffect::CEffect_TestEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_TestEffect::CEffect_TestEffect(const CEffect_TestEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_TestEffect::~CEffect_TestEffect()
{
	Free();
}

HRESULT CEffect_TestEffect::Ready_GameObject(void)
{
	Add_Component();

	m_fMaxFrame = 21.f;

	return S_OK;
}

_int CEffect_TestEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{	
		return OBJ_DEAD;
	}

	m_fFrame += (m_fMaxFrame * fTimeDelta * 0.5);

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CEffect_TestEffect::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{	
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}

	if (pReceiveInfo.NowEnergy == pReceiveInfo.MaxEnergy)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}
}

void CEffect_TestEffect::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	if (m_bDead == false)
	{
		Ui_Print();
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);

	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEffect_TestEffect::Ui_Print(void)
{	
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1.f, 1.f, 0.f, 1.f);

	matWorld._11 = 1.f;
	matWorld._22 = 1.f;
	matWorld._41 = 0.f;
	matWorld._42 = 0.f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CEffect_TestEffect::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CEffect_TestEffect* CEffect_TestEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CEffect_TestEffect* pInstance = new CEffect_TestEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CEffect_TestEffect::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Environment_Rain"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Environment_Rain", pComponent });

	return S_OK;
}

void CEffect_TestEffect::Free(void)
{
	__super::Free();
}



