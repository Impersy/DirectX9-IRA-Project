#include "stdafx.h"
#include "..\Header\Effect_AlertRect.h"
#include "Export_Function.h"

CEffect_AlertRect::CEffect_AlertRect(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_AlertRect::CEffect_AlertRect(const CEffect_AlertRect & rhs)
	: CEffect(rhs)
{
}

CEffect_AlertRect::~CEffect_AlertRect()
{
	Free();
}

HRESULT CEffect_AlertRect::Ready_GameObject(const _vec3& vScale)
{
	Add_Component();

	m_pTransformCom->m_vScale = vScale;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = { pTransform->m_vInfo[INFO_POS].x , 0.03f , pTransform->m_vInfo[INFO_POS].z};

	return S_OK;
}

_int CEffect_AlertRect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_fAccTime += fTimeDelta;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return 0;
}

void CEffect_AlertRect::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fAccTime > 2.f)
		m_bDead = true;
}

void CEffect_AlertRect::Render_GameObject()
{
	if(m_fAccTime < 1.f)
		__super::Render_GameObject();
}

HRESULT CEffect_AlertRect::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_AlertRect"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_AlertRect", pComponent });

	return S_OK;
}

CEffect_AlertRect* CEffect_AlertRect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vScale)
{
	CEffect_AlertRect* pInstance = new CEffect_AlertRect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_AlertRect::Free(void)
{
	__super::Free();
}


