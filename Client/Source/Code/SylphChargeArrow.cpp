#include "stdafx.h"
#include "..\Header\SylphChargeArrow.h"
#include "Export_Function.h"
#include "Effect_Player_Arrow_Hit.h"
#include "Player.h"
#include "Effect_Player_Damage_Font.h"
#include "CollisionMgr.h"
#include "Fire_Hit.h"
#include "Night_Hit.h"
#include "Night_Big.h"
#include "Fire_Big.h"
#include "Time_Big.h"
#include "Time_Hit.h"
#include "Effect_AlertCircle_Player.h"
#include <SoundMgr.h>


CSylphChargeArrow::CSylphChargeArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CArrow(pGraphicDev)
{
}

CSylphChargeArrow::CSylphChargeArrow(const CSylphChargeArrow& rhs)
	: CArrow(rhs)
{
}

CSylphChargeArrow::~CSylphChargeArrow()
{
	Free();
}

HRESULT CSylphChargeArrow::Ready_GameObject(void)
{


	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	ARROW_TYPE type = pPlayer->m_Arrow_Type;

	if (type == ARROW_TYPE_SYLPH)
		m_Arrow_Type = ARROW_TYPE_CHARGE_SYLPH;
	else if(type == ARROW_TYPE_FIRE)
		m_Arrow_Type = ARROW_TYPE_FIRE_CHARGE;
	else if (type == ARROW_TYPE_NIGHT)
		m_Arrow_Type = ARROW_TYPE_NIGHT_CHARGE;
	else if (type == ARROW_TYPE_TIME)
		m_Arrow_Type = ARROW_TYPE_TIME_CHARGE;

	if (m_Arrow_Type == ARROW_TYPE_CHARGE_SYLPH) {
		m_Dis = 5.f;
		m_Size = { 3.f * PUBLIC_SCALE, 3.f * PUBLIC_SCALE, 1.f };
		m_Size *= 2.f;
		m_fPower = 4170.f;
	}
	else if (m_Arrow_Type == ARROW_TYPE_FIRE_CHARGE) {
		m_Dis = 5.f;
		m_Size = { 3.5f * PUBLIC_SCALE, 2.f * PUBLIC_SCALE, 1.f };
		m_Size *= 2.f;
		m_fPower = 9568.f;
	}
	else if (m_Arrow_Type == ARROW_TYPE_NIGHT_CHARGE) {
		m_Dis = 6.f;
		m_Size = { 3.f * PUBLIC_SCALE, 1.f * PUBLIC_SCALE, 1.f };
		m_Size *= 2.f;
		m_fPower = 12170.f;
	}
	else if (m_Arrow_Type == ARROW_TYPE_TIME_CHARGE) {
		m_Dis = 5.f;
		m_Size = { 3.f * PUBLIC_SCALE, 1.5f * PUBLIC_SCALE, 1.f };
		m_Size *= 2.f;
		m_fPower = 13560.f;
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

		m_Cri_List.push_back(Final_Damage());
	}
	
	__super::Ready_GameObject();

	return S_OK;
}

_int CSylphChargeArrow::Update_GameObject(const _float& fTimeDelta)
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

	if (m_Arrow_Type == ARROW_TYPE_NIGHT_CHARGE) {


		if (m_AccTime > 1.f && m_AccTime < 1.1f) {
			CSoundMgr::Get_Instance()->PlaySound(L"Light_Charge_Shot.wav", CSoundMgr::NIGHT_BIG_HIT, 0.95f);
		}

		if (m_AccTime > 4.f && m_AccTime < 4.1f) {
			CSoundMgr::Get_Instance()->PlaySound(L"Light_Charge_Shot.wav", CSoundMgr::NIGHT_BIG_HIT2, 0.85f);
		}

		if (m_AccTime > 7.f && m_AccTime < 7.1f) {
			CSoundMgr::Get_Instance()->PlaySound(L"Light_Charge_Shot.wav", CSoundMgr::NIGHT_BIG_HIT3, 0.85f);
		}

		

		m_Alert_Time += 2.f * Time * 6.f;

		if (m_Alert_Time > 2.f) {
			m_Alert_Time = 0.f;
			m_Is_Alter = true;
		}

		if (m_Is_Alter == true) {
			CGameObject* pGameObject;

			pGameObject = CEffect_AlertCircle_Player::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], { 8.f,8.f,1.f }, 2.f, 0.f, false);

			if (pGameObject == nullptr)
				return 0;

			pGameLogicLayer->Add_BulletObject(pGameObject);
			m_Is_Alter = false;
		}

	}

	
	m_pTransformCom->Move_Pos(&(m_vDir * Time * m_fSpeed));

	
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	if (m_Arrow_Type != ARROW_TYPE_NIGHT_CHARGE)
		CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_ARROW, this);

	__super::Update_GameObject(Time);

	return 0;
}

void CSylphChargeArrow::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CSylphChargeArrow::Render_GameObject()
{
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	
	if (m_Arrow_Type == ARROW_TYPE_CHARGE_SYLPH) {
		m_pTextureCom[ARROW_TYPE_CHARGE_SYLPH]->Set_Texture(0);
	}
	else if (m_Arrow_Type == ARROW_TYPE_FIRE_CHARGE) {
		m_pTextureCom[ARROW_TYPE_FIRE_CHARGE]->Set_Texture(0);
	}
	else if (m_Arrow_Type == ARROW_TYPE_NIGHT_CHARGE) {
		m_pTextureCom[ARROW_TYPE_NIGHT_CHARGE]->Set_Texture(0);
	}
	else if (m_Arrow_Type == ARROW_TYPE_TIME_CHARGE) {
		m_pTextureCom[ARROW_TYPE_TIME_CHARGE]->Set_Texture(0);
	}
	


	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CSylphChargeArrow::Add_Component(void)
{
	
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom[ARROW_TYPE_CHARGE_SYLPH] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Arrow_Sylph_Idle"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Arrow_Sylph_Idle", pComponent });

	pComponent = m_pTextureCom[ARROW_TYPE_FIRE_CHARGE] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Arrow_Fire"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Arrow_Fire", pComponent });

	pComponent = m_pTextureCom[ARROW_TYPE_NIGHT_CHARGE] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Arrow_Night"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Arrow_Night", pComponent });

	pComponent = m_pTextureCom[ARROW_TYPE_TIME_CHARGE] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Player_Arrow_Time"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Player_Arrow_Time", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	 
	m_pColliderCom->Set_Radius(6.f);
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}


void CSylphChargeArrow::SetUp_OnTerrain(void)
{
	

}

void CSylphChargeArrow::Change_State(void)
{
	



}

void CSylphChargeArrow::Frame_Check(const _float& fTimeDelta)
{
	
}

CSylphChargeArrow* CSylphChargeArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir,_float Angle)
{
	CSylphChargeArrow* pInstance = new CSylphChargeArrow(pGraphicDev);

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

void CSylphChargeArrow::Create_Hit_Effect(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;


	if (m_Arrow_Type == ARROW_TYPE_FIRE_CHARGE) {
		pGameObject = CFire_Big::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject == nullptr)
			return;

		pGameLogicLayer->Add_BulletObject(pGameObject);
	}
	else if (m_Arrow_Type == ARROW_TYPE_NIGHT_CHARGE) {
		
	}
	else if (m_Arrow_Type == ARROW_TYPE_TIME_CHARGE) {


		pGameObject = CTime_Hit::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject == nullptr)
			return;

		dynamic_cast<CTime_Hit*>(pGameObject)->m_Is_Charge = true;

		pGameLogicLayer->Add_BulletObject(pGameObject);



		CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

		CGameObject* pGameObject = nullptr;


		_vec3 Ram;

		for (int i = 0; i < 10; i++) {
			_vec3 pos = m_pTransformCom->m_vInfo[INFO_POS];

			if (g_eCurScene == SCENE_BOSS1) {

				pos.y += 23.f;
			}
			else if (g_eCurScene == SCENE_BOSS2) {
				pos.y += 28.f;
				pos.z += 5.f;
			}
			else {
				pos.y += 20.f;
			}
			

			Ram.x = 5.f * (_float)(rand() % 10 - 5);
			Ram.y = 2.f * (_float)(rand() % 10 + 1);
			Ram.z = 5.f * (_float)(rand() % 10 - 5);

			pos += Ram;

			pGameObject = CTime_Big::Create(m_pGraphicDev, pos);

			if (pGameObject == nullptr)
				return;

			pGameLogicLayer->Add_BulletObject(pGameObject);


		}
	}
	else {
		pGameObject = CEffect_Player_Arrow_Hit::Create(m_pGraphicDev, ARROW_TYPE_CHARGE_SYLPH, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject == nullptr)
			return;

		pGameLogicLayer->Add_BulletObject(pGameObject);
	}


	

	
}

bool CSylphChargeArrow::Final_Damage(void)
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

	m_Damage_List.push_back((int)m_fDamage);

	
	return Critical;
}

void CSylphChargeArrow::Create_Damage_Font(void)
{
	
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	CGameObject* pGameObject;

	int k = 0;


	for (int i = 0; i < m_fAttack_Num; i++) {

		int j = 0;

		Create_Font_List(m_Damage_List.front());


		if (m_Arrow_Type == ARROW_TYPE_CHARGE_SYLPH) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ARROW_NORMAL_HIT);
			CSoundMgr::Get_Instance()->PlaySound(L"Normal_Hit.wav", CSoundMgr::ARROW_NORMAL_HIT, 0.75f);
			
		}
		else if (m_Arrow_Type == ARROW_TYPE_FIRE_CHARGE) {
			// Â÷Áö Hit FIRE_BIG_HIT
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::FIRE_BOW_HIT);
			CSoundMgr::Get_Instance()->PlaySound(L"Fire_Charge_Hit.wav", CSoundMgr::FIRE_BOW_HIT, 0.15f);

		}
		else if (m_Arrow_Type == ARROW_TYPE_NIGHT_CHARGE) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::NIGHT_BOW_HIT);
			CSoundMgr::Get_Instance()->PlaySound(L"LightningBow_Hit.wav", CSoundMgr::NIGHT_BOW_HIT, 0.75f);

		}
		else if (m_Arrow_Type == ARROW_TYPE_TIME_CHARGE) {
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::TIME_BOW_HIT);
			CSoundMgr::Get_Instance()->PlaySound(L"Time_Hit.wav", CSoundMgr::TIME_BOW_HIT, 0.85f);

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

void CSylphChargeArrow::Create_Font_List(int damage)
{
	int temp = damage;
	int cnt = 0;

	while (temp > 0) {
		temp /= 10;
		cnt++;
	}

	temp = damage;

	for (int i = 0; i < cnt; i++) {

		if (i == cnt - 1) {
			m_Font_List.emplace_front(temp);
			break;
		}

		m_Font_List.emplace_front(temp % 10);
		temp /= 10;

	}
}

void CSylphChargeArrow::Free(void)
{
	__super::Free();
}