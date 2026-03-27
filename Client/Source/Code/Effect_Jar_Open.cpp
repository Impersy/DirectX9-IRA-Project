#include "stdafx.h"
#include "..\Header\Effect_Jar_Open.h"
#include "Export_Function.h"

CEffect_Jar_Open::CEffect_Jar_Open(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Jar_Open::CEffect_Jar_Open(const CEffect_Jar_Open& rhs)
	: CEffect(rhs)
{
}

CEffect_Jar_Open::~CEffect_Jar_Open()
{
	Free();
}

HRESULT CEffect_Jar_Open::Ready_GameObject(const _vec3& vPos)
{
	Add_Component();

	m_pTransformCom->m_vScale = { 15.f, 15.f, 15.f };
	_vec3 vPosition = vPos;
	vPosition.z -= 5.f;
	m_pTransformCom->m_vInfo[INFO_POS] = vPosition;
	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(30.f));

	m_fMaxFrame = 19.f;

	return S_OK;
}

_int CEffect_Jar_Open::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_fFrame += m_fMaxFrame * fTimeDelta * 2.5f;

	return 0;
}

void CEffect_Jar_Open::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fFrame > m_fMaxFrame)
	{
		m_bDead = true;
		m_fFrame = m_fMaxFrame;
	}
}

void CEffect_Jar_Open::Render_GameObject()
{
	__super::Render_GameObject();
}

HRESULT CEffect_Jar_Open::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));

	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_JarOpen"));

	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_JarOpen", pComponent });

	return S_OK;
}

CEffect_Jar_Open* CEffect_Jar_Open::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos)
{
	CEffect_Jar_Open* pInstance = new CEffect_Jar_Open(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Jar_Open::Free(void)
{
	__super::Free();
}
