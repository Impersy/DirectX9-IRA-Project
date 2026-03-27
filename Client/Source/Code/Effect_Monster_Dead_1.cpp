#include "stdafx.h"
#include "..\Header\Effect_Monster_Dead_1.h"
#include "Export_Function.h"

#include "Effect_Monster_Dead_2.h"

CEffect_Monster_Dead_1::CEffect_Monster_Dead_1(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Monster_Dead_1::CEffect_Monster_Dead_1(const CEffect_Monster_Dead_1& rhs)
	: CEffect(rhs)
{
}

CEffect_Monster_Dead_1::~CEffect_Monster_Dead_1()
{
	Free();
}

HRESULT CEffect_Monster_Dead_1::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;
	m_pTransformCom->m_vInfo[INFO_POS].y = m_vPos.y;

	m_pTransformCom->m_vScale = { 7.f , 7.f, 1.f };
	m_fMaxFrame = 9.f;

	return S_OK;
}

_int CEffect_Monster_Dead_1::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{	
		return OBJ_DEAD;
	}

	m_fFrame += m_fMaxFrame * fTimeDelta;

	if ((_uint)m_fFrame == 5.f)
	{
		Create_Next_Effect();
	}

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	__super::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;
}

void CEffect_Monster_Dead_1::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{	
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}
}

void CEffect_Monster_Dead_1::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Monster_Dead_1::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Monster_Dead_A"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Monster_Dead_A", pComponent });

	return S_OK;
}

CEffect_Monster_Dead_1* CEffect_Monster_Dead_1::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CEffect_Monster_Dead_1* pInstance = new CEffect_Monster_Dead_1(pGraphicDev);
	pInstance->m_vPos = vPos;
	pInstance->m_vPos.y += 4.f;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Monster_Dead_1::Free(void)
{
	__super::Free();
}

void CEffect_Monster_Dead_1::Create_Next_Effect(void)
{
	CLayer* pGameLogicLayer;
	pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	pGameObject = CEffect_Monster_Dead_2::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}


