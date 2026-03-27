#include "stdafx.h"
#include "..\Header\Effect_Player_Foot.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"


CEffect_Player_Foot::CEffect_Player_Foot(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Player_Foot::CEffect_Player_Foot(const CEffect_Player_Foot & rhs)
	: CEffect(rhs)
{
}

CEffect_Player_Foot::~CEffect_Player_Foot()
{
	Free();
}

HRESULT CEffect_Player_Foot::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);



	if (m_Foot_Type == FOOT_TYPE_WATER) {
		m_iAlpha = 50;
		_vec3 Scale = { 1.68f , 0.94f, 1.f };
		m_pTransformCom->m_vScale = Scale * 1.f * PUBLIC_SCALE;
		m_fMaxFrame = 16.f;
		m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y - 4.5f * PUBLIC_SCALE, m_vPos.z - 1.5f * PUBLIC_SCALE);
	}
	else {
		_vec3 Scale = { 1.f , 1.f, 1.f };

		if (m_Is_Reverse) {
			Scale = { -1.f , 1.f, 1.f };
		}
		else {
			Scale = { 1.f , 1.f, 1.f };
		}

		m_iAlpha = 150;
		m_pTransformCom->m_vScale = Scale * 1.3f * PUBLIC_SCALE;
		m_fMaxFrame = 13.f;
		m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y - 4.0f * PUBLIC_SCALE, m_vPos.z);
	}
	


	return S_OK;
}

_int CEffect_Player_Foot::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;


	
	m_fFrame += m_fMaxFrame * fTimeDelta * 3.f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CEffect_Player_Foot::LateUpdate_GameObject()
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

void CEffect_Player_Foot::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/*m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, false);*/
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


	if (m_Foot_Type == FOOT_TYPE_WATER)
		m_pTextureCom->Set_Texture((_uint)m_fFrame);
	else {
		m_pTextureCom_Sub->Set_Texture((_uint)m_fFrame);
	}

	m_pBufferCom->Render_Buffer();



	/*m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, true);*/
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CEffect_Player_Foot::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Water_Wave"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Water_Wave", pComponent });

	pComponent = m_pTextureCom_Sub = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Foot_Step"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Foot_Step", pComponent });

	return S_OK;
}

CEffect_Player_Foot* CEffect_Player_Foot::Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 pos , FOOT_TYPE type, bool rev)
{
	CEffect_Player_Foot* pInstance = new CEffect_Player_Foot(pGraphicDev);

	pInstance->m_vPos = pos;
	pInstance->m_Foot_Type = type;
	pInstance->m_Is_Reverse = rev;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Player_Foot::Free(void)
{
	__super::Free();
}


