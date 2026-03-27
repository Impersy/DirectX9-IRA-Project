#include "stdafx.h"
#include "..\Header\Main_Lobby_Book.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"
#include "Interaction_Button.h"
#include "KeyMgr.h"
#include "Story_Book.h"


CMain_Lobby_Book::CMain_Lobby_Book(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CMain_Lobby_Book::CMain_Lobby_Book(const CMain_Lobby_Book & rhs)
	: CEffect(rhs)
{
}

CMain_Lobby_Book::~CMain_Lobby_Book()
{
	Free();
}

HRESULT CMain_Lobby_Book::Ready_GameObject(void)
{
	Add_Component();



	m_iAlpha = 255;

	m_fFrame = 0.f;
	m_fMaxFrame = 3.f;

	_vec3 Scale = { 5.f * 1.f , 5.f * 1.f, 1.f };

	m_pTransformCom->m_vScale = Scale * 2.f;

	m_pTransformCom->Set_Pos(383.f, 8.f, 384.f);

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));




	return S_OK;
}

_int CMain_Lobby_Book::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;


	if (!m_bTest)
	{

		//Create_Active_Button_Effect();

		m_bTest = true;
	}


	// 플레이어 Pos 받아 MasterPos와 구충돌
	_vec3 vPlayerPos;
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	_float x = vPlayerPos.x;
	_float z = vPlayerPos.z;
	_float radius1 = 3.0f;

	_float x1 = m_pTransformCom->m_vInfo[INFO_POS].x;
	_float z1 = m_pTransformCom->m_vInfo[INFO_POS].z;
	_float radius2 = 3.0f;

	_float dx = x - x1;
	_float dz = z - z1;
	_float distance = sqrt(dx * dx + dz * dz);

	if (distance <= radius1 + radius2)
	{
		if (g_Story_Book == false) {

			if (CKeyMgr::Get_Instance()->Key_Down(KEY_E)) {

				CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

				CGameObject* pGameObject = nullptr;

				pGameObject = CStory_Book::Create(m_pGraphicDev, 1200);
				
				pLayer->Add_GameObject(L"Story_Book", pGameObject);

			}
		}
	}


	
	m_fFrame += m_fMaxFrame * Time * 3.f;

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0.f;
	}


	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CMain_Lobby_Book::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CMain_Lobby_Book::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((int)m_fFrame);
	

	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMain_Lobby_Book::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_MainLobby_Book"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_MainLobby_Book", pComponent });



	return S_OK;
}

void CMain_Lobby_Book::Create_Active_Button_Effect(void)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_UI");

	CGameObject* pGameObject = nullptr;
	pGameObject = CInteraction_Button::Create(m_pGraphicDev, TARGET_APOSTLE_TIME, m_pTransformCom->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pLayer->Add_BulletObject(pGameObject);
}

CMain_Lobby_Book* CMain_Lobby_Book::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMain_Lobby_Book* pInstance = new CMain_Lobby_Book(pGraphicDev);

	

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMain_Lobby_Book::Free(void)
{
	__super::Free();
}


