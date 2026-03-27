#include "stdafx.h"
#include "..\Header\Effect_Docheol_Tile.h"
#include "Export_Function.h"

CEffect_Docheol_Tile::CEffect_Docheol_Tile(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Docheol_Tile::CEffect_Docheol_Tile(const CEffect_Docheol_Tile & rhs)
	: CEffect(rhs)
{
}

CEffect_Docheol_Tile::~CEffect_Docheol_Tile()
{
	Free();
}

HRESULT CEffect_Docheol_Tile::Ready_GameObject(const _vec3& vPos , const _vec3& vScale, const _int& iTextureNum , const _float& fAliveTime)
{
	Add_Component();

	m_pTransformCom->m_vScale = vScale;

	m_iTextureNum = iTextureNum;

	m_fAliveTime = fAliveTime;

	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(90.f));

	m_pTransformCom->m_vInfo[INFO_POS] = vPos;
	m_pTransformCom->m_vInfo[INFO_POS].y = 0.2f;

	return S_OK;
}

_int CEffect_Docheol_Tile::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	m_fAccTime += fTimeDelta;

	return 0;
}

void CEffect_Docheol_Tile::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	if (m_fAccTime > m_fAliveTime)
		m_bDead = true;
}

void CEffect_Docheol_Tile::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture(m_iTextureNum);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CEffect_Docheol_Tile::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Effect_Docheol_Tile"));
	
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Effect_Docheol_Tile", pComponent });

	return S_OK;
}

CEffect_Docheol_Tile* CEffect_Docheol_Tile::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos , const _vec3& vScale, const _int& iTextureNum , const _float& fAliveTime)
{
	CEffect_Docheol_Tile* pInstance = new CEffect_Docheol_Tile(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos , vScale , iTextureNum , fAliveTime)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffect_Docheol_Tile::Free(void)
{
	__super::Free();
}


