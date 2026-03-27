#include "stdafx.h"
#include "..\Header\Story_Book.h"
#include "Export_Function.h"
#include "KeyMgr.h"

CStory_Book::CStory_Book(LPDIRECT3DDEVICE9 pGraphicDev)
	: CApostle_Skill_UI(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CStory_Book::CStory_Book(const CStory_Book& rhs)
	: CApostle_Skill_UI(rhs)
{
}

CStory_Book::~CStory_Book()
{
	Free();
}

HRESULT CStory_Book::Ready_GameObject(void)
{
	Add_Component();
	m_fFrame = 0;
	m_iAlpha = 50;

	g_UI_Render = false;

	g_Story_Book = true;
	
	return S_OK;
}

_int CStory_Book::Update_GameObject(const _float& fTimeDelta)
{	

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	

	m_iAlpha += 7.f;

	if (m_iAlpha > 255.f) {
		m_iAlpha = 255.f;
	}

	if (CKeyMgr::Get_Instance()->Key_Down(KEY_E)) {
		m_fFrame += 1;

		if (m_fFrame > 5.f) {
			m_fFrame = 0.f;
			return OBJ_DEAD;
		}
	}

	
	Engine::Add_RenderGroup(RENDER_UI, this);

	__super::Update_GameObject(Time);

	return 0;
}

void CStory_Book::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CStory_Book::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pTextureCom->Set_Texture((int)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Apostle_Skill_Time_illust(1200 * 0.53f, 885 * 0.53f, { 0.f,0.f,0.f });

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CStory_Book::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Story_Book"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Story_Book", pComponent });

	return S_OK;
}

void CStory_Book::Change_State(void)
{

}

void CStory_Book::Frame_Check(const _float& fTimeDelta)
{

}

void CStory_Book::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CStory_Book* CStory_Book::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float size)
{
	CStory_Book* pInstance = new CStory_Book(pGraphicDev);

	pInstance->m_Size = size;
	pInstance->m_InitSize = size;


	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

void CStory_Book::Free(void)
{
	g_UI_Render = true;
	g_Story_Book = false;
	__super::Free();
}
