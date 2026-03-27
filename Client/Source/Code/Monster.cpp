#include "stdafx.h"
#include "..\Header\Monster.h"
#include "Export_Function.h"
#include "Fire_Brave.h"
#include "Effect_Player_Damage_Font.h"
#include "Monster_Shadow.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr), m_eState(MONSTER_IDLE)
	, m_bCheck(false), m_pTextureCom_2(nullptr), m_fSpeed(3.f), m_fFrame(0.f), m_eName(NAME_END), m_iMonsterHp(3), m_HitCount(0)
{
}

CMonster::CMonster(const CMonster & rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTextureCom(rhs.m_pTextureCom), m_pTransformCom(rhs.m_pTransformCom), m_eState(MONSTER_IDLE)
	, m_bCheck(false), m_pTextureCom_2(nullptr), m_fSpeed(3.f), m_fFrame(0.f), m_eName(NAME_END), m_iMonsterHp(3), m_HitCount(0)
{
}

CMonster::~CMonster()
{
	Free();
}

HRESULT CMonster::Ready_GameObject(void)
{
	CScene* pScene = Engine::Get_Scene();

	pScene->Increase_MonsterCnt();

	return S_OK;
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{	
	__super::Update_GameObject(fTimeDelta);


	return 0;
}

void CMonster::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMonster::Render_GameObject()
{
}

CSoundMgr::CHANNELID CMonster::Get_SoundChannel(void)
{
	if (m_bySoundChannel > 10 || m_bySoundChannel <= 0)
		m_bySoundChannel = 1;

	switch (m_bySoundChannel)
	{
	case 1: ++m_bySoundChannel; return  CSoundMgr::MONSTER_ATTACK;
	case 2: ++m_bySoundChannel; return  CSoundMgr::MONSTER_ATTACK2;
	case 3: ++m_bySoundChannel; return  CSoundMgr::MONSTER_ATTACK3;
	case 4: ++m_bySoundChannel; return  CSoundMgr::MONSTER_ATTACK4;
	case 5: ++m_bySoundChannel; return  CSoundMgr::MONSTER_ATTACK5;
	case 6: ++m_bySoundChannel; return  CSoundMgr::MONSTER_ATTACK6;
	case 7: ++m_bySoundChannel; return  CSoundMgr::MONSTER_ATTACK7;
	case 8: ++m_bySoundChannel; return  CSoundMgr::MONSTER_ATTACK8;
	case 9: ++m_bySoundChannel; return  CSoundMgr::MONSTER_ATTACK9;
	case 10: ++m_bySoundChannel; return  CSoundMgr::MONSTER_ATTACK10;
	}
}

void CMonster::GetDamage_Update(const _float& fTimeDelta)
{
	m_fDamageTimeDelta += fTimeDelta;

	if (!m_Damage_List.empty())
	{
		if (m_fDamageTimeDelta > 0.05f)
		{
			m_iMonsterCurHP -= m_Damage_List.front();
			m_Damage_List.pop_front();

			m_fDamageTimeDelta = 0.f;
		}
	}
}


void CMonster::Free(void)
{
	__super::Free();
}
