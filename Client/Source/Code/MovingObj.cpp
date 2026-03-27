#include "stdafx.h"
#include "..\Header\MovingObj.h"
#include "Export_Function.h"

CMovingObj::CMovingObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CMovingObj::CMovingObj(const CMovingObj& rhs)
	: CDynamicObject(rhs)
{
}

CMovingObj::~CMovingObj()
{
}

HRESULT CMovingObj::Ready_GameObject(void)
{
	__super::Ready_GameObject();

	m_vDir = { 0.f , 1.f , 0.f };

	return S_OK;
}

_int CMovingObj::Update_GameObject(const _float& fTimeDelta)
{	
	if (!m_bInit)
	{
		if (FLOATING == m_eID)
		{
			m_fSpeed = (rand() % 40 - 20) / 10.f;
			m_fAccTime = rand() % 20 / 10.f;
		}
		else if (HISTORIC_FLOATING == m_eID)
		{
			m_fInitSpeed = 3.f;
			m_fSpeed = m_fInitSpeed;
		}
		else if (TEMPLE1 == m_eID)
		{
			m_fSpeed = 1.f;
			m_vOriginPos = m_pTransformCom->m_vInfo[INFO_POS];

			if (!lstrcmp(m_pObjKey, L"Temple_0"))
				m_vMovedOriginPos = { 384.76f, 18.2f, 391.80f };
			else if (!lstrcmp(m_pObjKey, L"Temple_0|Clone0"))
				m_vMovedOriginPos = { 392.60f, 18.2f, 391.80f };
			else if (!lstrcmp(m_pObjKey, L"Temple_1"))
				m_vMovedOriginPos = { 384.86f, 23.7f, 393.00f };
			else if (!lstrcmp(m_pObjKey, L"Temple_1|Clone0"))
				m_vMovedOriginPos = { 392.41f, 23.7f, 393.00f };

			m_vDir = m_vOriginPos - m_vMovedOriginPos;
			D3DXVec3Normalize(&m_vDir, &m_vDir);
		}

		m_bInit = true;
	}

	if (m_bActivated)
	{
		if (FLOATING == m_eID || TEMPLE1 == m_eID)
			m_fAccTime += fTimeDelta;

		if (FLOATING == m_eID && m_fAccTime > 2.f)
		{
			m_fAccTime = 0.f;
			m_vDir *= -1.f;
		}

		if (TEMPLE1 == m_eID && !m_bInitialMove && m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;
			m_vDir *= -1.f;
		}

		if (HISTORIC_FLOATING == m_eID)
		{
			if (!m_bDown && 0.f > m_fSpeed)
			{
				m_fSpeed = -m_fInitSpeed;
				m_bDown = true;
			}
			else if (m_bDown && 0.f < m_fSpeed)
			{
				m_fSpeed = m_fInitSpeed;
				m_bDown = false;
				m_pTransformCom->m_vInfo[INFO_POS] = m_vOriginPos;
			}

		}

		if (TEMPLE1 == m_eID && m_bInitialMove)
		{
			if (D3DXVec3Length(&(m_vMovedOriginPos - m_pTransformCom->m_vInfo[INFO_POS])) < 3.f)
			{
				m_bInitialMove = false;

				if (!lstrcmp(m_pObjKey, L"Temple_0"))
					m_vDir = { -1.f, -1.f, 0.f };
				else if (!lstrcmp(m_pObjKey, L"Temple_0|Clone0"))
					m_vDir = { 1.f, -1.f, 0.f };
				else if (!lstrcmp(m_pObjKey, L"Temple_1"))
					m_vDir = { -1.f, 1.f, 0.f };
				else if (!lstrcmp(m_pObjKey, L"Temple_1|Clone0"))
					m_vDir = { 1.f, 1.f, 0.f };
				D3DXVec3Normalize(&m_vDir, &m_vDir);
			}
		}

		if (FLOATING == m_eID || (TEMPLE1 == m_eID))
			m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;
		else if (HISTORIC_FLOATING == m_eID)
		{
			if (!m_bDown)
			{
				m_fSpeed -= 4.f * fTimeDelta;
			}
			else
			{
				m_fSpeed += 4.f * fTimeDelta;
			}

			m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;
		}
	}

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CMovingObj::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMovingObj::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (-1 != m_byDrawID)
		m_pTextureCom[m_eID]->Set_Texture(m_byDrawID);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CMovingObj* CMovingObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMovingObj* pInstance = new CMovingObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMovingObj::Free(void)
{
	__super::Free();
}

HRESULT CMovingObj::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	if (SCENE_BOSS2 == g_eCurScene)
	{
		pComponent = m_pTextureCom[FLOATING] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"DocheolObj"));
		if (nullptr == m_pTextureCom[FLOATING]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"DocheolObj", pComponent });
	}
	else if (SCENE_REGEN == g_eCurScene)
	{
		pComponent = m_pTextureCom[TEMPLE1] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Temple_"));
		if (nullptr == m_pTextureCom[TEMPLE1]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"Temple_", pComponent });
	}
	else if (SCENE_POT == g_eCurScene)
	{
		pComponent = m_pTextureCom[HISTORIC_FLOATING] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"HistoricSitesObj4_"));
		if (nullptr == m_pTextureCom[HISTORIC_FLOATING]) return E_FAIL;
		m_uMapComponent[ID_STATIC].insert({ L"HistoricSitesObj4_", pComponent });
	}


	return S_OK;
}

void CMovingObj::SetUp_OnTerrain(void)
{
}
