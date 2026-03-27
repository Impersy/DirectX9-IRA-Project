#include "stdafx.h"
#include "..\Header\Effect_Doewole_Slam.h"
#include "Export_Function.h"

CEffect_Doewole_Slam::CEffect_Doewole_Slam(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Doewole_Slam::CEffect_Doewole_Slam(const CEffect_Doewole_Slam & rhs)
	: CEffect(rhs)
{
}

CEffect_Doewole_Slam::~CEffect_Doewole_Slam()
{
	Free();
}

HRESULT CEffect_Doewole_Slam::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vScale = {15.f , 15.f, 15.f };

	m_fMaxFrame = 6.f;

	CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole_Body", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = { pTransform->m_vInfo[INFO_POS].x , 13.f , pTransform->m_vInfo[INFO_POS].z };

	return S_OK;
}

_int CEffect_Doewole_Slam::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	m_fFrame += m_fMaxFrame * fTimeDelta;

	return 0;
}

void CEffect_Doewole_Slam::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Doewole_Slam::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Doewole_Slam::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Doewole_Slam"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Doewole_Slam", pComponent });

	return S_OK;
}

CEffect_Doewole_Slam* CEffect_Doewole_Slam::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Doewole_Slam* pInstance = new CEffect_Doewole_Slam(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Doewole_Slam::Free(void)
{
	__super::Free();
}


