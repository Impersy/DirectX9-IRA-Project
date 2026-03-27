#include "stdafx.h"
#include "..\Header\Effect_Docheol_Electronic.h"
#include "Export_Function.h"

CEffect_Docheol_Electronic::CEffect_Docheol_Electronic(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_Electronic::CEffect_Docheol_Electronic(const CEffect_Docheol_Electronic & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_Electronic::~CEffect_Docheol_Electronic()
{
	Free();
}

HRESULT CEffect_Docheol_Electronic::Ready_GameObject(const _vec3& vPos , const _vec3& vScale)
{
	Add_Component();

	m_pTransformCom->m_vScale = vScale;
	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_vTemp = m_pTransformCom->m_vInfo[INFO_POS];

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(45.f));

	m_fMaxFrame = 5.f;
	return S_OK;
}

_int CEffect_Docheol_Electronic::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += m_fMaxFrame * fTimeDelta * 1.4f;

	if (_int(m_fFrame) % 2 == 0)
		m_pTransformCom->m_vInfo[INFO_POS] = { m_vTemp.x  , m_vTemp.y + 10.f  , m_vTemp.z };
	else
		m_pTransformCom->m_vInfo[INFO_POS] = { m_vTemp.x  , m_vTemp.y - 10.f  , m_vTemp.z };

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CEffect_Docheol_Electronic::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}

}

void CEffect_Docheol_Electronic::Render_GameObject()
{
	
	__super::Render_GameObject();
	
}

HRESULT CEffect_Docheol_Electronic::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_Electronic"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_Electronic", pComponent });

	return S_OK;
}

CEffect_Docheol_Electronic* CEffect_Docheol_Electronic::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale)
{
	CEffect_Docheol_Electronic* pInstance = new CEffect_Docheol_Electronic(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_Electronic::Free(void)
{
	__super::Free();
}


