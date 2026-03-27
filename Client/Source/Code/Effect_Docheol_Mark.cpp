#include "stdafx.h"
#include "..\Header\Effect_Docheol_Mark.h"
#include "Export_Function.h"

CEffect_Docheol_Mark::CEffect_Docheol_Mark(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_Mark::CEffect_Docheol_Mark(const CEffect_Docheol_Mark & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_Mark::~CEffect_Docheol_Mark()
{
	Free();
}

HRESULT CEffect_Docheol_Mark::Ready_GameObject(const _vec3& vPos , const _vec3& vScale, const _float& fAliveTime)
{
	Add_Component();

	m_pTransformCom->m_vScale = vScale;

	m_fAliveTime = fAliveTime;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x , 0.2f , vPos.z };
	
	return S_OK;
}

_int CEffect_Docheol_Mark::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);
	
	m_fAccTime += fTimeDelta;

	return 0;
}

void CEffect_Docheol_Mark::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fAccTime > m_fAliveTime)
		m_bDead = true;
}

void CEffect_Docheol_Mark::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CEffect_Docheol_Mark::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_Mark"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_Mark", pComponent });

	return S_OK;
}

CEffect_Docheol_Mark* CEffect_Docheol_Mark::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos,const _vec3& vScale , const _float& fAliveTime)
{
	CEffect_Docheol_Mark* pInstance = new CEffect_Docheol_Mark(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale,fAliveTime)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_Mark::Free(void)
{
	__super::Free();
}


