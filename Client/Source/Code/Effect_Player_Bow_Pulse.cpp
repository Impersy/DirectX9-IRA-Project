#include "stdafx.h"
#include "..\Header\Effect_Player_Bow_Pulse.h"
#include "Export_Function.h"
#include "SylphBow.h"


CEffect_Player_Bow_Pulse::CEffect_Player_Bow_Pulse(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Player_Bow_Pulse::CEffect_Player_Bow_Pulse(const CEffect_Player_Bow_Pulse & rhs)
	: CEffect(rhs)
{
}

CEffect_Player_Bow_Pulse::~CEffect_Player_Bow_Pulse()
{
	Free();
}

HRESULT CEffect_Player_Bow_Pulse::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 0.95f , 1.98f, 1.f };

	m_pTransformCom->m_vScale = Scale * 2.0f * PUBLIC_SCALE;

	m_fMaxFrame = 9.f;
;

	return S_OK;
}

_int CEffect_Player_Bow_Pulse::Update_GameObject(const _float& fTimeDelta)
{

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;


	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	_vec3 Axis = { -1.f,0.f,0.f };
	D3DXVec3Normalize(&Axis, &Axis);

	_vec3 Dir = { (float)(ptCursor.x - WINCX * 0.5),float(ptCursor.y - WINCY * 0.5),0 };
	D3DXVec3Normalize(&Dir, &Dir);


	_vec3 EffectDir = { Dir.x,0.f,-Dir.y };
	

	CSylphBow* pSylphBow = dynamic_cast<CSylphBow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"SylphBow"));

	_vec3 vPos = pSylphBow->m_vPos;

	vPos += EffectDir * 5.f * PUBLIC_SCALE;

	m_pTransformCom->Set_Pos(vPos.x, vPos.y - 2.f, vPos.z);





	m_fFrame += m_fMaxFrame * Time * 1.5f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CEffect_Player_Bow_Pulse::LateUpdate_GameObject()
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

void CEffect_Player_Bow_Pulse::Render_GameObject()
{

	__super::Render_GameObject();
}

HRESULT CEffect_Player_Bow_Pulse::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Effect_BowPulse_Sylph"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Effect_BowPulse_Sylph", pComponent });

	return S_OK;
}

CEffect_Player_Bow_Pulse* CEffect_Player_Bow_Pulse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Player_Bow_Pulse* pInstance = new CEffect_Player_Bow_Pulse(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Player_Bow_Pulse::Free(void)
{
	__super::Free();
}


