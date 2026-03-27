#include "stdafx.h"
#include "..\Header\SylphBow.h"
#include "Export_Function.h"
#include "Player.h"

CSylphBow::CSylphBow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBow(pGraphicDev)
{
}

CSylphBow::CSylphBow(const CSylphBow& rhs)
	: CBow(rhs)
{
}

CSylphBow::~CSylphBow()
{
	Free();
}

HRESULT CSylphBow::Ready_GameObject(void)
{
	Add_Component();

	

	__super::Ready_GameObject();

	return S_OK;
}

_int CSylphBow::Update_GameObject(const _float& fTimeDelta)
{

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;


	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	m_Bow_Type = pPlayer->m_Bow_Type;

	if (pPlayer->Get_Player_Info().Nowhp == 0)
		m_bRender = false;

	if (m_bDead)
		return OBJ_DEAD;

	if(m_bRender == true)
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



	if (m_Bow_Type == BOW_SYLPH) {
		Sylph_Frame(Time);
	}
	else if (m_Bow_Type == BOW_FIRE) {
		Fire_Frame(Time);
	}
	else if (m_Bow_Type == BOW_NIGHT) {
		Night_Frame(Time);
	}
	else if (m_Bow_Type == BOW_TIME) {
		Time_Frame(Time);
	}



	if (m_Bow_Type == BOW_SYLPH) {
		m_Size = { -2.6f * PUBLIC_SCALE,2.6f * PUBLIC_SCALE,1.f };
		m_OffSet = { -0.2f * PUBLIC_SCALE ,2.6f * PUBLIC_SCALE ,0.f };
		m_Dis = 3.f;
		m_Y_Set = 2.2f;
	}
	else if (m_Bow_Type == BOW_FIRE) {
		m_Size = { -4.27f * PUBLIC_SCALE,6.35f * PUBLIC_SCALE,1.f };
		m_OffSet = { -0.2f * PUBLIC_SCALE ,-0.3f * PUBLIC_SCALE ,0.f };
		m_Dis = 3.f;
		m_Y_Set = -2.4f;
	}
	else if (m_Bow_Type == BOW_NIGHT) {
		m_Size = { -5.12f * PUBLIC_SCALE,7.f * PUBLIC_SCALE,1.f };
		m_OffSet = { -0.2f * PUBLIC_SCALE ,-0.3f * PUBLIC_SCALE ,0.f };
		m_Dis = 4.f;
		m_Y_Set = -2.0f;
	}
	else if (m_Bow_Type == BOW_TIME) {
		m_Size = { -4.f * PUBLIC_SCALE,6.f * PUBLIC_SCALE,1.f };
		m_OffSet = { -0.2f * PUBLIC_SCALE ,-0.1f * PUBLIC_SCALE ,0.f };
		m_Dis = 3.f;
		m_Y_Set = -2.0f;
	}



	Update_Bow_State();


	Engine::Add_RenderGroup(RENDER_ALPHA, this);


	__super::Update_GameObject(Time);

	return 0;
}

void CSylphBow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	Compute_ViewZ(&m_vPos);
}

void CSylphBow::Render_GameObject()
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



	if (m_Bow_Type == BOW_SYLPH) {
		m_pTextureCom[BOW_SYLPH]->Set_Texture((_int)m_fFrame);
	}
	else if (m_Bow_Type == BOW_FIRE) {
		m_pTextureCom[BOW_FIRE]->Set_Texture((_int)m_fFireFrame);
	}
	else if (m_Bow_Type == BOW_NIGHT) {
		m_pTextureCom[BOW_NIGHT]->Set_Texture((_int)m_fNightFrame);
	}
	else if (m_Bow_Type == BOW_TIME) {
		m_pTextureCom[BOW_TIME]->Set_Texture((_int)m_fTimeFrame);
	}



	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CSylphBow::Update_Bow_State()
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
	D3DXMatrixTranslation(&matTrans_Pre, m_Dis * PUBLIC_SCALE, 0.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_Bow_Pos.x + m_OffSet.x, m_Bow_Pos.y + m_OffSet.y, m_Bow_Pos.z + m_OffSet.z);


	m_vPos = { m_Bow_Pos.x + m_OffSet.x, m_Bow_Pos.y + m_OffSet.y - m_Y_Set, m_Bow_Pos.z + m_OffSet.z };


	Axis = { 1.f,0.f,0.f };

	m_Bow_Angle = acos(D3DXVec3Dot(&Axis, &Dir));

	if (WINCY * 0.5 < ptCursor.y)
		m_Bow_Angle = 2.f * D3DX_PI - m_Bow_Angle;

	D3DXMatrixRotationAxis(&matRev, &Player_Axis, - m_Bow_Angle);
	

	m_MatWorld = matScale * matRot * matTrans_Pre * matRev * matTrans;

	m_Pre_Bow_Angle = m_Bow_Angle;
}

HRESULT CSylphBow::Add_Component(void)
{

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });


	pComponent = m_pTextureCom[BOW_SYLPH] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Bow_Sylph"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Bow_Sylph", pComponent });

	pComponent = m_pTextureCom[BOW_FIRE] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Bow_Fire"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Bow_Fire", pComponent });

	pComponent = m_pTextureCom[BOW_NIGHT] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Bow_Night"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Bow_Night", pComponent });

	pComponent = m_pTextureCom[BOW_TIME] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Bow_Time"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Bow_Time", pComponent });




	return S_OK;
}




CSylphBow* CSylphBow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float Angle)
{
	CSylphBow* pInstance = new CSylphBow(pGraphicDev);

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

void CSylphBow::Sylph_Frame(const _float& fTimeDelta)
{
	if (m_bRender == true) {
		if (m_bCharge == true) {

			m_fFrame += 2.f * fTimeDelta * 1.f * 1.5f;
			if (m_fFrame > 2.f) {
				m_fFrame = 2.f;
			}

		}
		else {
			m_fFrame += 2.f * fTimeDelta * 2.f;
			if (m_fFrame > 2.f) {
				m_fFrame = 0.f;
			}

		}
	}
	else {
		m_fFrame = 0.f;
	}
}

void CSylphBow::Fire_Frame(const _float& fTimeDelta)
{
	if (m_bRender == true) {
		if (m_bCharge == true) {

			m_fFireFrame += 11.f * fTimeDelta * 1.f;
			if (m_fFireFrame > 7.f) {
				m_fFireFrame = 7.f;
			}

		}
		else {
			m_fFireFrame += 11.f * fTimeDelta * 4.f;
			if (m_fFireFrame > 11.f) {
				m_fFireFrame = 0.f;
			}

		}
	}
	else {
		m_fFireFrame = 0.f;
	}
}

void CSylphBow::Night_Frame(const _float& fTimeDelta)
{
	if (m_bRender == true) {
		if (m_bCharge == true) {

			m_fNightFrame += 7.f * fTimeDelta * 1.f;
			if (m_fNightFrame > 4.f) {
				m_fNightFrame = 4.f;
			}

		}
		else {
			m_fNightFrame += 7.f * fTimeDelta * 4.f;
			if (m_fNightFrame > 7.f) {
				m_fNightFrame = 0.f;
			}

		}
	}
	else {
		m_fNightFrame = 0.f;
	}
}

void CSylphBow::Time_Frame(const _float& fTimeDelta)
{
	if (m_bRender == true) {
		if (m_bCharge == true) {

			m_fTimeFrame += 10.f * fTimeDelta * 1.f * 1.3f;
			if (m_fTimeFrame > 5.f) {
				m_fTimeFrame = 5.f;
			}

		}
		else {
			m_fTimeFrame += 10.f * fTimeDelta * 4.f;
			if (m_fTimeFrame > 10.f) {
				m_fTimeFrame = 0.f;
			}

		}
	}
	else {
		m_fTimeFrame = 0.f;
	}
}

void CSylphBow::Free(void)
{
	__super::Free();
}