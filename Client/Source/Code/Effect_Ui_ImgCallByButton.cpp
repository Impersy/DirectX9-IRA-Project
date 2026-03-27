#include "stdafx.h"
#include "..\Header\Effect_Ui_ImgCallByButton.h"
#include "Export_Function.h"

CEffect_Ui_ImgCallByButton::CEffect_Ui_ImgCallByButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Ui_ImgCallByButton::CEffect_Ui_ImgCallByButton(const CEffect_Ui_ImgCallByButton& rhs)
	: CEffect(rhs)
{
}

CEffect_Ui_ImgCallByButton::~CEffect_Ui_ImgCallByButton()
{
	Free();
}

HRESULT CEffect_Ui_ImgCallByButton::Ready_GameObject(void)
{
	Add_Component();

	m_fMaxFrame = 20.f;

	return S_OK;
}

_int CEffect_Ui_ImgCallByButton::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += (m_fMaxFrame * fTimeDelta * 0.5f);

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CEffect_Ui_ImgCallByButton::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{	
		m_fFrame = m_fMaxFrame;
	}

	if (m_fFrame == m_fMaxFrame)
	{
		m_bDead = true;
	}

}

void CEffect_Ui_ImgCallByButton::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	if (m_iItemNum > 2)
	{
		_int iTemp_ItemImgNum = (m_iItemNum - 3);
		m_pTextureCom->Set_Texture(iTemp_ItemImgNum);
	}

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

void CEffect_Ui_ImgCallByButton::Ui_Print(void)
{	
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	matWorld._11 = 1.3f;
	matWorld._22 = 1.3f;
	matWorld._41 = 0.f;
	matWorld._42 = 9.2f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CEffect_Ui_ImgCallByButton::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CEffect_Ui_ImgCallByButton* CEffect_Ui_ImgCallByButton::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CEffect_Ui_ImgCallByButton* pInstance = new CEffect_Ui_ImgCallByButton(pGraphicDev);

	pInstance->m_iItemNum = _iNumber;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CEffect_Ui_ImgCallByButton::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Ui_ImgCallByButton"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Ui_ImgCallByButton", pComponent });

	return S_OK;
}

void CEffect_Ui_ImgCallByButton::Free(void)
{
	__super::Free();
}



