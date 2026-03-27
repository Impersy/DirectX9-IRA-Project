#include "stdafx.h"
#include "..\Header\Effect_AlertCircle2.h"
#include "Export_Function.h"
#include <Docheol.h>

CEffect_AlertCircle2::CEffect_AlertCircle2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_AlertCircle2::CEffect_AlertCircle2(const CEffect_AlertCircle2& rhs)
	: CEffect(rhs)
{
}

CEffect_AlertCircle2::~CEffect_AlertCircle2()
{
	Free();
}

HRESULT CEffect_AlertCircle2::Ready_GameObject(const _vec3& vPos, const _vec3& vScale, const _float& fAliveTime, const _float& fTraceTime)
{
	Add_Component();

	m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x , 0.4f , vPos.z };
	m_pTransformCom->m_vScale = vScale;
	m_fAliveTime = fAliveTime;
	m_fTraceTime = fTraceTime;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CEffect_AlertCircle2::Update_GameObject(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	

	if (m_fTraceTime > m_fAccTime)
	{
		m_pTransformCom->m_vInfo[INFO_POS] = { pPlayerTransformCom->m_vInfo[INFO_POS].x, 1.f ,pPlayerTransformCom->m_vInfo[INFO_POS].z };
		
		CDocheol* pDocheol = dynamic_cast<CDocheol*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Docheol")); 
		pDocheol->Set_vTemp(m_pTransformCom->m_vInfo[INFO_POS]);
	}

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CEffect_AlertCircle2::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fAccTime > m_fAliveTime)
		m_bDead = true;
}

void CEffect_AlertCircle2::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CEffect_AlertCircle2::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_AlertCircle2"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_AlertCircle2", pComponent });

	return S_OK;
}

CEffect_AlertCircle2* CEffect_AlertCircle2::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vScale, const _float& fAliveTime, const _float& fTraceTime)
{
	CEffect_AlertCircle2* pInstance = new CEffect_AlertCircle2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, vScale, fAliveTime, fTraceTime)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_AlertCircle2::Free(void)
{
	__super::Free();
}


