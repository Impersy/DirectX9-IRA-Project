#include "stdafx.h"
#include "..\Header\Boss_Bar_Hp_Delay.h"
#include "Export_Function.h"

#include "Doewole_Body.h"
#include "Docheol.h"

CBoss_Bar_Hp_Delay::CBoss_Bar_Hp_Delay(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev), m_iBarHp(0), m_iReceiveHp(0), m_bCount(false), m_fDelay(0)
{
	//m_tINFO[0]->iMax = 5;
}

CBoss_Bar_Hp_Delay::CBoss_Bar_Hp_Delay(const CBoss_Bar_Hp_Delay& rhs)
	: CFrontUi(rhs), m_iBarHp(0), m_iReceiveHp(0), m_bCount(false), m_fDelay(0)
{
}

CBoss_Bar_Hp_Delay::~CBoss_Bar_Hp_Delay()
{
}

HRESULT CBoss_Bar_Hp_Delay::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;

	return S_OK;
}

_int CBoss_Bar_Hp_Delay::Update_GameObject(const _float& fTimeDelta)
{	
	if (!m_bBarCheck)
	{
		if (m_eBoss_Name == BOSS_DOEWOLE)
		{
			m_iBarHp = DOEWOLE_MAXHP;
		}

		if (m_eBoss_Name == BOSS_DOCHEOL)
		{
			m_iBarHp = DOCHEOL_MAXHP;
		}
		m_bBarCheck = true;
	}

	Frame_Check(fTimeDelta);

	if (m_eBoss_Name == BOSS_DOEWOLE)
	{
		CDoewole_Body* pMonster = dynamic_cast<CDoewole_Body*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Doewole_Body"));
		if (nullptr != pMonster)
		{
			m_iReceiveHp = pMonster->Get_CurHp();
		}
		else if (pMonster == nullptr)
		{
			m_bDead = true;
		}
	}
	else if (m_eBoss_Name == BOSS_DOCHEOL)
	{
		CDocheol* pMonster = dynamic_cast<CDocheol*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Docheol"));
		if (nullptr != pMonster)
		{
			m_iReceiveHp = pMonster->Get_CurHp();
		}
		else if (pMonster == nullptr)
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

	if (0 == (_uint)m_fDelay && (false == m_bCount))
		m_bCount = true;

	if (m_iBarHp > m_iReceiveHp && m_bCount)
	{
		if ((m_iBarHp - m_iReceiveHp) > 2000)
		{
			m_iBarHp -= 2000;
			m_bCount = false;
		}
		else if ((m_iBarHp - m_iReceiveHp) < 500)
		{	
			m_iBarHp -= ((m_iBarHp - m_iReceiveHp));
			m_bCount = false;
		}
		else
		{
			m_iBarHp -= 500;
			m_bCount = false;
		}
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBoss_Bar_Hp_Delay::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CBoss_Bar_Hp_Delay::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 120, 120, 120));
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_BossBar_Print(4, m_eBoss_Name, m_iBarHp); //서순 주의

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBoss_Bar_Hp_Delay::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_BossBar_Hp"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_BossBar_Hp", pComponent });

	return S_OK;
}

void CBoss_Bar_Hp_Delay::Change_State(void)
{

}

void CBoss_Bar_Hp_Delay::Frame_Check(const _float& fTimeDelta)
{
	m_fDelay += 2.f * fTimeDelta;

	if (8.f < m_fDelay)
	{
		m_fDelay = 0.f;
	}
}

void CBoss_Bar_Hp_Delay::Boss_Dead_Delay(const _float& fTimeDelta)
{
	m_fDelay_Dead += m_fDelay_Dead_Max * fTimeDelta;

	if (m_fDelay_Dead > m_fDelay_Dead_Max)
	{
		m_fDelay_Dead = m_fDelay_Dead_Max;
	}
}

void CBoss_Bar_Hp_Delay::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBoss_Bar_Hp_Delay* CBoss_Bar_Hp_Delay::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber, BOSS_NAME _Name)
{
	CBoss_Bar_Hp_Delay* pInstance = new CBoss_Bar_Hp_Delay(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;
	pInstance->m_eBoss_Name = _Name;

	return pInstance;
}

void CBoss_Bar_Hp_Delay::Free(void)
{
	__super::Free();
}
