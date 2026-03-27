#include "stdafx.h"
#include "CollisionMgr.h"
#include "Collider.h"
#include "Player.h"
#include "Night_Circle.h"
#include "Fire_Wave.h"
#include "Fire_Big.h"
#include "Night_Big.h"
#include "Layer.h"

#include "SylphArrow.h"
#include "Doewole_Body.h"
#include "DoewoleBullet_Circle.h"
#include "DoewoleBullet_Standard.h"
#include "DoewoleBullet_Circle.h"
#include "DestructibleObj.h"
#include "Jar.h"

#include "Monster.h"
#include "MutationEvilSoul.h"

#include "Bullet.h"
#include <DoewoleBullet_Thorn.h>
#include "DocheolBullet_DoorBullet.h"
#include "Docheol_SpecialDoor.h"




IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
	Weapon.resize(4);
	Acc.resize(4);
	Space.resize(10);

	ITEM_INFO item_NULL = { ITEM_END, 0, "", "", "", "", 0, 0, 0.f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	ITEM_INFO item_05 = { ITEM_WEAPON, 5, "바람 정령의 활",		"일반", " [ 차지 공격시 강한 바람을 담은 화살을 발사한다 ] ", " [ 바람 정령의 힘이 깃든 활, 매우 가볍고 빠르다 ] ", 12, 16, 0.f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100 };

	for (int i = 0; i < 4; i++)
	{
		Weapon[i] = item_NULL;
		Acc[i] = item_NULL;
	}
	for (int i = 0; i < 10; i++)
	{
		Space[i] = item_NULL;
	}

	Weapon[0] = item_05;
}


CCollisionMgr::~CCollisionMgr()
{
}


void CCollisionMgr::Collision_Update()
{
	Collision_Sphere_Player_Bullet(m_CollisionGroup[OBJ_PLAYER], m_CollisionGroup[OBJ_BULLET]);

	Collision_Sphere_Boss_Arrow(m_CollisionGroup[OBJ_BOSS], m_CollisionGroup[OBJ_ARROW]);
	Collision_Sphere_Monster_Arrow(m_CollisionGroup[OBJ_MONSTER], m_CollisionGroup[OBJ_ARROW]);

	Collision_Sphere_Landscape_Arrow(m_CollisionGroup[OBJ_LANDSCAPE], m_CollisionGroup[OBJ_ARROW]);

	Collision_Sphere_Boss_NIGHT(m_CollisionGroup[OBJ_BOSS], m_CollisionGroup[OBJ_APOSTLE_NIGHT]);
	Collision_Sphere_Monster_NIGHT(m_CollisionGroup[OBJ_MONSTER], m_CollisionGroup[OBJ_APOSTLE_NIGHT]);

	Collision_Sphere_Boss_SKILL(m_CollisionGroup[OBJ_BOSS], m_CollisionGroup[OBJ_SKILL]);
	Collision_Sphere_Monster_SKILL(m_CollisionGroup[OBJ_MONSTER], m_CollisionGroup[OBJ_SKILL]);

	Collision_Sphere_Boss_FIRE(m_CollisionGroup[OBJ_BOSS], m_CollisionGroup[OBJ_APOSTLE_COURAGE]);
	Collision_Sphere_Monster_FIRE(m_CollisionGroup[OBJ_MONSTER], m_CollisionGroup[OBJ_APOSTLE_COURAGE]);

	Collision_Sphere_Monster_FIRE_Big(m_CollisionGroup[OBJ_MONSTER], m_CollisionGroup[OBJ_CHARGE_ARROW]);
	Collision_Sphere_Boss_FIRE_Big(m_CollisionGroup[OBJ_BOSS], m_CollisionGroup[OBJ_CHARGE_ARROW]);

	Collision_Sphere_Special_Bullet(m_CollisionGroup[OBJ_SPECIAL], m_CollisionGroup[OBJ_BULLET]);	// 안원 추가 빨파초흰검 위함

	Collision_Sphere_Landscape_Bullet(m_CollisionGroup[OBJ_LANDSCAPE], m_CollisionGroup[OBJ_BULLET]);

	Clear_CollisionGroup();
}

void CCollisionMgr::Collision_Sphere_Player_Bullet(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (dynamic_cast<CDoewoleBullet_Thorn*>(Src) && !dynamic_cast<CDoewoleBullet_Thorn*>(Src)->Get_Render())
				continue;
			
			if (Check_Sphere(Dest, Src))
			{
				if (dynamic_cast<CPlayer*>(Dest) && dynamic_cast<CBullet*>(Src))
				{
					if (dynamic_cast<CPlayer*>(Dest)->m_bImmuned == false) {
						dynamic_cast<CPlayer*>(Dest)->m_bHit = true;
						dynamic_cast<CBullet*>(Src)->m_bHit = true;
					}
						
				}
			}

		}
	}
}

void CCollisionMgr::Collision_Sphere_Boss_Arrow(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (dynamic_cast<CDoewole*>(Dest))
			{
				if (!dynamic_cast<CBoss*>(Dest)->m_bRender || dynamic_cast<CDoewole*>(Dest)->Get_State() == CDoewole::BOSS_DEAD)
					continue;
			}

			if (Check_Sphere(Dest, Src))
			{	
				if (dynamic_cast<CBoss*>(Dest) && dynamic_cast<CArrow*>(Src))
				{
					
					dynamic_cast<CArrow*>(Src)->m_bHit = true;
					dynamic_cast<CBoss*>(Dest)->m_bHit = true;
					dynamic_cast<CBoss*>(Dest)->m_Damage_List = dynamic_cast<CArrow*>(Src)->m_Damage_List;

				}

				
			}
		}

	}

}



void CCollisionMgr::Collision_Sphere_Monster_Arrow(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dest, Src))
			{
				
				if (dynamic_cast<CMonster*>(Dest) && dynamic_cast<CArrow*>(Src))
				{
					dynamic_cast<CArrow*>(Src)->m_bHit = true;
					dynamic_cast<CMonster*>(Dest)->m_bHit = true;
					dynamic_cast<CMonster*>(Dest)->m_Damage_List = dynamic_cast<CArrow*>(Src)->m_Damage_List;
				}
			}
		}

	}

}

void CCollisionMgr::Collision_Sphere_Landscape_Arrow(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere_Static(Dest, Src))
			{
				if (dynamic_cast<CDestructibleObj*>(Dest) && dynamic_cast<CArrow*>(Src))
				{
					if (dynamic_cast<CDestructibleObj*>(Dest)->Get_HitCnt() < dynamic_cast<CDestructibleObj*>(Dest)->Get_MaxFrameCnt()-1)
					{
						dynamic_cast<CDestructibleObj*>(Dest)->Set_IsHit(true);
						dynamic_cast<CArrow*>(Src)->Set_Dead();
					}
				}
				else if (dynamic_cast<CJar*>(Dest) && dynamic_cast<CArrow*>(Src))
				{
					dynamic_cast<CJar*>(Dest)->Set_IsHit(true);
					dynamic_cast<CArrow*>(Src)->Set_Dead();
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Sphere_Landscape_Bullet(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere_Static(Dest, Src))
			{
				if (dynamic_cast<CDestructibleObj*>(Dest) && dynamic_cast<CBullet*>(Src))
				{
					if (dynamic_cast<CDestructibleObj*>(Dest)->Get_HitCnt() < dynamic_cast<CDestructibleObj*>(Dest)->Get_MaxFrameCnt() - 1)
					{
						dynamic_cast<CBullet*>(Src)->Set_Dead();
					}

				}
				else if (dynamic_cast<CJar*>(Dest) && dynamic_cast<CArrow*>(Src))
				{
					dynamic_cast<CBullet*>(Src)->Set_Dead();
				}
			}
		}
	}
}

void CCollisionMgr::Collision_Sphere_Boss_NIGHT(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	

	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (dynamic_cast<CDoewole*>(Dest))
			{
				if (!dynamic_cast<CBoss*>(Dest)->m_bRender || dynamic_cast<CDoewole*>(Dest)->Get_State() == CDoewole::BOSS_DEAD)
					continue;
			}

			if (Check_Sphere(Dest, Src))
			{
				if (dynamic_cast<CBoss*>(Dest) && dynamic_cast<CNight_Circle*>(Src))
				{
					
					if (dynamic_cast<CNight_Circle*>(Src)->m_bHiting) {

						dynamic_cast<CBoss*>(Dest)->m_bHit = true;
						dynamic_cast<CNight_Circle*>(Src)->m_bHit = true;
						dynamic_cast<CBoss*>(Dest)->m_Damage_List = dynamic_cast<CNight_Circle*>(Src)->m_Damage_List;
						dynamic_cast<CNight_Circle*>(Src)->TargetPos_List.emplace_back(dynamic_cast<CBoss*>(Dest)->Get_TransCom()->m_vInfo[INFO_POS]);

					}
				}

			}
		}

	}

}

void CCollisionMgr::Collision_Sphere_Monster_NIGHT(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{

			if (Check_Sphere(Dest, Src))
			{
				if (dynamic_cast<CMonster*>(Dest) && dynamic_cast<CNight_Circle*>(Src))
				{

					if (dynamic_cast<CNight_Circle*>(Src)->m_bHiting) {

						dynamic_cast<CMonster*>(Dest)->m_bHit = true;
						dynamic_cast<CNight_Circle*>(Src)->m_bHit = true;
						dynamic_cast<CMonster*>(Dest)->m_Damage_List = dynamic_cast<CNight_Circle*>(Src)->m_Damage_List;
						dynamic_cast<CNight_Circle*>(Src)->TargetPos_List.emplace_back(dynamic_cast<CMonster*>(Dest)->Get_TransCom()->m_vInfo[INFO_POS]);
					}
				}

			}
		}

	}

}

void CCollisionMgr::Collision_Sphere_Boss_FIRE(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (dynamic_cast<CDoewole*>(Dest))
			{
				if (!dynamic_cast<CBoss*>(Dest)->m_bRender || dynamic_cast<CDoewole*>(Dest)->Get_State() == CDoewole::BOSS_DEAD)
					continue;
			}

			if (Check_Sphere(Dest, Src))
			{
				if (dynamic_cast<CBoss*>(Dest) && dynamic_cast<CFire_Wave*>(Src))
				{

					if (dynamic_cast<CFire_Wave*>(Src)->m_bHiting) {

						dynamic_cast<CBoss*>(Dest)->m_bHit = true;
						dynamic_cast<CFire_Wave*>(Src)->m_bHit = true;
						dynamic_cast<CBoss*>(Dest)->m_Damage_List = dynamic_cast<CFire_Wave*>(Src)->m_Damage_List;
						dynamic_cast<CFire_Wave*>(Src)->TargetPos_List.emplace_back(dynamic_cast<CBoss*>(Dest)->Get_TransCom()->m_vInfo[INFO_POS]);

					}
				}

			}
		}

	}
}

void CCollisionMgr::Collision_Sphere_Monster_FIRE(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{

			if (Check_Sphere(Dest, Src))
			{
				if (dynamic_cast<CMonster*>(Dest) && dynamic_cast<CFire_Wave*>(Src))
				{

					if (dynamic_cast<CFire_Wave*>(Src)->m_bHiting) {

						dynamic_cast<CMonster*>(Dest)->m_bHit = true;
						dynamic_cast<CFire_Wave*>(Src)->m_bHit = true;
						dynamic_cast<CMonster*>(Dest)->m_Damage_List = dynamic_cast<CFire_Wave*>(Src)->m_Damage_List;
						dynamic_cast<CFire_Wave*>(Src)->TargetPos_List.emplace_back(dynamic_cast<CMonster*>(Dest)->Get_TransCom()->m_vInfo[INFO_POS]);
					}
				}

			}
		}

	}
}

void CCollisionMgr::Collision_Sphere_Boss_SKILL(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (dynamic_cast<CDoewole*>(Dest))
			{
				if (!dynamic_cast<CBoss*>(Dest)->m_bRender || dynamic_cast<CDoewole*>(Dest)->Get_State() == CDoewole::BOSS_DEAD)
					continue;
			}

			if (Check_Sphere(Dest, Src))
			{
				if (dynamic_cast<CBoss*>(Dest) && dynamic_cast<CNight_Big*>(Src))
				{

					if (dynamic_cast<CNight_Big*>(Src)->m_bHiting) {

						dynamic_cast<CBoss*>(Dest)->m_bHit = true;
						dynamic_cast<CNight_Big*>(Src)->m_bHit = true;
						dynamic_cast<CBoss*>(Dest)->m_Damage_List = dynamic_cast<CNight_Big*>(Src)->m_Damage_List;
						dynamic_cast<CNight_Big*>(Src)->TargetPos_List.emplace_back(dynamic_cast<CBoss*>(Dest)->Get_TransCom()->m_vInfo[INFO_POS]);

					}
				}

			}
		}

	}
}

void CCollisionMgr::Collision_Sphere_Monster_SKILL(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{

			if (Check_Sphere(Dest, Src))
			{
				if (dynamic_cast<CMonster*>(Dest) && dynamic_cast<CNight_Big*>(Src))
				{

					if (dynamic_cast<CNight_Big*>(Src)->m_bHiting) {

						dynamic_cast<CMonster*>(Dest)->m_bHit = true;
						dynamic_cast<CNight_Big*>(Src)->m_bHit = true;
						dynamic_cast<CNight_Big*>(Src)->TargetPos_List.emplace_back(dynamic_cast<CMonster*>(Dest)->Get_TransCom()->m_vInfo[INFO_POS]);
					}
				}

			}
		}

	}
}

void CCollisionMgr::Collision_Sphere_Monster_FIRE_Big(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{

	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{

			if (Check_Sphere(Dest, Src))
			{
				if (dynamic_cast<CMonster*>(Dest) && dynamic_cast<CFire_Big*>(Src))
				{

					if (dynamic_cast<CFire_Big*>(Src)->m_bHiting) {

						dynamic_cast<CMonster*>(Dest)->m_bHit = true;
						dynamic_cast<CFire_Big*>(Src)->m_bHit = true;
						dynamic_cast<CFire_Big*>(Src)->TargetPos_List.emplace_back(dynamic_cast<CMonster*>(Dest)->Get_TransCom()->m_vInfo[INFO_POS]);
					}
				}

			}
		}

	}

}

void CCollisionMgr::Collision_Sphere_Boss_FIRE_Big(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{

	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (dynamic_cast<CDoewole*>(Dest))
			{
				if (!dynamic_cast<CBoss*>(Dest)->m_bRender || dynamic_cast<CDoewole*>(Dest)->Get_State() == CDoewole::BOSS_DEAD)
					continue;
			}

			if (Check_Sphere(Dest, Src))
			{
				if (dynamic_cast<CDoewole*>(Dest) && dynamic_cast<CFire_Big*>(Src))
				{

					if (dynamic_cast<CFire_Big*>(Src)->m_bHiting) {

						dynamic_cast<CBoss*>(Dest)->m_bHit = true;
						dynamic_cast<CFire_Big*>(Src)->m_bHit = true;
						dynamic_cast<CFire_Big*>(Src)->TargetPos_List.emplace_back(dynamic_cast<CBoss*>(Dest)->Get_TransCom()->m_vInfo[INFO_POS]);
					}
				}

			}
		}

	}

}

void CCollisionMgr::Collision_Sphere_Special_Bullet(list<CGameObject*> _Dest, list<CGameObject*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dest, Src))
			{
				if (dynamic_cast<CDocheol_SpecialDoor*>(Dest) && dynamic_cast<CDocheolBullet_DoorBullet*>(Src))
				{
					dynamic_cast<CDocheol_SpecialDoor*>(Dest)->m_bHit = true;
					dynamic_cast<CDocheolBullet_DoorBullet*>(Src)->m_bHit = true;

					int iColor = dynamic_cast<CDocheolBullet_DoorBullet*>(Src)->Get_Color();
					dynamic_cast<CDocheol_SpecialDoor*>(Dest)->Set_BulletColor(iColor);
				}
			}
		}

	}
}


bool CCollisionMgr::Check_Sphere(CGameObject* pDest, CGameObject* pSrc)
{
	CCollider* pColliderDest = dynamic_cast<CCollider*>((pDest)->Get_Component(L"Proto_Collider", ID_DYNAMIC));
	CCollider* pColliderSrc = dynamic_cast<CCollider*>((pSrc)->Get_Component(L"Proto_Collider", ID_DYNAMIC));

	_vec3 v1 = pColliderDest->Get_SpherePos();
	_vec3 v2 = pColliderSrc->Get_SpherePos();

	_float r1 = pColliderDest->Get_Radius();
	_float r2 = pColliderSrc->Get_Radius();

	return (r1 + r2) > D3DXVec3Length(&(v1 - v2));
}

bool CCollisionMgr::Check_Sphere_Static(CGameObject* pDest, CGameObject* pSrc)
{
	CCollider* pColliderDest = dynamic_cast<CCollider*>((pDest)->Get_Component(L"Proto_Collider", ID_STATIC));
	CCollider* pColliderSrc = dynamic_cast<CCollider*>((pSrc)->Get_Component(L"Proto_Collider", ID_DYNAMIC));

	_vec3 v1 = pColliderDest->Get_SpherePos();
	_vec3 v2 = pColliderSrc->Get_SpherePos();

	_float r1 = pColliderDest->Get_Radius();
	_float r2 = pColliderSrc->Get_Radius();

	return (r1 + r2) > D3DXVec3Length(&(v1 - v2));
}

void CCollisionMgr::Get_ItemMgr_Info(CInventoryMgr* _pInvenMgr)
{
	for (int i = 0; i < 4; i++)
	{
		Weapon[i] = _pInvenMgr->Get_Weapon_Info(i);
	}
	for (int i = 0; i < 4; i++)
	{
		Acc[i] = _pInvenMgr->Get_Acc_Info(i);
	}
	for (int i = 0; i < 10; i++)
	{
		Space[i] = _pInvenMgr->Get_Space_Info(i);
	}
}

ITEM_INFO CCollisionMgr::Set_ItemMgr_Info(_int _iCategory, _int _InvenNum)
{	
	if (_iCategory == 1)
	{
		return Weapon[_InvenNum];
	}

	if (_iCategory == 2)
	{
		return Acc[_InvenNum];
	}

	if (_iCategory == 3)
	{
		return Space[_InvenNum];
	}

	ITEM_INFO Temp_Info = {};

	return Temp_Info;
}


void CCollisionMgr::Free(void)
{
}

void CCollisionMgr::Clear_CollisionGroup()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		m_CollisionGroup[i].clear();
	}
}

void CCollisionMgr::Add_CollisionObject(OBJ_TYPE _eID, CGameObject* _pGameObject)
{
	if (_pGameObject == nullptr || OBJ_END <= _eID)
		return;

	m_CollisionGroup[_eID].push_back(_pGameObject);
}

