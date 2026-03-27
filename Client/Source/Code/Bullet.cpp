#include "stdafx.h"
#include "..\Header\Bullet.h"
#include "Export_Function.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fSpeed(0.f), m_fFrame(0.f), m_pColliderCom(nullptr), m_eState(BULLET_IDLE)
{
}

CBullet::CBullet(const CBullet& rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(nullptr), m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_bCheck(false), m_fSpeed(0.f), m_fFrame(0.f), m_pColliderCom(nullptr), m_eState(BULLET_IDLE)

{
}

CBullet::~CBullet()
{
	Free();
}

HRESULT CBullet::Ready_GameObject(void)
{
	
	return S_OK;
}

_int CBullet::Update_GameObject(const _float& fTimeDelta)
{	
	//CGameObject::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CBullet::LateUpdate_GameObject()
{

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CBullet::Render_GameObject()
{
}

void CBullet::Free(void)
{
	__super::Free();
}
