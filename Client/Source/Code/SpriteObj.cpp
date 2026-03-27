#include "stdafx.h"
#include "..\Header\SpriteObj.h"
#include "Export_Function.h"
#include "Effect_Door_Fire.h"

CSpriteObj::CSpriteObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CSpriteObj::CSpriteObj(const CSpriteObj& rhs)
	: CDynamicObject(rhs)
{
}

CSpriteObj::~CSpriteObj()
{
}

HRESULT CSpriteObj::Ready_GameObject(void)
{
	__super::Ready_GameObject();

	

	

	return S_OK;
}

_int CSpriteObj::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_eID == DOOR_FIRE_NORMAL || m_eID == DOOR_FIRE_BOSS)
	{
		if (!m_bEffectMake)
		{
			if (m_bDoorActivated)
			{
				CGameObject* pEffect = CEffect_Door_Fire::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS] , _vec3(5.f, 5.f, 5.f) , 2.f);
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
				pLayer->Add_BulletObject(pEffect);
				
				m_bEffectMake = true;
			}
		}
	}



	if (!m_bFrameDefine)
	{
		if (m_eID == BUSH1 || m_eID == BUSH2 || m_eID == BUSH3 || m_eID == BUSH4)
		{
			m_fFrame = rand() % 700 / 100.f;
			m_fFrameSpeed = rand() % 50 / 100.f + 0.5f;
		}
		else
			m_fFrameSpeed = 1.f;

		
		m_bFrameDefine = true;
	}

	if (m_bSprite)
	{
		if (m_eID == DOOR_HEAD || m_eID == DOOR_PILLAR || m_eID == DOOR_PILLAR2 ||
			m_eID == DOOR_HEAD_NORMAL || m_eID == DOOR_PILLAR_NORMAL || m_eID == DOOR_PILLAR2_NORMAL || m_eID == DOOR_SIDEDOOR ||
			m_eID == DOOR_FIRE_NORMAL || m_eID == DOOR_FIRE_BOSS)
		{
			if(m_bDoorActivated)
				m_fFrame += m_pTextureCom[m_eID]->Get_MaxFrameCnt() * fTimeDelta * m_fFrameSpeed;
		}
		else
 			m_fFrame += m_pTextureCom[m_eID]->Get_MaxFrameCnt() * fTimeDelta * m_fFrameSpeed;
	}
	
	if (m_pTextureCom[m_eID]->Get_MaxFrameCnt()-1.f < m_fFrame)
	{
		if (m_eID == DOOR_HEAD || m_eID == DOOR_PILLAR || m_eID == DOOR_PILLAR2 ||
			m_eID == DOOR_HEAD_NORMAL || m_eID == DOOR_PILLAR_NORMAL || m_eID == DOOR_PILLAR2_NORMAL || m_eID == DOOR_SIDEDOOR ||
			m_eID == HISTORIC_SHINING1 || m_eID == HISTORIC_SHINING2 || m_eID == HISTORIC_SHINING3 || m_eID == OPENJAR_CENTER ||
			m_eID == TEMPLE_BODY || m_eID == TEMPLE_HEAD)
		{
			m_fFrame = m_pTextureCom[m_eID]->Get_MaxFrameCnt() - 1.f;
			m_fFrameSpeed *= -1.f;
		}
		else
			m_fFrame = 0.f;
	}

	if (m_fFrame < 0.f)
	{
		m_fFrame = 0.f;
		m_fFrameSpeed *= -1.f;
	}


	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CSpriteObj::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CSpriteObj::Render_GameObject()
{
	if (m_eID == DOOR_FIRE_NORMAL || m_eID == DOOR_FIRE_BOSS)
	{
		if (!m_bDoorActivated)
			return;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom[m_eID]->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CSpriteObj* CSpriteObj::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSpriteObj* pInstance = new CSpriteObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSpriteObj::Free(void)
{
	__super::Free();
}
