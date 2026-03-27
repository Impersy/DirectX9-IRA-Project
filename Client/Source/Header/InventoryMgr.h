#pragma once
#include "Base.h"
#include "GameObject.h"
#include "Include.h"

class CInventoryMgr
{
public:
	CInventoryMgr();
	~CInventoryMgr();

public:
	void	Add_Weapon(void);
	void	Rooting_Item_In_Inven(_int _iItemNumber);

	_int				Find_Empty_WeaponSlot(void);

	_int				Find_Empty_SpaceSlot(void);

	vector<ITEM_INFO>	Get_Weapon(void) { return Weapon; }
	vector<ITEM_INFO>	Get_Acc(void) { return Acc; }
	vector<ITEM_INFO>	Get_Space(void) { return Space; }

	_bool				Get_Down(void) { return m_bDown; }

	void	Set_Weapon(_int _Index, ITEM_INFO _Temp_Info);
	void	Set_Acc(_int _Index, ITEM_INFO _Temp_Info);
	void	Set_Space(_int _Index, ITEM_INFO _Temp_Info);
	void	Set_Down(_bool _On_Off) { m_bDown = _On_Off; }

	ITEM_INFO Get_Weapon_Info(_int _num) { return Weapon[_num]; }
	ITEM_INFO Get_Acc_Info(_int _num) { return Acc[_num]; }
	ITEM_INFO Get_Space_Info(_int _num) { return Space[_num]; }

	ITEM_INFO Set_Weapon_Info(_int _num, ITEM_INFO _Info) { Weapon[_num] = _Info; }
	ITEM_INFO Set_Acc_Info(_int _num, ITEM_INFO _Info) { Acc[_num] = _Info; }
	ITEM_INFO Set_Space_Info(_int _num, ITEM_INFO _Info) { Space[_num] = _Info; }






public:
	vector<ITEM_INFO> INFO;
	vector<ITEM_INFO> Weapon;
	vector<ITEM_INFO> Acc;
	vector<ITEM_INFO> Space;

	_bool m_bDown;

private:
	void		Free(void);

};

