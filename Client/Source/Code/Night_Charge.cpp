#include "stdafx.h"
#include "..\Header\Night_Charge.h"
#include "Export_Function.h"
#include "Player.h"
#include "SylphBow.h"

CNight_Charge::CNight_Charge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CNight_Charge::CNight_Charge(const CNight_Charge & rhs)
	: CEffect(rhs)
{
}

CNight_Charge::~CNight_Charge()
{
	Free();
}

HRESULT CNight_Charge::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 6.f , 6.f, 1.f };

	m_pTransformCom->m_vScale = Scale * PUBLIC_SCALE;

	m_fFrame = 0.f;
	m_fMaxFrame = 20.f;

	

	return S_OK;
}

_int CNight_Charge::Update_GameObject(const _float& fTimeDelta)
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

	vPos += EffectDir * 6.f * PUBLIC_SCALE;

	m_pTransformCom->Set_Pos(vPos.x, vPos.y - 2.f, vPos.z);




	m_fFrame += m_fMaxFrame * Time * 1.5f;

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0.f;
	}


	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CNight_Charge::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CNight_Charge::Render_GameObject()
{

	__super::Render_GameObject();
}

HRESULT CNight_Charge::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Bow_Night_Charge"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Bow_Night_Charge", pComponent });

	return S_OK;
}

CNight_Charge* CNight_Charge::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNight_Charge* pInstance = new CNight_Charge(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNight_Charge::Free(void)
{
	__super::Free();
}


