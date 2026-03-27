#include "stdafx.h"
#include "..\Header\Effect_Docheol_Meteor.h"
#include "Export_Function.h"
#include <Effect_Docheol_MeteorExplosion.h>
#include <SoundMgr.h>

CEffect_Docheol_Meteor::CEffect_Docheol_Meteor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_Meteor::CEffect_Docheol_Meteor(const CEffect_Docheol_Meteor & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_Meteor::~CEffect_Docheol_Meteor()
{
	Free();
}

HRESULT CEffect_Docheol_Meteor::Ready_GameObject(const _vec3& vDestPos, const _vec3& vStartPos)
{
	Add_Component();

	m_vDestPos = vDestPos;

	m_vDir = vDestPos - vStartPos;
	
	_float fLength = D3DXVec3Length(&m_vDir);
	m_fSpeed = fLength / 1.f;

	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_pTransformCom->m_vInfo[INFO_POS] = vStartPos;

	m_pTransformCom->m_vScale = { 4.f, 4.f , 4.f };

	m_fMaxFrame = 14.f;
	
	return S_OK;
}

_int CEffect_Docheol_Meteor::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		Create_DeathEffect();
		return OBJ_DEAD;
	}

	m_fAccTime += fTimeDelta;

	__super::Update_GameObject(fTimeDelta);

	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

	return 0;
}

void CEffect_Docheol_Meteor::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
		m_fFrame = 0.f;

	_vec3 vDiff = m_vDestPos - m_pTransformCom->m_vInfo[INFO_POS];

	if (D3DXVec3Length(&vDiff) < 3.f)
		m_bDead = true;

	if (m_fAccTime > 1.f)
		m_bDead = true;
}

void CEffect_Docheol_Meteor::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_Meteor::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_Meteor"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_Meteor", pComponent });

	return S_OK;
}

CEffect_Docheol_Meteor* CEffect_Docheol_Meteor::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vDestPos, const _vec3& vStartPos)
{
	CEffect_Docheol_Meteor* pInstance = new CEffect_Docheol_Meteor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vDestPos, vStartPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_Meteor::Free(void)
{
	__super::Free();
}

void CEffect_Docheol_Meteor::Create_DeathEffect()
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pEffect;

	pEffect = CEffect_Docheol_MeteorExplosion::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS],{10.f , 10.f, 10.f});
	pLayer->Add_BulletObject(pEffect);

	
}


