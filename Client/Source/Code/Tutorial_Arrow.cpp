#include "stdafx.h"
#include "..\Header\Tutorial_Arrow.h"
#include "Export_Function.h"

CTutorial_Arrow::CTutorial_Arrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CTutorial_Arrow::CTutorial_Arrow(const CTutorial_Arrow & rhs)
	: CEffect(rhs)
{
}

CTutorial_Arrow::~CTutorial_Arrow()
{
	Free();
}

HRESULT CTutorial_Arrow::Ready_GameObject(const _vec3& vPos , const _vec3& vScale)
{
	Add_Component();

	m_fMaxFrame = 5.f;

	m_pTransformCom->m_vScale = vScale;
	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));
	
	return S_OK;
}

_int CTutorial_Arrow::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	m_fFrame += m_fMaxFrame * fTimeDelta * m_fFrameSpeed;
	
	return 0;
}

void CTutorial_Arrow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_fFrameSpeed *= -1.f;
	}

	if (m_fFrame < 0.f)
	{
		m_fFrame = 0.f;
		m_fFrameSpeed *= -1.f;
	}
}

void CTutorial_Arrow::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CTutorial_Arrow::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Tutorial_Arrow"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Tutorial_Arrow", pComponent });

	return S_OK;
}

CTutorial_Arrow* CTutorial_Arrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale)
{
	CTutorial_Arrow* pInstance = new CTutorial_Arrow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTutorial_Arrow::Free(void)
{
	__super::Free();
}


