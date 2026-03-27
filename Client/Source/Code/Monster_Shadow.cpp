#include "stdafx.h"
#include "..\Header\Monster_Shadow.h"
#include "Export_Function.h"

CMonster_Shadow::CMonster_Shadow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)

{
}

CMonster_Shadow::CMonster_Shadow(const CMonster_Shadow & rhs)
	: CGameObject(rhs.m_pGraphicDev)
{
}

CMonster_Shadow::~CMonster_Shadow()
{
	Free();
}


HRESULT CMonster_Shadow::Ready_GameObject(CGameObject* pOwner, const _vec3& vScale)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pOwner = pOwner;

	m_pTransformCom->m_vScale = vScale;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));


	return S_OK;
}

_int CMonster_Shadow::Update_GameObject(const _float& fTimeDelta)
{
	m_bDead = m_pOwner->Get_Dead();

	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	CTransform* pOwnerTransformCom = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform();
	if (nullptr == pOwnerTransformCom) return -1;

	//m_pTransformCom->m_vInfo[INFO_POS] = { pOwnerTransformCom->m_vInfo[INFO_POS].x - 0.1f , 0.01f, pOwnerTransformCom->m_vInfo[INFO_POS].z + 0.3f};
	m_pTransformCom->m_vInfo[INFO_POS] = { pOwnerTransformCom->m_vInfo[INFO_POS].x , 0.2f, pOwnerTransformCom->m_vInfo[INFO_POS].z};

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CMonster_Shadow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CMonster_Shadow::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
HRESULT CMonster_Shadow::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Shadow"));
	if (nullptr == m_pTextureCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Shadow", pComponent });

	return S_OK;
}

CMonster_Shadow * CMonster_Shadow::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner, const _vec3& vScale)
{
	CMonster_Shadow * pInstance = new CMonster_Shadow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner, vScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMonster_Shadow::Free(void)
{
	__super::Free();
}


