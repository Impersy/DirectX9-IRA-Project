#include "stdafx.h"
#include "..\Header\Effect_Docheol_FireCircle.h"
#include "Export_Function.h"

CEffect_Docheol_FireCircle::CEffect_Docheol_FireCircle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_FireCircle::CEffect_Docheol_FireCircle(const CEffect_Docheol_FireCircle & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_FireCircle::~CEffect_Docheol_FireCircle()
{
	Free();
}

HRESULT CEffect_Docheol_FireCircle::Ready_GameObject(const _vec3& vPos , const _vec3& vScale,TYPE _e)
{
	Add_Component();

	m_fMaxFrame = 18.f;

	m_eType = _e;
	switch (m_eType)
	{
	case CEffect_Docheol_FireCircle::INTRO:
		m_pTransformCom->m_vScale = vScale;
		m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x + 5.f , 35.f , 130.f };
		break;
	case CEffect_Docheol_FireCircle::SLAM:
		m_pTransformCom->m_vScale = vScale;
		m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x , 0.14f , vPos.z };
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
		break;
	default:
		break;
	}

	return S_OK;
}

_int CEffect_Docheol_FireCircle::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	switch (m_eType)
	{
	case CEffect_Docheol_FireCircle::INTRO:
		m_fFrame += m_fMaxFrame * fTimeDelta * 0.5f;
		break;
	case CEffect_Docheol_FireCircle::SLAM:
		m_fFrame += m_fMaxFrame * fTimeDelta * 1.f;
		break;
	default:
		break;
	}

	m_fFrame += m_fMaxFrame * fTimeDelta * 0.5f;

	return 0;
}

void CEffect_Docheol_FireCircle::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}
}

void CEffect_Docheol_FireCircle::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_FireCircle::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_FireCircle"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_FireCircle", pComponent });

	return S_OK;
}

CEffect_Docheol_FireCircle* CEffect_Docheol_FireCircle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale,TYPE _e)
{
	CEffect_Docheol_FireCircle* pInstance = new CEffect_Docheol_FireCircle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale, _e)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_FireCircle::Free(void)
{
	__super::Free();
}


