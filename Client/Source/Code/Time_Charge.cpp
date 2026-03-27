#include "stdafx.h"
#include "..\Header\Time_Charge.h"
#include "Export_Function.h"
#include "Player.h"
#include "SylphBow.h"

CTime_Charge::CTime_Charge(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CTime_Charge::CTime_Charge(const CTime_Charge & rhs)
	: CEffect(rhs)
{
}

CTime_Charge::~CTime_Charge()
{
	Free();
}

HRESULT CTime_Charge::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 6.f , 6.f, 1.f };

	m_pTransformCom->m_vScale = Scale * PUBLIC_SCALE;

	m_fFrame = 0.f;
	m_fMaxFrame = 14.f;

	

	return S_OK;
}

_int CTime_Charge::Update_GameObject(const _float& fTimeDelta)
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

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0.f;
	}


	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CTime_Charge::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CTime_Charge::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	m_pTextureCom->Set_Texture((_int)m_fFrame);


	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
}

HRESULT CTime_Charge::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Bow_Time_Charge"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Bow_Time_Charge", pComponent });

	return S_OK;
}

CTime_Charge* CTime_Charge::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTime_Charge* pInstance = new CTime_Charge(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTime_Charge::Free(void)
{
	__super::Free();
}


