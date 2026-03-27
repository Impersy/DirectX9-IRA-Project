#include "stdafx.h"
#include "..\Header\SylphArrow.h"
#include "Export_Function.h"
#include "Effect_Player_Arrow_Hit.h"
#include "Effect_Player_Damage_Font.h"
#include "Player.h"
#include "CollisionMgr.h"
#include "Fire_Hit.h"
#include "Night_Hit.h"
#include "Time_Hit.h"
#include <SoundMgr.h>

CSylphArrow::CSylphArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CArrow(pGraphicDev)
{
}

CSylphArrow::CSylphArrow(const CSylphArrow& rhs)
	: CArrow(rhs)
{
}

CSylphArrow::~CSylphArrow()
{
	Free();
}

HRESULT CSylphArrow::Ready_GameObject(void)
{

	

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	m_Arrow_Type = pPlayer->m_Arrow_Type;



	if (m_Arrow_Type == ARROW_TYPE_SYLPH) {
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOW_FIRE);
		CSoundMgr::Get_Instance()->PlaySound(L"Bow_Fire.wav", CSoundMgr::BOW_FIRE, 0.35f);
		m_Dis = 5.f;
		m_Size = { 3.f * PUBLIC_SCALE, 3.f * PUBLIC_SCALE, 1.f };
		m_fPower = 1872.f;
	}
	else if (m_Arrow_Type == ARROW_TYPE_FIRE) {
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::FIRE_BOW_FIRE);
		CSoundMgr::Get_Instance()->PlaySound(L"FireBow_Fire.wav", CSoundMgr::FIRE_BOW_FIRE, 0.15f);
		m_Dis = 5.f;
		m_Size = { 3.5f * PUBLIC_SCALE, 2.f * PUBLIC_SCALE, 1.f };
		m_fPower = 3753.f;
	}
	else if (m_Arrow_Type == ARROW_TYPE_NIGHT) {
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::NIGHT_BOW_FIRE);
		CSoundMgr::Get_Instance()->PlaySound(L"LightningBow_Fire.wav", CSoundMgr::NIGHT_BOW_FIRE, 0.35f);
		m_Dis = 6.f;
		m_Size = { 3.f * PUBLIC_SCALE, 1.f * PUBLIC_SCALE, 1.f };
		m_fPower = 4816.f;
	}
	else if (m_Arrow_Type == ARROW_TYPE_TIME) {
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TIME_BOW_FIRE);
		CSoundMgr::Get_Instance()->PlaySound(L"Time_Fire.wav", CSoundMgr::TIME_BOW_FIRE, 0.55f);
		m_Dis = 5.f;
		m_Size = { 3.f * PUBLIC_SCALE, 1.5f * PUBLIC_SCALE, 1.f };
		m_fPower = 3547.f;
	}



	Add_Component();

	m_fSpeed = 90.f * PUBLIC_SCALE;

	m_pTransformCom->Set_Scale_Ratio(m_Size);

	m_pTransformCom->Rotation(ROT_Y, m_Arrow_Angle);

	m_vDir = m_target_Dir;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	_vec3 Dir;
	D3DXVec3Normalize(&Dir, &m_vDir);

	m_Fire_Pos += Dir * m_Dis;

	m_pTransformCom->Set_Pos(m_Fire_Pos.x, m_Fire_Pos.y - 2.f, m_Fire_Pos.z);

	

	for (int i = 0; i < m_fAttack_Num; i++) {

		m_Cri_List.emplace_back(Final_Damage());
	}

	
	__super::Ready_GameObject();

	return S_OK;
}

_int CSylphArrow::Update_GameObject(const _float& fTimeDelta)
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

	
	m_AccTime += m_AccMaxTime * Time * 0.5f;

	if (m_AccTime > m_AccMaxTime) {
		m_AccTime = 0.f;
		m_bDead = true;
	}

	m_pTransformCom->Move_Pos(&(m_vDir * Time * m_fSpeed));

	

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_ARROW, this);

	__super::Update_GameObject(Time);

	return 0;
}

void CSylphArrow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CSylphArrow::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	if (m_Arrow_Type == ARROW_TYPE_SYLPH) {
		m_pTextureCom[ARROW_TYPE_SYLPH]->Set_Texture(0);
	}
	else if (m_Arrow_Type == ARROW_TYPE_FIRE) {
		m_pTextureCom[ARROW_TYPE_FIRE]->Set_Texture(0);
	}
	else if (m_Arrow_Type == ARROW_TYPE_NIGHT) {
		m_pTextureCom[ARROW_TYPE_NIGHT]->Set_Texture(0);
	}
	else if (m_Arrow_Type == ARROW_TYPE_TIME) {
		m_pTextureCom[ARROW_TYPE_TIME]->Set_Texture(0);
	}
	


	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSylphArrow::Add_Component(void)
{
	
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom[ARROW_TYPE_SYLPH] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Arrow_Sylph_Idle"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Arrow_Sylph_Idle", pComponent });

	pComponent = m_pTextureCom[ARROW_TYPE_FIRE] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Arrow_Fire"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Arrow_Fire", pComponent });

	pComponent = m_pTextureCom[ARROW_TYPE_NIGHT] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Arrow_Night"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Arrow_Night", pComponent });

	pComponent = m_pTextureCom[ARROW_TYPE_TIME] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Arrow_Time"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Arrow_Time", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	 
	m_pColliderCom->Set_Radius(3.f);
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });


	return S_OK;
}


void CSylphArrow::SetUp_OnTerrain(void)
{
	

}

void CSylphArrow::Change_State(void)
{
	



}

void CSylphArrow::Frame_Check(const _float& fTimeDelta)
{
	
}

CSylphArrow* CSylphArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir,_float Angle)
{
	CSylphArrow* pInstance = new CSylphArrow(pGraphicDev);

	if (pInstance != nullptr) {
		pInstance->m_target_Dir = vDir;
		pInstance->m_Fire_Pos = vPos;
		pInstance->m_Arrow_Angle = Angle;
	}

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}


	return pInstance;
}

bool CSylphArrow::Final_Damage(void)
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

void CSylphArrow::Create_Hit_Effect(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	if (m_Arrow_Type == ARROW_TYPE_FIRE) {
		pGameObject = CFire_Hit::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], true);

		if (pGameObject == nullptr)
			return;

		pGameLogicLayer->Add_BulletObject(pGameObject);
	}
	else if (m_Arrow_Type == ARROW_TYPE_NIGHT) {
		pGameObject = CNight_Hit::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject == nullptr)
			return;

		pGameLogicLayer->Add_BulletObject(pGameObject);
	}
	else if (m_Arrow_Type == ARROW_TYPE_TIME) {

		pGameObject = CTime_Hit::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject == nullptr)
			return;


		pGameLogicLayer->Add_BulletObject(pGameObject);


	}
	else {
		pGameObject = CEffect_Player_Arrow_Hit::Create(m_pGraphicDev, ARROW_TYPE_SYLPH, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject == nullptr)
			return;

		pGameLogicLayer->Add_BulletObject(pGameObject);
	}

	
}

void CSylphArrow::Create_Damage_Font(void)
{

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	int k = 0;

	for (int i = 0; i < m_fAttack_Num; i++) {

		int j = 0;
		

		Create_Font_List(m_Damage_List.front());

		if (m_Arrow_Type == ARROW_TYPE_SYLPH) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ARROW_NORMAL_HIT);
			CSoundMgr::Get_Instance()->PlaySound(L"Normal_Hit.wav", CSoundMgr::ARROW_NORMAL_HIT, 0.55f);
		}
		else if (m_Arrow_Type == ARROW_TYPE_FIRE) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::FIRE_BOW_HIT);
			CSoundMgr::Get_Instance()->PlaySound(L"FireBow_Hit.wav", CSoundMgr::FIRE_BOW_HIT, 0.55f);
		}
		else if (m_Arrow_Type == ARROW_TYPE_NIGHT) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::NIGHT_BOW_HIT);
			CSoundMgr::Get_Instance()->PlaySound(L"LightningBow_Hit.wav", CSoundMgr::NIGHT_BOW_HIT, 0.55f);
		}
		else if (m_Arrow_Type == ARROW_TYPE_TIME) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TIME_BOW_HIT);
			CSoundMgr::Get_Instance()->PlaySound(L"Time_Hit.wav", CSoundMgr::TIME_BOW_HIT, 0.55f);
		}
		
		
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

void CSylphArrow::Create_Font_List(int damage)
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

void CSylphArrow::Free(void)
{



	__super::Free();
}