#include "stdafx.h"
#include "..\Header\Effect_Docheol_FloorFire.h"
#include "Export_Function.h"

CEffect_Docheol_FloorFire::CEffect_Docheol_FloorFire(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_FloorFire::CEffect_Docheol_FloorFire(const CEffect_Docheol_FloorFire & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_FloorFire::~CEffect_Docheol_FloorFire()
{
	Free();
}

HRESULT CEffect_Docheol_FloorFire::Ready_GameObject(const _vec3& vPos , const _float& fAliveTime)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 40.f , 30.f, 20.f };

	m_fMaxFrame = 23.f;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x +2.f, 0.2f , vPos.z + 5.f };

	m_fAliveTime = fAliveTime;
	
	return S_OK;
}

_int CEffect_Docheol_FloorFire::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	m_fFrame += m_fMaxFrame * fTimeDelta * 0.55f;

	m_fAccTime += fTimeDelta;

	return 0;
}

void CEffect_Docheol_FloorFire::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0.f;
		++m_iReplayCnt;
	}

	if (m_iReplayCnt > 2)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}

	if (m_fAccTime > m_fAliveTime)
		m_bDead = true;
}

void CEffect_Docheol_FloorFire::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_FloorFire::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_FloorFire"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_FloorFire", pComponent });

	return S_OK;
}

CEffect_Docheol_FloorFire* CEffect_Docheol_FloorFire::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _float& fAliveTime)
{
	CEffect_Docheol_FloorFire* pInstance = new CEffect_Docheol_FloorFire(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , fAliveTime)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_FloorFire::Free(void)
{
	__super::Free();
}


