#include "stdafx.h"
#include "..\Header\EvilSoul.h"
#include "Export_Function.h"
#include "MonsterBullet.h"

#include "Effect_Monster_Dead_1.h"
#include "Effect_Birth_Charge.h"
#include "Effect_Birth_LandMark.h"
#include "Effect_Summon.h"
#include "Effect_Summon_Wave.h"
#include "Item_Coin.h"
#include "Tutorial.h"

CEvilSoul::CEvilSoul(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev), m_pTextureCom_135_1(nullptr), m_pTextureCom_135_2(nullptr), m_eHead(HEAD_FRONT)
{
}

CEvilSoul::CEvilSoul(const CEvilSoul& rhs)
	: CMonster(rhs), m_pTextureCom_135_1(nullptr), m_pTextureCom_135_2(nullptr), m_eHead(HEAD_FRONT)
{
}

CEvilSoul::~CEvilSoul()
{
	Free();
}


HRESULT CEvilSoul::Ready_GameObject(const _vec3 vPos)
{
	Add_Component();
	m_eName = NAME_SOUL;

	
	if (g_eCurScene == SCENE_NORMAL)
		m_pTransformCom->Set_Pos(_float(rand() % 200) + 300.f, 4.2f, _float(rand() % 200) + 300.f);
	else {
		m_pTransformCom->Set_Pos(_float(rand() % 200) + 257.f, 5.f, _float(rand() % 200) + 257.f);
		m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	}

	m_iMonsterMaxHP = 80000;
	m_iMonsterCurHP = m_iMonsterMaxHP;

	//m_pTransformCom->Set_Pos(_float(rand() % 200) + 257.f, 5.f, _float(rand() % 200) + 257.f);
	//m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));
	m_pTransformCom->UpdatePos_OnWorld();
	m_fSpeed = 3.f;

	m_pTransformCom->m_vScale = { 5.f , 5.f , 1.f };

	m_bBirthEffect[BIRTH_LANDMARK] = true;
	m_bBirthEffect[BIRTH_CHARGE] = true;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_APPEAR);
	CSoundMgr::Get_Instance()->PlaySound(L"Monster_Appear_02.wav", CSoundMgr::MONSTER_APPEAR, 0.7f);

	if (g_eCurScene != SCENE_TUTORIAL)
	{
		Create_Effect_Birth_LandMark();
		Create_Effect_Birth_Charge();
	}


	__super::Ready_GameObject();

	return S_OK;
}

_int CEvilSoul::Update_GameObject(const _float& fTimeDelta)
{
	
	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_DEATH);
		CSoundMgr::Get_Instance()->PlaySound(L"Evilsoul_Death.wav", CSoundMgr::MONSTER_DEATH, 0.7f);

		Create_Coin();
		Create_Dead_Effect();

		if (g_eCurScene == SCENE_TUTORIAL)
		{
			CTutorial* pTutoScene = dynamic_cast<CTutorial*> (Engine::Get_Scene());
			pTutoScene->Set_MonsterCntDown();

		}


		CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

		pLayer->m_Normal_Stage_Mon_Cnt += 1;

		return OBJ_DEAD;
	}

	if (m_bBirthEffect[BIRTH_LANDMARK] && g_eCurScene != SCENE_TUTORIAL)
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

	if (m_bBirthEffect[BIRTH_SUMMON] && g_eCurScene != SCENE_TUTORIAL)
	{
		m_bBirthEffect[BIRTH_SUMMON] = false;
		m_bBirthEffect[BIRTH_DONE] = true;
	}

	Frame_Check(fTimeDelta);
	SetUp_OnTerrain();
	__super::Update_GameObject(fTimeDelta);

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	 
	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3	vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];

	if (g_eCurScene != SCENE_TUTORIAL)
	{
		m_pTransformCom->Chase_Target(&vPlayerPos, m_fSpeed, fTimeDelta, m_eName);
	}
	else
	{
		CTutorial* pTutoScene = dynamic_cast<CTutorial*> (Engine::Get_Scene());
		if (pTutoScene->Get_ChasePlayer())
		{
			m_pTransformCom->Chase_Target(&vPlayerPos, m_fSpeed, fTimeDelta, m_eName);
			Head_Check(vDir);
		}
		
	}

	if (((vDir.x > -15.0f) && (vDir.x < 15.0f)) && ((vDir.z > -15.0f) && (vDir.z < 15.0f)) && !m_bCheck)
		Change_State();

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_MONSTER, this);

	return OBJ_NOEVENT;
}

void CEvilSoul::LateUpdate_GameObject()
{	
	if (m_bHit)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_HIT);
		CSoundMgr::Get_Instance()->PlaySound(L"Evilsoul_Hit.wav", CSoundMgr::MONSTER_HIT, 0.7f);

		m_iMonsterHp -= 1;

		if (m_iMonsterHp)
		{	
			m_HitCount = 1;
			m_bHit = false;
		}
		else
		{
			m_bDead = true;
		}
	}

	__super::LateUpdate_GameObject();
}

void CEvilSoul::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

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
		switch (m_eState)
		{
		case MONSTER_IDLE:
			m_pTextureCom->Set_Texture((_uint)m_fFrame);
			break;
		case MONSTER_MOVE:
			m_pTextureCom_2->Set_Texture((_uint)m_fFrame);
			break;
		case MONSTER_ATTACK:
			m_pTextureCom->Set_Texture((_uint)m_fFrame);
			break;
		case MONSTER_END:
			break;
		default:
			break;
		}
	}
	else if (HEAD_BACK == m_eHead)
	{
		switch (m_eState)
		{
		case MONSTER_IDLE:
			m_pTextureCom_135_1->Set_Texture((_uint)m_fFrame);
			break;
		case MONSTER_MOVE:
			m_pTextureCom_135_2->Set_Texture((_uint)m_fFrame);
			break;
		case MONSTER_ATTACK:
			m_pTextureCom_135_1->Set_Texture((_uint)m_fFrame);
			break;
		case MONSTER_END:
			break;
		default:
			break;
		}
	}

	if ((m_bBirthEffect[BIRTH_SUMMON] || m_bBirthEffect[BIRTH_DONE]) && g_eCurScene != SCENE_TUTORIAL)
		m_pBufferCom->Render_Buffer();
	else if (g_eCurScene == SCENE_TUTORIAL)
		m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CEvilSoul::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	//45 IDLE, ATTACK
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_EvilSoul_45_Idle"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_EvilSoul_45_Idle", pComponent });
	//45 MOVE
	pComponent = m_pTextureCom_2 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_EvilSoul_45_Move"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_EvilSoul_45_Move", pComponent });

	//135 IDLE, ATTACK
	pComponent = m_pTextureCom_135_1 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_EvilSoul_135_Idle"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_EvilSoul_135_Idle", pComponent });
	//135 MOVE
	pComponent = m_pTextureCom_135_2 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_EvilSoul_135_Move"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_EvilSoul_135_Move", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	 
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(5.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CEvilSoul::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	 
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CEvilSoul::Change_State(void)
{
	if (0.f == m_fFrame)
	{
		m_eState = MONSTER_ATTACK;
		CSoundMgr::CHANNELID eID = Get_SoundChannel();
		CSoundMgr::Get_Instance()->StopSound(eID);
		CSoundMgr::Get_Instance()->PlaySound(L"Monster_2_Attack.wav", eID, 0.7f);
		Create_Bullet();
	}
}

void CEvilSoul::Frame_Check(const _float& fTimeDelta)
{
	if (m_eState == MONSTER_IDLE)
	{
		m_fFrame += 7.f * fTimeDelta;

		if (7.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bCheck = false;
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

void CEvilSoul::Head_Check(const _vec3 vDir)
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
	else
	{
		m_pTransformCom->Scale_x();
	}
}

HRESULT CEvilSoul::Create_Bullet(void)
{	
	TIME_STOP_S_OK

	_vec3 vMonster_Pos = (m_pTransformCom->m_vInfo[INFO_POS]);
	CGameObject*	pGameObject = nullptr;
	CGameObject* pBulletObject = CMonsterBullet::Create(m_pGraphicDev, vMonster_Pos, true);
	 
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
	pLayer->Add_BulletObject(pBulletObject);

	return S_OK;
}

void CEvilSoul::Create_Effect_Birth_LandMark(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Birth_LandMark* pEffect = CEffect_Birth_LandMark::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
void CEvilSoul::Create_Effect_Birth_Charge(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Birth_Charge* pEffect = CEffect_Birth_Charge::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
void CEvilSoul::Create_Effect_Summon(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Summon* pEffect = CEffect_Summon::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
void CEvilSoul::Create_Effect_Summon_Wave(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Summon_Wave* pEffect = CEffect_Summon_Wave::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

CEvilSoul* CEvilSoul::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3	vPos)
{
	CEvilSoul* pInstance = new CEvilSoul(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEvilSoul::Create_Dead_Effect(void)
{
	TIME_STOP_VOID

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	pGameObject = CEffect_Monster_Dead_1::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CEvilSoul::Create_Coin(void)
{
	TIME_STOP_VOID

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CGameObject* pGameObject = nullptr;

	_int iCoinCount = (_int)((rand() % 5) + 1);
	for (int i = 0; i < iCoinCount; i++)
	{
		pGameObject = CItem_Coin::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);
		if (pGameObject == nullptr)
			return;
		pGameLogicLayer->Add_BulletObject(pGameObject);
	}
}


void CEvilSoul::Free(void)
{
	

	__super::Free();
}
