 #include "..\..\Header\TerrainTex.h"


CTerrainTex::CTerrainTex(void)
	:CVIBuffer()
	, m_pPos(nullptr)
	, m_dwCntX(0)
	, m_dwCntZ(0)
	, m_dwVtxItv(0)
	, m_fDetailLevel(1.f)
	, m_iHeightMapLevel(20.f)
	, m_pHeightMapFilePath(nullptr)
{
}

CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
	, m_pPos(nullptr)
	, m_dwCntX(0)
	, m_dwCntZ(0)
	, m_dwVtxItv(0)
	, m_fDetailLevel(1.f)
	, m_iHeightMapLevel(20.f)
	, m_pHeightMapFilePath(nullptr)
{
}

CTerrainTex::CTerrainTex(const CTerrainTex & rhs)
	:CVIBuffer(rhs)
	, m_hFile(rhs.m_hFile)
	, m_fh(rhs.m_fh)
	, m_ih(rhs.m_ih)
	, m_pPos(rhs.m_pPos)
	, m_dwCntX(rhs.m_dwCntX)
	, m_dwCntZ(rhs.m_dwCntZ)
	, m_dwVtxItv(rhs.m_dwVtxItv)
	, m_fDetailLevel(rhs.m_fDetailLevel)
	, m_iHeightMapLevel(rhs.m_iHeightMapLevel)
	, m_pHeightMapFilePath(rhs.m_pHeightMapFilePath)

{
}

CTerrainTex::~CTerrainTex()
{
}

HRESULT CTerrainTex::Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv, _vec3* pPos
	, _float fDetailLevel, _int iHeightMapLevel, LPCWSTR pfilePath)
{
	m_dwFVF = FVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = dwCntX * dwCntZ;

	m_dwCntX = dwCntX;
	m_dwCntZ = dwCntZ;
	m_dwVtxItv = dwVtxItv;

	Safe_Delete_Array(m_pPos);
	m_pPos = new _vec3[m_dwVtxCnt];
	memcpy(m_pPos, pPos, sizeof(_vec3) * m_dwVtxCnt);

	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	_ulong	dwByte = 0;

	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// Lock : 버텍스 버퍼 메모리 공간의 접근을 막음, 버텍스 버퍼에 저장되어 있는 첫 번째 버텍스의 주소를 추출

	_ulong	dwIndex = 0;

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			//pVertex[dwIndex].vPos = { _float(j * dwVtxItv), 0.f, _float(i * dwVtxItv) };
			pVertex[dwIndex].vPos = m_pPos[dwIndex];

			//m_pPos[dwIndex] = pVertex[dwIndex].vPos;

			pVertex[dwIndex].vNormal = { 0.f, 0.f, 0.f };

			pVertex[dwIndex].vTexUV = { _float(j) / (dwCntX - 1) * fDetailLevel,
										_float(i) / (dwCntZ - 1) * fDetailLevel
			};
		}
	}

	INDEX32* pIndex = nullptr;

	_ulong	dwTriIdx = 0;

	_vec3	vDest, vSrc, vNormal;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			// 오른쪽 위

			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriIdx]._2 = dwIndex + 1;

			vDest = pVertex[pIndex[dwTriIdx]._1].vPos - pVertex[pIndex[dwTriIdx]._0].vPos;
			vSrc = pVertex[pIndex[dwTriIdx]._2].vPos - pVertex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSrc);

			pVertex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._2].vNormal += vNormal;
			dwTriIdx++;

			// 왼쪽 아래

			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;

			vDest = pVertex[pIndex[dwTriIdx]._1].vPos - pVertex[pIndex[dwTriIdx]._0].vPos;
			vSrc = pVertex[pIndex[dwTriIdx]._2].vPos - pVertex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSrc);

			pVertex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._2].vNormal += vNormal;

			dwTriIdx++;
		}
	}

	Safe_Delete_Array(pPos);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

void CTerrainTex::Render_Buffer(void)
{
	__super::Render_Buffer();
}

//void CTerrainTex::Update_Buffer_Data(void)
//{
//	Ready_Buffer(m_dwCntX, m_dwCntZ, m_dwVtxItv, m_pHeightMapFilePath);
//}

void CTerrainTex::Set_HeightMapFilePath(const _tchar* pPath)
{
	if (m_pHeightMapFilePath)
		Safe_Delete_Array(m_pHeightMapFilePath);

	m_pHeightMapFilePath = new _tchar[MAX_PATH];
	lstrcpy(m_pHeightMapFilePath, pPath);
}

void CTerrainTex::Set_VtxPos(_vec3* pVtxPos, _ulong dwVtxCnt)
{
	VTXTEX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// Lock : 버텍스 버퍼 메모리 공간의 접근을 막음, 버텍스 버퍼에 저장되어 있는 첫 번째 버텍스의 주소를 추출

	_ulong	dwIndex = 0;

	Safe_Delete_Array<_vec3*>(m_pPos);

	m_pPos = new _vec3[dwVtxCnt];

	for (_ulong i = 0; i < m_dwCntZ; ++i)
	{
		for (_ulong j = 0; j < m_dwCntX; ++j)
		{
			dwIndex = i * m_dwCntX + j;

			pVertex[dwIndex].vPos = pVtxPos[dwIndex];

			m_pPos[dwIndex] = pVtxPos[dwIndex];

			pVertex[dwIndex].vNormal = { 0.f, 0.f, 0.f };

			pVertex[dwIndex].vTexUV = { _float(j) / (m_dwCntX - 1) * m_fDetailLevel,
										_float(i) / (m_dwCntZ - 1) * m_fDetailLevel
			};
		}
	}

	INDEX32* pIndex = nullptr;

	_ulong	dwTriIdx = 0;

	_vec3	vDest, vSrc, vNormal;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < m_dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < m_dwCntX - 1; ++j)
		{
			dwIndex = i * m_dwCntX + j;

			// 오른쪽 위

			pIndex[dwTriIdx]._0 = dwIndex + m_dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + m_dwCntX + 1;
			pIndex[dwTriIdx]._2 = dwIndex + 1;

			vDest = pVertex[pIndex[dwTriIdx]._1].vPos - pVertex[pIndex[dwTriIdx]._0].vPos;
			vSrc = pVertex[pIndex[dwTriIdx]._2].vPos - pVertex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSrc);

			pVertex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._2].vNormal += vNormal;
			dwTriIdx++;

			// 왼쪽 아래

			pIndex[dwTriIdx]._0 = dwIndex + m_dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;

			vDest = pVertex[pIndex[dwTriIdx]._1].vPos - pVertex[pIndex[dwTriIdx]._0].vPos;
			vSrc = pVertex[pIndex[dwTriIdx]._2].vPos - pVertex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSrc);

			pVertex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._2].vNormal += vNormal;

			dwTriIdx++;
		}
	}

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();
}


Engine::CComponent* CTerrainTex::Clone(void)
{	
	return new CTerrainTex(*this);
}

CTerrainTex * CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv, _vec3* pPos
	,_float fDetailLevel, _int iHeightMapLevel, LPCWSTR pfilePath)
{
	CTerrainTex * pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv, pPos, fDetailLevel, iHeightMapLevel, pfilePath)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void Engine::CTerrainTex::Free(void)
{
	if (false == m_bClone)
	{
		Safe_Delete_Array(m_pPos);
		Safe_Delete_Array(m_pHeightMapFilePath);
	}

	__super::Free();
}
