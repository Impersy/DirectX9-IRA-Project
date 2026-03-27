#include "stdafx.h"
#include "..\Header\Fire_Big.h"
#include "Export_Function.h"
#include "CollisionMgr.h"
#include "Player.h"
#include "Fire_Hit.h"
#include "Fire_Brave.h"
#include "Effect_Player_Damage_Font.h"

CFire_Big::CFire_Big(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CFire_Big::CFire_Big(const CFire_Big & rhs)
	: CEffect(rhs)
{
}

CFire_Big::~CFire_Big()
{
	Free();
}

HRESULT CFire_Big::Ready_GameObject(void)
{
	Add_Component();

	
	m_pTransformCom->m_vScale = { 6.f * 5.f * PUBLIC_SCALE , 6.f * 5.f * PUBLIC_SCALE, 1.f };
	
	m_fMaxFrame = 34.f;
	m_vPos.y += 8.f;
	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;



	if (g_eCurScene == SCENE_BOSS2) {
		m_pTransformCom->m_vScale *= 1.5f;
	}


	m_pColliderCom->Set_Radius(30.f);

	for (int i = 0; i < m_fAttack_Num; i++) {

		m_Cri_List.emplace_back(Final_Damage());
	}




	return S_OK;
}

_int CFire_Big::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;


	m_fTime += 3.f * Time * 1.f;

	if (m_fTime > 3.f) {
		m_fTime = 0.f;
		m_bHiting = true;
	}

	if (m_bHit) {
		Create_Damage_Font();
		m_bHit = false;
		m_bHiting = false;
	}




	m_fFrame += m_fMaxFrame * Time * 1.0f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_CHARGE_ARROW, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CFire_Big::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CFire_Big::Render_GameObject()
{

	__super::Render_GameObject();
}

HRESULT CFire_Big::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Fire_Big"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Fire_Big", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));

	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });


	return S_OK;
}

CFire_Big* CFire_Big::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CFire_Big* pInstance = new CFire_Big(pGraphicDev);

	pInstance->m_vPos = vPos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

bool CFire_Big::Final_Damage(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	int CriticalRate = (int)(pPlayer->m_Critical_Rate);

	bool Critical = true;


	m_fRandom_Value = (float)(rand() % (2 * (587 / 8 - 587 / 16 + 1)) - (587 / 8 - 587 / 16 + 1));

	m_fDamage = (m_fPower * 0.5f + m_fRandom_Value) * (1.f + 1.35f * Critical);

	m_Damage_List.emplace_back((int)m_fDamage);



	return Critical;
}

void CFire_Big::Create_Hit_Effect(void)
{
}

void CFire_Big::Create_Damage_Font(void)
{

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	for (int p = 0; p < TargetPos_List.size(); p++) {

		int k = 0;

		for (int i = 0; i < m_fAttack_Num; i++) {

			int j = 0;


			Create_Font_List(m_Damage_List.front());

			for (auto iter : m_Font_List) {
				_vec3 pos = TargetPos_List[p];
				pos.z -= 1.f;

				if (m_Cri_List.front() == true) {
					pos.x += 1.7f * j;
					pos.y += 2.f * k;
				}
				else {
					pos.x += 1.7f * 0.7 * j;
					pos.y += 2.f * k;
				}

				pGameObject = CEffect_Player_Damage_Font::Create(m_pGraphicDev, pos, (int)iter, m_Cri_List.front());


				if (pGameObject == nullptr)
					return;


				pGameLogicLayer->Add_BulletObject(pGameObject);

				j++;
			}

			m_Cri_List.pop_front();
			m_Damage_List.pop_front();
			m_Font_List.clear();
			m_Font_List.resize(0);

			k++;
		}

		for (int i = 0; i < m_fAttack_Num; i++) {

			m_Cri_List.emplace_back(Final_Damage());
		}


		// È«¿° Å¸°Ý ÀÌÆåÆ® Ãß°¡

		_vec3 pos = TargetPos_List[p];
		pos.z -= 1.f;

		pGameObject = CFire_Hit::Create(m_pGraphicDev, pos, true);

		if (pGameObject == nullptr)
			return;

		pGameLogicLayer->Add_BulletObject(pGameObject);


	
	}

	TargetPos_List.clear();
	TargetPos_List.resize(0);

	

}

void CFire_Big::Create_Font_List(int damage)
{

	int temp = (int)damage;
	int cnt = 0;

	while (temp > 0) {
		temp /= 10;
		cnt++;
	}

	temp = (int)damage;

	for (int i = 0; i < cnt; i++) {

		if (i == cnt - 1) {
			m_Font_List.emplace_front(temp);
			break;
		}

		m_Font_List.emplace_front(temp % 10);
		temp /= 10;

	}

}

void CFire_Big::Free(void)
{
	__super::Free();
}


