#include "stdafx.h"
#include "..\Header\Effect_AlertCircle_Player.h"
#include "Export_Function.h"
#include "Night_Big.h"
#include "CollisionMgr.h"
#include <SoundMgr.h>

CEffect_AlertCircle_Player::CEffect_AlertCircle_Player(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_AlertCircle_Player::CEffect_AlertCircle_Player(const CEffect_AlertCircle_Player& rhs)
	: CEffect(rhs)
{
}

CEffect_AlertCircle_Player::~CEffect_AlertCircle_Player()
{
	Free();
}

HRESULT CEffect_AlertCircle_Player::Ready_GameObject(const _vec3& vPos, const _vec3& vScale, const _float& fAliveTime, const _float& fTraceTime, _bool bSpreadMode)
{
	Add_Component();


	m_pTransformCom->m_vInfo[INFO_POS] = vPos;
	m_pTransformCom->m_vInfo[INFO_POS].y = 0.1f;
	m_vMaxScale = vScale;
	m_fAliveTime = fAliveTime;
	m_fTraceTime = fTraceTime;
	m_bSpreadMode = bSpreadMode;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	return S_OK;
}

_int CEffect_AlertCircle_Player::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	m_fAccTime += Time;

	if (m_Thunder_Time >= 0.f)
		m_Thunder_Time += Time;

	if (m_bDead)
		return OBJ_DEAD;

	if (m_bSpreadMode)
	{
		m_pTransformCom->m_vScale.x += 0.3f * Time * 100.f;
		m_pTransformCom->m_vScale.y += 0.3f * Time * 100.f;
		m_pTransformCom->m_vScale.z += 0.3f * Time * 100.f;

		if (m_pTransformCom->m_vScale.x >= m_vMaxScale.x) m_pTransformCom->m_vScale.x = m_vMaxScale.x;
		if (m_pTransformCom->m_vScale.y >= m_vMaxScale.y) m_pTransformCom->m_vScale.y = m_vMaxScale.y;
		if (m_pTransformCom->m_vScale.z >= m_vMaxScale.z) m_pTransformCom->m_vScale.z = m_vMaxScale.z;
	}
	else
		m_pTransformCom->m_vScale = m_vMaxScale;

	if (m_Thunder_Time > 0.1f) {
		m_Thunder = true;
		m_Thunder_Time = -1.f;
	}

	if (m_Thunder) {
		
		CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

		CGameObject* pGameObject;

		pGameObject = CNight_Big::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS]);

		if (pGameObject == nullptr)
			return 0;

		pGameLogicLayer->Add_BulletObject(pGameObject);

		m_Thunder = false;
	}




	__super::Update_GameObject(Time);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);


	return 0;
}

void CEffect_AlertCircle_Player::LateUpdate_GameObject()
{
	

	if (m_fAccTime > m_fAliveTime)
		m_bDead = true;

	__super::LateUpdate_GameObject();

}

void CEffect_AlertCircle_Player::Render_GameObject()
{
	if (m_bRender == false)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, m_R, m_G, m_B));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);


	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);



	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CEffect_AlertCircle_Player::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_AlertCircle"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_AlertCircle", pComponent });

	
	return S_OK;
}

CEffect_AlertCircle_Player* CEffect_AlertCircle_Player::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vScale, const _float& fAliveTime, const _float& fTraceTime, _bool bSpreadMode)
{
	CEffect_AlertCircle_Player* pInstance = new CEffect_AlertCircle_Player(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, vScale, fAliveTime, fTraceTime, bSpreadMode)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_AlertCircle_Player::Free(void)
{
	__super::Free();
}


