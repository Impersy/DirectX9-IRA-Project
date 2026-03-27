#include "stdafx.h"
#include "..\Header\Effect_Door_Fire.h"
#include "Export_Function.h"
#include "Docheol.h"

CEffect_Door_Fire::CEffect_Door_Fire(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Door_Fire::CEffect_Door_Fire(const CEffect_Door_Fire & rhs)
	: CEffect(rhs)
{
}

CEffect_Door_Fire::~CEffect_Door_Fire()
{
	Free();
}

HRESULT CEffect_Door_Fire::Ready_GameObject(const _vec3& vPos , const _vec3& vScale , const _float& fFrameSpeed)
{
	Add_Component();

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_pTransformCom->m_vScale = vScale;

	m_fMaxFrame = 20.f;

	m_fFrameSpeed = fFrameSpeed;
	
	return S_OK;
}

_int CEffect_Door_Fire::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	m_fFrame += m_fMaxFrame * fTimeDelta * m_fFrameSpeed;
	
	return 0;
}

void CEffect_Door_Fire::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}
}

void CEffect_Door_Fire::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Door_Fire::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Door_Fire"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Door_Fire", pComponent });

	return S_OK;
}

CEffect_Door_Fire* CEffect_Door_Fire::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale , const _float& fFrameSpeed)
{
	CEffect_Door_Fire* pInstance = new CEffect_Door_Fire(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale , fFrameSpeed)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Door_Fire::Free(void)
{
	__super::Free();
}


