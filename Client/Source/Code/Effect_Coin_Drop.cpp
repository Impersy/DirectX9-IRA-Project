#include "stdafx.h"
#include "..\Header\Effect_Coin_Drop.h"
#include "Export_Function.h"

CEffect_Coin_Drop::CEffect_Coin_Drop(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev), m_pReceive_CoinPos(nullptr)
{
}

CEffect_Coin_Drop::CEffect_Coin_Drop(const CEffect_Coin_Drop& rhs)
	: CEffect(rhs), m_pReceive_CoinPos(rhs.m_pReceive_CoinPos)
{
}

CEffect_Coin_Drop::~CEffect_Coin_Drop()
{
	Free();

	//if (m_pReceive_CoinPos)
	//{
	//	delete m_pReceive_CoinPos;
	//	m_pReceive_CoinPos = nullptr;
	//}
}

HRESULT CEffect_Coin_Drop::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vInfo[INFO_POS] = { m_pReceive_CoinPos->x, m_pReceive_CoinPos->y, m_pReceive_CoinPos->z };
	m_pTransformCom->m_vInfo[INFO_POS].z = (m_pReceive_CoinPos->z - 0.1f);
	m_pTransformCom->m_vScale = { 8.f , 8.f, 1.f };
	m_fMaxFrame = 7.f;

	return S_OK;
}

_int CEffect_Coin_Drop::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{	
		return OBJ_DEAD;
	}

	m_pTransformCom->m_vInfo[INFO_POS] = *m_pReceive_CoinPos;
	m_pTransformCom->m_vInfo[INFO_POS].z = (m_pReceive_CoinPos->z - 0.1f);
	m_pTransformCom->UpdatePos_OnWorld();

	m_fFrame += m_fMaxFrame * fTimeDelta;

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	__super::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;
}

void CEffect_Coin_Drop::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{	
		m_fFrame = 0.f;
		m_bDead = true;
	}
}

void CEffect_Coin_Drop::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Coin_Drop::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Coin_Drop"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Coin_Drop", pComponent });

	return S_OK;
}

CEffect_Coin_Drop* CEffect_Coin_Drop::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* _pPos)
{
	CEffect_Coin_Drop* pInstance = new CEffect_Coin_Drop(pGraphicDev);

	pInstance->m_pReceive_CoinPos = new _vec3();
	pInstance->m_pReceive_CoinPos = _pPos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Coin_Drop::Free(void)
{
	__super::Free();
}