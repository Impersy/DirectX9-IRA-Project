#include "stdafx.h"
#include "..\Header\Effect_Stone_Hit.h"
#include "Export_Function.h"
#include "Debris.h"

CEffect_Stone_Hit::CEffect_Stone_Hit(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Stone_Hit::CEffect_Stone_Hit(const CEffect_Stone_Hit& rhs)
	: CEffect(rhs)
{
}

CEffect_Stone_Hit::~CEffect_Stone_Hit()
{
	Free();
}

HRESULT CEffect_Stone_Hit::Ready_GameObject(const _vec3& vPos, _bool bIsLast)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 13.f, 13.f, 13.f };
	_vec3 vPosition = vPos;
	vPosition.z -= 2.f;
	m_pTransformCom->m_vInfo[INFO_POS] = vPosition;

	m_bIsLast = bIsLast;

	m_fMaxFrame = 4.f;

	//Create_Debris();

	return S_OK;
}

_int CEffect_Stone_Hit::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fFrame += m_fMaxFrame * fTimeDelta * 3.f;

	return 0;
}

void CEffect_Stone_Hit::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Stone_Hit::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Stone_Hit::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));

	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"StoneBox_HitEffect_"));

	m_uMapComponent[ID_STATIC].insert({ L"StoneBox_HitEffect_", pComponent });

	return S_OK;
}

CEffect_Stone_Hit* CEffect_Stone_Hit::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, _bool bIsLast)
{
	CEffect_Stone_Hit* pInstance = new CEffect_Stone_Hit(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, bIsLast)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Stone_Hit::Free(void)
{
	__super::Free();
}

void CEffect_Stone_Hit::Create_Debris(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
	for (int i = 0; i < 9; ++i)
	{
		//CDebris* pDebris = CDebris::Create(m_pGraphicDev, i, m_pTransformCom->m_vInfo[INFO_POS]);
		//pLayer->Add_BulletObject(pDebris);
	}
}