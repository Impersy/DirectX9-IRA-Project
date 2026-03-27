#include "stdafx.h"
#include "..\Header\Doewole.h"
#include "Export_Function.h"
#include "Effect_Doewole_Vanish.h"
#include "Effect_Doewole_StandardAttack.h"
#include "Effect_Doewole_ChargeExplosion.h"
#include "Effect_Doewole_Loof.h"
#include "DoewoleBullet_SwordShot.h"
#include "DoewoleBullet_Circle.h"
#include "Effect_AlertRect.h"
#include "DoewoleBullet_Thorn.h"
#include <Effect_AlertCircle.h>
#include "CollisionMgr.h"
#include <SoundMgr.h>
#include "StaticCamera.h"

CDoewole::CDoewole(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBoss(pGraphicDev)
{
}

CDoewole::CDoewole(const CDoewole & rhs)
	: CBoss(rhs)
{
}

CDoewole::~CDoewole()
{
	
}

HRESULT CDoewole::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vInfo[INFO_POS] = { 257.f +128.f , 0.f , 257.f + 128.f };

	return S_OK;
}

_int CDoewole::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	State_Update(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CDoewole::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}


void CDoewole::Idle(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	if (m_bRage)
	{
		m_iMaxPattern = 11.f;
	}

	if(1 == m_iPattern)
	{ 
		if (m_fAccTime > 6.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = MOVE;
			++m_iPattern;
		
			TIME_STOP_VOID

			// 사라지는 이펙트 생성
			CGameObject* pEffect = CEffect_Doewole_Vanish::Create(m_pGraphicDev);
			 
			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			pLayer->Add_BulletObject(pEffect);
		}
	}

	else if (2 == m_iPattern)
	{
		if (m_fAccTime > 0.5f)
		{
			m_fAccTime = 0.f;
			m_eCurState = STANDARD_ATTACK;
			++m_iPattern;
		}
	}

	else if (3 == m_iPattern)
	{
		if (m_fAccTime > 0.5f)
		{
			m_fAccTime = 0.f;
			if (!m_bRage)
				m_eCurState = SCRATCH_ATTACK;
			else
				m_eCurState = UPGRADE_SCRATCH_ATTACK;
			++m_iPattern;
		}
	}

	else if (4 == m_iPattern)
	{
		if (m_fAccTime > 0.5f)
		{
			m_fAccTime = 0.f;
			m_eCurState = MOVE;
			++m_iPattern;

			// 사라지는 이펙트 생성
			CGameObject* pEffect = CEffect_Doewole_Vanish::Create(m_pGraphicDev);

			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			pLayer->Add_BulletObject(pEffect);
		}
	}

	else if (5 == m_iPattern)
	{
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = OUTSTRECTH_ATTACK;
			++m_iPattern;
		}
	}

	else if (6 == m_iPattern)
	{
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;
			if (!m_bRage)
				m_eCurState = SCRATCH_ATTACK;
			else
				m_eCurState = UPGRADE_SCRATCH_ATTACK;
			++m_iPattern;
		}
	}

	else if (7 == m_iPattern)
	{
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;
			if (!m_bRage)
				m_eCurState = SMASH_ATTACK;
			else
				m_eCurState = UPGRADE_SMASH_ATTACK;
			++m_iPattern;
		}
	}

	else if (8 == m_iPattern)
	{
		if (m_fAccTime > 2.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = MOVE;
			++m_iPattern;

			// 사라지는 이펙트 생성
			CGameObject* pEffect = CEffect_Doewole_Vanish::Create(m_pGraphicDev);

			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			pLayer->Add_BulletObject(pEffect);
		}
	}

	else if (9 == m_iPattern)
	{
		if (m_fAccTime > 3.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = UPGRADE_SMASH_ATTACK;
			++m_iPattern;
		}
	}

	else if (10 == m_iPattern)
	{
		if (m_fAccTime > 0.5f)
		{
			m_fAccTime = 0.f;
			m_eCurState = UPGRADE_SCRATCH_ATTACK;
			++m_iPattern;
		}
	}

	else if (11 == m_iPattern)
	{
		if (m_fAccTime > 2.f)
		{
			m_fAccTime = 0.f;
			m_eCurState = BULLET_ACTIVATE_ATTACK;
			++m_iPattern;
		}
	}
}

void CDoewole::Move(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	// 플레이어 근처로 이동한다
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	 

	_vec3 vDiff = pPlayerTransform->m_vInfo[INFO_POS] - m_pTransformCom->m_vInfo[INFO_POS];
	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &vDiff);

	if (D3DXVec3Length(&vDiff) > 30.f && !m_bChasePlayer)
		m_pTransformCom->m_vInfo[INFO_POS] += vDir * m_fMoveSpeed * fTimeDelta;
	else
	{
		if (!m_bChasePlayer)
		{
			m_bChasePlayer = true;
			m_fAccTime = 0.f;
		}

		if (m_fAccTime > 1.f)
		{
			m_eCurState = IDLE;
			m_fAccTime = 0.f;
			m_bChasePlayer = false;

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE);
			CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_appear.wav", CSoundMgr::DOEWOLE, 0.75f);
		}
	}
}

void CDoewole::Standard_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	
	if (m_iStandardAttackCnt < 2)
	{
		if (m_fAccTime > 1.f) // 1초간격으로 쏜다
		{
			m_fAccTime = 0.f;

			TIME_STOP_VOID

			// Standard Bullet 생성
			Create_SwordBullet();

			// Standard_Attack 이펙트 생성
			CGameObject* pEffect = CEffect_Doewole_StandardAttack::Create(m_pGraphicDev);
			 
			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			pLayer->Add_BulletObject(pEffect);

			++m_iStandardAttackCnt;
			
		}
	}
	else if(m_iStandardAttackCnt >=2 && m_fAccTime > 1.f)
	{
		m_bAttackToIdle = true;

		if (m_fAccTime > 2.f)
		{
			m_eCurState = IDLE;
			m_fAccTime = 0.f;
			m_bAttackToIdle = false;
		}
	}
}

void CDoewole::OutStretch_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	m_fAccTime2 += fTimeDelta;
	m_fAccTime3 += fTimeDelta;

	if (!m_bEffect)
	{
		TIME_STOP_VOID

		// ChargeExplosion 이펙트 생성
		CGameObject* pEffect = CEffect_Doewole_ChargeExplosion::Create(m_pGraphicDev);

		CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
		pLayer->Add_BulletObject(pEffect);

		// Loof 이펙트 생성
		pEffect = CEffect_Doewole_Loof::Create(m_pGraphicDev);
		 
		pLayer->Add_BulletObject(pEffect);

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"Doewole_Casting.wav", CSoundMgr::DOEWOLE_EFFECT, 0.75f);

		m_bEffect = true;
	}

	if (m_fAccTime > 4.f)
	{
		m_bAttackToIdle = true;														  

		if (m_fAccTime > 5.f)
		{
			m_eCurState = IDLE;
			m_fAccTime = 0.f;
			m_bAttackToIdle = false;
			m_fAccTime2 = 0.f;
		}
	}

	if (m_fAccTime2 > 0.3f)
	{
		Create_CircleBullet();
		m_fAccTime2 = 0.f;
	}

	if (m_fAccTime3 > 0.6f)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE_BULLET);
		CSoundMgr::Get_Instance()->PlaySound(L"Dowole_CircleBullet.wav", CSoundMgr::DOEWOLE_BULLET, 0.75f);
		m_fAccTime3 = 0.f;
	}
}

void CDoewole::Smash_Attack(const _float& fTimeDelta)
{

}

void CDoewole::Scratch_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	if (!m_bDisappear)
	{
		if (!m_bSound)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE);
			CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_move.wav", CSoundMgr::DOEWOLE, 0.75f);
			m_bSound = true;
		}

		if (m_fAccTime > 2.f)
		{
			if (!m_bChasePlayer)
			{
				// 플레이어 한테 이동
				CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
				 
				m_pTransformCom->m_vInfo[INFO_POS] = { pPlayerTransform->m_vInfo[INFO_POS].x , 0.f ,pPlayerTransform->m_vInfo[INFO_POS].z + 15.f };
				m_bChasePlayer = true;

				TIME_STOP_VOID

				// AlertRect 이펙트 생성
				_vec3 vScale;
				if (!m_bRight) vScale = { -30.f , 30.f,30.f };
				else vScale = { 30.f , 30.f,30.f };

				CGameObject* pEffect = CEffect_AlertRect::Create(m_pGraphicDev , vScale);
				 
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
				if (!m_bRight)
					pLayer->Add_GameObject(L"AlertRect_Right", pEffect);
				else 
					pLayer->Add_GameObject(L"AlertRect_Left", pEffect);
							
				m_bRight = !m_bRight;
			}
		}

		if (m_fAccTime > 3.f)
		{
			m_fAccTime = 0.f;
			m_bDisappear = true;
			m_bChasePlayer = false;

			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE);
			CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_claw.wav", CSoundMgr::DOEWOLE, 0.75f);

			m_bSound = false;
		}
	}
}

void CDoewole::Area_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	m_fAccTime2 += fTimeDelta;
	m_bDisappear = false;

	if (!m_bSound)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE);
		CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_move.wav", CSoundMgr::DOEWOLE, 0.75f);

		m_bSound = true;
	}

	_vec3	vDestPos = { 128.f + 257.f , 0.f, 128.f + 257.f};
	_vec3 vDir;

	if (!m_bAreaAttack)
	{
		if (m_fAccTime > 1.f)
		{
			// 맵 중앙으로 이동
			vDir = vDestPos - m_pTransformCom->m_vInfo[INFO_POS];
			D3DXVec3Normalize(&vDir, &vDir);
			m_pTransformCom->m_vInfo[INFO_POS] += vDir * 30.f * fTimeDelta;

			if (D3DXVec3Length(&(vDestPos - m_pTransformCom->m_vInfo[INFO_POS])) < 2.f)
			{
				m_pTransformCom->m_vInfo[INFO_POS] = vDestPos;
				m_bAreaAttack = true;
				m_fAccTime = 0.f;
			}
		}
	}
	else
	{
		AreaAtaackPattern(fTimeDelta);
	}
}

void CDoewole::Upgrade_Smash_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	m_bDisappear = true;
}

void CDoewole::Upgrade_Scratch_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	if (!m_bDisappear)
	{
		if (m_fAccTime > 2.f)
		{
			if (!m_bChasePlayer)
			{
				// 플레이어 한테 이동
				CTransform* pPlayerTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
				 
				m_pTransformCom->m_vInfo[INFO_POS] = { pPlayerTransform->m_vInfo[INFO_POS].x , 0.f ,pPlayerTransform->m_vInfo[INFO_POS].z + 15.f };
				m_bChasePlayer = true;

				if (m_iScratchCnt <= 1)
				{
					TIME_STOP_VOID

					// AlertRect 이펙트 생성
					_vec3 vScale;
					if (!m_bRight) vScale = { -30.f , 30.f,30.f };
					else vScale = { 30.f , 30.f,30.f };

					CGameObject* pEffect = CEffect_AlertRect::Create(m_pGraphicDev, vScale);
					 
					
					if (!m_bRight)
						pLayer->Add_GameObject(L"AlertRect_Right", pEffect);
					else
						pLayer->Add_GameObject(L"AlertRect_Left", pEffect);

					m_bRight = !m_bRight;
				}
				else if (m_iScratchCnt == 2)
				{
					TIME_STOP_VOID

					// AlertCircle 이펙트 생성
					_vec3 vPos = { m_pTransformCom->m_vInfo[INFO_POS].x, 0.f , m_pTransformCom->m_vInfo[INFO_POS].z };
					CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos, _vec3(20.f, 20.f, 20.f), 1.5f, 0.f, FALSE);
					 
					pLayer->Add_BulletObject(pEffect);
				}
				else
				{
					TIME_STOP_VOID

					// AlertRect 이펙트 생성
					_vec3 vScale1 = { -30.f , 30.f,30.f };
					_vec3 vScale2 = { 30.f , 30.f,30.f };

					CGameObject* pEffect = CEffect_AlertRect::Create(m_pGraphicDev, vScale1);
					 
					pLayer->Add_BulletObject(pEffect);

					pEffect = CEffect_AlertRect::Create(m_pGraphicDev, vScale2);
					 
					pLayer->Add_BulletObject(pEffect);
				}

				m_iScratchCnt++;

			}
		}

		if (m_fAccTime > 3.f)
		{
			m_fAccTime = 0.f;
			m_bDisappear = true;
			m_bChasePlayer = false;
		}
	}
}

void CDoewole::Bullet_Activate_Attack(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	m_fAccTime2 += fTimeDelta;
	
	_vec3	vDestPos = { 128.f + 257.f, 0.f, 128.f + 257.f };
	_vec3 vDir;

	if (!m_bBullet_Active_Attack)
	{
		m_bDisappear = false;

		if (m_fAccTime > 1.f)
		{
			// 맵 중앙으로 이동
			vDir = vDestPos - m_pTransformCom->m_vInfo[INFO_POS];
			D3DXVec3Normalize(&vDir, &vDir);
			m_pTransformCom->m_vInfo[INFO_POS] += vDir * 30.f * fTimeDelta;

			if (D3DXVec3Length(&(vDestPos - m_pTransformCom->m_vInfo[INFO_POS])) < 1.f)
			{
				m_pTransformCom->m_vInfo[INFO_POS] = vDestPos;
				m_bBullet_Active_Attack = true;
				m_fAccTime = 0.f;
			}
		}
	}
	else
	{
		m_bDisappear = true;
		Bullet_ActivePattern(fTimeDelta);
	}
}

void CDoewole::Boss_Dead(const _float& fTimeDelta)
{
	if (m_bBossDead == false) {
		CAMERA_TARGET_CHANGE(0.5f);
		m_bBossDead = true;
	}

	if (!m_bSound)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE);
		CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Death.wav", CSoundMgr::DOEWOLE, 0.75f);
		CAMERA_SHAKE(2.f);

		m_bSound = true;
	}
}

void CDoewole::Create_SwordBullet()
{
	TIME_STOP_VOID

	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CGameObject* pBulletObject = nullptr;

	for (size_t i = 0; i < 16; i++)
	{
		_float cosTheta = cos(D3DXToRadian(360.f / 16.f) * i);
		_float sinTheta = sin(D3DXToRadian(360.f / 16.f) * i);

		_float	fAngle = (360.f / 16.f) * i;

		_vec3 vPos = { m_pTransformCom->m_vInfo[INFO_POS].x + cosTheta , 3.f ,  m_pTransformCom->m_vInfo[INFO_POS].z + sinTheta };

		pBulletObject = CDoewoleBullet_SwordShot::Create(m_pGraphicDev, vPos , fAngle);

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOEWOLE);
		CSoundMgr::Get_Instance()->PlaySound(L"Dowole_StandardAttack.wav", CSoundMgr::DOEWOLE, 0.75f);

		pLayer->Add_BulletObject(  pBulletObject);
	}
}

void CDoewole::Create_CircleBullet()
{
	TIME_STOP_VOID

	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CGameObject* pBulletObject = nullptr;

	for (size_t i = 0; i < 8; i++)
	{
		_float cosTheta = cos(D3DXToRadian(360.f / 8.f) * i);
		_float sinTheta = sin(D3DXToRadian(360.f / 8.f) * i);

		_vec3 vPos = { m_pTransformCom->m_vInfo[INFO_POS].x + cosTheta , 3.f ,  m_pTransformCom->m_vInfo[INFO_POS].z + sinTheta };

		pBulletObject = CDoewoleBullet_Circle::Create(m_pGraphicDev, vPos);
		 
		pLayer->Add_BulletObject(pBulletObject);
	}
}

void CDoewole::AreaAtaackPattern(const _float& fTimeDelta)
{

	_vec3 vPos;
	_vec3 vPos2;
	_vec3 vPos3;

	m_fAccTime3 += fTimeDelta;
	
	if (m_iThornCnt <= 4)
	{
		if (m_fAccTime > 0.03f)
		{
			if (!m_bTest)
			{
				TIME_STOP_VOID

				for (int i = 0; i < 256; i += 12)
				{
					vPos = { (_float)i + 257.f, 5.f, m_fThornZ + 257.f };
					vPos2 = { vPos.x - 1.f  , 5.f , vPos.z };
					vPos3 = { vPos.x , 0.5f , vPos.z  };

					CGameObject* pBulletObject = nullptr;
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

					// 오른쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));
					 
					pLayer->Add_BulletObject(  pBulletObject);

					// 왼쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));
					 
					pLayer->Add_BulletObject(  pBulletObject);

					// AlertCircle 이펙트 생성
					CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, 0.f,FALSE);
					 
					pLayer->Add_BulletObject(  pEffect);

					m_fAccTime = 0.f;

					if (m_fAccTime3 > 1.1f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET, 0.75f);

					if (m_fAccTime3 > 1.2f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET2, 0.75f);

					if (m_fAccTime3 > 1.3f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET3, 0.75f);

					if (m_fAccTime3 > 1.4f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET4, 0.75f);

					if (m_fAccTime3 > 1.5f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET5, 0.75f);

					if (m_fAccTime3 > 1.6f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET6, 0.75f);

					if (m_fAccTime3 > 1.7f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET7, 0.75f);
				}

				m_fThornZ += 6.f;

				if (m_fThornZ >= 256.f)
				{
					m_bTest = true;
					m_fThornZ = 3.f;
					m_iThornCnt++;
					m_fAccTime3 = 0.f;
				}
			}
			else
			{
				TIME_STOP_VOID

				for (int i = 0; i < 256; i += 12)
				{
					vPos = { (_float)i + 6.f + 257.f, 5, 256.f - m_fThornZ + 257.f };
					vPos2 = { vPos.x - 1.f  , 5 , vPos.z };
					vPos3 = { vPos.x , 0.2f , vPos.z };

					CGameObject* pBulletObject = nullptr;
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

					// 오른쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));
					 
					pLayer->Add_BulletObject(  pBulletObject);

					// 왼쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));
					 
					pLayer->Add_BulletObject(  pBulletObject);

					// AlertCircle 이펙트 생성
					CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, 0.f, FALSE);
					 
					pLayer->Add_BulletObject(  pEffect);

					m_fAccTime = 0.f;
					m_fAccTime2 = 0.f;

					if (m_fAccTime3 > 1.1f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET, 0.75f);

					if (m_fAccTime3 > 1.2f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET2, 0.75f);

					if (m_fAccTime3 > 1.3f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET3, 0.75f);

					if (m_fAccTime3 > 1.4f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET4, 0.75f);

					if (m_fAccTime3 > 1.5f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET5, 0.75f);

					if (m_fAccTime3 > 1.6f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET6, 0.75f);

					if (m_fAccTime3 > 1.7f)
						CSoundMgr::Get_Instance()->PlaySound(L"Dowole_Pattern_groundthorn.wav", CSoundMgr::DOEWOLE_BULLET7, 0.75f);

				}
				m_fThornZ += 6.f;

				if (m_fThornZ >= 256.f)
				{
					m_bTest = false;
					m_fThornZ = 0.f;
					m_iThornCnt++;
					m_fAccTime3 = 0.f;
				}
			}
		}
	}

	if (m_iThornCnt >= 5)
	{
		m_bDisappear = true;

		TIME_STOP_VOID

		if (m_fAccTime > 2.f)
		{
			
			m_fAccTime2 += fTimeDelta;

			if (m_fAccTime2 > 0.04f)
			{
				if (!m_bCrossTron)
				{
					// 좌
					vPos = { m_fThronX + 257.f, 5.f, 128.f + 257.f };
					vPos2 = { vPos.x - 1.f  , 5.f , vPos.z };
					vPos3 = { vPos.x , 0.2f , vPos.z  };

					CGameObject* pBulletObject = nullptr;
					CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

					// 오른쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					// 왼쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					// AlertCircle 이펙트 생성
					CGameObject* pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, 0.f, FALSE); pLayer->Add_BulletObject(  pEffect);

					// 우
					vPos = { 256.f - m_fThronX + 257.f , 5.f, 128.f + 257.f }; vPos2 = { vPos.x  - 1.f , 5.f , vPos.z  }; vPos3 = { vPos.x  , 0.2f , vPos.z  };

					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));
					pLayer->Add_BulletObject(  pBulletObject);

					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));
					pLayer->Add_BulletObject(  pBulletObject);

				
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, 0.f, FALSE);
					pLayer->Add_BulletObject(  pEffect);
					// 아래
					vPos = { 128.f + 257.f, 5.f, m_fThronX + 257.f };vPos2 = { vPos.x - 1.f , 5.f , vPos.z };vPos3 = { vPos.x , 0.2f , vPos.z };

					// 오른쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));pLayer->Add_BulletObject(  pBulletObject);
					// 왼쪽 가시
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));pLayer->Add_BulletObject(  pBulletObject);
					// AlertCircle 이펙트 생성
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, 0.f, FALSE);
					pLayer->Add_BulletObject(  pEffect);

					// 위
					vPos = { 128.f + 257.f, 5.f, (256.f-m_fThronX) + 257.f };
					vPos2 = { vPos.x - 1.f  , 5.f , vPos.z };
					vPos3 = { vPos.x  , 0.2f , vPos.z  };

					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));
					pLayer->Add_BulletObject(  pBulletObject);

					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));
					pLayer->Add_BulletObject(  pBulletObject);

			
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, 0.f, FALSE);
					pLayer->Add_BulletObject(  pEffect);

					// 우상
					vPos = { (256.f - m_fThronX) + 257.f, 5.f, (256.f - m_fThronX) + 257.f }; vPos2 = { vPos.x - 1.f , 5.f , vPos.z }; vPos3 = { vPos.x , 0.2f , vPos.z };
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f));pLayer->Add_BulletObject(  pBulletObject);
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f));pLayer->Add_BulletObject(  pBulletObject);
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, 0.f, FALSE); pLayer->Add_BulletObject(  pEffect);

					// 우하
					vPos = { (256.f - m_fThronX) + 257.f, 5.f, m_fThronX + 257.f }; vPos2 = { vPos.x - 1.f , 5.f , vPos.z }; vPos3 = { vPos.x , 0.2f , vPos.z };
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, 0.f, FALSE); pLayer->Add_BulletObject(  pEffect);

					// 좌하
					vPos = { m_fThronX + 257.f, 5.f, m_fThronX + 257.f }; vPos2 = { vPos.x - 1.f , 5.f , vPos.z }; vPos3 = { vPos.x , 0.2f , vPos.z };
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, 0.f, FALSE); pLayer->Add_BulletObject(  pEffect);

					// 좌상
					vPos = { m_fThronX + 257.f, 5.f, (256.f - m_fThronX) + 257.f }; vPos2 = { vPos.x - 1.f , 5.f , vPos.z }; vPos3 = { vPos.x , 0.2f , vPos.z };
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos, _vec3(-5.f, 6.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					pBulletObject = CDoewoleBullet_Thorn::Create(m_pGraphicDev, vPos2, _vec3(5.f, 5.f, 5.f)); pLayer->Add_BulletObject(  pBulletObject);
					pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos3, _vec3(5.f, 5.f, 5.f), 1.5f, 0.f, FALSE); pLayer->Add_BulletObject(  pEffect);
					m_fThronX -= 6.f;
					m_fAccTime2 = 0.f;

					if (m_fThronX < 0.f)
					{
						m_bCrossTron = true;
						m_fAccTime = 0.f;
						m_eCurState = IDLE;
					}
				}
			}
		}
	}
}

void CDoewole::Bullet_ActivePattern(const _float& fTimeDelta)
{

}

HRESULT CDoewole::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CDoewole::State_Update(const _float& fTimeDelta)
{
	if (m_eCurState != m_ePreState)
	{
		m_fFrame = 0.f;
		m_fAccTime = 0.f;

		m_iStandardAttackCnt = 0;

		m_bAttackToIdle = false;
		m_bEffect = false;

		m_fAccTime2 = 0.f;

		m_bDisappear = false;
		m_bChasePlayer = false;

		m_bRight = false;
		m_bAreaAttack = false;
		m_bTest = false;
		m_fThornZ = 0.f;
		m_iThornCnt = 0;
		m_bCrossTron = false;
		m_fThronX = 128.f;

		m_iScratchCnt = 0;
		m_bBullet_Active_Attack = false;

		m_bSound = false;
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

	if (m_iPattern > m_iMaxPattern)
		m_iPattern = 2;
}
_float CDoewole::CalculateZ(_float fX)
{
	if (fX <= 128.f)
		return (-1 / 128.f) * fX * fX + 2.f * fX;
	else
		return (1.f / 128.f) * fX * fX - 2.f * fX + 256.f;

	
}
CDoewole * CDoewole::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoewole * pInstance = new CDoewole(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDoewole::Free(void)
{
	__super::Free();
}


