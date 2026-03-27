#include "..\..\Header\Calculator.h"
#include "TerrainTex.h"
#include "Transform.h"

CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
{
}


CCalculator::~CCalculator()
{
}

HRESULT CCalculator::Ready_Calculator(void)
{
	return S_OK;
}

_float CCalculator::Compute_HeightOnTerrain(_vec3 * pPos, const _vec3 * pTerrainVtxPos, const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong& dwVtxItv)
{

	_ulong	dwIndex = _ulong(pPos->z /dwVtxItv) * dwCntX + _ulong(pPos->x / dwVtxItv);

	_float	fRatioX = (pPos->x - pTerrainVtxPos[dwIndex + dwCntX].x) / dwVtxItv;
	_float	fRatioZ = (pTerrainVtxPos[dwIndex + dwCntX].z - pPos->z) / dwVtxItv;

	D3DXPLANE		Plane;

	// 오른쪽 위 삼각형
	if (fRatioX > fRatioZ)
	{
		D3DXPlaneFromPoints(&Plane, 
			&pTerrainVtxPos[dwIndex + dwCntX], 
			&pTerrainVtxPos[dwIndex + dwCntX + 1], 
			&pTerrainVtxPos[dwIndex + 1]);
	}
	// 왼쪽 아래 삼각형
	else
	{
		D3DXPlaneFromPoints(&Plane,
			&pTerrainVtxPos[dwIndex + dwCntX],
			&pTerrainVtxPos[dwIndex + 1],
			&pTerrainVtxPos[dwIndex]);
	}
	
  	pPos->y = (-Plane.a * pPos->x - Plane.c * pPos->z - Plane.d) / Plane.b;

	return pPos->y;
}

_vec3 CCalculator::Picking_OnTerrain(HWND hWnd, CTerrainTex * pTerrainBufferCom, CTransform * pTerrainTransCom)
{
	POINT		ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_vec3	vMousePos;
	
	// 윈도우 -> 투영
	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영 -> 뷰 스페이스
	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3	vRayPos, vRayDir;

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMousePos - vRayPos;
	
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	// 월드 -> 로컬

	_matrix	matWorld = *(pTerrainTransCom->Get_WorldMatrixPointer());
	D3DXMatrixInverse(&matWorld, 0, &matWorld);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

	_ulong	dwVtxCntX = pTerrainBufferCom->Get_VtxCntX();
	_ulong	dwVtxCntZ = pTerrainBufferCom->Get_VtxCntZ();

	const _vec3*	pTerrainVtx = pTerrainBufferCom->Get_VtxPos();

	_ulong	dwVtxIdx[3]{};

	_float fU, fV, fDist;

	for (_ulong i = 0; i < dwVtxCntZ; ++i)
	{
		for (_ulong j = 0; j < dwVtxCntX; ++j)
		{
			_ulong	dwIndex = i * dwVtxCntX + j;

			// 오른쪽 위
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + dwVtxCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			// D3DXIntersectTri
			// V1 + U(V2 - V1) + V(V3 - V1) 이 공식에 근거한 값이 결과 나옴

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[1]], 
				&pTerrainVtx[dwVtxIdx[0]], 
				&pTerrainVtx[dwVtxIdx[2]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return  _vec3(pTerrainVtx[dwVtxIdx[1]].x + (pTerrainVtx[dwVtxIdx[0]].x - pTerrainVtx[dwVtxIdx[1]].x) * fU,
								0.f,
					          pTerrainVtx[dwVtxIdx[1]].z + (pTerrainVtx[dwVtxIdx[2]].z - pTerrainVtx[dwVtxIdx[1]].z) * fV);
			}
		
			// 왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwVtxCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (D3DXIntersectTri(&pTerrainVtx[dwVtxIdx[2]], 
				&pTerrainVtx[dwVtxIdx[1]], 
				&pTerrainVtx[dwVtxIdx[0]],
				&vRayPos, &vRayDir,
				&fU, &fV, &fDist))
			{
				return  _vec3(pTerrainVtx[dwVtxIdx[2]].x + (pTerrainVtx[dwVtxIdx[1]].x - pTerrainVtx[dwVtxIdx[2]].x) * fU,
					0.f,
							pTerrainVtx[dwVtxIdx[2]].z + (pTerrainVtx[dwVtxIdx[0]].z - pTerrainVtx[dwVtxIdx[2]].z) * fV);
			}
		}
	}
	
	return _vec3(0.f, 0.f, 0.f);
}

CCalculator * CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator *	pInstance = new CCalculator(pGraphicDev);

	if (FAILED(pInstance->Ready_Calculator()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	
	return pInstance;
}

CComponent * CCalculator::Clone(void)
{
	return new CCalculator(*this);
}


void CCalculator::Free(void)
{
	__super::Free();
}
