#include "stdafx.h"
#include "..\Header\Effect_Doewole_Vanish.h"
#include "Export_Function.h"

CEffect_Doewole_Vanish::CEffect_Doewole_Vanish(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Doewole_Vanish::CEffect_Doewole_Vanish(const CEffect_Doewole_Vanish & rhs)
	: CEffect(rhs)
{
}

CEffect_Doewole_Vanish::~CEffect_Doewole_Vanish()
{
	Free();
}

HRESULT CEffect_Doewole_Vanish::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 15.f , 15.f, 15.f };

	m_fMaxFrame = 5.f;

	CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole_Body", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = pTransform->m_vInfo[INFO_POS];

	return S_OK;
}

_int CEffect_Doewole_Vanish::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	m_fFrame += m_fMaxFrame * fTimeDelta;


	return 0;
}

void CEffect_Doewole_Vanish::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Doewole_Vanish::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Doewole_Vanish::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Doewole_Vanish"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Doewole_Vanish", pComponent });

	return S_OK;
}

CEffect_Doewole_Vanish* CEffect_Doewole_Vanish::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Doewole_Vanish* pInstance = new CEffect_Doewole_Vanish(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Doewole_Vanish::Free(void)
{
	__super::Free();
}


