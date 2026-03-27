#include "..\..\Header\SphereTex.h"

CSphereTex::CSphereTex(void)
{
}

CSphereTex::CSphereTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CSphereTex::CSphereTex(const CSphereTex & rhs)
	:CVIBuffer(rhs)
{
}

CSphereTex::~CSphereTex()
{
}

HRESULT CSphereTex::Ready_Buffer(void)
{
	m_dwFVF = FVF_COL;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 168;
	m_dwTriCnt = 25*26;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer())) return E_FAIL;

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// Lock : 버텍스 버퍼 메모리 공간의 접근을 막음, 버텍스 버퍼에 저장되어 있는 첫 번째 버텍스의 주소를 추출
	
	for (_ulong i = 0; i < m_dwVtxCnt; i++)
	{
		float theta = D3DX_PI * (float)(i % 26) / 13.0f;
		float phi = D3DX_PI * 2.0f * (float)(i / 26) / 26.0f;
		float sinTheta = sinf(theta);
		float cosTheta = cosf(theta);
		float sinPhi = sinf(phi);
		float cosPhi = cosf(phi);

		pVertex[i].vPos = { sinTheta * cosPhi , cosTheta , sinTheta * sinPhi };
		pVertex[i].dwColor = 0x00000000;
	}

	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;

	_ulong	dwTriIdx = 0;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 26; j++)
		{
			int index = i * 26 + j;
			int nextIndex = index + 26;
			if (j == 25) nextIndex -= 26;

			pIndex[index]._0 = index;
			pIndex[index]._1 = nextIndex;
			pIndex[index]._2 = index + 1;

			/*WORD indices[] = { index, nextIndex, index + 1 };
			pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NUM_VERTICES, indices, 3);

			indices[1] = nextIndex + 1;
			indices[2] = nextIndex;
			pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NUM_VERTICES, indices, 3);*/
		}
	}
	
	m_pIB->Unlock();

	return S_OK;
}

void CSphereTex::Render_Buffer(void)
{
	__super::Render_Buffer();
}


Engine::CComponent* CSphereTex::Clone(void)
{	
	return new CSphereTex(*this);
}

CSphereTex * CSphereTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphereTex * pInstance = new CSphereTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void Engine::CSphereTex::Free(void)
{
	__super::Free();
}
