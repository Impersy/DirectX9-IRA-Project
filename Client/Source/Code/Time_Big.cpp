#include "stdafx.h"
#include "..\Header\Time_Big.h"
#include "Export_Function.h"
#include "TimeArrow.h"
#include <SoundMgr.h>
CTime_Big::CTime_Big(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CTime_Big::CTime_Big(const CTime_Big & rhs)
	: CEffect(rhs)
{
}

CTime_Big::~CTime_Big()
{
	Free();
}

HRESULT CTime_Big::Ready_GameObject(void)
{
	Add_Component();

	
	m_pTransformCom->m_vScale = { 3.f * 2.5f * PUBLIC_SCALE , 3.f * 2.5f * PUBLIC_SCALE, 1.f };


	m_fFrame = 0.f;
	m_fMaxFrame = 17.f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TIME_CHARGE_FIRE2);
	CSoundMgr::Get_Instance()->PlaySound(L"Time_Fire.wav", CSoundMgr::TIME_CHARGE_FIRE2, 0.95f);

	
	return S_OK;
}

_int CTime_Big::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;


	m_fAccTime += 2.f * Time * 1.f;



	m_fFrame += m_fMaxFrame * Time * 1.0f;
	

	m_Alert_Time += 2.f * Time * 0.5f;

	if (m_Alert_Time > 2.f) {
		m_Alert_Time = 0.f;
		m_Is_Alert = true;
	}

	if (m_Is_Alert == true) {

		CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

		CGameObject* pGameObject = nullptr;


		pGameObject = CTimeArrow::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);


		pGameLogicLayer->Add_BulletObject(pGameObject);

		
		m_Is_Alert = false;
	}






	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CTime_Big::LateUpdate_GameObject()
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

void CTime_Big::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	
	m_pTextureCom->Set_Texture((_uint)m_fFrame);


	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
}

HRESULT CTime_Big::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Bow_Time_Pulse"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Bow_Time_Pulse", pComponent });

	return S_OK;
}

CTime_Big* CTime_Big::Create(LPDIRECT3DDEVICE9 pGraphicDev,  _vec3 vPos)
{
	CTime_Big* pInstance = new CTime_Big(pGraphicDev);

	
	pInstance->m_vPos = vPos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTime_Big::Free(void)
{
	__super::Free();
}


