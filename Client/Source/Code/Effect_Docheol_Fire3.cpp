#include "stdafx.h"
#include "..\Header\Effect_Docheol_Fire3.h"
#include "Export_Function.h"
#include "Docheol.h"

CEffect_Docheol_Fire3::CEffect_Docheol_Fire3(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_Fire3::CEffect_Docheol_Fire3(const CEffect_Docheol_Fire3 & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_Fire3::~CEffect_Docheol_Fire3()
{
	Free();
}

HRESULT CEffect_Docheol_Fire3::Ready_GameObject(const _vec3& vPos , const _vec3& vScale , const _float& fFrameSpeed)
{
	Add_Component();

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_pTransformCom->m_vScale = vScale;

	m_fMaxFrame = 33.f;

	m_fFrameSpeed = fFrameSpeed;

	m_fFrame = (rand() % 600) / 10.f;
	
	return S_OK;
}

_int CEffect_Docheol_Fire3::Update_GameObject(const _float& fTimeDelta)
{
	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Docheol", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];

	m_pTransformCom->m_vInfo[INFO_POS].x += 2.f;
	m_pTransformCom->m_vInfo[INFO_POS].y += 14.f;
	m_pTransformCom->m_vInfo[INFO_POS].z += 5.f;

	CDocheol* pDocheol = dynamic_cast<CDocheol*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Docheol"));
	if (pDocheol->Get_State() == CDocheol::BOSS_DEAD)
		m_bDead = true;

	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	m_fFrame += m_fMaxFrame * fTimeDelta * m_fFrameSpeed;
	
	return 0;
}

void CEffect_Docheol_Fire3::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0.f;
	}
}

void CEffect_Docheol_Fire3::Render_GameObject()
{
	CDocheol* pDocheol = dynamic_cast<CDocheol*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Docheol"));

	if (!pDocheol->Get_Render())
		return;
	
	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_Fire3::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_Fire3"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_Fire3", pComponent });

	return S_OK;
}

CEffect_Docheol_Fire3* CEffect_Docheol_Fire3::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale , const _float& fFrameSpeed)
{
	CEffect_Docheol_Fire3* pInstance = new CEffect_Docheol_Fire3(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale , fFrameSpeed)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_Fire3::Free(void)
{
	__super::Free();
}


