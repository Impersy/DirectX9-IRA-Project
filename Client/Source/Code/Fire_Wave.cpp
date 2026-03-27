#include "stdafx.h"
#include "..\Header\Fire_Wave.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"
#include "Night_Circle.h"
#include "Effect_Player_Damage_Font.h"
#include "CollisionMgr.h"
#include "Fire_Hit.h"
#include <SoundMgr.h>



CFire_Wave::CFire_Wave(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CFire_Wave::CFire_Wave(const CFire_Wave & rhs)
	: CEffect(rhs)
{
}

CFire_Wave::~CFire_Wave()
{
	Free();
}

HRESULT CFire_Wave::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 3.f * 10.f , 3.f * 10.f, 1.f };

	m_iAlpha = 255;
	m_pTransformCom->m_vScale = Scale * PUBLIC_SCALE;

	m_fFrame = 0.f;
	m_fMaxFrame = 8.f;

	m_vPos.x -= 3.5f;
	m_vPos.y += 15.5f;
	m_vPos.z += -5.5f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;

	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(200.f));
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(-60.f));

	m_pColliderCom->Set_Radius(200.f);

	for (int i = 0; i < m_fAttack_Num; i++) {

		m_Cri_List.emplace_back(Final_Damage());
	}

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::KAFF_S);
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::KAFF_L);
	CSoundMgr::Get_Instance()->PlaySound(L"Kaff_Attack.wav", CSoundMgr::KAFF_L, 0.95f);

	return S_OK;
}

_int CFire_Wave::Update_GameObject(const _float& fTimeDelta)
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


	m_fFrame += m_fMaxFrame * Time * 1.f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_APOSTLE_COURAGE, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CFire_Wave::LateUpdate_GameObject()
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

void CFire_Wave::Render_GameObject()
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

HRESULT CFire_Wave::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Courage_Wave"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Courage_Wave", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	 
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

CFire_Wave* CFire_Wave::Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 pos)
{
	CFire_Wave* pInstance = new CFire_Wave(pGraphicDev);

	pInstance->m_vPos = pos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

bool CFire_Wave::Final_Damage(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));

	int CriticalRate = (int)(pPlayer->m_Critical_Rate);

	bool Critical = true;


	m_fRandom_Value = (float)(rand() % (2 * (587 / 8 - 587 / 16 + 1)) - (587 / 8 - 587 / 16 + 1));

	m_fDamage = (m_fPower * 0.5f + m_fRandom_Value) * (1.f + 1.35f * Critical);

	m_Damage_List.emplace_back((int)m_fDamage);



	return Critical;

}

void CFire_Wave::Create_Hit_Effect(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	//CGameObject* pGameObject;

	/*pGameObject = CEffect_Player_Arrow_Hit::Create(m_pGraphicDev, ARROW_TYPE_SYLPH, m_pTransformCom->m_vInfo[INFO_POS]);

	if (pGameObject == nullptr)
		return;


	pGameLogicLayer->Add_BulletObject(pGameObject);*/
}

void CFire_Wave::Create_Damage_Font(void)
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

				dynamic_cast<CEffect_Player_Damage_Font*>(pGameObject)->m_Is_Fire = true;

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

		pGameObject = CFire_Hit::Create(m_pGraphicDev, pos , false);

		if (pGameObject == nullptr)
			return;

		pGameLogicLayer->Add_BulletObject(pGameObject);

	}

	TargetPos_List.clear();
	TargetPos_List.resize(0);

}

void CFire_Wave::Create_Font_List(int damage)
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

void CFire_Wave::Free(void)
{
	__super::Free();
}


