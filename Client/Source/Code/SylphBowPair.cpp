#include "stdafx.h"
#include "..\Header\SylphBowPair.h"
#include "Export_Function.h"
#include "Player.h"

CSylphBowPair::CSylphBowPair(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBow(pGraphicDev)
{
}

CSylphBowPair::CSylphBowPair(const CSylphBowPair& rhs)
	: CBow(rhs)
{
}

CSylphBowPair::~CSylphBowPair()
{
	Free();
}

HRESULT CSylphBowPair::Ready_GameObject(void)
{
	Add_Component();

	

	m_pTransformCom->Rotation(ROT_Y, m_Bow_Angle);

	

	m_pTransformCom->Set_Pos(m_Bow_Pos.x, m_Bow_Pos.y, m_Bow_Pos.z);

	


	
	__super::Ready_GameObject();

	return S_OK;
}

_int CSylphBowPair::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;

	if (m_bRender == true)
		m_bRealRender = true;

	if (m_bRender == false) {
		m_Time += 3.f * Time * 0.55f;
		if (m_Time > 1.f) {
			m_Time = 0.f;
			m_bRealRender = false;
		}
	}


	if (m_bRealRender == false) {

		m_iAlpha -= 7;

		if (m_iAlpha < 0) {
			m_iAlpha = 0;
		}
	}
	else {
		m_iAlpha = 255;
	}


	if (m_bRender == true) {
		if (m_bCharge == true) {
			m_fFrame += 2.f * Time * 1.f * 1.5f;
			if (m_fFrame > 2.f) {
				m_fFrame = 2.f;
			}
		}
		else {
			m_fFrame += 2.f * Time * 2.f;
			if (m_fFrame > 2.f) {
				m_fFrame = 0.f;
			}
		}
	}
	else {
		m_fFrame = 0.f;
	}

	Update_Bow_State();




	Engine::Add_RenderGroup(RENDER_ALPHA, this);


	__super::Update_GameObject(Time);

	return 0;
}

void CSylphBowPair::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	Compute_ViewZ(&m_vPos);
}

void CSylphBowPair::Render_GameObject()
{

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	if (pPlayer->m_Bow_Type != BOW_SYLPH) {
		return;
	}


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

	m_pTextureCom[BOW_SYLPH]->Set_Texture((_int)m_fFrame);

	

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);



	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CSylphBowPair::Update_Bow_State()
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

	D3DXMatrixScaling(&matScale, -2.6f * PUBLIC_SCALE, 2.6f * PUBLIC_SCALE, 1.f);
	D3DXMatrixRotationY(&matRot, 220);
	D3DXMatrixTranslation(&matTrans_Pre, 3.f * PUBLIC_SCALE, 0.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_Bow_Pos.x - 0.2f * PUBLIC_SCALE, m_Bow_Pos.y - 2.6f * PUBLIC_SCALE , m_Bow_Pos.z);


	m_vPos = { m_Bow_Pos.x , m_Bow_Pos.y - 2.0f, m_Bow_Pos.z };

	Axis = { 1.f,0.f,0.f };

	m_Bow_Angle = acos(D3DXVec3Dot(&Axis, &Dir));

	if (WINCY * 0.5 < ptCursor.y)
		m_Bow_Angle = 2.f * D3DX_PI - m_Bow_Angle;

	D3DXMatrixRotationAxis(&matRev, &Player_Axis, -m_Bow_Angle);


	_vec3 Dist = m_vPos - m_Bow_Pos;

	m_MatWorld = matScale * matRot * matTrans_Pre * matRev * matTrans;

	m_Pre_Bow_Angle = m_Bow_Angle;


}

HRESULT CSylphBowPair::Add_Component(void)
{

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom[BOW_SYLPH] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Bow_Sylph_Pair"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Bow_Sylph_Pair", pComponent });


	return S_OK;
}




CSylphBowPair* CSylphBowPair::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float Angle)
{
	CSylphBowPair* pInstance = new CSylphBowPair(pGraphicDev);

	if (pInstance != nullptr) {
		
		pInstance->m_Bow_Pos = vPos;
		pInstance->m_Bow_Angle = Angle;
	}

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

void CSylphBowPair::Free(void)
{
	__super::Free();
}