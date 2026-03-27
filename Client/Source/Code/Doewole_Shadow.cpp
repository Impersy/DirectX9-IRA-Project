#include "stdafx.h"
#include "..\Header\Doewole_Shadow.h"
#include "Export_Function.h"

CDoewole_Shadow::CDoewole_Shadow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDoewole(pGraphicDev)

{
}

CDoewole_Shadow::CDoewole_Shadow(const CDoewole_Shadow & rhs)
	: CDoewole(rhs.m_pGraphicDev)
{
}

CDoewole_Shadow::~CDoewole_Shadow()
{
}

HRESULT CDoewole_Shadow::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 20.f , 20.f , 20.f };

	_vec3	vRight;
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CDoewole_Shadow::Update_GameObject(const _float& fTimeDelta)
{
	m_bDead = m_pOwner->Get_Dead();

	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	// Doewole의 위치에 맞게 조정
	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 0.01f, pDoewoleTransformCom->m_vInfo[INFO_POS].z};

	CBoss::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CDoewole_Shadow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CDoewole_Shadow::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
HRESULT CDoewole_Shadow::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Shadow"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Shadow", pComponent });

	return S_OK;
}

CDoewole_Shadow * CDoewole_Shadow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoewole_Shadow * pInstance = new CDoewole_Shadow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewole_Shadow::Free(void)
{
	__super::Free();
}


