#include "stdafx.h"
#include "..\Header\Doewole_LeftClaw.h"
#include "Export_Function.h"
#include "Effect_Doewole_ChargeCircle.h"
#include "DoewoleBullet_Standard.h"
#include "Effect_Doewole_Slam.h"
#include "Effect_AlertCircle.h"
#include <Effect_Doewole_Hurt.h>
#include <Effect_Doewole_Circle.h>
#include <Doewole_RightClaw.h>
#include <DoewoleBullet_StopGo.h>
#include <DoewoleBullet_Thorn.h>
#include <SoundMgr.h>
#include "StaticCamera.h"

CDoewole_LeftClaw::CDoewole_LeftClaw(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDoewole(pGraphicDev)

{
}

CDoewole_LeftClaw::CDoewole_LeftClaw(const CDoewole_LeftClaw& rhs)
	: CDoewole(rhs.m_pGraphicDev)
{
}

CDoewole_LeftClaw::~CDoewole_LeftClaw()
{

}

HRESULT CDoewole_LeftClaw::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vScale = { -20.f , 20.f , 1.f };

	return S_OK;
}

_int CDoewole_LeftClaw::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	State_Update(fTimeDelta);

	CBoss::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return 0;
}

void CDoewole_LeftClaw::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDoewole_LeftClaw::Render_GameObject()
{
	if (!m_bRender)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	switch (m_eCurState)
	{
	case CDoewole::IDLE:
		m_pTextureCom[STAND]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::MOVE:
		break;
	case CDoewole::STANDARD_ATTACK:
		m_pTextureCom[STANDARD_ATTACK]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::OUTSTRECTH_ATTACK:
		if (!m_bUp || m_bDown)
			m_pTextureCom[UP]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureCom[OUTSTRECTH_ATTACK]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::SMASH_ATTACK:
		if (!m_bUp)
			m_pTextureCom[UP]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureCom[SMASH]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::SCRATCH_ATTACK:
		if (!m_bWait)
			m_pTextureCom[SCRATCH]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureCom[STAND]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::AREA_ATTACK:
		if (!m_bUp)
			m_pTextureCom[UP]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureCom[SMASH]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::UPGRADE_SMASH_ATTACK:
		if (!m_bLSmashWait)
		{
			if(!m_bSmash)
				m_pTextureCom[UP]->Set_Texture((_uint)m_fFrame);
			else
				m_pTextureCom[SMASH]->Set_Texture((_uint)m_fFrame);
		}
		else
			m_pTextureCom[STAND]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::UPGRADE_SCRATCH_ATTACK:
		if (!m_bCrossScratch)
		{
			if (!m_bSmash)
			{
				if (!m_bWait)
					m_pTextureCom[SCRATCH]->Set_Texture((_uint)m_fFrame);
				else
					m_pTextureCom[STAND]->Set_Texture((_uint)m_fFrame);
			}
			else if (m_bSmash)
				m_pTextureCom[SMASH]->Set_Texture((_uint)m_fFrame);
		}
		else
			m_pTextureCom[CROSS_SCRATCH]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::BULLET_ACTIVATE_ATTACK:
		if (!m_bUp)
			m_pTextureCom[UP]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureCom[SMASH]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::BOSS_DEAD:
		m_pTextureCom[DEAD]->Set_Texture((_uint)m_fFrame);
		break;
	case CDoewole::STATE_END:
		break;
	default:
		break;
	}
	m_pBufferCom->Render_Buffer();

	//m_pSphereBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDoewole_LeftClaw::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom[STAND] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_Stand"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_Stand", pComponent });

	pComponent = m_pTextureCom[STANDARD_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_StandardAttack"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_StandardAttack", pComponent });

	pComponent = m_pTextureCom[OUTSTRECTH_ATTACK] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_OutStretchAttack"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_OutStretchAttack", pComponent });

	pComponent = m_pTextureCom[UP] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_Up"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_Up", pComponent });

	pComponent = m_pTextureCom[SMASH] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_Smash"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_Smash", pComponent });

	pComponent = m_pTextureCom[SCRATCH] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_Scratch"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_Scratch", pComponent });

	pComponent = m_pTextureCom[CROSS_SCRATCH] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_CrossScratch"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_CrossScratch", pComponent });

	pComponent = m_pTextureCom[DEAD] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Doewole_Claw_Death"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Doewole_Claw_Death", pComponent });

	return S_OK;
}

void CDoewole_LeftClaw::State_Update(const _float& fTimeDelta)
{
	m_eCurState = dynamic_cast<CDoewole*>(m_pOwner)->Get_State();

	if (m_eCurState != m_ePreState)
	{
		m_fFrame = 0.f;
		m_fAccTime = 0.f;

		m_bUp = false;
		m_bDown = false;

		m_iReplayCnt = 0;

		m_bSmash = false;
		m_bAlert = false;

		m_bWait = true;

		m_bHurt = false;

		m_bLSmashWait = false;
		m_bCircleEffect = false;
		m_bSmashEffect = false;
		m_bMakeThorn = false;
		m_bCrossScratch = false;

		for (int i = 0; i < 10; ++i)
		{
			m_bSound[i] = false;
		}

		m_fAccTime3 = 0.f;

	}

	switch (m_eCurState)
	{
	case CDoewole::IDLE:
		Idle(fTimeDelta);
		break;
	case CDoewole::MOVE:
		Move(fTimeDelta);
		break;
	case CDoewole::STANDARD_ATTACK:
		Standard_Attack(fTimeDelta);
		break;
	case CDoewole::OUTSTRECTH_ATTACK:
		OutStretch_Attack(fTimeDelta);
		break;
	case CDoewole::SMASH_ATTACK:
		Smash_Attack(fTimeDelta);
		break;
	case CDoewole::SCRATCH_ATTACK:
		Scratch_Attack(fTimeDelta);
		break;
	case CDoewole::AREA_ATTACK:
		Area_Attack(fTimeDelta);
		break;
	case CDoewole::UPGRADE_SMASH_ATTACK:
		Upgrade_Smash_Attack(fTimeDelta);
		break;
	case CDoewole::UPGRADE_SCRATCH_ATTACK:
		Upgrade_Scratch_Attack(fTimeDelta);
		break;
	case CDoewole::BULLET_ACTIVATE_ATTACK:
		Bullet_Activate_Attack(fTimeDelta);
		break;
	case CDoewole::BOSS_DEAD:
		Boss_Dead(fTimeDelta);
		break;
	case CDoewole::STATE_END:
		break;
	default:
		break;
	}

	m_ePreState = m_eCurState;
}

void CDoewole_LeftClaw::Idle(const _float& fTimeDelta)
{
	m_bRender = true;

	m_fFrame += m_fMaxFrame * fTimeDelta;

	if (m_fMaxFrame < m_fFrame)
		m_fFrame = 0.f;

	// ================Doewole의 위치에 맞게 조정============
	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	

	m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y += 20.f;
	m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f;
	m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
	// =====================================================

	m_fMaxFrame = 10.f;
}

void CDoewole_LeftClaw::Move(const _float& fTimeDelta)
{
	m_bRender = false;
}

void CDoewole_LeftClaw::Standard_Attack(const _float& fTimeDelta)
{
	m_bRender = true;
	m_fMaxFrame = 7.f;

	_bool bGoToIdle = dynamic_cast<CDoewole*> (m_pOwner)->Get_AttackToIdle();

	if (!bGoToIdle)
	{
		m_fFrame += m_fMaxFrame * fTimeDelta * 2;

		if (m_fMaxFrame < m_fFrame)
			m_fFrame = m_fMaxFrame;
	}
	else
	{
		m_fFrame -= m_fMaxFrame * fTimeDelta * 2;

		if (m_fFrame < 0.f)
			m_fFrame = 0.f;
	}

	// ================Doewole의 위치에 맞게 조정============
	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	

	m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
	m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame;
	m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f + m_fFrame * (-2.5f);
	m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
	// =====================================================

}

void CDoewole_LeftClaw::OutStretch_Attack(const _float& fTimeDelta)
{
	m_bRender = true;

	if (!m_bUp && !m_bDown)
	{
		m_fMaxFrame = 6.f;

		m_fFrame += m_fMaxFrame * fTimeDelta * 2;

		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
		m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * 1.f;
		m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f;
		m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
		// =====================================================

		if (m_fMaxFrame < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bUp = true;
		}
	}
	else if (m_bUp && !m_bDown)
	{
		m_fMaxFrame = 9.f;

		m_fFrame += m_fMaxFrame * fTimeDelta;

		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
		m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + 6 * 2.f;
		m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f + 5.f;
		m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
		// =====================================================

		if (m_fMaxFrame < m_fFrame)
		{
			m_fFrame = 0.f;
			++m_iReplayCnt;
		}

		if (m_iReplayCnt == 3)
		{
			m_bDown = true;
			m_bUp = false;
			m_fFrame = 6.f;
		}
	}

	if (m_bDown)
	{
		m_fMaxFrame = 6.f;

		m_fFrame -= m_fMaxFrame * fTimeDelta * 2;

		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
		m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * 1.f;
		m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f;
		m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
		// =====================================================

		if (m_fFrame < 0.f)
		{
			m_fFrame = 0.f;
			//m_bDown = false;
		}
	}

	/*if (m_eCurState = CDoewole::IDLE)
		m_bDown = false;*/
}

void CDoewole_LeftClaw::Smash_Attack(const _float& fTimeDelta)
{
	m_bRender = true;

	m_fAccTime += fTimeDelta;

	if (!m_bUp)
	{
		m_fMaxFrame = 6.f;

		m_fFrame += m_fMaxFrame * fTimeDelta * 2;

		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
		m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * 1.7f;
		m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f - m_fFrame * 1.7f;
		// =====================================================

		if (m_fFrame == 5.f)
			m_fAccTime = 0.f;

		if (m_fAccTime < 2.f)
		{
			if (m_fFrame > 6.f)
				m_fFrame = 6.f;


			// 위험 Circle 이펙트 생성
			if (!m_bAlert)
			{
				m_bAlert = true;

				_vec3 vPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 0.001f , pDoewoleTransformCom->m_vInfo[INFO_POS].z };

				TIME_STOP_VOID

				CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos, _vec3(25.f, 25.f, 25.f), 2.f, 0.f, TRUE);
				
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
				pLayer->Add_BulletObject(  pEffect);
			}
		}

		if (m_fMaxFrame < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bUp = true;
			m_fAccTime = 0.f;
			m_bAlert = false;
		}
	}
	else
	{
		m_fMaxFrame = 13.f;

		m_fFrame += m_fMaxFrame * fTimeDelta * 2;

		if (m_fFrame == 5.f)
			m_fAccTime = 0.f;

		if (m_fAccTime < 2.f)
		{
			if (m_fFrame > 6.f)
			{
				m_fFrame = 6.f;
				if (!m_bSmash)
				{
					m_bSmash = true;

					CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
					

					_vec3 vEffectPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 0.3f , pDoewoleTransformCom->m_vInfo[INFO_POS].z };

					TIME_STOP_VOID

					// ChargeCircle 이펙트 생성
					CGameObject* pEffect = CEffect_Doewole_ChargeCircle::Create(m_pGraphicDev , vEffectPos);
					
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
					pLayer->Add_BulletObject(  pEffect);

					// Smash 이펙트 생성
					pEffect = CEffect_Doewole_Slam::Create(m_pGraphicDev);

					CAMERA_SHAKE(0.3f);
					
					pLayer = Engine::Get_Layer(L"Layer_GameLogic");
					pLayer->Add_BulletObject(  pEffect);

					// Standard Bullet 생성
					for (int i = 0; i < 20; ++i)
					{
						CGameObject* pBullet = CDoewoleBullet_Standard::Create(m_pGraphicDev);
						 
						pLayer = Engine::Get_Layer(L"Layer_GameLogic");
						pLayer->Add_BulletObject(  pBullet);
					}
				}
			}
		}


		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];

		if (m_fFrame <= 6)
		{
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame * 0.7f;
			m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f - 6 * 2.f;

			if (m_fFrame >= 3)
			{
				if (!m_bSound[0])
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE);
					CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_slam.wav", CSoundMgr::DOEWOLE, 0.75f);
					m_bSound[0] = true;
				}
			}
		}
		else
		{
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - (6 * 0.7f) + ((m_fFrame - 1.f) - 6.f) * 0.7f;
			m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f - 6 * 2.f + ((m_fFrame - 1.f) - 6.f) * 2.f;
		}
		// =====================================================

		if (m_fMaxFrame < m_fFrame)
		{
			m_fFrame = m_fMaxFrame;
			m_bUp = false;
			m_fAccTime = 0.f;
			dynamic_cast<CDoewole*> (m_pOwner)->Set_State(CDoewole::IDLE);
			m_bSmash = false;
		}
	}
}

void CDoewole_LeftClaw::Scratch_Attack(const _float& fTimeDelta)
{
	m_bRender = dynamic_cast<CDoewole*>(m_pOwner)->Get_Disappear();

	if (!m_bRender)
		m_fFrame = 0.f;

	if (!m_bWait)
	{
		m_fMaxFrame = 7.f;

		if (m_bRender)
		{
			m_fFrame += m_fMaxFrame * fTimeDelta * 2.f;

			if (!m_bHurt)
			{
				m_bHurt = true;

				CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"AlertRect_Left", L"Proto_Transform", ID_DYNAMIC));
				_vec3 vPos = { pTransform->m_vInfo[INFO_POS].x , 1.f , pTransform->m_vInfo[INFO_POS].z };

				TIME_STOP_VOID

				// Hurt 이펙트 생성
				for (int i = 0; i < 3; ++i)
				{
					vPos.z -= i * 2.f;
					vPos.x -= i * 1.f;

					CGameObject* pEffect = CEffect_Doewole_Hurt::Create(m_pGraphicDev, vPos, _vec3(-30.f, 30.f, 30.f));
					
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
					pLayer->Add_BulletObject(pEffect);
				}
			}
		}

		if (m_fFrame > m_fMaxFrame)
		{
			m_fFrame = m_fMaxFrame;
		}

		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];

		m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f - m_fFrame * 4.f;

		if (m_fFrame < 5.f)
			m_pTransformCom->m_vInfo[INFO_POS].z -= m_fFrame * 3.f;

		if (m_fFrame < 7.f)
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame;
		else
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame + 11.f;
	}
	else
	{
		m_fMaxFrame = 10.f;

		// ================Doewole의 위치에 맞게 조정============
		CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
		

		m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
		m_pTransformCom->m_vInfo[INFO_POS].y += 20.f;
		m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f;
		m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
		// =====================================================
	}
}

void CDoewole_LeftClaw::Area_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	m_bRender = dynamic_cast<CDoewole*>(m_pOwner)->Get_Disappear();

	

	if (m_bRender)
	{
		if (!m_bUp)
		{
			m_fMaxFrame = 6.f;

			m_fFrame += m_fMaxFrame * fTimeDelta * 1.f;

			// ================Doewole의 위치에 맞게 조정============
			CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
			

			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * 2.2f;
			m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f - m_fFrame * 1.7f;
			// =====================================================

			if (fabs(m_fFrame - 5.f) < 0.01f)
				m_fAccTime = 0.f;

			if (m_fAccTime < 1.f)
			{
				if (m_fFrame > 6.f)
					m_fFrame = 6.f;
			}

			if (m_fMaxFrame < m_fFrame)
			{
				m_fFrame = 0.f;
				m_bUp = true;
				m_fAccTime = 0.f;
			}
		}
		else
		{
			m_fAccTime3 += fTimeDelta;

			m_fMaxFrame = 13.f;

			m_fFrame += m_fMaxFrame * fTimeDelta * 2;

			if (m_fFrame - 5.f < 0.01f)
			{
				m_fAccTime = 0.f;
			}

			if (m_fAccTime < 2.f)
			{
				if (m_fFrame > 6.f)
				{
					m_fFrame = 6.f;
				}
			}

			

			// ================Doewole의 위치에 맞게 조정============
			CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
			

			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];

			if (m_fAccTime3 > 1.2f)
			{
				if (!m_bSound[0])
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE_BULLET);
					CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET, 0.75f);

					m_bSound[0] = true;
				}
			}

			

			if (m_fAccTime3 > 1.3f)
			{
				if (!m_bSound[1])
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE_BULLET2);
					CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET2, 0.75f);
					m_bSound[1] = true;
				}
				
			}

			if (m_fAccTime3 > 1.4f)
			{
				if (!m_bSound[2])
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE_BULLET3);
					CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET3, 0.75f);
					m_bSound[2] = true;
				}
				
			}

			if (m_fAccTime3 > 1.5f)
			{
				if (!m_bSound[3])
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE_BULLET4);
					CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET4, 0.75f);
					m_bSound[3] = true;
				}
			}
				

			if (m_fAccTime3 > 1.6f)
			{
				if (!m_bSound[4])
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE_BULLET5);
					CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET5, 0.75f);
					m_bSound[4] = true;
				}
			}
				

			if (m_fAccTime3 > 1.7f)
			{
				if (!m_bSound[5])
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE_BULLET6);
					CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET6, 0.75f);
					m_bSound[5] = true;
				}
				
			}
				
			if (m_fAccTime3 > 1.8f)
			{
				if (!m_bSound[6])
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE_BULLET7);
					CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET7, 0.75f);
					m_bSound[6] = true;
				}
			}
				
			if (m_fFrame <= 6)
			{
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame * 0.7f;
				m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f - 6 * 2.f;

				if (!m_bSound[7])
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE);
					CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_slam.wav", CSoundMgr::DOEWOLE, 0.75f);
					m_bSound[7] = true;

					CAMERA_SHAKE(0.3f);
				}
			}
			else
			{
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - (6 * 0.7f) + ((m_fFrame - 1.f) - 6.f) * 0.7f;
				m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f - 6 * 2.f + ((m_fFrame - 1.f) - 6.f) * 2.f;


				m_bSmash = false;
			}

			if (m_fMaxFrame < m_fFrame)
			{
				m_fFrame = m_fMaxFrame;
				m_bUp = false;
				m_fAccTime = 0.f;
			}
		}
	}
}

void CDoewole_LeftClaw::Upgrade_Smash_Attack(const _float& fTimeDelta)
{
	m_bRender = true;

	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	

	TIME_STOP_VOID

	if (!m_bLSmashWait)
	{
		if (!m_bSmash)
		{
			// 들어올린다
			m_fMaxFrame = 6.f;

			m_fFrame += m_fMaxFrame * fTimeDelta;

			if (!m_bAlert)
			{
				m_bAlert = true;

				_vec3 vPos = { m_pTransformCom->m_vInfo[INFO_POS].x , 0.001f , m_pTransformCom->m_vInfo[INFO_POS].z };

				CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos, _vec3(20.f, 20.f, 20.f), 3.f, 0.f, TRUE);
				
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
				pLayer->Add_BulletObject(  pEffect);
			}

			if (m_fFrame > m_fMaxFrame)
			{
				m_fFrame = m_fMaxFrame;
				m_fAccTime += fTimeDelta;

				// 이펙트 한번 생성
				if (!m_bCircleEffect)
				{
					m_bCircleEffect = true;

					CGameObject* pEffect = CEffect_Doewole_Circle::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);
					
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
					pLayer->Add_BulletObject(  pEffect);

				}

				if (m_fAccTime > 2.1f)
				{
					m_bSmash = true;
					m_fAccTime = 0.f;
					m_fFrame = 0.f;
					m_bCircleEffect = false;
					m_bAlert = false;
				}
			}

			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * 2.2f;
			m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f;
		}
		else
		{
			if (!m_bSound[0])
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE);
				CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_slam.wav", CSoundMgr::DOEWOLE, 0.75f);
				m_bSound[0] = true;

				CAMERA_SHAKE(0.3f);
			}

			// 내려찍는다
			m_fMaxFrame = 13.f;

			m_fAccTime += fTimeDelta;

			m_fFrame += m_fMaxFrame * fTimeDelta ;

			if (fabs(m_fFrame - 5.f) < 0.1f)
				m_fAccTime = 0.f;

			// 내려 찍힌 상태
			if (m_fAccTime < 1.f)
			{
				m_fFrame = 6.f;
				if (!m_bSmashEffect)
				{
					_vec3 vEffectPos = { m_pTransformCom->m_vInfo[INFO_POS].x , 0.3f , m_pTransformCom->m_vInfo[INFO_POS].z };

					CGameObject* pEffect = CEffect_Doewole_ChargeCircle::Create(m_pGraphicDev, vEffectPos);
					
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
					pLayer->Add_BulletObject(  pEffect);

					for (int i = 0; i < 40; ++i)
					{
						_vec3 vPos = { m_pTransformCom->m_vInfo[INFO_POS].x , 3.f , m_pTransformCom->m_vInfo[INFO_POS].z };
						CDoewoleBullet_StopGo* pBullet = CDoewoleBullet_StopGo::Create(m_pGraphicDev, vPos, CLAW_LEFT);
						 
						pLayer = Engine::Get_Layer(L"Layer_GameLogic");
						pLayer->Add_BulletObject(  pBullet);
					}


					m_bSmashEffect = true;
				}
			}

			if (m_fFrame > m_fMaxFrame)
			{
				m_bSmash = false;
				m_bLSmashWait = true;
				m_fFrame = 0.f;
				m_bSmashEffect = false;

				// 오른손아 일해
				CDoewole_RightClaw* pRightClaw = dynamic_cast<CDoewole_RightClaw*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Doewole_RightClaw"));
				pRightClaw->Set_SmashWait(false);
			}

			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
			// 내려가고
			if(m_fFrame <= 6.f)
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * -0.3f;
			// 올라가고
			else
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + (6.f-m_fFrame) * -0.3f;
			m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f;
		}
	}
	else
	{
		m_fAccTime += fTimeDelta;

		m_fMaxFrame = 10.f;

		m_fFrame += m_fMaxFrame * fTimeDelta;

		if (m_fFrame > m_fMaxFrame)
			m_fFrame = 0.f;
	}
}

void CDoewole_LeftClaw::Upgrade_Scratch_Attack(const _float& fTimeDelta)
{
	m_bRender = dynamic_cast<CDoewole*>(m_pOwner)->Get_Disappear();

	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	if (!m_bRender)
		m_fFrame = 0.f;

	CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
	

	if (!m_bCrossScratch)
	{
		if (!m_bSmash)
		{
			if (!m_bWait)
			{
				m_fMaxFrame = 7.f;

				if (m_bRender)
				{
					m_fFrame += m_fMaxFrame * fTimeDelta * 2.f;

					if (!m_bHurt)
					{
						TIME_STOP_VOID

						m_bHurt = true;

						_vec3 vPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 0.1f , pDoewoleTransformCom->m_vInfo[INFO_POS].z - 6.f };

						// Hurt 이펙트 생성
						for (int i = 0; i < 3; ++i)
						{
							vPos.z -= i * 2.f;
							vPos.x -= i * 1.f;

							CGameObject* pEffect = CEffect_Doewole_Hurt::Create(m_pGraphicDev, vPos, _vec3(-30.f, 30.f, 30.f));
							
							pLayer->Add_BulletObject(pEffect);
						}
					}
				}

				if (m_fFrame > m_fMaxFrame)
				{
					m_fFrame = m_fMaxFrame;
				}

				// ================Doewole의 위치에 맞게 조정============
				

				m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];

				m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f - m_fFrame * 4.f;

				if (m_fFrame < 5.f)
					m_pTransformCom->m_vInfo[INFO_POS].z -= m_fFrame * 3.f;

				if (m_fFrame < 7.f)
					m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame;
				else
					m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame + 11.f;
			}
			else
			{
				m_fMaxFrame = 10.f;

				// ================Doewole의 위치에 맞게 조정===========

				m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f;
				m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f;
				m_pTransformCom->m_vInfo[INFO_POS].z -= 0.1f;
				// =====================================================
			}
		}
		else if (m_bSmash)
		{
			m_fMaxFrame = 6.f;

			m_fFrame += m_fMaxFrame * fTimeDelta * 2;

			if (m_fAccTime < 2.f)
			{
				if (m_fFrame > 6.f)
				{
					m_fFrame = 6.f;

					if (!m_bMakeThorn)
					{
						TIME_STOP_VOID

						CGameObject* pGameObject = nullptr;

						_vec3 vPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 5.f , pDoewoleTransformCom->m_vInfo[INFO_POS].z - 3.f };
						// 오른쪽 가시
						pGameObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-10.f, 10.f, 10.f));
						 
						pLayer->Add_BulletObject(pGameObject);

						// 왼쪽 가시
						pGameObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(10.f, 10.f, 10.f));
						 
						pLayer->Add_BulletObject(pGameObject);

						
						CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE);
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_slam.wav", CSoundMgr::DOEWOLE, 0.75f);
						CAMERA_SHAKE(0.3f);
							
						
						// Smash 이펙트 생성
						pGameObject = CEffect_Doewole_Slam::Create(m_pGraphicDev);
						 
						pLayer = Engine::Get_Layer(L"Layer_GameLogic");
						pLayer->Add_BulletObject(pGameObject);

						_vec3 vEffectPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 0.3f , pDoewoleTransformCom->m_vInfo[INFO_POS].z };

						// ChargeCircle 이펙트 생성
						CGameObject* pEffect = CEffect_Doewole_ChargeCircle::Create(m_pGraphicDev, vEffectPos);
						
						CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
						pLayer->Add_BulletObject(pEffect);

						// Standard Bullet 생성
						for (int i = 0; i < 50; ++i)
						{
							CGameObject* pBullet = CDoewoleBullet_Standard::Create(m_pGraphicDev);
							 
							pLayer = Engine::Get_Layer(L"Layer_GameLogic");
							pLayer->Add_BulletObject(pBullet);
						}

						m_bMakeThorn = true;
						m_bHurt = false;
					}
				}
			}


			// ================Doewole의 위치에 맞게 조정============
			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];

			if (m_fFrame <= 6)
			{
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame * 0.7f;
				m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f - 6 * 2.f;
			}
		}
	}
	else
	{
		if (m_bRender)
		{
			m_fMaxFrame = 13.f ;

			if(m_fFrame < 9.f)
				m_fFrame += m_fMaxFrame * fTimeDelta * 2.f;
			else
				m_fFrame += m_fMaxFrame * fTimeDelta * 3.f;

			if (m_fFrame > m_fMaxFrame)
				m_fFrame = m_fMaxFrame;

			// ================Doewole의 위치에 맞게 조정============
			CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
			

			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
			m_pTransformCom->m_vInfo[INFO_POS].y += 25.f;

			if (m_fFrame < 9.f)
				m_pTransformCom->m_vInfo[INFO_POS].x += m_fFrame * 0.2f;
			else
			{
				m_pTransformCom->m_vInfo[INFO_POS].x -= m_fFrame * 1.2f;
				m_pTransformCom->m_vInfo[INFO_POS].z -= m_fFrame * 0.7f;
			}
				
			if (m_fFrame > 11.f)
				m_pTransformCom->m_vInfo[INFO_POS].y += 3.f;
			if (m_fFrame > 12.f)
				m_pTransformCom->m_vInfo[INFO_POS].y += 3.f;

			if (!m_bHurt)
			{
				TIME_STOP_VOID

				m_bHurt = true;

				_vec3 vPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 0.1f , pDoewoleTransformCom->m_vInfo[INFO_POS].z - 6.f};

				// Hurt 이펙트 생성
				for (int i = 0; i < 3; ++i)
				{
					vPos.z -= i * 2.f;
					vPos.x -= i * 1.f;

					CGameObject* pEffect = CEffect_Doewole_Hurt::Create(m_pGraphicDev, vPos, _vec3(-30.f, 30.f, 30.f));
					
					pLayer->Add_BulletObject(pEffect);
				}
			}
		}
	}
}

void CDoewole_LeftClaw::Bullet_Activate_Attack(const _float& fTimeDelta)
{
	TIME_STOP_VOID

	m_bRender = dynamic_cast<CDoewole*>(m_pOwner)->Get_Disappear();

	if (m_bRender)
	{
		m_fAccTime += fTimeDelta;

		if (!m_bUp)
		{
			m_fMaxFrame = 6.f;

			m_fFrame += m_fMaxFrame * fTimeDelta ;

			// ================Doewole의 위치에 맞게 조정============
			CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
			

			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];
			m_pTransformCom->m_vInfo[INFO_POS].y += 20.f + m_fFrame * 1.7f;
			m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f;
			// =====================================================

			if (m_fFrame == 5.f)
				m_fAccTime = 0.f;

			if (m_fAccTime < 1.5f)
			{
				if (m_fFrame > 6.f)
					m_fFrame = 6.f;

				// 위험 Circle 이펙트 생성
				if (!m_bAlert)
				{
					m_bAlert = true;

					_vec3 vPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 0.001f , pDoewoleTransformCom->m_vInfo[INFO_POS].z };

					CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos, _vec3(25.f, 25.f, 25.f), 1.5f, 0.f, TRUE);
					
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
					pLayer->Add_BulletObject(pEffect);
				}
			}

			if (m_fMaxFrame < m_fFrame)
			{
				m_fFrame = 0.f;
				m_bUp = true;
				m_fAccTime = 0.f;
				m_bAlert = false;
			}
		}
		else
		{
			if (!m_bSound[0])
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE);
				CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_slam.wav", CSoundMgr::DOEWOLE, 0.75f);
				CAMERA_SHAKE(0.3f);
				m_bSound[0] = true;
			}

			


			m_fMaxFrame = 13.f;

			m_fFrame += m_fMaxFrame * fTimeDelta * 2;

			if (m_fFrame == 5.f)
				m_fAccTime = 0.f;

			if (m_fAccTime < 2.f)
			{
				if (m_fFrame > 6.f)
				{
					m_fFrame = 6.f;
					if (!m_bSmash)
					{
						m_bSmash = true;

						CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
						

						_vec3 vEffectPos = { pDoewoleTransformCom->m_vInfo[INFO_POS].x , 0.3f , pDoewoleTransformCom->m_vInfo[INFO_POS].z };


						// ChargeCircle 이펙트 생성
						CGameObject* pEffect = CEffect_Doewole_ChargeCircle::Create(m_pGraphicDev, vEffectPos);
						
						CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
						pLayer->Add_BulletObject(pEffect);

						// Smash 이펙트 생성
						pEffect = CEffect_Doewole_Slam::Create(m_pGraphicDev);
						
						pLayer = Engine::Get_Layer(L"Layer_GameLogic");
						pLayer->Add_BulletObject(pEffect);

						//// Standard Bullet 생성
						//for (int i = 0; i < 20; ++i)
						//{
						//	CGameObject* pBullet = CDoewoleBullet_Standard::Create(m_pGraphicDev);
						//	 
						//	pLayer = Engine::Get_Layer(L"Layer_GameLogic");
						//	pLayer->Add_BulletObject(pBullet);
						//}

						dynamic_cast<CDoewole*>(m_pOwner)->Set_BulletGo();
					}
				}
			}


			// ================Doewole의 위치에 맞게 조정============
			CTransform* pDoewoleTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole", L"Proto_Transform", ID_DYNAMIC));
			

			m_pTransformCom->m_vInfo[INFO_POS] = pDoewoleTransformCom->m_vInfo[INFO_POS];

			if (m_fFrame <= 6)
			{
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - m_fFrame * 0.7f;
				m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f;

			}
			else
			{
				m_pTransformCom->m_vInfo[INFO_POS].y += 20.f - (6 * 0.7f) + ((m_fFrame - 1.f) - 6.f) * 0.7f;
				m_pTransformCom->m_vInfo[INFO_POS].x -= 15.f;
			}
			// =====================================================

			if (m_fMaxFrame < m_fFrame)
			{
				m_fFrame = m_fMaxFrame;
				m_bUp = false;
				m_fAccTime = 0.f;
				dynamic_cast<CDoewole*> (m_pOwner)->Set_State(CDoewole::IDLE);
				m_bSmash = false;
			}
		}
	}
}

void CDoewole_LeftClaw::Boss_Dead(const _float& fTimeDelta)
{
	m_fMaxFrame = 12.f;

	m_fFrame += m_fMaxFrame * fTimeDelta;

	if (m_fFrame > m_fMaxFrame)
		m_bDead = true;
}

CDoewole_LeftClaw* CDoewole_LeftClaw::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoewole_LeftClaw* pInstance = new CDoewole_LeftClaw(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewole_LeftClaw::Free(void)
{
	__super::Free();
}


