#include "stdafx.h"
#include "..\Header\Effect_Docheol_FireBall.h"
#include "Export_Function.h"
#include "DocheolBullet_SwingBullet.h"
#include "Effect_Docheol_FirePulse.h"
#include <Effect_Docheol_Mark2.h>

CEffect_Docheol_FireBall::CEffect_Docheol_FireBall(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_FireBall::CEffect_Docheol_FireBall(const CEffect_Docheol_FireBall & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_FireBall::~CEffect_Docheol_FireBall()
{
	Free();
}

HRESULT CEffect_Docheol_FireBall::Ready_GameObject(const _vec3& vPos , const _vec3& vScale, TYPE _eType)
{
	Add_Component();

	m_fMaxFrame = 23.f;

	m_pTransformCom->m_vScale = vScale;

	m_vCenterPos = vPos;

	m_eType = _eType;

	m_fRadius = 120.f;

	_float fDistance = 150.f;

	switch (m_eType)
	{
	case CEffect_Docheol_FireBall::ONE:
		m_fAngle = 30.f;
		break;
	case CEffect_Docheol_FireBall::TWO:
		m_fAngle = 150.f;
		break;
	case CEffect_Docheol_FireBall::THREE:
		m_fAngle = 270.f;
		break;
	default:
		break;
	}

	//m_pTransformCom->m_vInfo[INFO_POS] = { m_vCenterPos.x + fDistance * cosf(D3DXToRadian(m_fAngle)) , m_vCenterPos.y , m_vCenterPos.z + fDistance * sinf(D3DXToRadian(m_fAngle)) };
	
	
	return S_OK;
}

_int CEffect_Docheol_FireBall::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		if(m_eType == THREE)
			Create_MarkEffect();


		return OBJ_DEAD;
	}

	m_fFrame += m_fMaxFrame * fTimeDelta;

	m_fAngle += 21.f * fTimeDelta;
	m_fRadius -= 10.f * fTimeDelta;

	m_pTransformCom->m_vInfo[INFO_POS] = { m_vCenterPos.x + m_fRadius * cosf(D3DXToRadian(m_fAngle)) , m_vCenterPos.y - 15.f , m_vCenterPos.z + m_fRadius * sinf(D3DXToRadian(m_fAngle)) };

	m_fAccTime += fTimeDelta;

	// 1초마다 한번씩 3방향 총알 발사
	if (m_fAccTime > 1.f)
	{
		Create_Bullet();
		m_fAccTime = 0.f;
		++m_iCnt;
	}

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CEffect_Docheol_FireBall::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
		m_fFrame = 0.f;

	/*if (m_fRadius < 7.f)
		m_bDead = true;*/

	if (m_iCnt > 8)
		m_bDead = true;
}

void CEffect_Docheol_FireBall::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Docheol_FireBall::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_FireBall"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_FireBall", pComponent });

	return S_OK;
}

void CEffect_Docheol_FireBall::Create_Bullet()
{
	CGameObject* pGameObject = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vDir;
	_vec3 vScale = { 8.f , 8.f , 8.f };

	for (int i = 0; i < 3; ++i)
	{
		vDir = { cosf(D3DXToRadian(m_fAngle + 120.f * i)) , 0.f , sinf(D3DXToRadian(m_fAngle + 120.f * i)) };

		pGameObject = CDocheolBullet_SwingBullet::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]  , vDir ,  vScale, 50.f  , 2.f);
		pLayer->Add_BulletObject(pGameObject);
	}

	vScale = { 10.f , 10.f , 10.f };
	// 이펙트 생성
	pGameObject = CEffect_Docheol_FirePulse::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS],vScale);
	pLayer->Add_BulletObject(pGameObject);

}

void CEffect_Docheol_FireBall::Create_MarkEffect()
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CTransform* pDocheolTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Docheol", L"Proto_Transform", ID_DYNAMIC));
	

	_vec3 vPos = pDocheolTransformCom->m_vInfo[INFO_POS];
	vPos.z -= 4.f;
	vPos.y -= 3.f;
	_vec3 vScale = { 20.f, 20.f, 20.f };

	pEffect = CEffect_Docheol_Mark2::Create(m_pGraphicDev, vPos, vScale, 100.f);
	pLayer->Add_BulletObject(pEffect);
}

CEffect_Docheol_FireBall* CEffect_Docheol_FireBall::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale, TYPE _eType)
{
	CEffect_Docheol_FireBall* pInstance = new CEffect_Docheol_FireBall(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale , _eType)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_FireBall::Free(void)
{
	__super::Free();
}


