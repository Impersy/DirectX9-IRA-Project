#include "stdafx.h"
#include "..\Header\Night_Pulse.h"
#include "Export_Function.h"
#include "SylphBow.h"


CNight_Pulse::CNight_Pulse(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CNight_Pulse::CNight_Pulse(const CNight_Pulse & rhs)
	: CEffect(rhs)
{
}

CNight_Pulse::~CNight_Pulse()
{
	Free();
}

HRESULT CNight_Pulse::Ready_GameObject(void)
{
	Add_Component();



	m_fMaxFrame = 17.f;
;

	return S_OK;
}

_int CNight_Pulse::Update_GameObject(const _float& fTimeDelta)
{

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;


	
	Update_State();



	m_fFrame += m_fMaxFrame * Time * 1.5f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CNight_Pulse::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}


	Compute_ViewZ(&m_vPos);
}

void CNight_Pulse::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_MatWorld);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

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

void CNight_Pulse::Update_State()
{
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	pPlayerTransformCom->Get_Info(INFO_POS, &m_Bow_Pos);

	_vec3       Player_Axis = { 0.f,1.f,0.f };

	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);

	_vec3 Axis = { -1.f,0.f,0.f };

	_vec3 Dir = { (float)(ptCursor.x - WINCX * 0.5),float(ptCursor.y - WINCY * 0.5),0 };
	D3DXVec3Normalize(&Dir, &Dir);


	m_Bow_Angle = acos(D3DXVec3Dot(&Axis, &Dir));

	if (WINCY * 0.5 < ptCursor.y)
		m_Bow_Angle = 2.f * D3DX_PI - m_Bow_Angle;


	_matrix matScale, matTrans_Pre, matRot, matTrans, matRev;

	D3DXMatrixIdentity(&m_MatWorld);

	D3DXMatrixScaling(&matScale, m_Size.x, m_Size.y, m_Size.z);
	D3DXMatrixRotationY(&matRot, 220);
	D3DXMatrixTranslation(&matTrans_Pre, m_Dis * PUBLIC_SCALE, 0.f, -0.4f);
	D3DXMatrixTranslation(&matTrans, m_Bow_Pos.x , m_Bow_Pos.y + m_OffSet.y, m_Bow_Pos.z );


	m_vPos = { m_Bow_Pos.x + m_OffSet.x, m_Bow_Pos.y + m_OffSet.y, m_Bow_Pos.z + m_OffSet.z };


	Axis = { 1.f,0.f,0.f };

	m_Bow_Angle = acos(D3DXVec3Dot(&Axis, &Dir));

	if (WINCY * 0.5 < ptCursor.y)
		m_Bow_Angle = 2.f * D3DX_PI - m_Bow_Angle;

	D3DXMatrixRotationAxis(&matRev, &Player_Axis, -m_Bow_Angle);


	m_MatWorld = matScale * matRot * matTrans_Pre * matRev * matTrans;

	m_Pre_Bow_Angle = m_Bow_Angle;

}

HRESULT CNight_Pulse::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Bow_Night_Pulse"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Bow_Night_Pulse", pComponent });

	return S_OK;
}

CNight_Pulse* CNight_Pulse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNight_Pulse* pInstance = new CNight_Pulse(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CNight_Pulse::Free(void)
{
	__super::Free();
}


