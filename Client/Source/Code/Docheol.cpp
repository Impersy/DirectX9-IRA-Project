#include "stdafx.h"
#include "..\Header\Docheol.h"
#include "Export_Function.h"
#include "CollisionMgr.h"
#include "Effect_Docheol_FloorFire.h"
#include "Effect_Docheol_Tile.h"
#include "Effect_Docheol_AppearMarkFire.h"
#include "Effect_Docheol_FloorFireRayUp.h"
#include "Effect_Docheol_FireExplosion.h"
#include <Effect_Docheol_FireSpread.h>
#include <Effect_Docheol_SwingEffect.h>
#include <DocheolBullet_SwingBullet.h>
#include <DocheolBullet_SlamExplosion.h>
#include "Effect_Docheol_Mark.h"
#include <Effect_AlertCircle.h>
#include <Effect_AlertCircle2.h>
#include <Effect_Docheol_Fire.h>
#include <Effect_Docheol_Meteor.h>
#include "Effect_Docheol_FireExplosion2.h"
#include "Effect_Docheol_Electronic.h"
#include "Docheol_SpecialDoor.h"
#include "DocheolBullet_DoorBullet.h"
#include "Effect_Docheol_Mark2.h"
#include <Effect_Docheol_FireBall.h>
#include <Effect_Docheol_FireExplosion3.h>
#include "Effect_Docheol_Fire3.h"
#include <Effect_Docheol_Circle.h>
#include <DocheolBullet_ChargeBullet.h>
#include "Effect_Docheol_ChargeMark.h"
#include <Effect_Docheol_Mark3.h>
#include <Effect_Docheol_Electronic2.h>
#include "StaticCamera.h"
#include "SoundMgr.h"
#include "Npc_ApostleNpc_Time.h"

CDocheol::CDocheol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBoss(pGraphicDev)
	, m_eCurState(INTRO1)
	, m_ePreState(INTRO1)
{
}

CDocheol::CDocheol(const CDocheol & rhs)
	: CBoss(rhs)
{
}

CDocheol::~CDocheol()
{
	Free();
}

HRESULT CDocheol::Ready_GameObject(void)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 40.f , 40.f  , 40.f };

	m_pTransformCom->m_vInfo[INFO_POS] = { 124.f , 40.f , 132.f };

	m_bRender = false;

	m_iBossMaxHP = DOCHEOL_MAXHP;
	m_iBossCurHP = m_iBossMaxHP;

	m_pColliderCom->Set_Radius(25.f);
	m_pColliderCom->Set_Offset(_vec3(0.f, -7.f, 10.f));

	m_fFrameSpeed = 1.f;

	return S_OK;
}

_int CDocheol::Update_GameObject(const _float& fTimeDelta)
{

	if (m_iBossCurHP <= 0.f) {
		g_Game_Clear = true;

		CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
		if (nullptr == pLayer) return E_FAIL;

		CGameObject* pGameObject = nullptr;

		pGameObject = CNpc_ApostleNpc_Time::Create(m_pGraphicDev);
		if (nullptr == pGameObject) return E_FAIL;
		if (FAILED(pLayer->Add_GameObject(L"Npc_ApostleNpc_Time", pGameObject)))
			return E_FAIL;
	}


	if (m_bDead)
	{
		
		Create_DeathEffect();
		return OBJ_DEAD;
	}

	if (GetAsyncKeyState('U') && !m_bTest)
	{
		m_iBossCurHP = -1;
		//eCurState = SPECIAL_PRATTERN;
		//Create_Fire3Effect();
		//m_bTest = true;
		//Create_CircleEffect();
	}

	// HP 체크
	if (m_iBossCurHP < 0)
	{
		m_eCurState = BOSS_DEAD;
		m_iBossCurHP = 0;
	}

	GetDamage_Update(fTimeDelta);

	Frame_Manage(fTimeDelta);

	if (!m_bRageUp)
	{
		if (m_iBossCurHP <= m_iBossMaxHP / 2)
		{
			m_eCurState = RAGE_UP;

			m_iBossCurHP = m_iBossMaxHP / 2;
			Initialize_Variable();
			m_bRageUp = true;
		}
		
	}

	State_Update(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_BOSS, this);

	return 0;
}

void CDocheol::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();


	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);


}

void CDocheol::Render_GameObject()
{
	if (!m_bRender)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_HitBlendRender) {
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(AlphaValue, R, G, B));
		m_pGraphicDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pGraphicDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	}

	switch (m_eCurState)
	{
	case INTRO1:
		m_pTextureCom[TEX_HANDUP_APPEAR]->Set_Texture((_uint)m_fFrame);
		break;
	case INTRO2:
		m_pTextureCom[TEX_APPEAR]->Set_Texture((_uint)m_fFrame);
		break;
	case IDLE:
		m_pTextureCom[TEX_IDLE]->Set_Texture((_uint)m_fFrame);
		break;
	case SWING:
		m_pTextureCom[TEX_SWING]->Set_Texture((_uint)m_fFrame);
		break;
	case TWOHANDSLAM:
		m_pTextureCom[TEX_TWOHANDSLAM]->Set_Texture((_uint)m_fFrame);
		break;
	case FULLSWING:
		m_pTextureCom[TEX_FULLSWING]->Set_Texture((_uint)m_fFrame);
		break;
	case RAGE_UP:
		m_pTextureCom[TEX_RAGE_UP]->Set_Texture((_uint)m_fFrame);
		break;
	case RAGE_IDLE:
		m_pTextureCom[TEX_RAGE_IDLE]->Set_Texture((_uint)m_fFrame);
		break;
	case RAGE_SWING:
		m_pTextureCom[TEX_RAGE_SWING]->Set_Texture((_uint)m_fFrame);
		break;
	case RAGE_TWOHANDSLAM:
		m_pTextureCom[TEX_RAGE_TWOHANDSLAM]->Set_Texture((_uint)m_fFrame);
		break;
	case RAGE_CHARGE:
		if(!m_bMotionIdle)
			m_pTextureCom[TEX_RAGE_CHARGE]->Set_Texture((_uint)m_fFrame); 
		else
			m_pTextureCom[TEX_RAGE_IDLE]->Set_Texture((_uint)m_fFrame);
		break;
	case RAGE_AREASLAM:
		if(m_bSlamShake)
			m_pTextureCom[TEX_RAGE_SLAMSHAKE]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureCom[TEX_RAGE_TWOHANDSLAM]->Set_Texture((_uint)m_fFrame);
		break;
	case BOSS_DEAD:
		m_pTextureCom[TEX_DEAD]->Set_Texture((_uint)m_fFrame);
		break;
	case SPECIAL_PRATTERN:
		if(m_bChargeMotion)
			m_pTextureCom[TEX_RAGE_CHARGE]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureCom[TEX_RAGE_IDLE]->Set_Texture((_uint)m_fFrame);
		break;
	}

	if (m_HitBlendRender) {
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		m_pGraphicDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	}

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CDocheol::Intro_HandUpAppear(const _float& fTimeDelta)
{
	TIME_STOP_VOID

	m_fSoundTime += fTimeDelta;
	
	if (!m_bSound[2])
	{
		if (m_fSoundTime > 0.1f)
		{
			CAMERA_TARGET_CHANGE(2.f)
				m_bSound[2] = true;
		}
		
	}

	if (m_fSoundTime > 9.5f)
	{
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_appear_3.wav", CSoundMgr::DOCHEOL_EFFECT3, 0.8f);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_appear_5.wav", CSoundMgr::DOCHEOL_EFFECT4, 0.8f);
			m_bSound[0] = true;
		}

		CAMERA_SHAKE(0.7f);
	}

	if (m_fSoundTime > 10.27f)
	{
		if (!m_bSound[1])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_appear_4", CSoundMgr::DOCHEOL_EFFECT2, 1.00f);
			m_bSound[1] = true;
		}

		CAMERA_SHAKE(0.5f);
	}



	m_fAccTime += fTimeDelta;

	if (m_fAccTime < 3.f)
		return;

	
	Create_IntroEffect(fTimeDelta);

	m_pTransformCom->m_vScale = { 20.f , 20.f  , 10.f };
	m_pTransformCom->m_vInfo[INFO_POS].y = 20.f;

	m_fMaxFrame = 13.f;

	if (m_bRender)
	{
		m_fFrame += m_fMaxFrame * fTimeDelta * 0.5f ;

		if (m_fFrame > m_fMaxFrame)
		{
			m_fFrame = m_fMaxFrame;
			m_fAccTime2 += fTimeDelta;

			if (m_fAccTime2 > 0.00f)
			{
				Create_IntroEffect2(fTimeDelta);
			}
		}
	}
}

void CDocheol::Intro_Appear(const _float& fTimeDelta)
{
	TIME_STOP_VOID

	m_fSoundTime += fTimeDelta;

	if (m_fSoundTime > 1.f)
	{
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_1.wav", CSoundMgr::DOCHEOL_EFFECT, 1.f);
			
			m_bSound[0] = true;
		}

		CAMERA_SHAKE(0.7f);
	}


	m_fMaxFrame = 24.f;

	m_pTransformCom->m_vScale = { 50.f , 50.f  , 50.f };
	m_pTransformCom->m_vInfo[INFO_POS].y = 28.f;

	if (m_fFrame < 13.f)
	{
		m_fFrame += m_fMaxFrame * fTimeDelta * 0.4f;	
	}
	else 
	{
		m_fAccTime += fTimeDelta;

		if(m_fAccTime > 0.2f)
		{
			m_fFrame += m_fMaxFrame * fTimeDelta * 0.3f;

			if (!m_bMake[0])
			{
				Create_TileEffect(fTimeDelta);
				m_bMake[0] = true;
			}
		}
		
	}

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;

		m_eCurState = IDLE;
		Initialize_Variable();
		
	}
}

void CDocheol::Idle(const _float& fTimeDelta)
{
	m_fMaxFrame = 7.f;

	m_fFrame += m_fMaxFrame * fTimeDelta;

	m_fAccTime += fTimeDelta;

	if (!m_bMake[0])
	{
		m_pTransformCom->m_vInfo[INFO_POS].x = 126.f;
		m_bMake[0] = true;
	}

	PlayerCheck();

	if (m_fFrame > m_fMaxFrame)
		m_fFrame = 0.f;

	switch (m_iPattern)
	{
	case 1:
		if (m_fAccTime > 0.9f)
		{
			if (!m_bSound[0])
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
				CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_BULLET, 1.f);
				m_bSound[0] = true;
			}
		}

		if (m_fAccTime > 1.f)
		{
			m_eCurState = SWING;
			Initialize_Variable();
			++m_iPattern;
		}
		break;
	case 2:
		if (m_fAccTime > 1.f)
		{
			m_eCurState = TWOHANDSLAM;
			Initialize_Variable();
			++m_iPattern;
		}
		break;
	case 3:
		if (m_fAccTime > 0.9f)
		{
			if (!m_bSound[0])
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
				CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_BULLET, 1.f);
				m_bSound[0] = true;
			}
		}

		if (m_fAccTime > 1.f)
		{
			m_eCurState = SWING;
			Initialize_Variable();
			++m_iPattern;
		}
		break;
	case 4:
		if (m_iPatternRelpayCnt == 0)
		{
			if (m_fAccTime > 1.f)
			{
				++m_iPatternRelpayCnt;
				m_eCurState = TWOHANDSLAM;
				Initialize_Variable();
				m_iHandSlamCnt = rand()%2;
			}
		}
		else
		{
			if (m_fAccTime > 0.02f)
			{
				++m_iPatternRelpayCnt;
				m_eCurState = TWOHANDSLAM;
				Initialize_Variable();
				m_iHandSlamCnt = rand() % 2;
			}
		}

		if (m_iPatternRelpayCnt == 2)
		{
			++m_iPattern;
			m_iPatternRelpayCnt = 0;
		}
	case 5:
		if (m_fAccTime > 1.f)
		{
			m_eCurState = FULLSWING;
			Initialize_Variable();
			++m_iPattern;
		}
		break;
	default:
		break;
	}
}

void CDocheol::Swing(const float& fTimeDelta)
{
	m_fSoundTime += fTimeDelta;

	if (m_fSoundTime > 0.03f)
	{
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_BULLET2, 1.f);
			m_bSound[0] = true;
		}
	}

	if (m_fSoundTime > 0.16f)
	{
		if (!m_bSound[1])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_BULLET3, 1.f);
			m_bSound[1] = true;
		}
	}

	m_fMaxFrame = 11.f;

	m_fFrame += m_fMaxFrame * fTimeDelta * 1.1f;

	m_fAccTime += fTimeDelta;

	if (!m_bMake[0])
	{
		m_pTransformCom->m_vInfo[INFO_POS].x = 126.f;
		m_bMake[0] = true;
	}

	if (m_fFrame > 5.f)
	{
		if (!m_bMake[1])
		{
			Create_SwingEffect(fTimeDelta);
			m_bMake[1] = true;
		}

		if (m_fAccTime > 0.13f)	// 총알 간격 조정
		{
			if (m_iCnt <= 2)
			{
				Create_SwingBullet(fTimeDelta);
				m_fAccTime = 0.f;
				++m_iCnt;
			}
		}
	}

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_eCurState = IDLE;
		Initialize_Variable();
	}

}

void CDocheol::TwoHandSlam(const float& fTimeDelta)
{
	m_fSoundTime += fTimeDelta;

	if (m_fSoundTime >= 0.f)
	{
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_1.wav", CSoundMgr::DOCHEOL, 0.6f);
			m_bSound[0] = true;
		}
	}

	if (m_fSoundTime >= 0.5f)
	{
		if (!m_bSound[1])
		{
			if(m_iHandSlamCnt % 2 == 0)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
				CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_2.wav", CSoundMgr::DOCHEOL2, 0.6f);

				
			}
			else
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
				CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_2_1.wav", CSoundMgr::DOCHEOL2, 0.6f);
			}
			
			m_bSound[1] = true;
		}
	}

	if (m_fSoundTime >= 0.8f)
	{
		if (!m_bSound[3])
		{
			CAMERA_SHAKE(0.5f);
			m_bSound[3] = true;
		}
	}

	m_fMaxFrame = 10.f;

	CGameObject* pBullet = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	if (!m_bMake[0])
	{
		Create_SlamAlertEffect(fTimeDelta);
		m_bMake[0] = true;
	}

	// 내리칠때 Circle Slam Effect 생성 
	if (m_fFrame >= 7.f && !m_bMake[1])
	{
		Create_SlamEffect(fTimeDelta);
		m_bMake[1] = true;
	}

	// TwoHandSlam 공격은 둘중 하나
	if (m_iHandSlamCnt % 2 == 0)
	{ // 0.5초후 폭발 시킴

		m_fFrame += m_fMaxFrame * fTimeDelta * 0.9f;

		if (m_fFrame >= 7.5f && !m_bMake[2])
		{
			Create_SlamExplosion(fTimeDelta);
			m_bMake[2] = true;
		}
	}
	else
	{
		if (!m_bSound[2])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_rage_7.wav", CSoundMgr::DOCHEOL_EFFECT, 1.f);

			m_bSound[2] = true;
		}

		m_fFrame += m_fMaxFrame * fTimeDelta * 0.8f;

		if (m_fFrame >= 7.5f && !m_bMake[4])
		{
			_vec3 vPos = (m_vTemp[0] + (m_pTransformCom->m_vInfo[INFO_POS] * 2.f)) / 3.f;
	
			pBullet = CDocheolBullet_SlamExplosion::Create(m_pGraphicDev, vPos);
			pLayer->Add_BulletObject(pBullet);

			m_bMake[4] = true;
		}

		if (m_fFrame >= 8.7f && !m_bMake[5])
		{
			_vec3 vPos = (m_vTemp[0] * 2.f + (m_pTransformCom->m_vInfo[INFO_POS])) / 3.f;

			pBullet = CDocheolBullet_SlamExplosion::Create(m_pGraphicDev, vPos);
			pLayer->Add_BulletObject(pBullet);

			m_bMake[5] = true;
		}


		if (m_fFrame >= 9.9f && !m_bMake[2])
		{
			Create_SlamExplosion(fTimeDelta);
			m_bMake[2] = true;
		}


		if (!m_bMake[3])
		{
			Create_Meteor(fTimeDelta);
			m_bMake[3] = true;
		}

	}

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0;
		m_eCurState = IDLE;
		++m_iHandSlamCnt;
		Initialize_Variable();
	}
}

void CDocheol::FullSwing(const float& fTimeDelta)
{
	m_fSoundTime += fTimeDelta;

	if (m_fSoundTime > 0.f)
	{
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_1.wav", CSoundMgr::DOCHEOL, 0.8f);

			m_bSound[0] = true;
		}
	}

	m_fMaxFrame = 11.f;

	m_fFrame += m_fMaxFrame * fTimeDelta * 0.7f;

	m_fAccTime += fTimeDelta;

	// 초기 각도 , 크기 설정
	if (!m_bMake[0])
	{
		if (m_pTransformCom->m_vScale.x > 0)
			m_fSwingEffectAngle = 180.f;
		else
			m_fSwingEffectAngle = 0.f;

		m_fSwingEffectScale = 30.f;

		m_bMake[0] = true;

		Create_ElctronicEffect(fTimeDelta);
	}

	// 9번 폭팔
	if (m_fFrame > 6.f && m_fAccTime > 0.1f && m_iCnt <=9)
	{
		++m_iCnt;
		Create_FullSwingEffect(fTimeDelta);
		m_fAccTime = 0.f;

		if (!m_bMake[1])
		{
			CGameObject* pEffect = nullptr;
			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			// 쿵 이펙트
			pEffect = CEffect_Docheol_FireCircle::Create(m_pGraphicDev,
				{ m_pTransformCom->m_vInfo[INFO_POS].x + 2.f, 2.f, m_pTransformCom->m_vInfo[INFO_POS].z + 15.f }
			,{70.f , 70.f , 70.f}, CEffect_Docheol_FireCircle::SLAM);
			pLayer->Add_BulletObject(pEffect);

			Create_FullSwingBullet(fTimeDelta);

			m_bMake[1] = true;
		}
	}

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0;
		m_eCurState = IDLE;
		Initialize_Variable();
	}
}

void CDocheol::Rage_Up(const float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;
	m_fAccTime2 += fTimeDelta;
	m_fAccTime3 += fTimeDelta;

	m_fMaxFrame = 32.f;

	m_fFrame += m_fMaxFrame * fTimeDelta * m_fFrameSpeed;

	//CAMERA_LEAN_X

	if (m_fAccTime > 3.f)
	{ // 8초마다 빨파초흰검 총알 4번 발사
		Create_RageUpGimic(fTimeDelta);
	}

	if (m_fAccTime < 44.f)
	{
		if (m_fFrame >= 4.f)
		{
			m_fFrame = 3.9f;
			m_fFrameSpeed = -0.2f;
		}
		else if (m_fFrame < 0.f)
		{
			m_fFrame = 0.f;
			m_fFrameSpeed = 0.2f;
		}
	}
	else
	{
		m_fFrameSpeed = 0.2f;

		if (m_fFrame > m_fMaxFrame)
		{
			m_fFrame = m_fMaxFrame;
			m_eCurState = RAGE_IDLE;
			m_iPattern = 1;
			Create_Fire3Effect();

			Initialize_Variable();
		}

		if (m_fFrame > 14.f  && m_fAccTime2 > 0.03f && m_iCnt <= 12)
		{
			m_fAccTime2 = 0.f;
			Create_RageUp_FireEffect(fTimeDelta);
			++m_iCnt;
		}

		if (m_fFrame > 24.f && !m_bMake[7])
		{
			_vec3 vScale = { 60.f , 60.f , 60.f };
			Create_FireCircleEffect(fTimeDelta , vScale , CEffect_Docheol_FireCircle::INTRO);
			Create_CircleEffect();
			m_bMake[7] = true;
			CAMERA_SHAKE(1.5f);
		}

		if (m_fFrame > 24.5f && !m_bMake[8])
		{
			_vec3 vScale = { 70.f , 70.f , 70.f };
			Create_FireCircleEffect(fTimeDelta , vScale , CEffect_Docheol_FireCircle::INTRO);
			//Create_CircleEffect();
			m_bMake[8] = true;
		}

		if (m_fFrame > 25.f && !m_bMake[9])
		{
			_vec3 vScale = { 80.f , 80.f , 80.f };
			Create_FireCircleEffect(fTimeDelta , vScale ,CEffect_Docheol_FireCircle::INTRO);
			//Create_CircleEffect();
			m_bMake[9] = true;
		}

		if (m_fFrame > 24.f && m_fAccTime3 > 0.5f && m_iCnt2 <=4)
		{
			++m_iCnt2;
			m_fAccTime3 = 0.f;
			Create_RageUpBullet(fTimeDelta);
		}
	}

	if (m_fAccTime > 33.f)
	{
		if (!m_bMake[4])
		{
			// 보스 중심으로 가는 3개의 구체 생성
			Create_FireBall(fTimeDelta);
			m_bMake[4] = true;

			//CAMERA_LEAN_X_RETURN
		}
	}

	if (m_fAccTime > 34.f)
	{
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_BULLET, 0.8f);

			m_bSound[0] = true;
		}
	}

	if (m_fAccTime > 35.f)
	{
		if (!m_bSound[1])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_BULLET2, 0.8f);

			m_bSound[1] = true;
		}
	}

	if (m_fAccTime > 36.f)
	{
		if (!m_bSound[2])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_BULLET3, 0.8f);

			m_bSound[2] = true;
		}
	}

	if (m_fAccTime > 37.f)
	{
		if (!m_bSound[3])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_BULLET, 0.8f);

			m_bSound[3] = true;
		}
	}

	if (m_fAccTime > 38.f)
	{
		if (!m_bSound[4])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_BULLET2, 0.8f);

			m_bSound[4] = true;
		}
	}


	if (m_fAccTime > 39.f)
	{
		if (!m_bSound[5])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_BULLET3, 0.8f);

			m_bSound[5] = true;
		}
	}

	if (m_fAccTime > 40.f)
	{
		if (!m_bSound[6])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_BULLET, 0.8f);

			m_bSound[6] = true;
		}
	}

	if (m_fAccTime > 41.f)
	{
		if (!m_bSound[7])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_BULLET2, 0.8f);

			m_bSound[7] = true;
		}
	}

	if (m_fAccTime > 42.f)
	{
		if (!m_bSound[8])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_BULLET3, 0.8f);

			m_bSound[8] = true;
		}
	}

	if (m_fAccTime > 47.0f)
	{
		if (!m_bSound[9])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_2_1.wav", CSoundMgr::DOCHEOL_EFFECT, 1.f);

			m_bSound[9] = true;
		}
	}

	if (m_fAccTime > 45.5f)
	{
		if (!m_bSound[10])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL, 1.f);
		}

		m_bSound[10] = true;
	}


	if (m_fAccTime > 47.f)
	{
		if (!m_bSound[11])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_BULLET, 1.f);

			m_bSound[11] = true;
		}
	}

	if (m_fAccTime > 47.5f)
	{
		if (!m_bSound[12])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_BULLET2, 1.f);

			m_bSound[12] = true;
		}
	}

	if (m_fAccTime > 48.f)
	{
		if (!m_bSound[13])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_BULLET3, 1.f);

			m_bSound[13] = true;
		}
	}
}

void CDocheol::Special_Pattern(const float& fTimeDelta)
{
	m_fSoundTime += fTimeDelta;

	if (m_fSoundTime > 3.7f)
	{
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Kaff_Appear.wav", CSoundMgr::DOCHEOL_EFFECT, 1.f);

			m_bSound[0] = true;
		}
	}
	
	if (m_fSoundTime > 6.7f)
	{
		if (!m_bSound[1])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Kaff_Appear.wav", CSoundMgr::DOCHEOL_EFFECT, 1.f);

			m_bSound[1] = true;
		}
	}

	if (m_fSoundTime > 20.f)
	{
		if (!m_bSound[2])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Kaff_Appear.wav", CSoundMgr::DOCHEOL_EFFECT, 1.f);

			m_bSound[2] = true;
		}
	}

	if (m_fSoundTime > 23.f)
	{
		if (!m_bSound[3])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Kaff_Appear.wav", CSoundMgr::DOCHEOL_EFFECT, 1.f);

			m_bSound[3] = true;
		}
	}

	if (m_bChargeMotion)
	{
		m_fMaxFrame = 7.f;

		m_fAccTime += fTimeDelta;

		m_fFrame += m_fMaxFrame * fTimeDelta;

		if (m_fFrame > m_fMaxFrame)
		{
			m_fFrame = 0.f;
			++m_iCnt;
		}

		if (m_iCnt == 3)
		{
			if (!m_bMake[0])
			{
				CGameObject* pEffect = nullptr;
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

				pEffect = CEffect_Docheol_Fire::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], { 60.f, 60.f , 1.f }, CEffect_Docheol_Fire::INTRO, 1.f);
				pLayer->Add_BulletObject(pEffect);
				m_bMake[0] = true;
			}

			m_fAccTime = 0.f;
		}



		if (m_iCnt >= 4 && m_fAccTime > 0.5f)
		{
			m_bRender = false;
			m_bChargeMotion = false;

			m_pTransformCom->m_vInfo[INFO_POS].z += 50.f;
			m_fAccTime = 0.f;
			m_fFrame = 0.f;
		}
	}
	else
	{
		m_fAccTime2 += fTimeDelta;
		m_fAccTime3 += fTimeDelta;

		m_fMaxFrame = 7.f;

		if (m_fAccTime2 > 1.5f)
		{
			if (!m_bMake[1])
			{
				CGameObject* pEffect = nullptr;
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

				pEffect = CEffect_Docheol_Fire::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], { 60.f, 60.f , 1.f }, CEffect_Docheol_Fire::SPECIAL, 1.f);
				pLayer->Add_BulletObject(pEffect);
				m_bMake[1] = true;

				CAMERA_LEAN_X
			}
		}

		if (m_fAccTime2 > 2.5f)
		{
			if (!m_bMake[6])
			{
				m_bRender = true;
				m_bMake[6] = true;
			}

			m_fFrame += m_fMaxFrame * fTimeDelta;

			if (m_fFrame > m_fMaxFrame)
				m_fFrame = 0.f;
		}

		if (m_fAccTime2 > 3.f)
		{
			m_fAccTime3 += fTimeDelta;

			if (m_fAccTime3 > 0.7f)
			{
				m_fAccTime3 = 0.f;
				Create_SpecialChargeBullet();

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
				CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_2_2.wav", CSoundMgr::DOCHEOL_BULLET, 0.17f);
			}
		}


		if (m_fAccTime2 > 15.f)
		{
			if (!m_bMake[2])
			{
				CGameObject* pEffect = nullptr;
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

				pEffect = CEffect_Docheol_Fire::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], { 60.f, 60.f , 1.f }, CEffect_Docheol_Fire::SPECIAL, 1.f);
				pLayer->Add_BulletObject(pEffect);
				m_bMake[2] = true;
			}
		}

		if (m_fAccTime2 > 16.f)
		{
			if (!m_bMake[3])
			{
				m_bRender = false;
				m_pTransformCom->m_vInfo[INFO_POS].z -= 50.f;
				m_bMake[3] = true;
			}
		}

		if (m_fAccTime2 > 17.f)
		{
			if (!m_bMake[4])
			{
				CGameObject* pEffect = nullptr;
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

				pEffect = CEffect_Docheol_Fire::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], { 60.f, 60.f , 1.f }, CEffect_Docheol_Fire::SPECIAL, 1.f);
				pLayer->Add_BulletObject(pEffect);
				m_bMake[4] = true;
			}
		}

		if (m_fAccTime2 > 18.f)
		{
			if (!m_bMake[5])
			{
				m_bRender = true;
				m_bMake[5] = true;
				m_eCurState = RAGE_IDLE;

				CAMERA_LEAN_X_RETURN
			}
		}

	}
}

void CDocheol::Rage_Idle(const float& fTimeDelta)
{
	m_fMaxFrame = 7.f;

	m_iMaxPattern = 11;

	m_fFrame += m_fMaxFrame * fTimeDelta;

	if (m_fFrame > m_fMaxFrame)
		m_fFrame = 0.f;

	m_fAccTime += fTimeDelta;

	PlayerCheck();

	switch (m_iPattern)
	{
	case 1:
		if (m_fAccTime > 0.9f)
		{
			if (!m_bSound[0])
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
				CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_BULLET, 1.f);
				m_bSound[0] = true;
			}
		}

		if (m_fAccTime > 1.f)
		{
			m_eCurState = RAGE_SWING;
			Initialize_Variable();
			++m_iPattern;
		}
		break;
	case 2:
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_BULLET, 1.f);
			m_bSound[0] = true;
		}
	
		if (m_fAccTime > 0.1f)
		{
			m_eCurState = RAGE_SWING;
			Initialize_Variable();
			++m_iPattern;
		}
		break;
	case 3:
		if (m_iPatternRelpayCnt == 0)
		{
			if (m_fAccTime > 1.f)
			{
				++m_iPatternRelpayCnt;
				m_eCurState = RAGE_TWOHANDSLAM;
				Initialize_Variable();
				m_iHandSlamCnt = rand() % 2;
			}
		}
		else
		{
			if (m_fAccTime > 0.02f)
			{
				++m_iPatternRelpayCnt;
				m_eCurState = RAGE_TWOHANDSLAM;
				Initialize_Variable();
				m_iHandSlamCnt = rand() % 2;
			}
		}

		if (m_iPatternRelpayCnt == 2)
		{
			++m_iPattern;
			m_iPatternRelpayCnt = 0;
		}
		break;
	case 4:
		if (!m_bChargePattern)
		{
			if (m_fAccTime > 1.f)
			{
				m_eCurState = RAGE_CHARGE;
				Initialize_Variable();
				++m_iPattern;
				m_bChargePattern = true;
			}
		}
		else
		{
			++m_iPattern;
		}
		break;
	case 5:
		if (m_iPatternRelpayCnt == 0)
		{
			if (m_fAccTime > 1.f)
			{
				++m_iPatternRelpayCnt;
				m_eCurState = RAGE_TWOHANDSLAM;
				Initialize_Variable();
				m_iHandSlamCnt = rand() % 2;
			}
		}
		else
		{
			if (m_fAccTime > 0.02f)
			{
				++m_iPatternRelpayCnt;
				m_eCurState = RAGE_TWOHANDSLAM;
				Initialize_Variable();
				m_iHandSlamCnt = rand() % 2;
			}
		}

		if (m_iPatternRelpayCnt == 2)
		{
			++m_iPattern;
			m_iPatternRelpayCnt = 0;
		}
		break;

	case 6:
		if (m_fAccTime > 0.9f)
		{
			if (!m_bSound[0])
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
				CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_BULLET, 1.f);
				m_bSound[0] = true;
			}
		}

		if (m_fAccTime > 1.f)
		{
			m_eCurState = RAGE_SWING;
			Initialize_Variable();
			++m_iPattern;
		}
		break;
	case 7:
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_BULLET, 1.f);
			m_bSound[0] = true;
		}

		if (m_fAccTime > 0.1f)
		{
			m_eCurState = RAGE_SWING;
			Initialize_Variable();
			++m_iPattern;
		}
		break;

	case 8:
		if (m_fAccTime > 1.f)
		{
			m_eCurState = RAGE_AREASLAM;
			Initialize_Variable();
			++m_iPattern;
		}
		break;

	case 9:
		if (m_fAccTime > 0.9f)
		{
			if (!m_bSound[0])
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
				CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_BULLET, 1.f);
				m_bSound[0] = true;
			}
		}

		if (m_fAccTime > 1.f)
		{
			m_eCurState = RAGE_SWING;
			Initialize_Variable();
			++m_iPattern;
		}
		break;
	case 10:
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_BULLET, 1.f);
			m_bSound[0] = true;
		}

		if (m_fAccTime > 0.1f)
		{
			m_eCurState = RAGE_SWING;
			Initialize_Variable();
			++m_iPattern;
		}
		break;

	case 11:
		if (m_fAccTime > 1.f)
		{
			m_eCurState = SPECIAL_PRATTERN;
			Initialize_Variable();
			++m_iPattern;
		}
		break;
	default:
		break;
	}
}

void CDocheol::Rage_Swing(const float& fTimeDelta)
{
	m_fSoundTime += fTimeDelta;

	if (m_fSoundTime > 0.03f)
	{
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_BULLET2, 1.f);
			m_bSound[0] = true;
		}
	}

	if (m_fSoundTime > 0.16f)
	{
		if (!m_bSound[1])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_BULLET3, 1.f);
			m_bSound[1] = true;
		}
	}

	m_fMaxFrame = 10.f;

	m_fFrame += m_fMaxFrame * fTimeDelta * 1.1f;

	m_fAccTime += fTimeDelta;

	if (m_fFrame > 5.f)
	{
		if (!m_bMake[1])
		{
			Create_SwingEffect(fTimeDelta);
			m_bMake[1] = true;
		}

		if (m_fAccTime > 0.13f)	// 총알 간격 조정
		{
			if (m_iCnt <= 2)
			{
				Create_SwingBullet(fTimeDelta);
				m_fAccTime = 0.f;
				++m_iCnt;
			}
		}
	}

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_eCurState = RAGE_IDLE;
		Initialize_Variable();
	}
}

void CDocheol::Rage_TwoHandSlam(const float& fTimeDelta)
{
	m_fSoundTime += fTimeDelta;

	if (m_fSoundTime >= 0.f)
	{
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_1.wav", CSoundMgr::DOCHEOL, 0.6f);
			m_bSound[0] = true;
		}
	}

	if (m_fSoundTime >= 0.5f)
	{
		if (!m_bSound[1])
		{
			if (m_iHandSlamCnt % 2 == 0)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
				CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_2.wav", CSoundMgr::DOCHEOL2, 0.6f);


			}
			else
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
				CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_2_1.wav", CSoundMgr::DOCHEOL2, 0.6f);
			}

			m_bSound[1] = true;
		}
	}

	if (m_fSoundTime >= 0.8f)
	{
		if (!m_bSound[3])
		{
			CAMERA_SHAKE(0.5f);
			m_bSound[3] = true;
		}
	}


	m_fMaxFrame = 10.f;

	CGameObject* pBullet = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	if (!m_bMake[0])
	{
		Create_SlamAlertEffect(fTimeDelta);
		m_bMake[0] = true;
	}

	// 내리칠때 Circle Slam Effect 생성 
	if (m_fFrame >= 7.f && !m_bMake[1])
	{
		Create_SlamEffect(fTimeDelta);
		m_bMake[1] = true;
	}

	// TwoHandSlam 공격은 둘중 하나
	if (m_iHandSlamCnt % 2 == 0)
	{ // 0.5초후 폭발 시킴

		m_fFrame += m_fMaxFrame * fTimeDelta * 0.9f;

		if (m_fFrame >= 7.5f && !m_bMake[2])
		{
			Create_SlamExplosion(fTimeDelta);
			m_bMake[2] = true;
		}
	}
	else
	{
		m_fFrame += m_fMaxFrame * fTimeDelta * 0.8f;

		if (m_fFrame >= 7.5f && !m_bMake[4])
		{
			_vec3 vPos = (m_vTemp[0] + (m_pTransformCom->m_vInfo[INFO_POS] * 2.f)) / 3.f;

			pBullet = CDocheolBullet_SlamExplosion::Create(m_pGraphicDev, vPos);
			pLayer->Add_BulletObject(pBullet);

			m_bMake[4] = true;
		}

		if (m_fFrame >= 8.7f && !m_bMake[5])
		{
			_vec3 vPos = (m_vTemp[0] * 2.f + (m_pTransformCom->m_vInfo[INFO_POS])) / 3.f;

			pBullet = CDocheolBullet_SlamExplosion::Create(m_pGraphicDev, vPos);
			pLayer->Add_BulletObject(pBullet);

			m_bMake[5] = true;
		}


		if (m_fFrame >= 9.9f && !m_bMake[2])
		{
			Create_SlamExplosion(fTimeDelta);
			m_bMake[2] = true;
		}


		if (!m_bMake[3])
		{
			Create_Meteor(fTimeDelta);
			m_bMake[3] = true;
		}

	}

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0;
		m_eCurState = RAGE_IDLE;
		++m_iHandSlamCnt;
		Initialize_Variable();
	}
}

void CDocheol::Rage_Charge(const float& fTimeDelta)
{
	if (!m_bMake[1])
	{
		_vec3 vScale = { 60.f , 60.f, 60.f };

		CGameObject* pEffect = nullptr;
		CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

		_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
		vPos.z += 12.f;
		vPos.x += 1.f;
		pEffect = CEffect_Docheol_FireCircle::Create(m_pGraphicDev, vPos, vScale, CEffect_Docheol_FireCircle::SLAM);
		pLayer->Add_BulletObject(pEffect);

		m_bMake[1] = true;
	}

	m_fAccTime += fTimeDelta;
	m_fAccTime2 += fTimeDelta;

	m_fSoundTime += fTimeDelta;

	if (m_fSoundTime > 2.f)
	{
		if(!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_2_2.wav", CSoundMgr::DOCHEOL_EFFECT, 0.8f);

			m_bSound[0] = true;
		}
	}

	if (m_fSoundTime > 2.4f)
	{
		if (!m_bSound[1])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_2_2.wav", CSoundMgr::DOCHEOL_EFFECT, 0.8f);

			m_bSound[1] = true;
		}
	}

	if (m_fSoundTime > 2.8f)
	{
		if (!m_bSound[2])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_2_2.wav", CSoundMgr::DOCHEOL_EFFECT, 0.8f);

			m_bSound[2] = true;
		}
	}

	if (m_fSoundTime > 3.2f)
	{
		if (!m_bSound[3])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_2_2.wav", CSoundMgr::DOCHEOL_EFFECT, 0.8f);

			m_bSound[3] = true;
		}
	}

	if (m_fSoundTime > 3.6f)
	{
		if (!m_bSound[4])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_2_2.wav", CSoundMgr::DOCHEOL_EFFECT, 0.8f);

			m_bSound[4] = true;
		}
	}

	if (m_fSoundTime > 4.0f)
	{
		if (!m_bSound[5])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_2_2.wav", CSoundMgr::DOCHEOL_EFFECT, 0.8f);

			m_bSound[5] = true;
		}
	}

	if (m_fSoundTime > 4.4f)
	{
		if (!m_bSound[6])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_2_2.wav", CSoundMgr::DOCHEOL_EFFECT, 0.8f);

			m_bSound[6] = true;
		}
	}

	if (m_fSoundTime > 6.0f)
	{
		if (!m_bSound[7])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_2_3.wav", CSoundMgr::DOCHEOL_EFFECT, 0.8f);

			m_bSound[7] = true;
		}
	}

	if (m_fSoundTime > 9.f)
	{
		if (!m_bSound[8])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_3.wav", CSoundMgr::DOCHEOL_EFFECT, 0.8f);

			m_bSound[8] = true;
		}
	}

	if (m_fSoundTime > 10.f)
	{
		if (!m_bSound[9])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_1_2.wav", CSoundMgr::DOCHEOL_EFFECT, 0.5f);

			m_bSound[9] = true;
		}
	}


	if (!m_bMotionIdle)
	{
		m_fMaxFrame = 7.f;
		m_fFrame += m_fMaxFrame * fTimeDelta * 1.f;

		if (m_fFrame > m_fMaxFrame)
		{
			m_fFrame = 0.f;
			m_bMotionIdle = true;
		}
	}
	else
	{
		m_fMaxFrame = 7.f;
		m_fFrame += m_fMaxFrame * fTimeDelta * 1.f;

		if (m_fFrame > m_fMaxFrame)
			m_fFrame = 0.f;

		
		if (m_fAccTime > 0.4f && m_iCnt <=15)
		{
			Create_ChargeBullet(fTimeDelta);
			++m_iCnt;
			m_fAccTime = 0.f;
		}

		if(m_bCreateMark && !m_bMake[0])
		{ 
			Create_Mark3();

			m_bMake[0] = true;
		}

		if (m_fAccTime2 > 15.f)
		{
			m_eCurState = RAGE_IDLE;
			Initialize_Variable();
		}
	}
}

void CDocheol::Rage_AreaSlam(const float& fTimeDelta)
{
	m_fAccTime2 += fTimeDelta;

	m_fSoundTime += fTimeDelta;

	if (m_fSoundTime > 0.f)
	{
		if (!m_bSound[0])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_rage_1.wav", CSoundMgr::DOCHEOL, 1.f);
			m_bSound[0] = true;
		}
	}

	if (m_fSoundTime > 1.8f)
	{
		if (!m_bSound[1])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_2.wav", CSoundMgr::DOCHEOL2, 0.4f);
			m_bSound[1] = true;
		}
	}

	if (m_fSoundTime > 1.8f)
	{
		if (!m_bSound[9])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET, 0.4f);
			m_bSound[9] = true;
		}
	}

	if (m_fSoundTime > 2.1f)
	{
		if (!m_bSound[10])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f);
			m_bSound[10] = true;
		}
	}

	if (m_fSoundTime > 2.4f)
	{
		if (!m_bSound[11])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET3, 0.4f);
			m_bSound[11] = true;
		}
	}

	if (m_fSoundTime > 2.7f)
	{
		if (!m_bSound[12])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET4);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET4, 0.4f);
			m_bSound[12] = true;
		}
	}

	if (m_fSoundTime > 3.0f)
	{
		if (!m_bSound[13])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET5);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET5, 0.4f);
			m_bSound[13] = true;
		}
	}

	//====================================================================
	
	if (m_fSoundTime > 2.4f)
	{
		if (!m_bSound[2])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_2.wav", CSoundMgr::DOCHEOL2, 0.4f);
			m_bSound[2] = true;
		}
	}

	if (m_fSoundTime > 2.4f)
	{
		if (!m_bSound[14])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET, 0.4f);
			m_bSound[14] = true;
		}
	}

	if (m_fSoundTime > 2.7f)
	{
		if (!m_bSound[15])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f);
			m_bSound[15] = true;
		}
	}

	if (m_fSoundTime > 3.0f)
	{
		if (!m_bSound[16])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET3, 0.4f);
			m_bSound[16] = true;
		}
	}

	if (m_fSoundTime > 3.3f)
	{
		if (!m_bSound[17])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET4);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET4, 0.4f);
			m_bSound[17] = true;
		}
	}

	if (m_fSoundTime > 3.6f)
	{
		if (!m_bSound[18])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET5);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET5, 0.4f);
			m_bSound[18] = true;
		}
	}

	//========================================================================

	if (m_fSoundTime > 3.8f)
	{
		if (!m_bSound[3])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_2.wav", CSoundMgr::DOCHEOL2, 0.4f);
			m_bSound[3] = true;
		}
	}

	if (m_fSoundTime > 3.8f){if   (!m_bSound[19]){CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET,      0.4f);      m_bSound[19] = true;}}
	if (m_fSoundTime > 4.1f) { if (!m_bSound[20]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[20] = true; } }
	if (m_fSoundTime > 4.4f) { if (!m_bSound[21]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[21] = true; } }
	if (m_fSoundTime > 4.7f) { if (!m_bSound[22]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET4); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[22] = true; } }
	if (m_fSoundTime > 5.0f){if   (!m_bSound[23]){CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET5);CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2,    0.4f);    m_bSound[23] = true;}}


	if (m_fSoundTime > 4.4f)
	{
		if (!m_bSound[3])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_2.wav", CSoundMgr::DOCHEOL2, 0.4f);
			m_bSound[3] = true;
		}
	}

	if (m_fSoundTime > 4.4f) { if (!m_bSound[24]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET,   0.4f);   m_bSound[24] = true; } }
	if (m_fSoundTime > 4.7f) { if (!m_bSound[25]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[25] = true; } }
	if (m_fSoundTime > 5.0f) { if (!m_bSound[26]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[26] = true; } }
	if (m_fSoundTime > 5.3f) { if (!m_bSound[27]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET4); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[27] = true; } }
	if (m_fSoundTime > 5.6f) { if (!m_bSound[28]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET5); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[28] = true; } }


	if (m_fSoundTime > 5.5f)
	{
		if (!m_bSound[4])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_2.wav", CSoundMgr::DOCHEOL2, 0.4f);
			m_bSound[4] = true;
		}
	}

	if (m_fSoundTime > 5.5f) { if (!m_bSound[29]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET,   0.4f);   m_bSound[29] = true; } }
	if (m_fSoundTime > 5.8f) { if (!m_bSound[30]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[30] = true; } }
	if (m_fSoundTime > 6.1f) { if (!m_bSound[31]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[31] = true; } }
	if (m_fSoundTime > 6.4f) { if (!m_bSound[32]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET4); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[32] = true; } }
	if (m_fSoundTime > 6.7f) { if (!m_bSound[33]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET5); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[33] = true; } }




	if (m_fSoundTime > 7.0f)
	{
		if (!m_bSound[5])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_2.wav", CSoundMgr::DOCHEOL2, 0.4f);
			m_bSound[5] = true;
		}
	}

	if (m_fSoundTime > 7.0f) { if (!m_bSound[34]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET,   0.4f);   m_bSound[34] = true; } }
	if (m_fSoundTime > 7.3f) { if (!m_bSound[35]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[35] = true; } }
	if (m_fSoundTime > 7.6f) { if (!m_bSound[36]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[36] = true; } }
	if (m_fSoundTime > 7.9f) { if (!m_bSound[37]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET4); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[37] = true; } }
	if (m_fSoundTime > 8.2f) { if (!m_bSound[38]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET5); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[38] = true; } }


	if (m_fSoundTime > 8.5f)
	{
		if (!m_bSound[6])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_2.wav", CSoundMgr::DOCHEOL2, 0.4f);
			m_bSound[6] = true;
		}
	}

	if (m_fSoundTime > 8.5f) { if (!m_bSound[39]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET,   0.4f);   m_bSound[39] = true; } }
	if (m_fSoundTime > 8.8f) { if (!m_bSound[40]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[40] = true; } }
	if (m_fSoundTime > 9.1f) { if (!m_bSound[41]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[41] = true; } }
	if (m_fSoundTime > 9.4f) { if (!m_bSound[42]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET4); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[42] = true; } }
	if (m_fSoundTime > 9.7f) { if (!m_bSound[43]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET5); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[43] = true; } }


	if (m_fSoundTime > 10.f)
	{
		if (!m_bSound[7])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_2.wav", CSoundMgr::DOCHEOL2, 0.4f);
			m_bSound[7] = true;
		}
	}

	if (m_fSoundTime > 10.0f) { if (!m_bSound[44]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET, 0.4f);   m_bSound[44] = true; } }
	if (m_fSoundTime > 10.3f) { if (!m_bSound[45]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[45] = true; } }
	if (m_fSoundTime > 10.6f) { if (!m_bSound[46]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[46] = true; } }
	if (m_fSoundTime > 10.9f) { if (!m_bSound[47]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET4); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[47] = true; } }
	if (m_fSoundTime > 11.2f) { if (!m_bSound[48]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET5); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[48] = true; } }
   
  
	if (m_fSoundTime > 11.5f)
	{
		if (!m_bSound[8])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_basicattack_2_2.wav", CSoundMgr::DOCHEOL2, 0.4f);
			m_bSound[8] = true;
		}
	}

	if (m_fSoundTime > 11.5f) { if (!m_bSound[49]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET, 0.4f);   m_bSound[49] = true; } }
	if (m_fSoundTime > 11.8f) { if (!m_bSound[50]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[50] = true; } }
	if (m_fSoundTime > 12.1f) { if (!m_bSound[51]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[51] = true; } }
	if (m_fSoundTime > 12.4f) { if (!m_bSound[52]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET4); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[52] = true; } }
	if (m_fSoundTime > 12.7f) { if (!m_bSound[53]) { CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET5); CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.4f); m_bSound[53] = true; } }



	/*if (m_fSoundTime > 2.3f)
	{
		if (!m_bSound[2])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET, 1.f);
			m_bSound[2] = true;
		}
	}

	if (m_fSoundTime > 2.5f)
	{
		if (!m_bSound[3])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET2, 0.7f);
			m_bSound[3] = true;
		}
	}
	if (m_fSoundTime > 2.7f)
	{
		if (!m_bSound[4])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_BULLET3);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_pattern_r_3_1.wav", CSoundMgr::DOCHEOL_BULLET3, 0.7f);
			m_bSound[4] = true;
		}
	}*/







	if (m_bSlamShake)
	{	
		// 2초정도 SlamShake
		m_fAccTime += fTimeDelta;
		m_fFrame = 0.f;

		if (m_fAccTime > 2.f)
			m_bSlamShake = false;

		if (!m_bMake[0])
		{
			Create_ElctronicEffect2(fTimeDelta);
			m_bMake[0] = true;
		}
	}
	else
	{
		m_fMaxFrame = 10.f;

		m_fFrame += m_fMaxFrame * fTimeDelta;

		m_fAccTime3 += fTimeDelta;

		if (m_fFrame > m_fMaxFrame)
		{
			m_fFrame = m_fMaxFrame;

			if (m_fAccTime3 > 1.5f)
			{
				m_fFrame = 0.f;
				m_fAccTime3 = 0.f;
			}

			++m_iCnt;
		}

		if (m_iRotCnt <= 6)
		{
			if (m_fAccTime2 > 0.2f && m_iExplosionCnt <= 7)
			{
				m_fAccTime2 = 0.f;

				m_vDir = { cosf(D3DXToRadian(m_fAngle)) , 0.f , sinf(D3DXToRadian(m_fAngle)) };

				Create_AreaSlam(fTimeDelta);

				++m_iExplosionCnt;
				m_fRadius += 30.f;

				if (m_iExplosionCnt == 8)
				{
					m_fRadius = 10.f;
					m_iExplosionCnt = 0.f;

					m_fAngle += 20.f;

					m_fTemp = 0.f;
					++m_iRotCnt;
				}
			}
		}

		if (m_iRotCnt == 7)
			m_eCurState = RAGE_IDLE;

	}
}

void CDocheol::Boss_Dead(const _float& fTimeDelta)
{
	m_fSoundTime += fTimeDelta;

	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	if (!m_bMake[3])
	{
		CAMERA_TARGET_CHANGE(0.7f)
		m_bMake[3] = true;
	}
	if (!m_bMake[5])
	{
		CAMERA_SHAKE(4.f)
			m_bMake[5] = true;
	}

	m_fAccTime += fTimeDelta;

	if (!m_bSound[0])
	{
		if (m_fAccTime > 0.5f)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_Death_1.wav", CSoundMgr::DOCHEOL, 1.f);
			m_bSound[0] = true;
		}
	}

	if (m_fSoundTime > 1.5f)
	{
		if (!m_bSound[1])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL2);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_Death_2.wav", CSoundMgr::DOCHEOL2, 1.f);
			m_bSound[1] = true;
		}
	}

	if (m_fSoundTime > 3.f)
	{
		if (!m_bSound[2])
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
			CSoundMgr::Get_Instance()->PlaySound(L"Docheol_Death_3.wav", CSoundMgr::DOCHEOL_EFFECT, 1.f);
			m_bSound[2] = true;
		}
	}

	if (!m_bMake[0])
	{
		// FloorFire 이펙트 생성
		pEffect = CEffect_Docheol_FloorFire::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS] , 4.f);
		pLayer->Add_BulletObject(pEffect);

		m_bMake[0] = true;
	}

	if (m_fFrame > 23.f)
	{
		if (!m_bMake[1])
		{
			_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
			vPos.x -= 6.f;
			vPos.z = 136.f;
			vPos.y = 24.f;
			_vec3 vSclae = { 30.f , 30.f, 1.f };

			pEffect = CEffect_Docheol_FloorFireRayUp::Create(m_pGraphicDev, vPos , vSclae);
			pLayer->Add_BulletObject(pEffect);
			m_bMake[1] = true;
		}
	}

	if (m_fFrame > 25.f)
	{
		if (!m_bMake[2])
		{
			_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
			vPos.x += 8.f;
			vPos.z = 135.f;
			vPos.y = 20.f;
			_vec3 vSclae = { 25.f , 25.f, 1.f };

			pEffect = CEffect_Docheol_FloorFireRayUp::Create(m_pGraphicDev, vPos, vSclae);
			pLayer->Add_BulletObject(pEffect);
			m_bMake[2] = true;
		}
	}


	if (m_fSoundTime > 3.f)
	{
		if (!m_bMake[4])
		{
			Create_DeathEffect();
			m_bMake[4] = true;
		}
	}

	


	m_fMaxFrame = 26.f;

	m_fFrame += m_fMaxFrame * fTimeDelta * 0.3f;

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame;
		m_bRender = false;
	}

}

HRESULT CDocheol::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom[TEX_APPEAR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_Appear"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_Appear", pComponent });

	pComponent = m_pTextureCom[TEX_HANDUP_APPEAR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_HandUpAppear"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_HandUpAppear", pComponent });

	pComponent = m_pTextureCom[TEX_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_Stand"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_Stand", pComponent });

	pComponent = m_pTextureCom[TEX_SWING] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_Swing"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_Swing", pComponent });

	pComponent = m_pTextureCom[TEX_TWOHANDSLAM] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_TwoHandSlam"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_TwoHandSlam", pComponent });

	pComponent = m_pTextureCom[TEX_FULLSWING] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_FullSwing"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_FullSwing", pComponent });

	pComponent = m_pTextureCom[TEX_RAGE_UP] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_RageUp"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_RageUp", pComponent });

	pComponent = m_pTextureCom[TEX_RAGE_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_RageStand"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_RageStand", pComponent });

	pComponent = m_pTextureCom[TEX_RAGE_SWING] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_RageSwing"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_RageSwing", pComponent });

	pComponent = m_pTextureCom[TEX_RAGE_TWOHANDSLAM] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_RageTwoHandSlam"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_RageTwoHandSlam", pComponent });

	pComponent = m_pTextureCom[TEX_RAGE_CHARGE] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_RageCharge"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_RageCharge", pComponent });

	pComponent = m_pTextureCom[TEX_RAGE_SLAMSHAKE] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_TwoHandSlamShake"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_TwoHandSlamShake", pComponent });

	pComponent = m_pTextureCom[TEX_DEAD] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Docheol_BossDead"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Docheol_BossDead", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(8.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CDocheol::State_Update(const _float& fTimeDelta)
{
	if (m_eCurState != m_ePreState)
	{
		Initialize_Variable();
	}

	switch (m_eCurState)
	{
	case INTRO1:
		Intro_HandUpAppear(fTimeDelta);
		break;
	case INTRO2:
		Intro_Appear(fTimeDelta); 
		break;
	case IDLE:
		Idle(fTimeDelta);
		break;
	case SWING:
		Swing(fTimeDelta);
		break;
	case TWOHANDSLAM:
		TwoHandSlam(fTimeDelta);
		break;
	case FULLSWING:
		FullSwing(fTimeDelta);
		break;
	case RAGE_UP:
		Rage_Up(fTimeDelta);
		break;
	case RAGE_IDLE:
		Rage_Idle(fTimeDelta);
		break;
	case RAGE_SWING:
		Rage_Swing(fTimeDelta);
		break;
	case RAGE_TWOHANDSLAM:
		Rage_TwoHandSlam(fTimeDelta);
		break;
	case RAGE_CHARGE:
		Rage_Charge(fTimeDelta);
		break;
	case RAGE_AREASLAM:
		Rage_AreaSlam(fTimeDelta);
		break;
	case SPECIAL_PRATTERN:
		Special_Pattern(fTimeDelta);
		break;
	case BOSS_DEAD:
		Boss_Dead(fTimeDelta);
		break;
	}

	m_ePreState = m_eCurState;

	if (m_iPattern > m_iMaxPattern)
		m_iPattern = 1;
}

void CDocheol::Frame_Manage(const _float& fTimeDelta)
{
	if (m_HitBlend) {

		m_HitBlendFrame += m_HitMaxFrame * fTimeDelta * 6.f;
		if (m_HitBlendFrame > m_HitMaxFrame) {
			m_HitBlendFrame = 0.f;
			m_HitBlendCnt++;
			if (m_HitBlendRender)
				m_HitBlendRender = false;
			else {
				m_HitBlendRender = true;
			}
		}
	}

	if (m_HitBlendCnt > m_HitBlendMaxCnt) {
		m_HitBlend = false;
		m_HitBlendRender = false;
		m_HitBlendCnt = 0;
	}
}

void CDocheol::GetDamage_Update(const _float& fTimeDelta)
{
	m_fDamageTimeDelta += fTimeDelta;

	if (!m_Damage_List.empty())
	{
		if (m_fDamageTimeDelta > 0.05f)
		{
			m_iBossCurHP -= m_Damage_List.front();
			m_Damage_List.pop_front();

			m_fDamageTimeDelta = 0.f;
		}
	}
}

void CDocheol::Create_IntroEffect(const _float& fTimeDelta)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	if (!m_bMake[0])
	{
		// FloorFire 이펙트 생성
		pEffect = CEffect_Docheol_FloorFire::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS] , 100.f);
		pLayer->Add_BulletObject(pEffect);
		
		// 첫번째 타일 이펙트 
		pEffect = CEffect_Docheol_Tile::Create(m_pGraphicDev, _vec3(130.42f, 0.2f, 132.70f), _vec3(133.f, 131.f, 1.f), 3, 0.3f);
		pLayer->Add_BulletObject(pEffect);

		CSoundMgr::Get_Instance()->PlaySound(L"Docheol_appear_1.wav", CSoundMgr::DOCHEOL_EFFECT, 0.8f);


		m_bMake[0] = true;
	}

	if (!m_bMake[1] && m_fAccTime > 3.4f)
	{
		// 두번째 타일 이펙트
		pEffect = CEffect_Docheol_Tile::Create(m_pGraphicDev, _vec3(128.f, 0.2f, 136.f), _vec3(91.f, 84.f, 1.f), 3, 0.3f);
		pLayer->Add_BulletObject(pEffect);

		m_bMake[1] = true;
	}

	if (!m_bMake[2] && m_fAccTime > 3.8f)
	{
		// 세번째 타일 이펙트
		pEffect = CEffect_Docheol_Tile::Create(m_pGraphicDev, _vec3(127.f, 0.2f, 136.0f), _vec3(69.f, 66.f, 1.f), 1, 0.3f);
		pLayer->Add_BulletObject(pEffect);

		m_bMake[2] = true;
	}

	if (!m_bMake[3] && m_fAccTime > 4.2f)
	{
		// 도철 AppearMarkFire
		pEffect = CEffect_Docheol_AppearMarkFire::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);
		pLayer->Add_BulletObject(pEffect);

		m_bMake[3] = true;
	}
}

void CDocheol::Create_IntroEffect2(const _float& fTimeDelta)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	if (!m_bMake[4])
	{
		pEffect = CEffect_Docheol_FireExplosion::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS] , CEffect_Docheol_FireExplosion::BOSS);
		pLayer->Add_BulletObject(pEffect);

		pEffect = CEffect_Docheol_FireSpread::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);
		pLayer->Add_BulletObject(pEffect);

		m_bMake[4] = true;

		m_eCurState = INTRO2;

		Initialize_Variable();
	}
}

void CDocheol::Create_TileEffect(const _float& fTimeDelta)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	// 첫번째 타일 이펙트 
	pEffect = CEffect_Docheol_Tile::Create(m_pGraphicDev, _vec3(130.42f, 0.2f, 132.70f), _vec3(133.f, 131.f, 1.f), 3, 1.6f);
	pLayer->Add_BulletObject(pEffect);

	// 두번째 타일 이펙트
	pEffect = CEffect_Docheol_Tile::Create(m_pGraphicDev, _vec3(128.f, 0.2f, 136.f), _vec3(91.f, 84.f, 1.f), 3, 1.6f);
	pLayer->Add_BulletObject(pEffect);

	// 세번째 타일 이펙트
	pEffect = CEffect_Docheol_Tile::Create(m_pGraphicDev, _vec3(127.f, 0.2f, 136.0f), _vec3(69.f, 66.f, 1.f), 1, 1.6f);
	pLayer->Add_BulletObject(pEffect);

	pEffect = CEffect_Docheol_Tile::Create(m_pGraphicDev, _vec3(128.f, 0.2f, 140.0f), _vec3(30.f, 27.f, 1.f), 0, 1.6f);
	
	pLayer->Add_BulletObject(pEffect);

	pEffect = CEffect_Docheol_Tile::Create(m_pGraphicDev, _vec3(83.f, 0.2f, 88.0f), _vec3(19.f, 19.f, 1.f), 2, 1.6f);
	
	pLayer->Add_BulletObject(pEffect);

	pEffect = CEffect_Docheol_Tile::Create(m_pGraphicDev, _vec3(43.f, 0.2f, 158.0f), _vec3(19.f, 19.f, 1.f), 2, 1.6f);
	
	pLayer->Add_BulletObject(pEffect);

	pEffect = CEffect_Docheol_Tile::Create(m_pGraphicDev, _vec3(43.f, 0.2f, 158.0f), _vec3(19.f, 19.f, 1.f), 2, 1.6f);
	
	pLayer->Add_BulletObject(pEffect);

	pEffect = CEffect_Docheol_Tile::Create(m_pGraphicDev, _vec3(217.f, 0.2f, 117.0f), _vec3(19.f, 19.f, 1.f), 2, 1.6f);
	
	pLayer->Add_BulletObject(pEffect);

	pEffect = CEffect_Docheol_Tile::Create(m_pGraphicDev, _vec3(166.f, 0.2f, 181.0f), _vec3(19.f, 19.f, 1.f), 2, 1.6f);
	
	pLayer->Add_BulletObject(pEffect);

	// 마지막 이펙트
	pEffect = CEffect_Docheol_FireCircle::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS] , {60.f , 60.f, 60.f}, CEffect_Docheol_FireCircle::INTRO);
	
	pLayer->Add_BulletObject(pEffect);
}

void CDocheol::Create_SwingEffect(const _float& fTimeDelta)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vScale;

	if(m_pTransformCom->m_vScale.x > 0)
		vScale = { 50.f, 50.f , 50.f };
	else
		vScale = { -50.f, 50.f , 50.f };

	pEffect = CEffect_Docheol_SwingEffect::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS] , vScale);
	pLayer->Add_BulletObject(pEffect);
}

void CDocheol::Create_SlamEffect(const _float& fTimeDelta)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	if(m_pTransformCom->m_vScale.x >0)
		vPos.x -= 25.f;
	else
		vPos.x += 25.f;
	vPos.z += 8.f;

	// 쿵 2개 이펙트
	pEffect = CEffect_Docheol_FireCircle::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], {50.f , 50.f , 1.f}, CEffect_Docheol_FireCircle::SLAM);
	pLayer->Add_BulletObject(pEffect);

	pEffect = CEffect_Docheol_FireCircle::Create(m_pGraphicDev, vPos, { 50.f , 50.f , 1.f },CEffect_Docheol_FireCircle::SLAM);
	pLayer->Add_BulletObject(pEffect);
	
	if (m_iHandSlamCnt % 2 == 1)
	{
		// 불 이펙트 2개
		pEffect = CEffect_Docheol_Fire::Create(m_pGraphicDev, { m_pTransformCom->m_vInfo[INFO_POS].x , 20.f , m_pTransformCom->m_vInfo[INFO_POS].z -4.f}
			, { 20.f , 20.f , 1.f} , CEffect_Docheol_Fire::SLAM);
		pLayer->Add_BulletObject(pEffect);

		pEffect = CEffect_Docheol_Fire::Create(m_pGraphicDev, { vPos.x , 20.f ,vPos.z - 4.f }
			, { 20.f , 20.f , 1.f }, CEffect_Docheol_Fire::SLAM);
		pLayer->Add_BulletObject(pEffect);
	}
}

void CDocheol::Create_SlamAlertEffect(const _float& fTimeDelta)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	 
	
	if (m_iHandSlamCnt%2 == 0)
	{
		m_vTemp[0] = pPlayerTransformCom->m_vInfo[INFO_POS];

		pEffect = CEffect_Docheol_Mark::Create(m_pGraphicDev, m_vTemp[0], _vec3{ 13.f, 13.f, 13.f }, 0.8f);
		pLayer->Add_BulletObject(pEffect);

		pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, { m_vTemp[0].x , 0.01f , m_vTemp[0].z }, _vec3{ 20.f , 20.f, 20.f }, 0.8f, 0.f, TRUE);
		pLayer->Add_BulletObject(pEffect);

		pEffect = CEffect_AlertCircle2::Create(m_pGraphicDev, { m_vTemp[0].x , 0.01f , m_vTemp[0].z }, _vec3{ 20.f , 20.f, 20.f }, 0.8f, 0.f);
		pLayer->Add_BulletObject(pEffect);
	}
	else
	{
		pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, { m_vTemp[0].x , 0.01f , m_vTemp[0].z }, _vec3{ 20.f , 20.f, 20.f }, 1.f, 0.7f , TRUE);
		pLayer->Add_BulletObject(pEffect);

		pEffect = CEffect_AlertCircle2::Create(m_pGraphicDev, { m_vTemp[0].x , 0.01f , m_vTemp[0].z }, _vec3{ 20.f , 20.f, 20.f }, 1.2f, 0.7f);
		pLayer->Add_BulletObject(pEffect);
	}
}

void CDocheol::Create_FullSwingEffect(const _float& fTimeDelta)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float m_fRadius = 30.f;
	_vec3 vPos;
	_float fAngle = 0.f;

	vPos = {  m_pTransformCom->m_vInfo[INFO_POS].x + m_fRadius * cosf(D3DXToRadian(m_fSwingEffectAngle))
			, 5.f 
			, m_pTransformCom->m_vInfo[INFO_POS].z +7.f + m_fRadius * sinf(D3DXToRadian(m_fSwingEffectAngle)) };


	pEffect = CEffect_Docheol_FireExplosion2::Create(m_pGraphicDev, vPos, _vec3{ m_fSwingEffectScale , m_fSwingEffectScale, 1.f });
	pLayer->Add_BulletObject(pEffect);

	if (m_pTransformCom->m_vScale.x > 0)
		m_fSwingEffectAngle += 30.f;
	else
		m_fSwingEffectAngle -= 30.f;

	m_fSwingEffectScale -= 3.f;
}

void CDocheol::Create_ElctronicEffect(const _float& fTimeDelta)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vPos;
	_vec3 vScale;

	if (m_pTransformCom->m_vScale.x > 0)
	{
		vPos = { m_pTransformCom->m_vInfo[INFO_POS].x - 28.f , m_pTransformCom->m_vInfo[INFO_POS].y , m_pTransformCom->m_vInfo[INFO_POS].z };
		vScale = { -15.f , 15.f , 15.f };
	}
	else
	{
		vPos = { m_pTransformCom->m_vInfo[INFO_POS].x + 28.f , m_pTransformCom->m_vInfo[INFO_POS].y , m_pTransformCom->m_vInfo[INFO_POS].z };
		vScale = { 15.f , 15.f , 15.f };
	}

	pEffect = CEffect_Docheol_Electronic::Create(m_pGraphicDev, vPos, vScale);
	pLayer->Add_BulletObject(pEffect);
	
}

void CDocheol::Create_FireBall(const _float& fTimeDelta)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	_vec3 vScale = { 10.f, 10.f, 10.f };

	pEffect = CEffect_Docheol_FireBall::Create(m_pGraphicDev, vPos, vScale, CEffect_Docheol_FireBall::ONE);
	pLayer->Add_BulletObject(pEffect);

	pEffect = CEffect_Docheol_FireBall::Create(m_pGraphicDev, vPos, vScale, CEffect_Docheol_FireBall::TWO);
	pLayer->Add_BulletObject(pEffect);

	pEffect = CEffect_Docheol_FireBall::Create(m_pGraphicDev, vPos, vScale, CEffect_Docheol_FireBall::THREE);
	pLayer->Add_BulletObject(pEffect);

}

void CDocheol::Create_Explosion3()
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	vPos.y -= 30.f;
	_vec3 vScale = { 80.f, 80.f, 80.f };

	pEffect = CEffect_Docheol_FireExplosion3::Create(m_pGraphicDev, vPos, vScale);
	pLayer->Add_BulletObject(pEffect);
}

void CDocheol::Create_RageUp_FireEffect(const _float& fTimeDelta)
{
	if (!m_bMake[6])
	{
		if (m_pTransformCom->m_vScale.x > 0)
			m_fSwingEffectAngle = 180.f;
		else
			m_fSwingEffectAngle = 0.f;

		m_fSwingEffectScale = 30.f;

		m_bMake[6] = true;
	}

	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float m_fRadius = 30.f;
	_vec3 vPos;
	_float fAngle = 0.f;
	_vec3 vScale = { 20.f , 20.f, 20.f };

	vPos = { m_pTransformCom->m_vInfo[INFO_POS].x + m_fRadius * cosf(D3DXToRadian(m_fSwingEffectAngle))
			, 5.f
			, m_pTransformCom->m_vInfo[INFO_POS].z + 7.f + m_fRadius * sinf(D3DXToRadian(m_fSwingEffectAngle)) };


	pEffect = CEffect_Docheol_Fire::Create(m_pGraphicDev, vPos, vScale , CEffect_Docheol_Fire::RAGE_UP);
	pLayer->Add_BulletObject(pEffect);

	if (m_pTransformCom->m_vScale.x > 0)
		m_fSwingEffectAngle += 30.f;
	else
		m_fSwingEffectAngle -= 30.f;

	m_fSwingEffectScale -= 3.f;

}

void CDocheol::Create_FireCircleEffect(const _float& fTimeDelta , const _vec3& vScale , CEffect_Docheol_FireCircle::TYPE eType)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	vPos.x -= 5.f;
	pEffect = CEffect_Docheol_FireCircle::Create(m_pGraphicDev, vPos, vScale , eType);
	
	pLayer->Add_BulletObject(pEffect);
}

void CDocheol::Create_Fire3Effect()
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	_vec3 vScale = { 60.f , 60.f , 60.f };
	
	vPos.x += 2.f;
	vPos.y += 14.f;
	vPos.z += 2.f;

	pEffect = CEffect_Docheol_Fire3::Create(m_pGraphicDev, vPos, vScale, 0.3f);
	
	pLayer->Add_BulletObject(pEffect);
}

void CDocheol::Create_CircleEffect()
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	vPos.x -= 5.f;

	_vec3 vScale = { 200.f , 200.f , 200.f };

	pEffect = CEffect_Docheol_Circle::Create(m_pGraphicDev, vPos, vScale);
	
	pLayer->Add_BulletObject(pEffect);
}

void CDocheol::Create_Mark3()
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	list<_vec3>::iterator iter = m_PosList.begin();

	_vec3 vPos;
	_vec3 vScale = { 5.f , 5.f , 5.f };

	for (; iter != m_PosList.end(); ++iter)
	{
		vPos = (*iter);

		pEffect = CEffect_Docheol_Mark3::Create(m_pGraphicDev, vPos, vScale , 1.2f);
		pLayer->Add_BulletObject(pEffect);
	}

	m_PosList.clear();
}

void CDocheol::Create_ElctronicEffect2(const _float& fTimeDelta)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	vPos.y = 0.1f;
	vPos.x -= 10.f;
	_vec3 vScale = { 20.f ,20.f , 20.f };

	pEffect = CEffect_Docheol_Electronic2::Create(m_pGraphicDev, vPos, vScale , 45.f);
	pLayer->Add_BulletObject(pEffect);

	vPos = m_pTransformCom->m_vInfo[INFO_POS];
	vPos.y = 0.1f;
	vPos.x += 10.f;
	vScale = { 15.f , 15.f , 15.f };

	pEffect = CEffect_Docheol_Electronic2::Create(m_pGraphicDev, vPos, vScale, 135.f);
	pLayer->Add_BulletObject(pEffect);


}

void CDocheol::Create_DeathEffect()
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	pEffect = CEffect_Docheol_Fire::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], { 40.f, 40.f , 1.f }, CEffect_Docheol_Fire::INTRO , 1.f);
	pLayer->Add_BulletObject(pEffect);

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	vPos.z -= 10.f;

	pEffect = CEffect_Docheol_FireSpread::Create(m_pGraphicDev, vPos, 1.f);
	pLayer->Add_BulletObject(pEffect);

}

void CDocheol::Create_SwingBullet(const _float& fTimeDelta)
{
	CGameObject* pBullet = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	if (m_iCnt == 0)
	{
		CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
		 

		_vec3 vDir = pPlayerTransformCom->m_vInfo[INFO_POS] - m_pTransformCom->m_vInfo[INFO_POS];
		vDir.y = 0.f;
		D3DXVec3Normalize(&vDir, &vDir);

		_float  fTheTha = acosf(vDir.x);

		if (vDir.z < 0)
		{
			fTheTha = 2 * D3DX_PI - fTheTha;
		}

		fTheTha = D3DXToDegree(fTheTha);

		for (int i = 0; i < 5; ++i)
		{
			_float fAngle = D3DXToRadian(fTheTha + 12.f * (-2 + i));
			m_vTemp[i] = { cos(fAngle) , 0.f , sin(fAngle) };
		}
	}
	
	_vec3 vScale = { 10.f, 10.f, 10.f };

	for (int i = 0; i < 5; ++i)
	{
		pBullet = CDocheolBullet_SwingBullet::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], m_vTemp[i] ,vScale, 100.f, 3.f);
		pLayer->Add_BulletObject(pBullet);
	}
}

void CDocheol::Create_SlamExplosion(const _float& fTimeDelta)
{
	CGameObject* pBullet = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	pBullet = CDocheolBullet_SlamExplosion::Create(m_pGraphicDev, m_vTemp[0]);
	pLayer->Add_BulletObject(pBullet);
}

void CDocheol::Create_Meteor(const _float& fTimeDelta)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	_int iMeteorCnt = 0;

	if (m_eCurState == TWOHANDSLAM) iMeteorCnt = 5;
	else iMeteorCnt = 10;

	for (int i = 0; i < iMeteorCnt; ++i)
	{
		_float fRandomX = _float(rand() % 200 + 40);
		_float fRandomZ = _float(rand() % 100 + 80);

		_float fRandomX2 = _float(rand() % 400 - 200);
		_float fRandomZ2 = _float(rand() % 400 +100);

		pEffect = CEffect_Docheol_Meteor::Create(m_pGraphicDev, {fRandomX , 3.f , fRandomZ}, { fRandomX2 , 300.f , fRandomZ2 });
		pLayer->Add_BulletObject(pEffect);
	}
}

void CDocheol::Create_FullSwingBullet(const _float& fTimeDelta)
{
	CGameObject* pBullet = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float cosTheta = 0.f;
	_float sinTheta = 0.f;
	_float fSpeed = 0.f;
	_float fAliveTime = 0.f;

	_vec3 vScale = { 10.f , 10.f, 10.f };

	for (int i = 0; i < 60; ++i)
	{
		cosTheta = cos(D3DXToRadian(360.f / 60.f) * i);
		sinTheta = sin(D3DXToRadian(360.f / 60.f) * i);

		fSpeed	= _float(rand() % 70 + 40);
		fAliveTime = _float((rand() % 30 + 10) / 10.f);

		pBullet = CDocheolBullet_SwingBullet::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], {cosTheta , 0.f , sinTheta}, vScale, fSpeed, fAliveTime);
		pLayer->Add_BulletObject(pBullet);
	}
}

void CDocheol::Create_RageUpBullet(const _float& fTimeDelta)
{
	CGameObject* pBullet = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float cosTheta = 0.f;
	_float sinTheta = 0.f;
	_float fSpeed = 80.f;
	_float fAliveTime = 2.f;
	_vec3 vScale = { 10.f , 10.f, 10.f };

	for (int i = 0; i < 40; ++i)
	{
		cosTheta = cos(D3DXToRadian(360.f / 40.f) * i + m_fAlphaAngle);
		sinTheta = sin(D3DXToRadian(360.f / 40.f) * i + m_fAlphaAngle);

		pBullet = CDocheolBullet_SwingBullet::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], { cosTheta , 0.f , sinTheta }, vScale, fSpeed, fAliveTime);
		 
		pLayer->Add_BulletObject(pBullet);

		m_fAlphaAngle += 10.f;
	}

	pBullet = CEffect_Docheol_FireCircle::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], { 100.f , 100.f , 1.f }, CEffect_Docheol_FireCircle::SLAM);
	pLayer->Add_BulletObject(pBullet);

}

void CDocheol::Create_ChargeBullet(const _float& fTimeDelta)
{
	CGameObject* pBullet = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float m_fRadius = 40.f;
	_vec3 vPos;
	_vec3 vScale = { 15.f , 15.f, 15.f };
	_vec3 vDir = { 1.f , 0.f , 0.f };
	_float m_fSpeed = 35.f;

	// 가운데 구
	if (m_iCnt == 0)
	{
		m_fAlphaAngle = 120.f;

		vPos = { m_pTransformCom->m_vInfo[INFO_POS].x, 11.f, m_pTransformCom->m_vInfo[INFO_POS].z - 8.f };
		pBullet = CDocheolBullet_ChargeBullet::Create(m_pGraphicDev, vPos, vDir, vScale, m_fSpeed, false);
		pLayer->Add_BulletObject(pBullet);

		vScale = { 10.f , 10.f, 10.f };
		vPos.y -= 3.f;
		vPos.z += 1.f;

		pBullet = CEffect_Docheol_ChargeMark::Create(m_pGraphicDev, vPos, vDir, vScale, m_fSpeed, false);
		pLayer->Add_BulletObject(pBullet);

	}
	else if (m_iCnt <= 6)
	{
		vPos = { m_pTransformCom->m_vInfo[INFO_POS].x + m_fRadius * cosf(D3DXToRadian(m_fAlphaAngle))
			, 10.f
			, m_pTransformCom->m_vInfo[INFO_POS].z + 7.f + m_fRadius * sinf(D3DXToRadian(m_fAlphaAngle)) };

		vDir = { cosf(D3DXToRadian(m_fAlphaAngle)) , 0.f , sinf(D3DXToRadian(m_fAlphaAngle)) };

		pBullet = CDocheolBullet_ChargeBullet::Create(m_pGraphicDev, vPos, vDir, vScale, m_fSpeed, true);
		pLayer->Add_BulletObject(pBullet);

		vScale = { 10.f , 10.f, 10.f };
		vPos.y -= 3.f;
		vPos.z += 1.f;

		pBullet = CEffect_Docheol_ChargeMark::Create(m_pGraphicDev, vPos, vDir, vScale, m_fSpeed, true);
		pLayer->Add_BulletObject(pBullet);

		m_fAlphaAngle += 60.f;
	}
	else if (m_iCnt == 9)
		m_bBulletGo = true;
	else if (m_iCnt == 13)
		m_bChargeBulletDead = true;
}

void CDocheol::Create_AreaSlam(const _float& fTimeDelta)
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vScale = { 20.f, 20.f , 20.f };

	_vec3 vPos;

	_matrix matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(90.f));

	for (int i = 0; i < 4; ++i)
	{
		vPos = m_pTransformCom->m_vInfo[INFO_POS] + m_fRadius * m_vDir;
		vPos.y = 1.f;

		pEffect = CEffect_AlertCircle::Create(m_pGraphicDev, vPos, vScale, 0.8f, 0.f, TRUE);
		pLayer->Add_BulletObject(pEffect);

		pEffect = CEffect_AlertCircle2::Create(m_pGraphicDev, vPos, vScale, 0.8f, 0.f);
		pLayer->Add_BulletObject(pEffect);
		vPos.y = 5.f;
		vPos.z -= m_fTemp;
		pEffect = CDocheolBullet_SlamExplosion::Create(m_pGraphicDev, vPos , 0.3f);
		pLayer->Add_BulletObject(pEffect);

		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRot);
		m_fTemp += 0.01f;

		
	}

	if (m_iExplosionCnt % 3 == 0)
	{
		for (int i = 0; i < 12; ++i)
		{
			vPos = m_pTransformCom->m_vInfo[INFO_POS];
			_vec3 vDir = { cosf(D3DXToRadian(m_fAngle + 30.f + (360.f / 12.f) * i)) , 0.f , sinf(D3DXToRadian(m_fAngle + 30.f + (360.f / 12.f) * i)) };
			pEffect = CDocheolBullet_SwingBullet::Create(m_pGraphicDev, vPos, vDir, { 8.f , 8.f , 8.f }, 60.f, 3.f);
			pLayer->Add_BulletObject(pEffect);
		}
	}


}

void CDocheol::Create_RageUpGimic(const _float& fTimeDelta)
{
	if (!m_bMake[0])
	{
		Create_SpecialDoor(fTimeDelta);
		Create_DoorBullet(fTimeDelta);
		m_bMake[0] = true;
	}

	if (m_fAccTime > 11.5f && !m_bMake[1])
	{
		Create_DoorBullet(fTimeDelta);
		m_bMake[1] = true;
	}

	if (m_fAccTime > 20.f && !m_bMake[2])
	{
		Create_DoorBullet(fTimeDelta);
		m_bMake[2] = true;
	}

	if (m_fAccTime > 28.5f && !m_bMake[3])
	{
		Create_DoorBullet(fTimeDelta);
		m_bMake[3] = true;
	}
}

void CDocheol::Create_DoorBullet(const _float& fTimeDelta)
{
	CGameObject* pGameObject = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	pGameObject = CDocheolBullet_DoorBullet::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);
	pLayer->Add_BulletObject(pGameObject);
}

void CDocheol::Create_SpecialDoor(const _float& fTimeDelta)
{
	CGameObject* pGameObject = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vPos = { 44.38f , 12.f, 156.95f };
	_vec3 vScale = { 10.f , 12.f, 1.f };

	pGameObject = CDocheol_SpecialDoor::Create(m_pGraphicDev, vPos , vScale , 0.f, 0.f , CDocheol_SpecialDoor::BLUE);
	pLayer->Add_BulletObject(pGameObject);

	vPos = { 83.54f , 12.f , 87.43f };
	vScale = { -10.f , 12.f , 1.f };

	pGameObject = CDocheol_SpecialDoor::Create(m_pGraphicDev, vPos, vScale, 0.f, 0.f , CDocheol_SpecialDoor::RED);
	pLayer->Add_BulletObject(pGameObject);
	
	vPos = { 217.21f , 12.f , 115.91f};
	vScale = { 10.f , 12.f , 1.f };

	pGameObject = CDocheol_SpecialDoor::Create(m_pGraphicDev, vPos, vScale, 0.f, 0.f , CDocheol_SpecialDoor::WHITE);
	pLayer->Add_BulletObject(pGameObject);

	vPos = { 165.64f , 13.f , 179.91f };
	vScale = { -10.f , 12.f , 1.f };

	pGameObject = CDocheol_SpecialDoor::Create(m_pGraphicDev, vPos, vScale, 0.f, 0.f , CDocheol_SpecialDoor::GREEN);
	pLayer->Add_BulletObject(pGameObject);
}

void CDocheol::Create_SpecialChargeBullet()
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float fRandX = rand() % 2300 / 10.f;
	_float fRandZ = (rand() % 500 + 2000) / 10.f;

	_vec3 vPos = { fRandX, 10.f, fRandZ };
	_vec3 vDir = { 0.f , 0.f , -1.f };
	_vec3 vScale = { 15.f , 15.f , 15.f };
	_float	fSpeed = 20.f;
	CGameObject* pBullet = CDocheolBullet_ChargeBullet::Create(m_pGraphicDev, vPos, vDir, vScale, fSpeed, true, CDocheolBullet_ChargeBullet::SPECIAL);
	pLayer->Add_BulletObject(pBullet);

	vScale = { 10.f , 10.f, 10.f };
	vPos.y -= 3.f;
	vPos.z += 1.f;

	pBullet = CEffect_Docheol_ChargeMark::Create(m_pGraphicDev, vPos, vDir, vScale, fSpeed, true, CEffect_Docheol_ChargeMark::SPECIAL);
	pLayer->Add_BulletObject(pBullet);

	m_fAccTime = 0.f;
}

void CDocheol::PlayerCheck()
{
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));


	if (m_pTransformCom->m_vInfo[INFO_POS].x > pPlayerTransformCom->m_vInfo[INFO_POS].x)
		m_pTransformCom->m_vScale = { 50.f , 50.f , 50.f };
	else
		m_pTransformCom->m_vScale = { -50.f , 50.f , 50.f };
}

CDocheol * CDocheol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDocheol * pInstance = new CDocheol(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDocheol::Initialize_Variable()
{
	m_fFrame = 0.f;
	m_fAccTime = 0.f;
	m_fAccTime2 = 0.f;
	m_fAccTime3 = 0.f;

	m_bRender = true;
	for (int i = 0; i < 10; ++i)
		m_bMake[i] = false;

	m_iCnt = 0;
	m_iCnt2 = 0;

	m_fAlphaAngle = 0.f;

	m_bCreateMark = false;
	m_bSlamShake = true;
	m_iExplosionCnt = 0;
	m_iRotCnt = 0.f;
	
	m_bChargeMotion = true;

	m_fSoundTime = 0.f;

	for (int i = 0; i < 55; ++i)
		m_bSound[i] = false;
}

void CDocheol::Free(void)
{
	__super::Free();
}


