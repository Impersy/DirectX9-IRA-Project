#include "stdafx.h"
#include "..\Header\Effect_Ui_LevelUp_Lion.h"
#include "Export_Function.h"

#include "Player.h"

CEffect_Ui_LevelUp_Lion::CEffect_Ui_LevelUp_Lion(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Ui_LevelUp_Lion::CEffect_Ui_LevelUp_Lion(const CEffect_Ui_LevelUp_Lion& rhs)
	: CEffect(rhs)
{
}

CEffect_Ui_LevelUp_Lion::~CEffect_Ui_LevelUp_Lion()
{
	Free();
}

HRESULT CEffect_Ui_LevelUp_Lion::Ready_GameObject(void)
{
	Add_Component();

	m_fMaxFrame = 9.f;

	return S_OK;
}

_int CEffect_Ui_LevelUp_Lion::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!m_bPrintCheck)
	{
		m_fFrame += (m_fMaxFrame * fTimeDelta);
	}
	else
	{
		m_fFrame += (m_fMaxFrame * fTimeDelta);
		m_fAlphaFrame += m_fAlphaFrame_Max * fTimeDelta;
		m_iAlpha = (_int)(255 - ((255 / 10) * m_fAlphaFrame));
	}

	if ((m_fFrame > m_fMaxFrame))
	{
		//m_fFrame = m_fMaxFrame;
		m_bPrintCheck = true;
	}
	if (m_fAlphaFrame > m_fAlphaFrame_Max)
	{
		m_fAlphaFrame = m_fAlphaFrame_Max;
		m_bDead = true;
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return OBJ_NOEVENT;
}

void CEffect_Ui_LevelUp_Lion::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CEffect_Ui_LevelUp_Lion::Render_GameObject()
{	
	if (!m_bPrintCheck)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

		if ((m_fFrame > m_fMaxFrame))
		{
			m_pTextureCom->Set_Texture((_int)9);
		}
		else if (m_fFrame <= m_fMaxFrame)
		{
		m_pTextureCom->Set_Texture((_int)m_fFrame);
		}

		_matrix matCamWorld;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

		Ui_Print();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
		Reset_Proj_Matrix();
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	else if (m_bPrintCheck)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 0, 0, 0));

		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);


		if ((m_fFrame > m_fMaxFrame))
		{
			m_pTextureCom->Set_Texture((_int)9);
		}
		else if(m_fFrame <= m_fMaxFrame)
		{
			m_pTextureCom->Set_Texture((_int)m_fFrame);
		}
		_matrix matCamWorld;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

		Ui_Print();
		m_pBufferCom->Render_Buffer();

		Reset_Proj_Matrix();
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

void CEffect_Ui_LevelUp_Lion::Ui_Print(void)
{	
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 170.f;
	matWorld._22 = 170.f;
	matWorld._41 = 2.f;
	matWorld._42 = 100.f + (m_fFrame * 5.f);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CEffect_Ui_LevelUp_Lion::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CEffect_Ui_LevelUp_Lion* CEffect_Ui_LevelUp_Lion::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Ui_LevelUp_Lion* pInstance = new CEffect_Ui_LevelUp_Lion(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CEffect_Ui_LevelUp_Lion::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Ui_LevelUp_Lion"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Ui_LevelUp_Lion", pComponent });

	return S_OK;
}

void CEffect_Ui_LevelUp_Lion::Free(void)
{
	__super::Free();
}



