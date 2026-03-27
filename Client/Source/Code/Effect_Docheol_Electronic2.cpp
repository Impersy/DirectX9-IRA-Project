#include "stdafx.h"
#include "..\Header\Effect_Docheol_Electronic2.h"
#include "Export_Function.h"

CEffect_Docheol_Electronic2::CEffect_Docheol_Electronic2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_Electronic2::CEffect_Docheol_Electronic2(const CEffect_Docheol_Electronic2 & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_Electronic2::~CEffect_Docheol_Electronic2()
{
	Free();
}

HRESULT CEffect_Docheol_Electronic2::Ready_GameObject(const _vec3& vPos , const _vec3& vScale , const _float& fRotYAngle)
{
	Add_Component();

	m_pTransformCom->m_vScale = vScale;
	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_vTemp = m_pTransformCom->m_vInfo[INFO_POS];

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(fRotYAngle));

	m_fMaxFrame = 11.f;
	return S_OK;
}

_int CEffect_Docheol_Electronic2::Update_GameObject(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	m_fFrame += m_fMaxFrame * fTimeDelta * 1.f;

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CEffect_Docheol_Electronic2::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
		m_fFrame = 0.f;


	if (m_fAccTime > 2.f)
		m_bDead = true;
}

void CEffect_Docheol_Electronic2::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_Electronic2::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_Electronic2"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_Electronic2", pComponent });

	return S_OK;
}

CEffect_Docheol_Electronic2* CEffect_Docheol_Electronic2::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale , const _float& fRotYAngle)
{
	CEffect_Docheol_Electronic2* pInstance = new CEffect_Docheol_Electronic2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale , fRotYAngle)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_Electronic2::Free(void)
{
	__super::Free();
}


