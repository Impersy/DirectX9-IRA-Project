#include "stdafx.h"
#include "..\Header\Item_Coin.h"
#include "Export_Function.h"

#include "Player.h"

#include "Effect_Coin_Drop.h"
#include "Effect_Coin_Ground.h"
#include "SoundMgr.h"

CItem_Coin::CItem_Coin(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev), m_bTrace(false), m_vCoinPos(nullptr)
{
}

CItem_Coin::CItem_Coin(const CItem_Coin& rhs)
	: CItem(rhs), m_bTrace(false), m_vCoinPos(rhs.m_vCoinPos)
{
}

CItem_Coin::~CItem_Coin()
{
	Free();

	//if (m_vCoinPos)
	//{
	//	delete m_vCoinPos;
	//	m_vCoinPos = nullptr;
	//}
}


HRESULT CItem_Coin::Ready_GameObject(_bool bIsEffectOn)
{
	Add_Component();

	m_bIsEffectOn = bIsEffectOn;

	m_pTransformCom->Set_Pos(m_vMasterPos.x, m_vMasterPos.y, m_vMasterPos.z);
	m_pTransformCom->UpdatePos_OnWorld();
	m_pTransformCom->Set_Scale(0.8f, 0.8f, 1.f);
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(15.f));

	m_vCoinPos = new _vec3();
	*m_vCoinPos = m_vMasterPos;
	return S_OK;
}

_int CItem_Coin::Update_GameObject(const _float& fTimeDelta)
{	
	if (!m_bDropOn && m_bIsEffectOn)
	{
		Create_Drop_Effect();
		m_bDropOn = true;
	}

	*m_vCoinPos = m_vMasterPos;

	if (m_bDropEffect_Off && (!m_bGroundEffect_On))
	{	
		Create_Ground_Effect();
		m_bGroundEffect_On = true;
	}

	if (!m_bIsEffectOn && !m_bGroundEffect_On)
	{
		Create_Ground_Effect();
		m_bGroundEffect_On = true;
	}

	if (m_fBounceCount == 2)
	{
		Trace_Check();
		Trace_Player(fTimeDelta);
		m_pTransformCom->Set_Pos(m_vMasterPos.x, m_vMasterPos.y, m_vMasterPos.z);
		m_pTransformCom->UpdatePos_OnWorld();
	}

	if (m_bDead)
	{
		_int iTemp_Plus_Coin = (_int)(rand() % 20);
		_int iTemp_Plus_Gem = (_int)(rand() % 10);

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
		pPlayer->Set_Player_Money(MONEY_INCREASE, iTemp_Plus_Coin);
		pPlayer->Set_Player_Gem(GEM_INCREASE, iTemp_Plus_Gem);

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ITEM_COIN);
		CSoundMgr::Get_Instance()->PlaySound(L"SoulGet.wav", CSoundMgr::ITEM_COIN, 0.8f);

		return OBJ_DEAD;
	}

	Angle_Update();

	if (m_vMasterPos.y >= 3.f && (m_bTrace == false))
	{
		Update_Speed(fTimeDelta);

		if (m_fJumpTime > 0)
		{
			m_vMasterPos.x += (m_DirectionRand.x * m_fJumpPower * fTimeDelta);
			m_vMasterPos.y += m_fJumpPower * fTimeDelta;
			m_vMasterPos.z += (m_DirectionRand.z * m_fJumpPower * fTimeDelta);
			m_fJumpTime -= fTimeDelta;

		}
		else
		{
			m_vVelocity += m_vAcceleration * fTimeDelta;
			m_vMasterPos += (m_vVelocity * fTimeDelta) * 0.1;

			if ((m_vMasterPos.y <= 3.f) && (m_fBounceCount == 0))
			{
				m_vMasterPos.y = 3.f;
				m_vVelocity.y = abs(m_vVelocity.y) * m_fBounce;
				m_fJumpTime = m_fMaxJumpTime;
				m_fBounceCount = 1;
			}
			else if ((m_vMasterPos.y <= 3) && (m_fBounceCount == 1))
			{
				m_vMasterPos.y = 3.f;
				m_vVelocity.y = 0;
				m_fBounceCount = 2;
			}
		}
		m_pTransformCom->Set_Pos(m_vMasterPos.x, m_vMasterPos.y, m_vMasterPos.z);
		m_pTransformCom->UpdatePos_OnWorld();
	}

	Frame_Check(fTimeDelta);
	SetUp_OnTerrain();
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return OBJ_NOEVENT;
}

void CItem_Coin::LateUpdate_GameObject()
{	
	__super::LateUpdate_GameObject();
}

void CItem_Coin::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CItem_Coin::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Item_Coin"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Item_Coin", pComponent });

	return S_OK;
}

void CItem_Coin::SetUp_OnTerrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", L"Proto_TerrainTex", ID_STATIC));
	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
}

void CItem_Coin::Change_State(void)
{

}

void CItem_Coin::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += 4.f * fTimeDelta;

	if (4.f < m_fFrame)
	{
		m_fFrame = 0.f;
		m_bCheck = false;
	}

	if (m_bDropEffect_On)
	{
		m_fEffectFrame += 8.f * fTimeDelta;

		if (m_fEffectFrame > 8.f)
		{
			m_bDropEffect_Off = true;
		}
	}
}

void CItem_Coin::Trace_Check(void)
{
	// 플레이어 Pos 받아 MasterPos와 구충돌
	_vec3 vPlayerPos;
	CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	vPlayerPos;

	_float x = vPlayerPos.x;
	_float z = vPlayerPos.z;
	_float radius1 = 6.0f;

	_float x1 = m_vMasterPos.x;
	_float z1 = m_vMasterPos.z;
	_float radius2 = 35.f;

	_float dx = x - x1;
	_float dz = z - z1;
	_float distance = sqrt(dx * dx + dz * dz);

	if (distance < radius1 + radius2)
		m_bTrace = true;

	else
		m_bTrace = false;
}

void CItem_Coin::Trace_Player(const _float& fTimeDelta)
{
	if (m_bTrace)
	{
		m_vAccel_Trace += (m_fDrag * 0.1f);

		_vec3	vPlayerPos;
		CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
		pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

		_vec3		vDir = vPlayerPos - m_vMasterPos;

		if (!(((vDir.x > -2.0f) && (vDir.x < 2.0f)) && ((vDir.z > -2.0f) && (vDir.z < 2.0f))))
		{
			m_vMasterPos += *D3DXVec3Normalize(&vDir, &vDir) * (m_fSpeed + m_vAccel_Trace) * fTimeDelta * 2;
			m_vMasterPos.y = 5.f;
		}
		else
		{
			m_bDead = true;
			*m_vCoinPos = { 0.f, 0.f, 0.f };
		}
	}
}


CItem_Coin* CItem_Coin::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _bool bIsEffectOn)
{
	CItem_Coin* pInstance = new CItem_Coin(pGraphicDev);

	pInstance->m_vMasterPos = vPos;

	if (FAILED(pInstance->Ready_GameObject(bIsEffectOn)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CItem_Coin::Create_Drop_Effect(void)
{
	////TIME_STOP_VOID

	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CGameObject* pGameObject = nullptr;
	pGameObject = CEffect_Coin_Drop::Create(m_pGraphicDev, m_vCoinPos);
	if (pGameObject == nullptr)
		return;
	pGameLogicLayer->Add_BulletObject(pGameObject);

	m_bDropEffect_On = true;

}

void CItem_Coin::Create_Ground_Effect(void)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	CGameObject* pGameObject = nullptr;
	pGameObject = CEffect_Coin_Ground::Create(m_pGraphicDev, m_vCoinPos);
	if (pGameObject == nullptr)
		return;
	pGameLogicLayer->Add_BulletObject(pGameObject);
}

void CItem_Coin::Angle_Update(void)
{	
	if (m_bAngleOn == false)
	{
		_float m_fAngle_X = (float)rand() / RAND_MAX * D3DX_PI * 2.0f;
		_float m_fAngle_Y = (float)rand() / RAND_MAX * D3DX_PI * 2.0f;
		m_DirectionRand = { sin(m_fAngle_X), 0.0f, cos(m_fAngle_Y) };
		m_bAngleOn = true;
	}
}

void CItem_Coin::Update_Speed(const _float& fTimeDelta)
{	
	m_vVelocity += m_DirectionRand * m_fSpeed * fTimeDelta;
	m_vAcceleration += -m_vVelocity * m_fDrag + m_vGravity;
}

void CItem_Coin::Free(void)
{
	__super::Free();
}