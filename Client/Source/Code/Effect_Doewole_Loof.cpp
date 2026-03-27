#include "stdafx.h"
#include "..\Header\Effect_Doewole_Loof.h"
#include "Export_Function.h"

CEffect_Doewole_Loof::CEffect_Doewole_Loof(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Doewole_Loof::CEffect_Doewole_Loof(const CEffect_Doewole_Loof & rhs)
	: CEffect(rhs)
{
}

CEffect_Doewole_Loof::~CEffect_Doewole_Loof()
{
	Free();
}

HRESULT CEffect_Doewole_Loof::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 15.f , 15.f, 15.f };

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(20.f));

	m_fMaxFrame = 11.f;

	CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole_Body", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = { pTransform->m_vInfo[INFO_POS].x  , pTransform->m_vInfo[INFO_POS].y + 15.f , pTransform->m_vInfo[INFO_POS].z - 1.f };

	return S_OK;
}

_int CEffect_Doewole_Loof::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += m_fMaxFrame * fTimeDelta;

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);
	
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CEffect_Doewole_Loof::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0;
		m_iReplayCnt++;
	}

	if(m_iReplayCnt == 5)
		m_bDead = true;
}

void CEffect_Doewole_Loof::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Doewole_Loof::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Doewole_Loof"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Doewole_Loof", pComponent });

	return S_OK;
}

CEffect_Doewole_Loof* CEffect_Doewole_Loof::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Doewole_Loof* pInstance = new CEffect_Doewole_Loof(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Doewole_Loof::Free(void)
{
	__super::Free();
}


