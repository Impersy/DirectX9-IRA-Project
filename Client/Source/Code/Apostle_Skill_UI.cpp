#include "stdafx.h"
#include "..\Header\Apostle_Skill_UI.h"
#include "Export_Function.h"

CApostle_Skill_UI::CApostle_Skill_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_fFrame(0.f), m_bCheck(false)
{
}

CApostle_Skill_UI::CApostle_Skill_UI(const CApostle_Skill_UI& rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTextureCom(rhs.m_pTextureCom), m_pTransformCom(rhs.m_pTransformCom)
	, m_fFrame(0.f), m_bCheck(false)
{
}

CApostle_Skill_UI::~CApostle_Skill_UI()
{
}

HRESULT CApostle_Skill_UI::Ready_GameObject(void)
{
	return S_OK;
}

_int CApostle_Skill_UI::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CApostle_Skill_UI::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CApostle_Skill_UI::Render_GameObject()
{
}

void CApostle_Skill_UI::Free(void)
{
	__super::Free();
}
