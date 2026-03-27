#pragma once

#include "Typedef.h"

BEGIN(Engine)

typedef	 struct tagVertexColor
{
	_vec3			vPos;		// 위치
	_ulong			dwColor;	// 색상

}VTXCOL;

const	_ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef	 struct tagVertexTexture
{
	_vec3			vPos;		// 위치
	_vec3			vNormal;
	_vec2			vTexUV;		

}VTXTEX;

const	_ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 ;

typedef	 struct tagVertexCube
{
	_vec3			vPos;		// 위치
	_vec3			vTexUV;		// 색상

}VTXCUBE;

const	_ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);


// D3DFVF_TEXCOORDSIZE3 : 텍스처의 UV값이 FLOAT형 3개의 크기 만큼이며 괄호 안 숫자의 의미는 본래 버텍스의 UV 값이 여러개 있을 수 잇는데 그 중 첫 번째 값을 사용하겠다는 의미

typedef	struct tagIndex16
{
	_ushort		_0;
	_ushort		_1;
	_ushort		_2;

}INDEX16;

typedef	struct tagIndex32
{
	_ulong		_0;
	_ulong		_1;
	_ulong		_2;

}INDEX32;


typedef struct tagItemInfo {

	ITEM_TYPE tType;				// 아이템 타입 (소모품이냐, 장비냐 구분)
	_int	iItemNumber;			// 아이템 넘버
	std::string sItemName;				// 아이템 이름
	std::string sItemGrade;				// 아이템 등급 (일반, 희귀, 영웅 정도)
	std::string sItemDescription_Normal;	// 일반 설명
	std::string sItemDescription_Space;	// 아이템 위에서 Space bar를 눌렀을 때 보여지는 설명

	// ITEM_TYPE 이 WEAPON 일 때, 
	_int	iMinAtt;				// 무기의 최소 공격력
	_int	iMaxAtt;				// 무기의 최대 공격력
	_float	fPlusSpeed;					// 추가 이동속도?

	_int	iPlus_Att;
	_int	iPlus_NowHp;
	_int	iPlus_MaxHp;
	_int	iPlus_NowEnergy;
	_int	iPlus_MaxEnergy;
	_int	iPlus_Key;
	_int	iPlus_Money;
	_int	iPlus_Gem;
	_int	iUpgrade;

	_int	iPrice;

	//tagItemInfo(ITEM_TYPE type, const std::string& name, const std::string& grade, const std::string& descNormal, const std::string& descSpace,
	//	 _int minAtt, _int maxAtt, _float speed, _int att, _int nowHp, _int maxHp, _int nowEnergy, _int maxEnergy, _int key,
	//	_int money, _int gem, _int upgrade, _int price)
	//	: tType(type), sItemName(name), sItemGrade(grade), sItemDescription_Normal(descNormal),	sItemDescription_Space(descSpace),
	//	iMinAtt(minAtt), iMaxAtt(maxAtt), fPlusSpeed(speed), iPlus_Att(att), iPlus_NowHp(nowHp), iPlus_MaxHp(maxHp), iPlus_NowEnergy(nowEnergy), iPlus_MaxEnergy(maxEnergy), iPlus_Key(key), 
	//	iPlus_Money(money), iPlus_Gem(gem), iUpgrade(upgrade), iPrice(price) {}
}ITEM_INFO;


typedef	struct tagFrontUiInfo
{
	_int		iNum;	//  FrontUi 에서 각 오브젝트들이 갖게 될 식별번호
	_int		iMax;	// 부모 자식 모두 갖게 될 Max 값 ex) MaxHp
	_int		iNow;	// 부모 자식 모두 갖게 될 Now 값 ex) NowHp

}FRONTUI;




//ITEM_INFO item1(타입, "이름", "등급", "기본 설명", "스페이스바 출력 설명", 무기 최소공격력, 무기 최대공격력, 추가 이동속도.f, 
   //			   추가 공격력, 추가 HP, 추가 MaxHP, 추가 현재에너지, 추가 맥스에너지, 추가 키, 추가 골드, 추가 젬, 가격);


END

typedef struct tagTerrainData
{
	_ulong	dwVtxCntX;
	_ulong	dwVtxCntZ;
	_ulong	dwVtxItv;
	_byte	byDrawID;
	_byte	byDrawOption;
	_float	fDetailLevel;
	_int	iHeightMapLevel;
} TERRAINDATA;

typedef struct tagTerrainDataPointer
{
	_ulong	dwVtxCnt;
	_vec3* pPos;
} TERRAINDATA_P;

typedef struct tagStaticObjectData
{
	_vec3	vScale;
	_vec3	vAngle;
	_vec3	vPos;
	_byte	byDrawID;
	_byte	byDrawOption;
	_vec3	vColliderPos;
	_float	fColliderRadius;
}STATICOBJDATA;