#include "stdafx.h"
#include "..\Header\Docheol_FloatingObj.h"
#include "Export_Function.h"

CDocheol_FloatingObj::CDocheol_FloatingObj(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandscape(pGraphicDev)
{
}

CDocheol_FloatingObj::CDocheol_FloatingObj(const CDocheol_FloatingObj& rhs)
	: CLandscape(rhs)

{
}

CDocheol_FloatingObj::~CDocheol_FloatingObj()
{
}

HRESULT CDocheol_FloatingObj::Ready_GameObject(const _vec3& vPos, const _vec3& vScale, const _vec3& vRot , const _int& iTextureNum)
{
	Add_Component();

	m_pTransformCom->m_vScale = vScale;

	m_iTextureNum = iTextureNum;
	m_vDir = { 0.f , 1.f , 0.f };
	m_fSpeed = 2.f;

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(vRot.x));
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(vRot.y));
	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(vRot.z));

	return S_OK;
}

_int CDocheol_FloatingObj::Update_GameObject(const _float& fTimeDelta)
{	
	m_fAccTime += fTimeDelta;

	if (m_fAccTime > 2.f)
	{
		m_fAccTime = 0.f;
		m_vDir *= -1.f;
	}

	CGameObject::Update_GameObject(fTimeDelta);

	m_pTransformCom->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;

	

	return 0;
}

void CDocheol_FloatingObj::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CDocheol_FloatingObj::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ***
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// 나중에 지워라 

	m_pTextureCom->Set_Texture(m_iTextureNum);
	m_pBufferCom->Render_Buffer();

	// ***
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// 나중에 지워라

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CDocheol_FloatingObj::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Object_DocheolFloatingObj"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Object_DocheolFloatingObj", pComponent });

	return S_OK;
}

CDocheol_FloatingObj* CDocheol_FloatingObj::Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos, const _vec3& vScale, const _vec3& vRot , const _int& iTextureNum)
{
	CDocheol_FloatingObj* pInstance = new CDocheol_FloatingObj(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale, vRot , iTextureNum)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDocheol_FloatingObj::Free(void)
{
	__super::Free();
}
