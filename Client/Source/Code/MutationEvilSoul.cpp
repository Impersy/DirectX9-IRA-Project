#include "stdafx.h"
#include "..\Header\MutationEvilSoul.h"
#include "Export_Function.h"
#include "MonsterBullet_2.h"

#include "Effect_Monster_Dead_1.h"
#include "Effect_Birth_Charge.h"
#include "Effect_Birth_LandMark.h"
#include "Effect_Summon.h"
#include "Effect_Summon_Wave.h"
#include "Item_Coin.h"
#include <Tutorial.h>

CMutationEvilSoul::CMutationEvilSoul(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev), m_eHead(HEAD_FRONT), m_Count(0)
{
}

CMutationEvilSoul::CMutationEvilSoul(const CMutationEvilSoul& rhs)
	: CMonster(rhs), m_eHead(HEAD_FRONT), m_Count(0)
{
}

CMutationEvilSoul::~CMutationEvilSoul()
{
	Free();
}


HRESULT CMutationEvilSoul::Ready_GameObject(const _vec3 vPos)
{
	Add_Component();
	m_eName = NAME_MUTATION;


	if (g_eCurScene == SCENE_NORMAL)
		m_pTransformCom->Set_Pos(_float(rand() % 150) + 350.f, 4.2f, _float(rand() % 150) + 350.f);
	else {
		m_pTransformCom->Set_Pos(_float(rand() % 200) + 257.f, 5.f, _float(rand() % 200) + 257.f);
		m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	}

	m_iMonsterMaxHP = 90000;
	m_iMonsterCurHP = m_iMonsterMaxHP;

	//m_pTransformCom->Set_Pos(_float(rand() % 200) + 257.f, 5.f, _float(rand() % 200) + 257.f);
	
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));
	m_pTransformCom->UpdatePos_OnWorld();

	m_bBirthEffect[BIRTH_LANDMARK] = true;
	m_bBirthEffect[BIRTH_CHARGE] = true;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_APPEAR);
	CSoundMgr::Get_Instance()->PlaySound(L"Monster_Appear_02.wav", CSoundMgr::MONSTER_APPEAR, 0.7f);

	Create_Effect_Birth_LandMark();
	Create_Effect_Birth_Charge();

	__super::Ready_GameObject();

	return S_OK;
}

_int CMutationEvilSoul::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_DEATH);
		CSoundMgr::Get_Instance()->PlaySound(L"MonsterDeath.wav", CSoundMgr::MONSTER_DEATH, 0.7f);

		Create_Coin();
		Create_Dead_Effect();
		CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

		pLayer->m_Normal_Stage_Mon_Cnt += 1;

		if (g_eCurScene == SCENE_TUTORIAL)
		{
			CTutorial* pTutoScene = dynamic_cast<CTutorial*> (Engine::Get_Scene());
			pTutoScene->Set_MonsterCntDown();

		}

		return OBJ_DEAD;
	}

	if (m_bBirthEffect[BIRTH_LANDMARK])
	{
		m_fAccumulatedTime += fTimeDelta;
		if (1.f < m_fAccumulatedTime)
		{
			m_bBirthEffect[BIRTH_LANDMARK] = false;
			m_bBirthEffect[BIRTH_CHARGE] = false;
			m_fAccumulatedTime = 0.f;
			m_bBirthEffect[BIRTH_SUMMON] = true;
			Create_Effect_Summon();
			Create_Effect_Summon_Wave();
		}
	}

	if (m_bBirthEffect[BIRTH_SUMMON])
	{
		m_bBirthEffect[BIRTH_SUMMON] = false;
		m_bBirthEffect[BIRTH_DONE] = true;
	}

	__super::Update_GameObject(fTimeDelta);
	Frame_Check(fTimeDelta);
	
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));

	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	Change_State();
	Head_Check((m_pTransformCom->Patrol_Map(m_fSpeed, fTimeDelta)));

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_MONSTER, this);

	return OBJ_NOEVENT;
}

void CMutationEvilSoul::LateUpdate_GameObject()
{	
	if (m_bHit)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_HIT);
		CSoundMgr::Get_Instance()->PlaySound(L"Monster_1_Hit.wav", CSoundMgr::MONSTER_HIT, 0.7f);

		m_iMonsterHp -= 1;

		if (m_iMonsterHp)
		{	
			m_HitCount = 1;
			m_bHit = false;
		}
		else
		{	
			m_HitCount = 1;
			m_bDead = true;
		}
	}

	__super::LateUpdate_GameObject();
}

void CMutationEvilSoul::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_HitCount && (m_HitCount < 8))
	{
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(100, 255, 255, 255));
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

		m_HitCount += 1;
	}

	if (HEAD_FRONT == m_eHead)
	{
		m_pTextureCom->Set_Texture((_uint)m_fFrame);

	}
	else if (HEAD_BACK == m_eHead)
	{
		m_pTextureCom_2->Set_Texture((_uint)m_fFrame);

	}

	if (m_bBirthEffect[BIRTH_SUMMON] || m_bBirthEffect[BIRTH_DONE])
		m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CMutationEvilSoul::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	//45 IDLE, ATTACK -> ALL
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_EvilSoul_45_All"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_EvilSoul_45_All", pComponent });

	//135 IDLE, ATTACK -> ALL
	pComponent = m_pTextureCom_2 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_EvilSoul_135_All"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_EvilSoul_135_All", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	 
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(5.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CMutationEvilSoul::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	 
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);

}

void CMutationEvilSoul::Change_State(void)
{
	if ((0.f == m_fFrame) && (5 == m_Count))
	{
		m_eState = MONSTER_ATTACK;
		m_Count = 0;

		if (g_eCurScene != SCENE_TUTORIAL)
		{
			Create_Bullet();
		}
		else
		{
			CTutorial* pTutoScene = dynamic_cast<CTutorial*> (Engine::Get_Scene());
			if (pTutoScene->Get_ChasePlayer())
			{
				Create_Bullet();
			}
		}
	}
}

void CMutationEvilSoul::Frame_Check(const _float& fTimeDelta)
{
	if (m_eState == MONSTER_IDLE)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bCheck = false;
			m_Count++;
		}
	}

	if (m_eState == MONSTER_MOVE)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bCheck = false;
		}
	}

	if (m_eState == MONSTER_ATTACK)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_eState = MONSTER_IDLE;
			m_bCheck = true;
		}
	}
}

void CMutationEvilSoul::Head_Check(const _vec3 vDir)
{
	TIME_STOP_VOID

	if (0.f < vDir.z)	// 플레이어가 몬스터의 위(뒤)쪽에 위치
		m_eHead = HEAD_BACK;
	else if (0.f > vDir.z)	// 플레이어가 몬스터의 아래(앞)쪽에 위치
		m_eHead = HEAD_FRONT;

	if (0.f < vDir.x)
	{
		m_pTransformCom->Reverse_Scale_x();
	}
}


HRESULT CMutationEvilSoul::Create_Bullet(void)
{
	TIME_STOP_S_OK


	_vec3 vMonster_Pos = (m_pTransformCom->m_vInfo[INFO_POS]);

	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CGameObject* pBulletObject = nullptr;

	CSoundMgr::CHANNELID eID = Get_SoundChannel();
	CSoundMgr::Get_Instance()->StopSound(eID);
	CSoundMgr::Get_Instance()->PlaySound(L"Monster_1_Attack.wav", eID, 0.7f);

	for (size_t i = 0; i < 8; i++)
	{
		pBulletObject = CMonsterBullet_2::Create(m_pGraphicDev, vMonster_Pos, (i + 1));
		 
		pLayer->Add_BulletObject(  pBulletObject);
	}
	return S_OK;
}

void CMutationEvilSoul::Create_Effect_Birth_LandMark(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Birth_LandMark* pEffect = CEffect_Birth_LandMark::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
void CMutationEvilSoul::Create_Effect_Birth_Charge(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Birth_Charge* pEffect = CEffect_Birth_Charge::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
void CMutationEvilSoul::Create_Effect_Summon(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Summon* pEffect = CEffect_Summon::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
void CMutationEvilSoul::Create_Effect_Summon_Wave(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Summon_Wave* pEffect = CEffect_Summon_Wave::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

CMutationEvilSoul* CMutationEvilSoul::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3	vPos)
{
	CMutationEvilSoul* pInstance = new CMutationEvilSoul(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMutationEvilSoul::Create_Dead_Effect(void)
{
	TIME_STOP_VOID

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	pGameObject = CEffect_Monster_Dead_1::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);

}

void CMutationEvilSoul::Create_Coin(void)
{
	TIME_STOP_VOID

		CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CGameObject* pGameObject = nullptr;

	_int iCoinCount = (_int)((rand() % 6) + 1);
	for (int i = 0; i < iCoinCount; i++)
	{
		pGameObject = CItem_Coin::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);
		if (pGameObject == nullptr)
			return;
		pGameLogicLayer->Add_BulletObject(pGameObject);
	}
}

void CMutationEvilSoul::Free(void)
{
	

	__super::Free();
}