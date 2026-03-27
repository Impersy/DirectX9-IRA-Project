#include "stdafx.h"
#include "..\Header\FrontUi.h"
#include "Export_Function.h"

CFrontUi::CFrontUi(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_fFrame(0.f), m_bCheck(false), m_eCategory(UI_NONE)
{
}

CFrontUi::CFrontUi(const CFrontUi& rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTextureCom(rhs.m_pTextureCom), m_pTransformCom(rhs.m_pTransformCom)
	, m_fFrame(0.f), m_bCheck(false), m_eCategory(UI_NONE)
{
}

CFrontUi::~CFrontUi()
{
	Free();
}

HRESULT CFrontUi::Ready_GameObject(void)
{
	return S_OK;
}

_int CFrontUi::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CFrontUi::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CFrontUi::Render_GameObject()
{
}

void CFrontUi::Free(void)
{
	__super::Free();
}
