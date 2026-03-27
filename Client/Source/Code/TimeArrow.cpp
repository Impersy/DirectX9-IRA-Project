#include "stdafx.h"
#include "..\Header\TimeArrow.h"
#include "Export_Function.h"
#include "Effect_Player_Arrow_Hit.h"
#include "Effect_Player_Damage_Font.h"
#include "Player.h"
#include "CollisionMgr.h"
#include "Fire_Hit.h"
#include "Night_Hit.h"
#include "Time_Hit.h"
#include <SoundMgr.h>


CTimeArrow::CTimeArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CArrow(pGraphicDev)
{
}

CTimeArrow::CTimeArrow(const CTimeArrow& rhs)
	: CArrow(rhs)
{
}

CTimeArrow::~CTimeArrow()
{
	Free();
}

HRESULT CTimeArrow::Ready_GameObject(void)
{

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	
	Add_Component();

	m_fSpeed = 50.f * PUBLIC_SCALE;

	m_pTransformCom->Set_Scale_Ratio(m_Size);


	m_Fire_Pos.y += 10.f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_Fire_Pos;

	m_vDir = { 0.f,-1.f,0.f };
	
	for (int i = 0; i < m_fAttack_Num; i++) {

		m_Cri_List.emplace_back(Final_Damage());
	}

	


	__super::Ready_GameObject();

	return S_OK;
}

_int CTimeArrow::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;


	if (m_bHit) {
		Create_Hit_Effect();
		Create_Damage_Font();
		m_bDead = true;
		return OBJ_DEAD;
	}

	if (m_pTransformCom->m_vInfo[INFO_POS].y < 0.f) {
		Create_Hit_Effect();
		m_bDead = true;
	}

	
	m_AccTime += m_AccMaxTime * Time * 0.5f;

	m_fIdleFrame += 6.f * Time * 1.f;

	if (m_AccTime > m_AccMaxTime) {
		m_AccTime = 0.f;
		m_bDead = true;
	}

	if (m_fIdleFrame > 6.f) {
		m_fIdleFrame = 0.f;
	}

	m_pTransformCom->Move_Pos(&(m_vDir * Time * m_fSpeed));

	

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_ARROW, this);

	__super::Update_GameObject(Time);

	return 0;
}

void CTimeArrow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CTimeArrow::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	
	m_pTextureCom[ARROW_TYPE_TIME]->Set_Texture((_uint)m_fIdleFrame);
	
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTimeArrow::Add_Component(void)
{
	
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom[ARROW_TYPE_TIME] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Time_Skill_Arrow"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Time_Skill_Arrow", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	 
	m_pColliderCom->Set_Radius(8.f);
	m_pColliderCom->Set_Offset({ 0.f,-3.f,0.f });
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });


	return S_OK;
}


void CTimeArrow::SetUp_OnTerrain(void)
{
	

}

void CTimeArrow::Change_State(void)
{
	



}

void CTimeArrow::Frame_Check(const _float& fTimeDelta)
{
	
}

CTimeArrow* CTimeArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CTimeArrow* pInstance = new CTimeArrow(pGraphicDev);

	if (pInstance != nullptr) {

		pInstance->m_Fire_Pos = vPos;
	}

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

bool CTimeArrow::Final_Damage(void)
{

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	int CriticalRate = (int)(pPlayer->m_Critical_Rate);

	bool Critical;

	int Random = rand() % 100 + 1;

	if (Random <= CriticalRate) {
		Critical = true;
	}
	else {
		Critical = false;
	}

	m_fRandom_Value = (float)(rand() % (2 * (587 / 8 - 587 / 16 + 1)) - (587 / 8 - 587 / 16 + 1));

	m_fDamage = (m_fPower * 0.5f + m_fRandom_Value) * (1.f + 1.35f * Critical);

	m_Damage_List.emplace_back((int)m_fDamage);

	

	return Critical;

}

void CTimeArrow::Create_Hit_Effect(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;


	pGameObject = CTime_Hit::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;

	pGameLogicLayer->Add_BulletObject(pGameObject);



}

void CTimeArrow::Create_Damage_Font(void)
{

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	int k = 0;

	for (int i = 0; i < m_fAttack_Num; i++) {

		int j = 0;
		
		Create_Font_List(m_Damage_List.front());


		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TIME_BOW_HIT);
		CSoundMgr::Get_Instance()->PlaySound(L"Time_Hit.wav", CSoundMgr::TIME_BOW_HIT, 0.55f);


		for (auto iter : m_Font_List) {
			_vec3 pos = m_pTransformCom->m_vInfo[INFO_POS];

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


}

void CTimeArrow::Create_Font_List(int damage)
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

void CTimeArrow::Free(void)
{
	__super::Free();
}