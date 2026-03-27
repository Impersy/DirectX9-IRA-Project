#include "stdafx.h"
#include "..\Header\Boss_Bar_Text.h"
#include "Export_Function.h"

#include "Doewole_Body.h"
#include "Docheol.h"

CBoss_Bar_Text::CBoss_Bar_Text(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CBoss_Bar_Text::CBoss_Bar_Text(const CBoss_Bar_Text& rhs)
	: CFrontUi(rhs)
{
}

CBoss_Bar_Text::~CBoss_Bar_Text()
{
}

HRESULT CBoss_Bar_Text::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CBoss_Bar_Text::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_eBoss_Name == BOSS_DOEWOLE)
	{
		CDoewole_Body* pMonster = dynamic_cast<CDoewole_Body*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Doewole_Body"));
		if (pMonster == nullptr)
		{
			m_bDead = true;
		}
	}
	else if (m_eBoss_Name == BOSS_DOCHEOL)
	{
		CDocheol* pMonster = dynamic_cast<CDocheol*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Doewole_Body"));
		if (pMonster == nullptr)
		{
			m_bDead = true;
		}
	}
	if (m_bDead)
	{
		Boss_Dead_Delay(fTimeDelta);
	}
	if (m_bDead && (m_fDelay_Dead == m_fDelay_Dead_Max))
	{
		return OBJ_DEAD;
	}
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBoss_Bar_Text::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBoss_Bar_Text::Render_GameObject()
{	
	_tchar				m_szString_Temp[128] = L"도 올";
	_tchar				m_szString_Temp_2[128] = L"[ 탐욕의 거대 사념체 ]";
	_tchar				m_szString_Temp_3[128] = L"[보스]";
	_tchar				m_szString_Temp_4[128] = L"[식물형]";
	Engine::Render_Font(L"Font_Title", m_szString_Temp, &_vec2(814.f, 26.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Engine::Render_Font(L"Font_Title_2", m_szString_Temp_2, &_vec2(759.f, 54.f), D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.f));
	Engine::Render_Font(L"Font_Title_2", m_szString_Temp_3, &_vec2(616.f, 54.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	Engine::Render_Font(L"Font_Title_2", m_szString_Temp_4, &_vec2(616.f, 99.f), D3DXCOLOR(0.66f, 0.66f, 0.f, 1.f));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	//m_pTransformCom->Ui_BossBar_Print(3); //서순 주의

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBoss_Bar_Text::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_BossBar_Icon_1"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_BossBar_Icon_1", pComponent });

	return S_OK;
}

void CBoss_Bar_Text::Change_State(void)
{

}

void CBoss_Bar_Text::Frame_Check(const _float& fTimeDelta)
{

}

void CBoss_Bar_Text::Boss_Dead_Delay(const _float& fTimeDelta)
{
	m_fDelay_Dead += m_fDelay_Dead_Max * fTimeDelta;

	if (m_fDelay_Dead > m_fDelay_Dead_Max)
	{
		m_fDelay_Dead = m_fDelay_Dead_Max;
	}
}

void CBoss_Bar_Text::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBoss_Bar_Text* CBoss_Bar_Text::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber, BOSS_NAME _Name)
{
	CBoss_Bar_Text* pInstance = new CBoss_Bar_Text(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;
	pInstance->m_eBoss_Name = _Name;

	return pInstance;
}

void CBoss_Bar_Text::Free(void)
{
	__super::Free();
}
