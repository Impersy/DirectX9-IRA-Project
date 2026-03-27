#include "stdafx.h"
#include "..\Header\InventoryMgr.h"
#include "Export_Function.h"
#include  "SoundMgr.h"

CInventoryMgr::CInventoryMgr()
{
	Weapon.resize(4);
	Acc.resize(4);
	Space.resize(10);
	INFO.resize(13);

	//ITEM_INFO item1(타입, "이름", "등급", "기본 설명", "스페이스바 출력 설명", 무기 최소공격력, 무기 최대공격력, 추가 이동속도.f, 추가 공격력, 추가 HP, 추가 MaxHP, 추가 현재에너지, 추가 맥스에너지, 추가 키, 추가 골드, 추가 젬, 가격);

	ITEM_INFO item_NULL = { ITEM_END, 0, "", "", "", "", 0, 0, 0.f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	ITEM_INFO item_02 = { ITEM_STATUS, 2, "체력 회복",		"소모품",	" [       체력을 즉시 1 개 만큼 회복합니다       ] ", " [        이 외에 특별한 효과는 없습니다        ] ", 0, 0, 0.f, 0, 1, 0, 0, 0, 0, 0, 0, 0, 100 };
	ITEM_INFO item_03 = { ITEM_STATUS, 3, "이해 안되는 책",	"소모품",	" [       최대 체력이 1 개 만큼 증가합니다       ] ", " [ 용책에게 간절히 소원을 빌어, 과제를 해달라고 ] ", 0, 0, 0.f, 0, 1, 1, 0, 0, 0, 0, 0, 0, 300 };	//책1
	ITEM_INFO item_04 = { ITEM_STATUS, 4, "진짜 어려운 책",	"소모품",	" [       최대 기력이 1 개 만큼 증가합니다       ] ", " [ 어느 현자가 말했다 너네도 다 잘거잖아!! 라고 ] ", 0, 0, 0.f, 0, 0, 0, 0, 1, 0, 0, 0, 0, 300 };	//책2

	ITEM_INFO item_05 = { ITEM_WEAPON, 5, "바람 정령의 활",		"일반", " [ 차지 공격시 강한 바람을 담은 화살을 발사한다 ] ", " [ 바람 정령의 힘이 깃든 활, 매우 가볍고 빠르다 ] ", 12, 16, 0.f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100 };
	ITEM_INFO item_06 = { ITEM_WEAPON, 6, "[카프] 홍염의 활",	"영웅", " [ 차지 공격시 불꽃의 힘을 담은 화살을 발사한다 ] ", " [ 사도 카프가 소중히 관리했다. 굉장히 따뜻하다 ] ", 24, 48, 0.f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500 };
	ITEM_INFO item_07 = { ITEM_WEAPON, 7, "[소] 원뢰의 활",		"영웅", " [ 차지 공격시 강한 낙뢰와 함께 화살을 발사한다 ] ", " [ 사도 소 와 닮은 활이다. 매우 차갑고 단단하다 ] ", 24, 48, 0.f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500 };
	ITEM_INFO item_08 = { ITEM_WEAPON, 8, "[티프] 차원의 활",	"영웅", " [ 차지 공격시 티프의 힘을 받아 화살을 발사한다 ] ", " [ 사도 티프가 힘을 담아둔 활, 더욱 강해진 느낌 ] ", 24, 48, 0.f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500 };

	ITEM_INFO item_09 = { ITEM_ACC, 9,   "대도적의 두건",		"영웅", " [       플레이어의 이동속도가 증가합니다       ] ", " [ 대도적이 사용하던 두건, 훔친 보물들은 어디에 ] ", 0, 0, 2.f, 0, 0, 0, 0, 1, 0, 0, 0, 0, 500 };
	ITEM_INFO item_010 = { ITEM_ACC, 10, "제일궁의 덧살",		"희귀", " [   플레이어의 추가 공격력이 소폭 증가합니다   ] ", " [ 원활한 사격을 위한 보조도구, 마치 용책이랄까 ] ", 0, 0, 0.f, 5, 0, 0, 0, 1, 0, 0, 0, 0, 300 };
	ITEM_INFO item_011 = { ITEM_ACC, 11, "홍길동의 짚신",		"일반", " [       플레이어의 이동속도가 증가합니다       ] ", " [ 짚으로 엉성하게 만든 신발, 이걸 신고가겠다니 ] ", 0, 0, 0.5f, 0, 0, 0, 0, 1, 0, 0, 0, 0, 100 };
	ITEM_INFO item_012 = { ITEM_ACC, 12, "매화그림 족자",		"영웅", " [   플레이어의 최대 능력치가 소폭 증가합니다   ] ", " [ 아름다운 그림이 그려진 족자, 집에 가고싶어요 ] ", 0, 0, 0.0f, 0, 1, 1, 0, 1, 0, 0, 0, 0, 500 };
	ITEM_INFO item_013 = { ITEM_ACC, 13, "고급가죽 털신",		"희귀", " [       플레이어의 이동속도가 증가합니다       ] ", " [ 장인이 만든 가죽신, 금강 제화 라고 적혀있다  ] ", 0, 0, 1.0f, 0, 1, 1, 0, 1, 0, 0, 0, 0, 200 };

	INFO[0] = item_NULL;
	INFO[1] = item_02;
	INFO[2] = item_03;
	INFO[3] = item_04;
	INFO[4] = item_05;
	INFO[5] = item_06;
	INFO[6] = item_07;
	INFO[7] = item_08;
	INFO[8] = item_09;
	INFO[9] = item_010;
	INFO[10] = item_011;
	INFO[11] = item_012;
	INFO[12] = item_013;

	for (int i = 0; i < 4; i++)
	{
		Weapon[i] = item_NULL;
		Acc[i] = item_NULL;
	}
	for (int i = 0; i < 10; i++)
	{
		Space[i] = item_NULL;
	}

	Add_Weapon();

	//


	/*
	* 
	* "최대 체력이 1 증가합니다"
	" [       체력을 즉시 1 개 만큼 회복합니다       ] "
	" [       최대 체력이 1 개 만큼 증가합니다       ] "
	" [       최대 기력이 1 개 만큼 증가합니다       ] "

	" [ 차지 공격시 강한 바람을 담은 화살을 발사한다 ] "
	" [ 바람 정령의 힘이 깃든 활, 매우 가볍고 빠르다 ] "

	" [ 차지 공격시 불꽃의 힘을 담은 화살을 발사한다 ] "
	" [ 사도 카프가 소중히 관리했다. 굉장히 따뜻하다 ] "

	" [ 차지 공격시 강한 낙뢰와 함께 화살을 발사한다 ] "
	" [ 사도 소 와 닮은 활이다. 매우 차갑고 단단하다 ] "

	" [ 차지 공격시 티프의 힘을 받아 화살을 발사한다 ] "
	" [ 사도 티프가 힘을 담아둔 활, 더욱 강해진 느낌 ] "
	
	" [        이 외에 특별한 효과는 없습니다        ] "

	" [       플레이어의 이동속도가 증가합니다       ] "
	원활한 사격을 위한 보조도구"
	짚으로 엉성하게 만든 신발",
	아름다운 그림이 그려진 족자"
	장인이 공들여 만든 가죽 신발


	" [   플레이어의 최대 능력치가 소폭 증가합니다   ] "
	" [ 대도적이 사용하던 두건, 훔친 보물들은 어디에 ] "
	" [ 원활한 사격을 위한 보조도구, 마치 용책이랄까 ] "
	" [ 짚으로 엉성하게 만든 신발, 이걸 신고가겠다니 ] "
	" [ 아름다운 그림이 그려진 족자, 집에 가고싶어요 ] "
	" [ 장인이 만든 가죽신, 금강 제화 라고 적혀있다  ] " 

	" [ API와 MFC를 겪은 쥬신인이 눈물을 흘리며 썼다 ] "
	" [ 용책에게 간절히 소원을 빌어, 과제를 해달라고 ] "

	
	*/
}

CInventoryMgr::~CInventoryMgr()
{
	Free();
}

void CInventoryMgr::Add_Weapon(void)
{	
	if (Weapon[0].tType == ITEM_END)
	{
		Weapon[0] = INFO[4];
	}
}

void CInventoryMgr::Rooting_Item_In_Inven(_int _iItemNumber)
{
	_int iTemp_Empty_SpaceSlot = Find_Empty_SpaceSlot();

	if ((iTemp_Empty_SpaceSlot > -1))
	{
		Space[iTemp_Empty_SpaceSlot] = INFO[_iItemNumber - 1];
	}

	//if ((_iItemNumber >= 5) && (_iItemNumber <= 8))
	//{	
	//	_int iTemp_Empty_WeaponSlot = Find_Empty_WeaponSlot();

	//	if ((iTemp_Empty_WeaponSlot > -1))
	//	{
	//		Weapon[iTemp_Empty_WeaponSlot] = INFO[_iItemNumber - 1];
	//	}
	//	else if ((iTemp_Empty_WeaponSlot == (-1)))
	//	{
	//		_int iTemp_Empty_SpaceSlot = Find_Empty_SpaceSlot();

	//		if ((iTemp_Empty_SpaceSlot > -1))
	//		{
	//			Space[iTemp_Empty_WeaponSlot] = INFO[_iItemNumber - 1];
	//		}
	//		else if (true)
	//		{
	//		}
	//	}
	//}
}

_int CInventoryMgr::Find_Empty_WeaponSlot(void)
{
	for (_int i = 0; i < Weapon.size(); ++i)
	{
		if (Weapon[i].tType == ITEM_END)
		{
			return i;
		}
	}

	return -1;
}

_int CInventoryMgr::Find_Empty_SpaceSlot(void)
{
	for (_int i = 0; i < Space.size(); ++i)
	{
		if (Space[i].tType == ITEM_END)
		{
			return i;
		}
	}

	return -1;
}

void CInventoryMgr::Set_Weapon(_int _Index, ITEM_INFO _Temp_Info)
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_CLICK);
	CSoundMgr::Get_Instance()->PlaySound(L"UI_item equip.wav", CSoundMgr::UI_CLICK, 0.8f);
	Weapon[_Index] = _Temp_Info;
}

void CInventoryMgr::Set_Acc(_int _Index, ITEM_INFO _Temp_Info)
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_CLICK);
	CSoundMgr::Get_Instance()->PlaySound(L"UI_item equip.wav", CSoundMgr::UI_CLICK, 0.8f);
	Acc[_Index] = _Temp_Info;
}

void CInventoryMgr::Set_Space(_int _Index, ITEM_INFO _Temp_Info)
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI_CLICK);
	CSoundMgr::Get_Instance()->PlaySound(L"UI_item drop.wav", CSoundMgr::UI_CLICK, 0.8f);
	Space[_Index] = _Temp_Info;
}

void CInventoryMgr::Free(void)
{

}
