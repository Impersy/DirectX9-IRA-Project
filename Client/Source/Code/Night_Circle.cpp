#include "stdafx.h"
#include "..\Header\Night_Circle.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"
#include "Night_Hit.h"
#include "CollisionMgr.h"
#include "Effect_Player_Damage_Font.h"
#include <SoundMgr.h>


CNight_Circle::CNight_Circle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CNight_Circle::CNight_Circle(const CNight_Circle & rhs)
	: CEffect(rhs)
{
}

CNight_Circle::~CNight_Circle()
{
	Free();
}

HRESULT CNight_Circle::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 6.4f * 5.f , 4.8f * 5.f, 1.f };

	m_iAlpha = 50;
	m_pTransformCom->m_vScale = 5 * Scale * PUBLIC_SCALE;

	m_fFrame = 0.f;
	m_fMaxFrame = 150.f;
	

	m_vPos.x -= 3.f;
	m_vPos.y = 1.f;
	m_vPos.z += 8.f;
	m_vPos.z += -2.f;


	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	m_pColliderCom->Set_Radius(100.f);


	for (int i = 0; i < m_fAttack_Num; i++) {

		m_Cri_List.emplace_back(Final_Damage());
	}

	

	return S_OK;
}

_int CNight_Circle::Update_GameObject(const _float& fTimeDelta)
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


	if (m_fAccTime <= 4.f) {

		m_iAlpha += 7;

		if (m_iAlpha > 255) {
			m_iAlpha = 255;
		}
	}

	m_fFrame += m_fMaxFrame * Time * 0.2f;

	if (m_fFrame > m_fMaxFrame)
	{
		m_fFrame = 0.f;
	}

	m_fAccTime += 4.f * Time * 0.25f;

	if (m_fAccTime > 4.f) {
		m_iAlpha -= 10;
	}

	if (m_iAlpha < 20) {
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SAW_S);
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SAW_E);
		CSoundMgr::Get_Instance()->PlaySound(L"So_Disappear.wav", CSoundMgr::SAW_E, 0.95f);
	}


	if (m_iAlpha < 0) {
		return OBJ_DEAD;
	}


	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_APOSTLE_NIGHT, this);

	__super::Update_GameObject(Time);

	return 0;
}

void CNight_Circle::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CNight_Circle::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	m_pTextureCom->Set_Texture((_uint)m_fFrame);
	

	m_pBufferCom->Render_Buffer();


	
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CNight_Circle::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Night_Circle"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Night_Circle", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	 
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

CNight_Circle* CNight_Circle::Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 pos, bool  m_Is_Boss2_Apostle)
{
	CNight_Circle* pInstance = new CNight_Circle(pGraphicDev);

	pInstance->m_vPos = pos;
	pInstance->m_Is_Boss2_Apostle = m_Is_Boss2_Apostle;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

bool CNight_Circle::Final_Damage(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	int CriticalRate = (int)(pPlayer->m_Critical_Rate);

	bool Critical = true;


	m_fRandom_Value = (float)(rand() % (2 * (587 / 8 - 587 / 16 + 1)) - (587 / 8 - 587 / 16 + 1));

	m_fDamage = (m_fPower * 0.5f + m_fRandom_Value) * (1.f + 1.35f * Critical);

	m_Damage_List.emplace_back((int)m_fDamage);



	return Critical;

}

void CNight_Circle::Create_Hit_Effect(void)
{
	
	
}

void CNight_Circle::Create_Damage_Font(void)
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
				if (g_eCurScene == SCENE_BOSS2 || g_eCurScene == SCENE_BOSS1)
					pos.y -= 10.f;

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


		// 타격 이펙트 추가
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SAW_HIT);
		CSoundMgr::Get_Instance()->PlaySound(L"So_Lighting.wav", CSoundMgr::SAW_HIT, 0.75f);

		_vec3 pos = TargetPos_List[p];
		pos.z -= 1.f;

		pGameObject = CNight_Hit::Create(m_pGraphicDev, pos);

		if (pGameObject == nullptr)
			return;

		if (m_Is_Boss2_Apostle == true)
			dynamic_cast<CNight_Hit*>(pGameObject)->m_Is_Boss2_Apostle = true;


		pGameLogicLayer->Add_BulletObject(pGameObject);


	}

	TargetPos_List.clear();
	TargetPos_List.resize(0);

}

void CNight_Circle::Create_Font_List(int damage)
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

void CNight_Circle::Free(void)
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::SAW_L);
	__super::Free();
}


