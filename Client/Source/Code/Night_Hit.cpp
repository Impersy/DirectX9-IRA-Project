#include "stdafx.h"
#include "..\Header\Night_Hit.h"
#include "Export_Function.h"

CNight_Hit::CNight_Hit(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CNight_Hit::CNight_Hit(const CNight_Hit & rhs)
	: CEffect(rhs)
{
}

CNight_Hit::~CNight_Hit()
{
	Free();
}

HRESULT CNight_Hit::Ready_GameObject(void)
{
	Add_Component();

	
	m_pTransformCom->m_vScale = { 4.f * 5.f * PUBLIC_SCALE , 6.f * 5.f * PUBLIC_SCALE, 1.f };

	m_fMaxFrame = 15.f;
	m_vPos.y += 20.f;
	m_vPos.z += 2.f;



	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;

	return S_OK;
}

_int CNight_Hit::Update_GameObject(const _float& fTimeDelta)
{
	if (g_eCurScene == SCENE_BOSS2 && m_Is_Boss2_Apostle == true) {
		CTransform* pTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Docheol", L"Proto_Transform", ID_DYNAMIC));
		
		m_pTransformCom->m_vInfo[INFO_POS].y = pTransCom->m_vInfo[INFO_POS].y + 10.f;
		m_pTransformCom->m_vInfo[INFO_POS].z = pTransCom->m_vInfo[INFO_POS].z - 2.f;
		m_pTransformCom->m_vScale = { 4.f * 5.f * 1.5f * PUBLIC_SCALE , 6.f * 5.f * 1.5f * PUBLIC_SCALE, 1.f };
	}
	else if (g_eCurScene == SCENE_BOSS1 && m_Is_Boss2_Apostle == true) {
		CTransform* pTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));

		m_pTransformCom->m_vInfo[INFO_POS].y = pTransCom->m_vInfo[INFO_POS].y + 10.f;
		m_pTransformCom->m_vInfo[INFO_POS].z = pTransCom->m_vInfo[INFO_POS].z - 2.f;
		m_pTransformCom->m_vScale = { 4.f * 5.f * 1.5f * PUBLIC_SCALE , 6.f * 5.f * 1.5f * PUBLIC_SCALE, 1.f };
		
	}
		


	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;

	m_fFrame += m_fMaxFrame * Time * 3.0f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CNight_Hit::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CNight_Hit::Render_GameObject()
{

	__super::Render_GameObject();
}

HRESULT CNight_Hit::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Night_Hit"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Night_Hit", pComponent });

	return S_OK;
}

CNight_Hit* CNight_Hit::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CNight_Hit* pInstance = new CNight_Hit(pGraphicDev);

	pInstance->m_vPos = vPos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNight_Hit::Free(void)
{
	__super::Free();
}


