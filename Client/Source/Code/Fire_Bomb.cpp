#include "stdafx.h"
#include "..\Header\Fire_Bomb.h"
#include "Export_Function.h"
#include "SylphBow.h"
#include "Player.h"
#include "Night_Circle.h"
#include <SoundMgr.h>

CFire_Bomb::CFire_Bomb(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CFire_Bomb::CFire_Bomb(const CFire_Bomb & rhs)
	: CEffect(rhs)
{
}

CFire_Bomb::~CFire_Bomb()
{
	Free();
}

HRESULT CFire_Bomb::Ready_GameObject(void)
{
	Add_Component();


	_vec3 Scale = { 3.2f * 12.f , 2.4f * 12.f, 1.f };

	m_iAlpha = 255;
	m_pTransformCom->m_vScale = Scale * PUBLIC_SCALE;

	m_fFrame = 0.f;
	m_fMaxFrame = 49.f;

	m_vPos.x -= 3.f;
	m_vPos.y += 9.f;
	m_vPos.z += -6.f;

	m_pTransformCom->m_vInfo[INFO_POS] = m_vPos;
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(35.f));




	return S_OK;
}

_int CFire_Bomb::Update_GameObject(const _float& fTimeDelta)
{
	CLayer* pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_float Time = pGameLogicLayer->m_fTimeDelta;

	

	m_fFrame += m_fMaxFrame * Time * 0.75f;


		

	if (m_fFrame > m_fMaxFrame)
	{
		return OBJ_DEAD;
		m_fFrame = 49.f;
	}


	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(Time);

	return 0;
}

void CFire_Bomb::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

void CFire_Bomb::Render_GameObject()
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

HRESULT CFire_Bomb::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Courage_Bomb"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Courage_Bomb", pComponent });



	return S_OK;
}

CFire_Bomb* CFire_Bomb::Create(LPDIRECT3DDEVICE9 pGraphicDev , _vec3 pos)
{
	CFire_Bomb* pInstance = new CFire_Bomb(pGraphicDev);

	pInstance->m_vPos = pos;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFire_Bomb::Free(void)
{
	

	__super::Free();
}


