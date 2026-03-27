#include "stdafx.h"
#include "..\Header\Effect_Docheol_AppearMarkFire.h"
#include "Export_Function.h"
#include <Effect_Docheol_FloorFireRayUp.h>
#include "Docheol.h"
#include <Effect_Docheol_FireSpread.h>
#include <Effect_Docheol_Fire.h>
#include <SoundMgr.h>

CEffect_Docheol_AppearMarkFire::CEffect_Docheol_AppearMarkFire(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_AppearMarkFire::CEffect_Docheol_AppearMarkFire(const CEffect_Docheol_AppearMarkFire & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_AppearMarkFire::~CEffect_Docheol_AppearMarkFire()
{
	Free();
}

HRESULT CEffect_Docheol_AppearMarkFire::Ready_GameObject(const _vec3& vPos)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 10.f , 10.f, 10.f };

	m_fMaxFrame = 16.f;

	//m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	m_pTransformCom->m_vInfo[INFO_POS] = { vPos.x + 3.f, 10.f , vPos.z + 2.9f };


	return S_OK;
}

_int CEffect_Docheol_AppearMarkFire::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		Create_FloorFireRayUp_Effect();
		CDocheol* pGameObject = dynamic_cast<CDocheol*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Docheol"));
		pGameObject->m_bRender = true;

		return OBJ_DEAD;
	}

	m_fAccTime += fTimeDelta;

	if(m_fAccTime < 1.5f)
		m_pTransformCom->m_vInfo[INFO_POS].y += 3.f * fTimeDelta;

	if (m_fAccTime > 2.5f)
	{
		m_fFrame += m_fMaxFrame * fTimeDelta * 0.5f;

		if (!m_bSound)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_appear_2.wav", CSoundMgr::DOCHEOL_EFFECT2, 0.8f);
			m_bSound = true;
		}
	}

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CEffect_Docheol_AppearMarkFire::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_bDead = true;
	}

}

void CEffect_Docheol_AppearMarkFire::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_AppearMarkFire::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_AppearMarkFire"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_AppearMarkFire", pComponent });

	return S_OK;
}

void CEffect_Docheol_AppearMarkFire::Create_FloorFireRayUp_Effect()
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pEffect = nullptr;

	pEffect = CEffect_Docheol_Fire::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS] , {40.f, 40.f , 1.f}, CEffect_Docheol_Fire::INTRO);
	pLayer->Add_BulletObject(pEffect);


}

CEffect_Docheol_AppearMarkFire* CEffect_Docheol_AppearMarkFire::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos)
{
	CEffect_Docheol_AppearMarkFire* pInstance = new CEffect_Docheol_AppearMarkFire(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_AppearMarkFire::Free(void)
{
	__super::Free();
}


