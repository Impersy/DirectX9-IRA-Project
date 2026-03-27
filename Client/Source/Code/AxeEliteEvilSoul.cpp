#include "stdafx.h"
#include "../Header/AxeEliteEvilSoul.h"
#include "Export_Function.h"
#include "Monster_Shadow.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "Effect_Whirlwind.h"
#include "Bullet_AxeEliteEvilSoul_Whirlwind.h"
#include "Bullet_AxeEliteEvilSoul_Leaf.h"
#include "Effect_Named_Monster_Dead.h"
#include "Effect_Birth_Charge.h"
#include "Effect_Birth_LandMark.h"
#include "Effect_Summon.h"
#include "Effect_Summon_Wave.h"
#include "Effect_Key.h"
#include "Terrain.h"
#include "SoundMgr.h"

CAxeEliteEvilSoul::CAxeEliteEvilSoul(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CAxeEliteEvilSoul::CAxeEliteEvilSoul(const CAxeEliteEvilSoul& rhs)
	: CMonster(rhs)
{
}

CAxeEliteEvilSoul::~CAxeEliteEvilSoul()
{
	Free();
}


HRESULT CAxeEliteEvilSoul::Ready_GameObject(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	m_iMonsterMaxHP = 200000;
	m_iMonsterCurHP = m_iMonsterMaxHP;

	m_vScaleOrigin = { 17.f, 17.f, 0.f };

	//m_pTransformCom->Set_Pos(_float(rand() % 200), 1.f, _float(rand() % 200));
	m_pTransformCom->Set_Pos(150.f + 257.f, 1.f, 150.f + 257.f);;
	m_pTransformCom->m_vScale = m_vScaleOrigin;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	m_eState = MONSTER_IDLE;

	m_bBirthEffect[BIRTH_LANDMARK] = true;
	m_bBirthEffect[BIRTH_CHARGE] = true;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::THORN_BREAK);
	CSoundMgr::Get_Instance()->PlaySound(L"Monster_Appear_02.wav", CSoundMgr::MONSTER_APPEAR, 0.7f);

	Create_Effect_Birth_LandMark();
	Create_Effect_Birth_Charge();

	__super::Ready_GameObject();

	return S_OK;
}

_int CAxeEliteEvilSoul::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_iMonsterCurHP < 0)
		m_eState = MONSTER_DEAD;

	GetDamage_Update(fTimeDelta);

	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_DEATH);
		CSoundMgr::Get_Instance()->PlaySound(L"MonsterDeath.wav", CSoundMgr::MONSTER_DEATH, 0.7f);

		CScene* pScene = Engine::Get_Scene();
		pScene->Decrease_MonsterCnt();

		Create_Dead_Effect();
		Create_Effect_Key();
		if (m_pShadow)
			m_pShadow->Set_Dead();
		return OBJ_DEAD;
	}

	if (m_bDeadEffect)
		m_eState = MONSTER_DEAD;

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

	Look_At_Player();
	
	SetUp_OnTerrain();

	Frame_Check(fTimeDelta);

	if(m_bBirthEffect[BIRTH_DONE])
		Change_State(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_MONSTER, this);

	return OBJ_NOEVENT;
}

void CAxeEliteEvilSoul::LateUpdate_GameObject()
{	
	if (m_bHit)
	{	
	//	--m_iMonsterHp;
	//	
	//	if (m_iMonsterHp)
	//	{
	//		m_HitCount = 1;
	//		m_bHit = false;
	//	}
	//	else
	//	{
	//		m_bDeadEffect = true;
	//	}
		m_bHit = false;
	}

	__super::LateUpdate_GameObject();

	__super::Compute_ViewZ(&m_pTransformCom->m_vInfo[INFO_POS]);
}

void CAxeEliteEvilSoul::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	

	//if (m_HitCount && (m_HitCount < 8))
	//{
		//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(100, 255, 255, 255));
		//m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		//m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

	//	m_HitCount += 1;
	//}

	Set_Texture();

	if(m_bBirthEffect[BIRTH_SUMMON] || m_bBirthEffect[BIRTH_DONE])
		m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CAxeEliteEvilSoul::Change_State(const _float& fTimeDelta)
{
	TIME_STOP_VOID
	_vec3 vDir, vPlayerPos; 
	CTransform* pPlayerTransformCom;
	switch (m_eState)
	{
	case MONSTER_IDLE:
		m_fAccumulatedTime += fTimeDelta;
		if (2.f < m_fAccumulatedTime)
		{
			m_fAccumulatedTime = 0.f;
			m_eState = MONSTER_ATTACK;
			m_fFrame = 0.f;
		}
		break;
	case MONSTER_ATTACK:
		m_fAccumulatedTime += fTimeDelta;
		if (1.f < m_fAccumulatedTime)
		{
			m_eState = MONSTER_MOVE;
			m_fFrame = 0.f;
			m_fAccumulatedTime = 0.f;

			pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
			if (nullptr == pPlayerTransformCom) return;
			pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
			m_vTargetPos = vPlayerPos;
			m_vTargetPos.y = m_pTransformCom->m_vInfo[INFO_POS].y;

			CSoundMgr::CHANNELID eID;
			eID = Get_SoundChannel();
			CSoundMgr::Get_Instance()->StopSound(eID);
			CSoundMgr::Get_Instance()->PlaySound(L"Monster_34_Spin.wav", eID, 0.9f);

			Create_Effect_Whirlwind();
			Create_Bullet_Whirlwind();

			eID = Get_SoundChannel();
			CSoundMgr::Get_Instance()->StopSound(eID);
			CSoundMgr::Get_Instance()->PlaySound(L"Monster_34_Bullet.wav", eID, 0.9f);

			Create_Bullet_Leaf(5);
			m_bLeafBulletAttack = true;
		}
		break;
	case MONSTER_MOVE:

		m_fAccumulatedTime += fTimeDelta;
		m_vDir = m_vTargetPos - m_pTransformCom->m_vInfo[INFO_POS];
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_vDir *= 90.f * fTimeDelta;
		m_pTransformCom->Move_Pos(&m_vDir);
		if (1.5f < m_fAccumulatedTime)
		{
			m_fAccumulatedTime = 0.f;
			m_eState = MONSTER_IDLE;
			m_fFrame = 0.f;
		}
		break;
	case MONSTER_DEAD:
		m_fAccumulatedTime += fTimeDelta;
		if (2.0f < m_fAccumulatedTime)
		{
			m_bDead = true;
			m_fFrame = 8.f;
		}
		break;
	}

	if (m_bLeafBulletAttack)
	{
		if (0.3f < m_fAccumulatedTime)
		{
			Create_Bullet_Leaf(3);

			m_bLeafBulletAttack = false;
		}
	}
}

void CAxeEliteEvilSoul::Frame_Check(const _float& fTimeDelta)
{
	TIME_STOP_VOID
	switch (m_eState)
	{
	case MONSTER_IDLE:
	{
		m_fFrame += 10.f * fTimeDelta;

		if (10.f < m_fFrame)
		{
			m_fFrame = 0.f;
			//m_eState = MONSTER_IDLE;
		}
	}
	break;
	case MONSTER_MOVE:
	{
		m_fFrame += 17.f * fTimeDelta;
		m_pTransformCom->m_vInfo[INFO_POS].y += 2.5f;

		if (5.f < m_fFrame)
		{
			m_fFrame = 0.f;
			//m_eState = MONSTER_IDLE;
		}
	}
	break;
	case MONSTER_ATTACK:
	{
		m_fFrame += 8.f * fTimeDelta;

		if (8.f < m_fFrame)
		{
			m_fFrame = 0.f;
			//m_eState = MONSTER_IDLE;
		}
	}
		break;
	case MONSTER_DEAD:
	{
		m_fFrame += 3.f * fTimeDelta;

		if (8.f < m_fFrame)
		{
			m_fFrame = 8.f;
			//m_eState = MONSTER_IDLE;
		}
	}
		break;
	}
}

void CAxeEliteEvilSoul::Check_Direction(const _vec3 vDir)
{
	TIME_STOP_VOID


	if (0.f < vDir.z && 0.f < vDir.x)	// 플레이어가 몬스터의 위(뒤)쪽에 위치
		m_eDir = RIGHTTOP;
	else if (0.f < vDir.z && 0.f >= vDir.x)
		m_eDir = LEFTTOP;
	else if (0.f >= vDir.z && 0.f < vDir.x)	// 플레이어가 몬스터의 아래(앞)쪽에 위치
		m_eDir = RIGHTBOTTOM;
	else if (0.f >= vDir.z && 0.f >= vDir.x)	
		m_eDir = LEFTBOTTOM;
	
	if (m_eDir == RIGHTTOP || m_eDir == RIGHTBOTTOM)
	{
		if(m_pTransformCom->m_vScale.x > 0.f)
			m_pTransformCom->m_vScale.x *= -1;
	}
	else if (m_eDir == LEFTTOP || m_eDir == LEFTBOTTOM)
	{
		if (m_pTransformCom->m_vScale.x < 0.f)
			m_pTransformCom->m_vScale.x *= -1;
	}
}

HRESULT CAxeEliteEvilSoul::Create_Bullet(void)
{
	TIME_STOP_S_OK


	//_vec3 vMonster_Pos = (m_pTransformCom->m_vInfo[INFO_POS]);

	//CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
	//CGameObject* pBulletObject = nullptr;

	//for (size_t i = 0; i < 8; i++)
	//{
	//	pBulletObject = CMonsterBullet_2::Create(m_pGraphicDev, vMonster_Pos, (i + 1));
	//	 
	//	pLayer->Add_BulletObject(  pBulletObject);
	//}
	return S_OK;
}

void CAxeEliteEvilSoul::Create_Effect_Whirlwind(void)
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Whirlwind* pEffect = CEffect_Whirlwind::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

void CAxeEliteEvilSoul::Create_Effect_Birth_LandMark(void)
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Birth_LandMark* pEffect = CEffect_Birth_LandMark::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

void CAxeEliteEvilSoul::Create_Effect_Birth_Charge(void)
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Birth_Charge* pEffect = CEffect_Birth_Charge::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

void CAxeEliteEvilSoul::Create_Effect_Summon(void)
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Summon* pEffect = CEffect_Summon::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

void CAxeEliteEvilSoul::Create_Effect_Summon_Wave(void)
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Summon_Wave* pEffect = CEffect_Summon_Wave::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

void CAxeEliteEvilSoul::Create_Effect_Key(void)
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Key* pEffect = CEffect_Key::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

void CAxeEliteEvilSoul::Create_Bullet_Whirlwind(void)
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CBullet_AxeEvilSoul_Whirlwind* pBullet = CBullet_AxeEvilSoul_Whirlwind::Create(m_pGraphicDev, this);
	if (nullptr == pBullet) return;
	pGameLogicLayer->Add_BulletObject(pBullet);
}

void CAxeEliteEvilSoul::Create_Bullet_Leaf(_int iCnt)
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	vPos.y = m_vTargetPos.y;
	_vec3 vDir = m_vTargetPos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	_float	arrAngles[13] = {};
	_bool	arrCheck[13] = {};
	_int	iNum = 0;

	for (int i = 0; i < 13; ++i)
	{
		arrAngles[i] = -.3f + .05f * i;
	}

	while (iNum <= iCnt)
	{
		_int iIdx = rand() % 13;
		if (arrCheck[iIdx] == 0)
		{
			CBullet_AxeEvilSoul_Leaf* pBullet = CBullet_AxeEvilSoul_Leaf::Create(m_pGraphicDev, this, vDir, arrAngles[iIdx]);
			if(nullptr == pBullet) return;
			pGameLogicLayer->Add_BulletObject(pBullet);
			arrCheck[iIdx] = 1;
			++iNum;
		}
	}
}

void CAxeEliteEvilSoul::Look_At_Player(void)
{
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	if (nullptr == pPlayerTransformCom) return;

	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3	vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];

	Check_Direction(vDir);
}

void CAxeEliteEvilSoul::SetUp_OnTerrain(void)
{
	_vec3		vPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos;

	_tchar szBuf[MAX_STR];
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Environment", L"Terrain"));
	pTerrain->Get_TerrainTexKey(szBuf);
	CTerrainTex * pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", szBuf, ID_STATIC));
	if (nullptr == pTerrainBufferCom) return;

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), pTerrainBufferCom->Get_VtxCntX(), pTerrainBufferCom->Get_VtxCntZ());


	//fHeight += 1.f;

	m_pTransformCom->Set_Pos(vPos.x, fHeight + 16.5f, vPos.z);

}

void CAxeEliteEvilSoul::Set_Texture(void)
{
	switch (m_eState)
	{
	case MONSTER_IDLE:
		if (LEFTBOTTOM == m_eDir || RIGHTBOTTOM == m_eDir)
			m_pTextureComStand[HEAD_FRONT]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureComStand[HEAD_BACK]->Set_Texture((_uint)m_fFrame);
		break;
	case MONSTER_ATTACK:
		if (LEFTBOTTOM == m_eDir || RIGHTBOTTOM == m_eDir)
			m_pTextureComAttack[HEAD_FRONT]->Set_Texture((_uint)m_fFrame);
		else
			m_pTextureComAttack[HEAD_BACK]->Set_Texture((_uint)m_fFrame);
		break;
	case MONSTER_MOVE:
		m_pTextureComAttackRot->Set_Texture((_uint)m_fFrame);
		break;
	case MONSTER_DEAD:
		m_pTextureComDeath->Set_Texture((_uint)m_fFrame);
		break;
	}
}

void CAxeEliteEvilSoul::Create_Dead_Effect(void)
{
	TIME_STOP_VOID
		
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	pGameObject = CEffect_Named_Monster_Dead::Create(m_pGraphicDev, this);

	if (pGameObject == nullptr) return;

	pGameLogicLayer->Add_BulletObject(pGameObject);

}

HRESULT CAxeEliteEvilSoul::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureComStand[HEAD_FRONT] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_AxeEliteEvilSoul_Stand045"));
	if (nullptr == m_pTextureComStand[HEAD_FRONT]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_AxeEliteEvilSoul_Stand045", pComponent });

	pComponent = m_pTextureComStand[HEAD_BACK] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_AxeEliteEvilSoul_Stand135"));
	if (nullptr == m_pTextureComStand[HEAD_BACK]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_AxeEliteEvilSoul_Stand135", pComponent });

	pComponent = m_pTextureComAttack[HEAD_FRONT] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_AxeEliteEvilSoul_Attack045"));
	if (nullptr == m_pTextureComAttack[HEAD_FRONT]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_AxeEliteEvilSoul_Attack045", pComponent });

	pComponent = m_pTextureComAttack[HEAD_BACK] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_AxeEliteEvilSoul_Attack135"));
	if (nullptr == m_pTextureComAttack[HEAD_BACK]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_AxeEliteEvilSoul_Attack135", pComponent });

	pComponent = m_pTextureComAttackRot = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_AxeEliteEvilSoul_AttackRot"));
	if (nullptr == m_pTextureComAttackRot) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_AxeEliteEvilSoul_AttackRot", pComponent });

	pComponent = m_pTextureComDeath = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_AxeEliteEvilSoul_Death"));
	if (nullptr == m_pTextureComDeath) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_AxeEliteEvilSoul_Death", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	if (nullptr == m_pColliderCom) return E_FAIL;
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(3.f);
	m_pColliderCom->Set_Offset(_vec3(0.f,-11.5f ,0.f));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

CAxeEliteEvilSoul* CAxeEliteEvilSoul::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAxeEliteEvilSoul* pInstance = new CAxeEliteEvilSoul(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CAxeEliteEvilSoul::Free(void)
{
	__super::Free();
}