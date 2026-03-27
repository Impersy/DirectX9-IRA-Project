#include "stdafx.h"
#include "..\Header\Effect_Jar_Item_Drop.h"
#include "Export_Function.h"
#include "Item_Drop.h"

CEffect_Jar_Item_Drop::CEffect_Jar_Item_Drop(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Jar_Item_Drop::CEffect_Jar_Item_Drop(const CEffect_Jar_Item_Drop& rhs)
	: CEffect(rhs)
{
}

CEffect_Jar_Item_Drop::~CEffect_Jar_Item_Drop()
{
	Free();
}

HRESULT CEffect_Jar_Item_Drop::Ready_GameObject(CGameObject* pOwner)
{
	Add_Component();
	m_pOwner = pOwner;

	m_pTransformCom->m_vScale = dynamic_cast<CItem*>(m_pOwner)->Get_Transform()->m_vScale * 1.5f;
	m_pTransformCom->m_vInfo[INFO_POS] = dynamic_cast<CItem*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	//m_pTransformCom->Rotation(ROT_X, D3DXToRadian(75.f));

	m_fMaxFrame = 13.f;

	return S_OK;
}

_int CEffect_Jar_Item_Drop::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pOwner->Get_Dead())
		return OBJ_DEAD;

	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	_float fAngle = -380.f * fTimeDelta;
	_vec3 vPos = dynamic_cast<CItem_Drop*>(m_pOwner)->Get_Transform()->m_vInfo[INFO_POS];
	vPos.x += 2.5f;
	vPos.y -= 2.f;
	_vec3 vDir = vPos - m_pTransformCom->m_vInfo[INFO_POS];
	if (3.f < D3DXVec3Length(&vDir))
	{
		m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(fAngle));
		m_pTransformCom->Move_Pos(&vPos, 15.f, fTimeDelta);
	}

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fFrame += m_fMaxFrame * fTimeDelta * 2.f;

	return 0;
}

void CEffect_Jar_Item_Drop::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Jar_Item_Drop::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Jar_Item_Drop::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));

	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_JarItemDrop"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_JarItemDrop", pComponent });

	return S_OK;
}

CEffect_Jar_Item_Drop* CEffect_Jar_Item_Drop::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner)
{
	CEffect_Jar_Item_Drop* pInstance = new CEffect_Jar_Item_Drop(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pOwner)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Jar_Item_Drop::Free(void)
{
	__super::Free();
}