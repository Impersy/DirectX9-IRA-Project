#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Include.h"
#include "InventoryMgr.h"

class CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)
private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	void				Add_CollisionObject(OBJ_TYPE _eID, CGameObject* _pGameObject);
	void				Collision_Update();
	void				Clear_CollisionGroup();

	void				Get_ItemMgr_Info(CInventoryMgr* _pInvenMgr);
	ITEM_INFO			Set_ItemMgr_Info(_int _iCategory, _int _InvenNum);


private:
	static void	Collision_Sphere_Player_Bullet(list<CGameObject*> _Dest, list<CGameObject*> _Src);
	static void	Collision_Sphere_Boss_Arrow(list<CGameObject*> _Dest, list<CGameObject*> _Src);
	static void	Collision_Sphere_Monster_Arrow(list<CGameObject*> _Dest, list<CGameObject*> _Src);
	static void	Collision_Sphere_Landscape_Arrow(list<CGameObject*> _Dest, list<CGameObject*> _Src);
	static void	Collision_Sphere_Landscape_Bullet(list<CGameObject*> _Dest, list<CGameObject*> _Src);

	static void	Collision_Sphere_Boss_NIGHT(list<CGameObject*> _Dest, list<CGameObject*> _Src);
	static void	Collision_Sphere_Monster_NIGHT(list<CGameObject*> _Dest, list<CGameObject*> _Src);

	static void	Collision_Sphere_Boss_FIRE(list<CGameObject*> _Dest, list<CGameObject*> _Src);
	static void	Collision_Sphere_Monster_FIRE(list<CGameObject*> _Dest, list<CGameObject*> _Src);

	static void	Collision_Sphere_Boss_SKILL(list<CGameObject*> _Dest, list<CGameObject*> _Src);
	static void	Collision_Sphere_Monster_SKILL(list<CGameObject*> _Dest, list<CGameObject*> _Src);

	static void	Collision_Sphere_Monster_FIRE_Big(list<CGameObject*> _Dest, list<CGameObject*> _Src);
	static void	Collision_Sphere_Boss_FIRE_Big(list<CGameObject*> _Dest, list<CGameObject*> _Src);

	static void	Collision_Sphere_Special_Bullet(list<CGameObject*> _Dest, list<CGameObject*> _Src);

	static bool Check_Sphere(CGameObject* pDest, CGameObject* pSrc);
	static bool Check_Sphere_Static(CGameObject* pDest, CGameObject* pSrc);



private:
	list<CGameObject*>			m_CollisionGroup[OBJ_END];

	vector<ITEM_INFO> Weapon;
	vector<ITEM_INFO> Acc;
	vector<ITEM_INFO> Space;

private:
	virtual void		Free(void);


	

};

