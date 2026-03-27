#include "stdafx.h"
#include "../Header/GreenEvilBat.h"
#include "Export_Function.h"
#include "Monster_Shadow.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "Effect_Whirlwind.h"
#include "Bullet_AxeEliteEvilSoul_Whirlwind.h"
#include "Bullet_GreenEvilBat_DoubleLeaf.h"
#include "Terrain.h"
#include "Effect_Birth_Charge.h"
#include "Effect_Birth_LandMark.h"
#include "Effect_Summon.h"
#include "Effect_Summon_Wave.h"
#include "Effect_Monster_Dead_1.h"
#include "Item_Coin.h"
#include "SoundMgr.h"

CGreenEvilBat::CGreenEvilBat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CGreenEvilBat::CGreenEvilBat(const CGreenEvilBat& rhs)
	: CMonster(rhs)
{
}

CGreenEvilBat::~CGreenEvilBat()
{
	Free();
}


HRESULT CGreenEvilBat::Ready_GameObject(const _vec3 vPos)
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	
	m_iMonsterMaxHP = 40000;
	m_iMonsterCurHP = m_iMonsterMaxHP;


	m_vScaleOrigin = { 6.f, 6.f, 0.f };

	m_fSpeed = 15.f;

	m_vDir = { 1.f, 0.f, 0.f };
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_eState = MONSTER_IDLE;
	m_eDir = RIGHTBOTTOM;

	//m_pTransformCom->Set_Pos(_float(rand() % 200), 1.f, _float(rand() % 200));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y + 10.f, vPos.z);
	m_pTransformCom->m_vScale = m_vScaleOrigin;
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	m_bBirthEffect[BIRTH_LANDMARK] = true;
	m_bBirthEffect[BIRTH_CHARGE] = true;

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_APPEAR);
	CSoundMgr::Get_Instance()->PlaySound(L"Monster_Appear_02.wav", CSoundMgr::MONSTER_APPEAR, 0.7f);

	Create_Effect_Birth_LandMark();
	Create_Effect_Birth_Charge();

	__super::Ready_GameObject();

	return S_OK;
}

_int CGreenEvilBat::Update_GameObject(const _float& fTimeDelta)
{	
	// HP 체크
	if (m_iMonsterCurHP < 0)
		m_bDead = true;

	GetDamage_Update(fTimeDelta);

	if (m_bDead)
	{
		Create_Coin();
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_DEATH);
		CSoundMgr::Get_Instance()->PlaySound(L"MonsterDeath.wav", CSoundMgr::MONSTER_DEATH, 0.7f);

		CScene* pScene = Engine::Get_Scene();
		pScene->Decrease_MonsterCnt();
		Create_Dead_Effect();
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

	//SetUp_OnTerrain();		// 서순!

	Frame_Check(fTimeDelta);

	if (m_bBirthEffect[BIRTH_DONE])
		Change_State(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_MONSTER, this);

	return OBJ_NOEVENT;
}

void CGreenEvilBat::LateUpdate_GameObject()
{	
	if (m_bHit)
	{	
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER_HIT);
		CSoundMgr::Get_Instance()->PlaySound(L"Monster_10_Hit.wav", CSoundMgr::MONSTER_HIT, 0.7f);

		//m_iMonsterHp -= 1;

		//if (m_iMonsterHp)
		//{	
		//	m_HitCount = 1;
		//	m_bHit = false;
		//}
		//else
		//{	
		//	m_HitCount = 1;
		//	m_bDead = true;
		//}
		m_bHit = false;
	}

	__super::LateUpdate_GameObject();

	__super::Compute_ViewZ(&m_pTransformCom->m_vInfo[INFO_POS]);
}

void CGreenEvilBat::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	if (m_HitCount && (m_HitCount < 8))
	{
		//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		//m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(100, 255, 255, 255));
		//m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		//m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

		m_HitCount += 1;
	}

	Set_Texture();

	if (m_bBirthEffect[BIRTH_SUMMON] || m_bBirthEffect[BIRTH_DONE])
		m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void CGreenEvilBat::Change_State(const _float& fTimeDelta)
{
	TIME_STOP_VOID
	_vec3 vDir, vPlayerPos;
	switch (m_eState)
	{
	case MONSTER_IDLE:
		m_fAccumulatedTime += fTimeDelta;
		if (2.5f < m_fAccumulatedTime)
		{
			m_fAccumulatedTime = 0.f;
			m_eState = MONSTER_MOVE;

			_matrix matRot;
			_float fAngle = (rand() % 90) * (-1) ^ (rand() % 2);
			D3DXMatrixRotationY(&matRot, D3DXToRadian(fAngle));
			D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRot);
			D3DXVec3Normalize(&m_vDir, &m_vDir);
		}
		break;
	case MONSTER_MOVE:
		m_fAccumulatedTime += fTimeDelta;

		Look_Moving_Dir();
		Move(fTimeDelta);

		if (3.f < m_fAccumulatedTime)
		{
			m_fAccumulatedTime = 0.f;
			m_eState = MONSTER_ATTACK;

			CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
			if (nullptr == pPlayerTransformCom) return;
			pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
			m_vTargetPos = vPlayerPos;
			m_vTargetPos.y = m_pTransformCom->m_vInfo[INFO_POS].y;
		}
		break;
	case MONSTER_ATTACK:
		m_fAccumulatedTime += fTimeDelta;
		m_bDoubleLeafBulletAttack = false;

		Look_At_Player();
		MovingShot(fTimeDelta);

		if (.8f < m_fAccumulatedTime)
		{
			m_fAccumulatedTime = 0.f;
			m_eState = MONSTER_MOVE;
			m_bDoubleLeafBulletAttack = true;
		}
		break;
	case MONSTER_DEAD:
		m_bDead = true;
		break;
	}
}

void CGreenEvilBat::Move(const _float& fTimeDelta)
{
	TIME_STOP_VOID
	_float fAngle = 0.f; 

	m_fAccumulatedTimeMove += fTimeDelta;

	if (2.5f < m_fAccumulatedTimeMove)
	{
		_matrix matRot;
		fAngle = (rand() % 90) * (-1) ^ (rand() % 2);
		D3DXMatrixRotationY(&matRot, D3DXToRadian(fAngle));
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRot);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_fAccumulatedTimeMove = 0.f;
	}

	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;
}

void CGreenEvilBat::MovingShot(const _float& fTimeDelta)
{
	TIME_STOP_VOID
	_float fAngle = 0.f;

	m_fAccumulatedTimeMove += fTimeDelta;
	m_fAccumulatedTimeBullet += fTimeDelta;

	if (1.5f < m_fAccumulatedTimeMove)
	{
		_matrix matRot;
		fAngle = (rand() % 90) * (-1) ^ (rand() % 2);
		D3DXMatrixRotationY(&matRot, D3DXToRadian(fAngle));
		D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRot);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_fAccumulatedTimeMove = 0.f;
	}

	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * 2.5f * fTimeDelta;

	if (0.1f < m_fAccumulatedTimeBullet)
	{
		++m_byLastChannel;
		if (m_byLastChannel > 10)
			m_byLastChannel = 1;
		Set_SoundChannel(m_byLastChannel);
		CSoundMgr::CHANNELID eID = Get_SoundChannel();

		CSoundMgr::Get_Instance()->StopSound(eID);
		CSoundMgr::Get_Instance()->PlaySound(L"Monster_3_Attack.wav", eID, 0.8f);

		Create_Bullet_DoubleLeaf();
		m_fAccumulatedTimeBullet = 0.f;
	}
}

void CGreenEvilBat::Check_Direction(const _vec3 vDir)
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

void CGreenEvilBat::Create_Bullet_DoubleLeaf()
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	vPos.y = m_vTargetPos.y;
	_vec3 vDir = m_vTargetPos - vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	CBullet_GreenEvilBat_DoubleLeaf* pBullet = CBullet_GreenEvilBat_DoubleLeaf::Create(m_pGraphicDev, this, vDir);
	if (nullptr == pBullet) return;
	pGameLogicLayer->Add_BulletObject(pBullet);

}

HRESULT CGreenEvilBat::Create_Bullet(void)
{
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

void CGreenEvilBat::Frame_Check(const _float& fTimeDelta)
{
	TIME_STOP_VOID
	m_fFrame += 15.f * fTimeDelta;

	if (6.f < m_fFrame)
	{
		m_fFrame = 0.f;
	}
}

void CGreenEvilBat::Look_At_Player(void)
{
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	if (nullptr == pPlayerTransformCom) return;

	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3	vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];

	Check_Direction(vDir);
}
void CGreenEvilBat::Look_Moving_Dir(void)
{
	Check_Direction(m_vDir);
}

void CGreenEvilBat::Create_Effect_Birth_LandMark(void)
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Birth_LandMark* pEffect = CEffect_Birth_LandMark::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
void CGreenEvilBat::Create_Effect_Birth_Charge(void)
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Birth_Charge* pEffect = CEffect_Birth_Charge::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
void CGreenEvilBat::Create_Effect_Summon(void)
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Summon* pEffect = CEffect_Summon::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}
void CGreenEvilBat::Create_Effect_Summon_Wave(void)
{
	TIME_STOP_VOID
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CEffect_Summon_Wave* pEffect = CEffect_Summon_Wave::Create(m_pGraphicDev, this);
	if (nullptr == pEffect) return;
	pGameLogicLayer->Add_BulletObject(pEffect);
}

void CGreenEvilBat::SetUp_OnTerrain(void)
{
	_vec3		vPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos;

	_tchar szBuf[MAX_STR];
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Environment", L"Terrain"));
	pTerrain->Get_TerrainTexKey(szBuf);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", szBuf, ID_STATIC));
	if (nullptr == pTerrainBufferCom) return;

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), pTerrainBufferCom->Get_VtxCntX(), pTerrainBufferCom->Get_VtxCntZ());


	//fHeight += 1.f;

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);

}

void CGreenEvilBat::Set_Texture(void)
{
	if (LEFTBOTTOM == m_eDir || RIGHTBOTTOM == m_eDir)
		m_pTextureCom[HEAD_FRONT]->Set_Texture((_uint)m_fFrame);
	else
		m_pTextureCom[HEAD_BACK]->Set_Texture((_uint)m_fFrame);
}

void CGreenEvilBat::Create_Dead_Effect(void)
{
	TIME_STOP_VOID

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject = nullptr;

	pGameObject = CEffect_Monster_Dead_1::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);

}

HRESULT CGreenEvilBat::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom[HEAD_FRONT] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_GreenEvilBat_45"));
	if (nullptr == m_pTextureCom[HEAD_FRONT]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_GreenEvilBat_45", pComponent });

	pComponent = m_pTextureCom[HEAD_BACK] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_GreenEvilBat_135"));
	if (nullptr == m_pTextureCom[HEAD_BACK]) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_GreenEvilBat_135", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	if (nullptr == m_pColliderCom) return E_FAIL;
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(1.5f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CGreenEvilBat::Create_Coin(void)
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

CGreenEvilBat* CGreenEvilBat::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3	vPos)
{
	CGreenEvilBat* pInstance = new CGreenEvilBat(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGreenEvilBat::Free(void)
{
	__super::Free();
}