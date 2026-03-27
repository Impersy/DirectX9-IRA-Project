#include "stdafx.h"
#include "..\Header\Docheol_SpecialDoor.h"
#include "Export_Function.h"
#include "CollisionMgr.h"
#include <Docheol.h>
#include "Effect_Stone_Destruction.h"
#include <SoundMgr.h>

CDocheol_SpecialDoor::CDocheol_SpecialDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandscape(pGraphicDev)
{
}

CDocheol_SpecialDoor::CDocheol_SpecialDoor(const CDocheol_SpecialDoor& rhs)
	: CLandscape(rhs)

{
}

CDocheol_SpecialDoor::~CDocheol_SpecialDoor()
{
}

HRESULT CDocheol_SpecialDoor::Ready_GameObject(const _vec3& vPos, const _vec3& vScale , const _float& vfAngleX, const _float& vfAngleZ , COLOR eColor)
{
	Add_Component();

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;
	m_pTransformCom->m_vScale = vScale;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(vfAngleX));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(vfAngleZ));

	m_eColor = eColor;

	return S_OK;
}

_int CDocheol_SpecialDoor::Update_GameObject(const _float& fTimeDelta)
{	
	CDocheol* pDocheol = dynamic_cast<CDocheol*> (Engine::Get_GameObject(L"Layer_GameLogic", L"Docheol"));

	if (pDocheol->Get_State() == CDocheol::RAGE_IDLE)
	{
		m_bDead = true;
		pDocheol->Set_Hp_Up(100000);
	}
	
	if (m_bDead)
	{
		Create_DeathEffect();

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::DOCHEOL_EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"Common_Liftdown.wav", CSoundMgr::DOCHEOL_EFFECT, 1.f);

		return OBJ_DEAD;
	}

	BulletColor_Check();

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);
	CCollisionMgr::GetInstance()->Add_CollisionObject(OBJ_SPECIAL, this);

	return 0;
}

void CDocheol_SpecialDoor::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDocheol_SpecialDoor::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pTextureCom->Set_Texture((int)m_eColor);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDocheol_SpecialDoor::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Object_DocheolSpecialDoor"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Object_DocheolSpecialDoor", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_ProtoComponent(L"Proto_Collider"));
	
	m_pColliderCom->Set_TransformCom(m_pTransformCom);
	m_pColliderCom->Set_Radius(10.f);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Collider", pComponent });

	return S_OK;
}

void CDocheol_SpecialDoor::BulletColor_Check()
{
	if (m_iBulletColor == -1)
		return;

	if (m_eColor == m_iBulletColor)
		m_bDead = true;
}

void CDocheol_SpecialDoor::Create_DeathEffect()
{
	CGameObject* pEffect = nullptr;
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_vec3 vScale = { 20.f , 20.f, 20.f };

	pEffect = CEffect_Stone_Destruction::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[INFO_POS], vScale);
	pLayer->Add_BulletObject(pEffect);
}

CDocheol_SpecialDoor* CDocheol_SpecialDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos, const _vec3& vScale , const _float& vfAngleX, const _float& vfAngleZ , COLOR eColor)
{
	CDocheol_SpecialDoor* pInstance = new CDocheol_SpecialDoor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale , vfAngleX , vfAngleZ , eColor)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDocheol_SpecialDoor::Free(void)
{
	__super::Free();
}
