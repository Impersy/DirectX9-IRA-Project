#include "stdafx.h"
#include "..\Header\GreenEvilSlime.h"
#include "Export_Function.h"
#include "MonsterBullet.h"

#include "Effect_Monster_Dead_1.h"
#include "Effect_Monster_Dead_2.h"
#include "Effect_Birth_Charge.h"
#include "Effect_Birth_LandMark.h"
#include "Effect_Summon.h"
#include "Effect_Summon_Wave.h"
#include "Item_Coin.h"
#include "Item_Drop.h"
#include <Tutorial.h>


CGreenEvilSlime::CGreenEvilSlime(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CGreenEvilSlime::CGreenEvilSlime(const CGreenEvilSlime& rhs)
	: CMonster(rhs)
{
}

CGreenEvilSlime::~CGreenEvilSlime()
{
	Free();
}

HRESULT CGreenEvilSlime::Ready_GameObject(const _vec3	vPos)
{
	Add_Component();
	m_eName = NAME_SLIME;

	if (g_eCurScene == SCENE_NORMAL)
		m_pTransformCom->Set_Pos(_float(rand() % 200) + 300.f, 4.2f, _float(rand() % 200) + 300.f);
	else {
		m_pTransformCom->Set_Pos(_float(rand() % 200) + 257.f, 5.f, _float(rand() % 200) + 257.f);
		m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	}

	//m_pTransformCom->Set_Pos(_float(rand() % 200) + 257.f, 4.f, _float(rand() % 200) + 257.f);	
	
	m_pTransformCom->m_vScale = { 3.f, 3.f, 1.f };
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));
	m_pTransformCom->UpdatePos_OnWorld();
	m_fSpeed = 3.f;

	m_pTransformCom->m_vScale = { 5.f , 5.f , 1.f };

	m_bBirthEffect[BIRTH_LANDMARK] = true;
	m_bBirthEffect[BIRTH_CHARGE] = true;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_APPEAR);
	CSoundMgr::Get_Instance()->PlaySound(L"Monster_Appear_02.wav", CSoundMgr::MONSTER_APPEAR, 0.7f);

	Create_Effect_Birth_LandMark();
	Create_Effect_Birth_Charge();

	__super::Ready_GameObject();

	return S_OK;
}

_int CGreenEvilSlime::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_DEATH);
		CSoundMgr::Get_Instance()->PlaySound(L"Evilsoul_Death.wav", CSoundMgr::MONSTER_DEATH, 0.7f);

		Create_Dead_Effect();
		Create_Coin();

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

	Frame_Check(fTimeDelta);

	SetUp_OnTerrain();

	__super::Update_GameObject(fTimeDelta);

	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	 
	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

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
		}
	}
	_vec3	vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];

	if (((vDir.x > -2.0f) && (vDir.x < 2.0f)) && ((vDir.z > -2.0f) && (vDir.z < 2.0f)) && !m_bCheck)
		Change_State(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_MONSTER, this);

	return OBJ_NOEVENT;
}

void CGreenEvilSlime::LateUpdate_GameObject()
{	
	if (m_bHit)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_HIT);
		CSoundMgr::Get_Instance()->PlaySound(L"Monster_14_Hit.wav", CSoundMgr::MONSTER_HIT, 0.7f);

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

void CGreenEvilSlime::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	if (m_HitCount && (m_HitCount < 8))
	{
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(100, 255, 255, 255));
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

		m_HitCount += 1;
	}

	switch (m_eState)
	{
	case MONSTER_IDLE:
		m_pTextureCom->Set_Texture((_uint)m_fFrame);
		break;
	case MONSTER_MOVE:
		break;
	case MONSTER_ATTACK:
		m_pTextureCom_2->Set_Texture((_uint)m_fFrame);
		break;
	case MONSTER_END:
		break;
	default:
		break;
	}

	if (m_bBirthEffect[BIRTH_SUMMON] || m_bBirthEffect[BIRTH_DONE])
		m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CGreenEvilSlime::Add_Component(void)
{
	// 준석 수정 (23.03.02)
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_GreenEvilSlime_Move"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_GreenEvilSlime_Move", pComponent });

	pComponent = m_pTextureCom_2 = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_GreenEvilSlime_Attack"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_GreenEvilSlime_Attack", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	 
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(5.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CGreenEvilSlime::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	 
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CGreenEvilSlime::Change_State(const _float& fTimeDelta)
{
	//m_fAccumulatedTime += fTimeDelta;
	if (0.f == m_fFrame)
	{
		//m_bAttack = true;
		//m_fAccumulatedTime = 0.f;
		m_eState = MONSTER_ATTACK;
	}

	//if (m_bAttack && m_fAccumulatedTime > 2.f)
	//{
	//	m_bAttack = false;
	//	CSoundMgr::CHANNELID eID = Get_SoundChannel();
	//	CSoundMgr::Get_Instance()->StopSound(eID);
	//	CSoundMgr::Get_Instance()->PlaySound(L"Monster_14_Attack.wav", eID, 0.7f);
	//}
}
void CGreenEvilSlime::Frame_Check(const _float& fTimeDelta)
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

	if (m_eState == MONSTER_ATTACK)
	{
		m_fFrame += 9.f * fTimeDelta;

		if (9.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_eState = MONSTER_IDLE;
			m_bCheck = true;
		}
	}
}

void CGreenEvilSlime::Create_Effect_Birth_LandMark(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Birth_LandMark* pEffect = CEffect_Birth_LandMark::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
void CGreenEvilSlime::Create_Effect_Birth_Charge(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Birth_Charge* pEffect = CEffect_Birth_Charge::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
void CGreenEvilSlime::Create_Effect_Summon(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Summon* pEffect = CEffect_Summon::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
void CGreenEvilSlime::Create_Effect_Summon_Wave(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Summon_Wave* pEffect = CEffect_Summon_Wave::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

CGreenEvilSlime* CGreenEvilSlime::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3	vPos)
{
	CGreenEvilSlime* pInstance = new CGreenEvilSlime(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGreenEvilSlime::Create_Dead_Effect(void)
{
	TIME_STOP_VOID

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	pGameObject = CEffect_Monster_Dead_1::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CGreenEvilSlime::Create_Coin(void)
{
	TIME_STOP_VOID

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	for (int i = 0; i < 3; i++)
	{
		pGameObject = CItem_Coin::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);
		if (pGameObject == nullptr)
			return;
		pGameLogicLayer->Add_BulletObject(pGameObject);
	}
}

void CGreenEvilSlime::Free(void)
{
	
	__super::Free();
}