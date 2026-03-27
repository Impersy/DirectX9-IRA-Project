#include "stdafx.h"
#include "..\Header\OpenJar.h"
#include "Export_Function.h"
#include "CollisionMgr.h"
#include "Effect_Jar_Open.h"
#include "Item_Drop.h"
#include "SoundMgr.h"

COpenJar::COpenJar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
	, m_pColliderCom(nullptr)
{
}

COpenJar::COpenJar(const COpenJar& rhs)
	: CDynamicObject(rhs)
	, m_pColliderCom(rhs.m_pColliderCom)
	, m_bHit(rhs.m_bHit)
{
}

COpenJar::~COpenJar()
{
	Free();
}

HRESULT COpenJar::Ready_GameObject(void)
{
	Add_Component();

	return S_OK;
}

_int COpenJar::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_bJarOpen && m_fSpeed > 17.f && m_bDropItem)
		return OBJ_DEAD;

	if (m_bJarOpen && m_fSpeed > 8.f && !m_bCreateEffect)
	{
		m_bCreateEffect = true;
		Create_Open_Effect();
	}

	if (m_bJarOpen && m_fSpeed > 13.f && !m_bDropItem)
	{
		m_bDropItem = true;
		_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
		//vPos.y -= 3.f;
		CItem_Drop* pItem = CItem_Drop::Create(m_pGraphicDev, vPos, 6);
		CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
		pGameLogicLayer->Add_BulletObject(pItem);
		return OBJ_DEAD;
	}

	Open_Jar(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	//Engine::Add_RenderGroup(RENDER_ALPHATEST, this);
	//CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_LANDSCAPE, this);

	return 0;
}

void COpenJar::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void COpenJar::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (-1 != m_byDrawID)
		m_pTextureCom[m_eID]->Set_Texture(m_byDrawID);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void COpenJar::Set_IsOpened(_bool bIsOpened)
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_INTERACTION);
	CSoundMgr::Get_Instance()->PlaySound(L"UI_jar open.wav", CSoundMgr::UI_INTERACTION, 0.9f);
	m_bJarOpen = bIsOpened;
}

COpenJar* COpenJar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	COpenJar* pInstance = new COpenJar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void COpenJar::Free(void)
{
	__super::Free();
}

HRESULT COpenJar::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Collider", pComponent });

	pComponent = m_pTextureCom[OPENJAR_BODY] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_TreasureJar_Rare_Body_"));
	if (nullptr == m_pTextureCom[OPENJAR_BODY]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Spr_TreasureJar_Rare_Body_", pComponent });

	pComponent = m_pTextureCom[OPENJAR_HEAD] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_TreasureJar_Rare_Head_"));
	if (nullptr == m_pTextureCom[OPENJAR_HEAD]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Spr_TreasureJar_Rare_Head_", pComponent });


	return S_OK;
}

void COpenJar::Open_Jar(const _float& fTimeDelta)
{
	if (m_bJarOpen && OPENJAR_HEAD == m_eID)
	{
		m_fSpeed += 20.f * fTimeDelta;
		m_pTransformCom->m_vInfo[INFO_POS].y += m_fSpeed * fTimeDelta;
	}
}

void COpenJar::Create_Open_Effect(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Jar_Open* pEffect = CEffect_Jar_Open::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
