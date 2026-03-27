#include "stdafx.h"
#include "..\Header\Item.h"
#include "Export_Function.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr), m_pColliderCom(nullptr), m_pCalculatorCom(nullptr), m_bCheck(false)
	, m_fFrame(0.f), m_Radius(0.f)
{
	INFO.resize(13);

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
}

CItem::CItem(const CItem & rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTextureCom(rhs.m_pTextureCom), m_pTransformCom(rhs.m_pTransformCom), m_pColliderCom(rhs.m_pColliderCom), m_pCalculatorCom(rhs.m_pCalculatorCom)
	, m_bCheck(rhs.m_bCheck), m_fFrame(rhs.m_fFrame), m_Radius(rhs.m_Radius)
{
}

CItem::~CItem()
{

}

HRESULT CItem::Ready_GameObject(void)
{
	return S_OK;
}

_int CItem::Update_GameObject(const _float& fTimeDelta)
{	
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CItem::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CItem::Render_GameObject()
{
}

void CItem::Free(void)
{
	__super::Free();
}
