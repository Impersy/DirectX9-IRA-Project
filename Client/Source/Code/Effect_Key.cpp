#include "stdafx.h"
#include "..\Header\Effect_Key.h"
#include "Export_Function.h"
#include "Monster.h"
#include "Player.h"

CEffect_Key::CEffect_Key(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Key::CEffect_Key(const CEffect_Key& rhs)
	: CEffect(rhs)
{
}

CEffect_Key::~CEffect_Key()
{
	Free();
}

HRESULT CEffect_Key::Ready_GameObject(CGameObject* pOwner)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 3.f, 3.f, 0.f };
	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CMonster*>(pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(30.f));

	m_fMaxFrame = 0.f;

	return S_OK;
}

_int CEffect_Key::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		pPlayer->Set_Key();
		return OBJ_DEAD;
	}

	__super::Update_GameObject(fTimeDelta);

	CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	_vec3 vDir = pTransform->m_vInfo[INFO_POS] - m_pTransformCom->m_vInfo[INFO_POS];
	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransformCom->m_vInfo[INFO_POS] += vDir * 6.f * fTimeDelta;
	


	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return 0;
}

void CEffect_Key::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	_vec3 vDir = pTransform->m_vInfo[INFO_POS] - m_pTransformCom->m_vInfo[INFO_POS];
	if (D3DXVec3Length(&vDir) < 3.f)
		m_bDead = true;

}

void CEffect_Key::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Key::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));

	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_KeyNamed"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_KeyNamed", pComponent });

	return S_OK;
}

CEffect_Key* CEffect_Key::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner)
{
	CEffect_Key* pInstance = new CEffect_Key(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Key::Free(void)
{
	__super::Free();
}
