#include "stdafx.h"
#include "..\Header\Debris.h"
#include "Export_Function.h"
#include "Terrain.h"

CDebris::CDebris(LPDIRECT3DDEVICE9 pGraphicDev)
	: CDynamicObject(pGraphicDev)
{
}

CDebris::CDebris(const CDebris& rhs)
	: CDynamicObject(rhs)
{
}

CDebris::~CDebris()
{
}

HRESULT CDebris::Ready_GameObject(_byte byDrawId, const _vec3 vPos)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_byDrawID = byDrawId;
	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	return S_OK;
}

_int CDebris::Update_GameObject(const _float& fTimeDelta)
{	
	__super::Update_GameObject(fTimeDelta);

	SetUp_OnTerrain();

	//Move();

	Engine::Add_RenderGroup(RENDER_ALPHATEST, this);

	return 0;
}

void CDebris::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

	//*** 나중에 지워라
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::Compute_ViewZ(&vPos);
}

void CDebris::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (-1 != m_byDrawID)
		m_pTextureCom[m_eID]->Set_Texture(m_byDrawID);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CDebris::Move(const _float& fTimeDelta)
{
	//if(0 < m_iTouchDownCnt)
}

CDebris* CDebris::Create(LPDIRECT3DDEVICE9 pGraphicDev, _byte byDrawId, const _vec3 vPos)
{
	CDebris* pInstance = new CDebris(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(byDrawId, vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDebris::Free(void)
{
	__super::Free();
}

HRESULT CDebris::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	if (nullptr == m_pBufferCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	if (nullptr == m_pTransformCom) return E_FAIL;
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_ProtoComponent(L"Proto_Calculator"));
	if (nullptr == m_pCalculatorCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	//pComponent = m_pTextureCom[DEBRIS_STONE] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Debris_Stone"));
	//if (nullptr == m_pCalculatorCom) return E_FAIL;
	//m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Debris_Stone", pComponent });

	pComponent = m_pTextureCom[DEBRIS_JAR] = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Spr_Jar_Debris_"));
	if (nullptr == m_pCalculatorCom) return E_FAIL;
	m_uMapComponent[ID_STATIC].insert({ L"Spr_Jar_Debris_", pComponent });


	return S_OK;
}

void CDebris::SetUp_OnTerrain(void)
{
	_vec3		vPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos;

	_tchar szBuf[MAX_STR];
	CTerrain* pTerrain = dynamic_cast<CTerrain*>(Engine::Get_GameObject(L"Layer_Environment", L"Terrain"));
	pTerrain->Get_TerrainTexKey(szBuf);
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"Layer_Environment", L"Terrain", szBuf, ID_STATIC));
	if (nullptr == pTerrainBufferCom) return;

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), pTerrainBufferCom->Get_VtxCntX(), pTerrainBufferCom->Get_VtxCntZ());

	m_pTransformCom->Set_Pos(vPos.x, fHeight + 1.f, vPos.z);
}

