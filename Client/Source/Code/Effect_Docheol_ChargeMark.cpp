#include "stdafx.h"
#include "..\Header\Effect_Docheol_ChargeMark.h"
#include "Export_Function.h"
#include <Docheol.h>

CEffect_Docheol_ChargeMark::CEffect_Docheol_ChargeMark(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_ChargeMark::CEffect_Docheol_ChargeMark(const CEffect_Docheol_ChargeMark & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_ChargeMark::~CEffect_Docheol_ChargeMark()
{
	Free();
}

HRESULT CEffect_Docheol_ChargeMark::Ready_GameObject(const _vec3& vPos , const _vec3& vDir, const _vec3& vScale, const _float& fSpeed, const _float& bMove , TYPE eType)
{
	Add_Component();

	m_pTransformCom->m_vScale = vScale;

	m_bMove = bMove;

	m_fSpeed = fSpeed;

	m_vDir = vDir;

	m_eType = eType;

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_fMaxFrame = 9.f;

	return S_OK;
}

_int CEffect_Docheol_ChargeMark::Update_GameObject(const _float& fTimeDelta)
{
	CDocheol* pDocheol = dynamic_cast<CDocheol*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Docheol"));

	// Dead üũ
	switch (m_eType)
	{
	case CEffect_Docheol_ChargeMark::CHARGE:
		if (pDocheol->Get_ChargeBulletDead())
			return OBJ_DEAD;
		break;
	case CEffect_Docheol_ChargeMark::SPECIAL:
		if (m_bDead)
			return OBJ_DEAD;
		break;
	}
	
	__super::Update_GameObject(fTimeDelta);
	
	m_fFrame += m_fMaxFrame * fTimeDelta * m_fFrameSpeed;

	m_fAccTime += fTimeDelta;
	
	switch (m_eType)
	{
	case CEffect_Docheol_ChargeMark::CHARGE:
		if (m_bMove)
		{
			if (pDocheol->Get_BulletGo())
				m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;
		}
		break;
	case CEffect_Docheol_ChargeMark::SPECIAL:
		if (m_fAccTime > 2.f)
			m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

		if (m_fAccTime > 5.f)
			m_bDead = true;
		break;
	default:
		break;
	}

	if (m_fFrame >= m_fMaxFrame)
		m_fFrame = m_fMaxFrame;

	return 0;
}

void CEffect_Docheol_ChargeMark::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	
}

void CEffect_Docheol_ChargeMark::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(200, 0, 0, 0));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CEffect_Docheol_ChargeMark::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_Mark2"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_Mark2", pComponent });

	return S_OK;
}

CEffect_Docheol_ChargeMark* CEffect_Docheol_ChargeMark::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vDir, const _vec3& vScale , const _float& fSpeed , const _float& bMove , TYPE eType)
{
	CEffect_Docheol_ChargeMark* pInstance = new CEffect_Docheol_ChargeMark(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vDir,vScale, fSpeed, bMove , eType)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_ChargeMark::Free(void)
{
	__super::Free();
}


