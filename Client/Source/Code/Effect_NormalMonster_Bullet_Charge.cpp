#include "stdafx.h"
#include "..\Header\Effect_NormalMonster_Bullet_Charge.h"
#include "Export_Function.h"

CEffect_NormalMonster_Bullet_Charge::CEffect_NormalMonster_Bullet_Charge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_NormalMonster_Bullet_Charge::CEffect_NormalMonster_Bullet_Charge(const CEffect_NormalMonster_Bullet_Charge & rhs)
	: CEffect(rhs)
{
}

CEffect_NormalMonster_Bullet_Charge::~CEffect_NormalMonster_Bullet_Charge()
{
	Free();
}

HRESULT CEffect_NormalMonster_Bullet_Charge::Ready_GameObject(CGameObject* pOwner)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pOwner = pOwner;

	m_pTransformCom->m_vScale = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vScale * 1.5f;
	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y += 2.2f;

	_vec3 vDir = *(dynamic_cast<CMonster*>(m_pOwner)->Get_Dir());
	if(0 <= vDir.x)
		m_pTransformCom->m_vInfo[INFO_POS].x += 2.2f;
	else
		m_pTransformCom->m_vInfo[INFO_POS].x -= 2.2f;
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	m_fMaxFrame = 18.f;

	return S_OK;
}

_int CEffect_NormalMonster_Bullet_Charge::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pOwner->Get_Dead())
		return OBJ_DEAD;

	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	//_vec3 vPos = dynamic_cast<CMonster*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	//_vec3 vDir = vPos - m_pTransformCom->m_vInfo[INFO_POS];
	//if( 3.f < D3DXVec3Length(&vDir))
	//	m_pTransformCom->Move_Pos(&vPos, 50.f, fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fFrame += m_fMaxFrame * fTimeDelta * 2.0f;
	m_fAccumulatedTime += fTimeDelta;

	if (1.f < m_fAccumulatedTime)
	{
		m_fAccumulatedTime = 0.f;
		m_bDead = true;
	}

	return 0;
}

void CEffect_NormalMonster_Bullet_Charge::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
	}
}

void CEffect_NormalMonster_Bullet_Charge::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_NormalMonster_Bullet_Charge::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Birth_Charge"));
	if (nullptr == m_pTextureCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Birth_Charge", pComponent });

	return S_OK;
}

CEffect_NormalMonster_Bullet_Charge* CEffect_NormalMonster_Bullet_Charge::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner)
{
	CEffect_NormalMonster_Bullet_Charge* pInstance = new CEffect_NormalMonster_Bullet_Charge(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_NormalMonster_Bullet_Charge::Free(void)
{
	__super::Free();
}


