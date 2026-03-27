#include "stdafx.h"
#include "..\Header\Effect_Doewole_ChargeCircle.h"
#include "Export_Function.h"

CEffect_Doewole_ChargeCircle::CEffect_Doewole_ChargeCircle(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Doewole_ChargeCircle::CEffect_Doewole_ChargeCircle(const CEffect_Doewole_ChargeCircle & rhs)
	: CEffect(rhs)
{
}

CEffect_Doewole_ChargeCircle::~CEffect_Doewole_ChargeCircle()
{
	Free();
}

HRESULT CEffect_Doewole_ChargeCircle::Ready_GameObject(const _vec3& vPos)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 30.f , 30.f, 30.f };

	m_fMaxFrame = 12.f;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	CTransform* pTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Doewole_Body", L"Proto_Transform", ID_DYNAMIC));

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	return S_OK;
}

_int CEffect_Doewole_ChargeCircle::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fFrame += m_fMaxFrame * fTimeDelta;

	return 0;
}

void CEffect_Doewole_ChargeCircle::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Doewole_ChargeCircle::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Doewole_ChargeCircle::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Doewole_ChargeCircle"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Doewole_ChargeCircle", pComponent });

	return S_OK;
}

CEffect_Doewole_ChargeCircle* CEffect_Doewole_ChargeCircle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos)
{
	CEffect_Doewole_ChargeCircle* pInstance = new CEffect_Doewole_ChargeCircle(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Doewole_ChargeCircle::Free(void)
{
	__super::Free();
}


