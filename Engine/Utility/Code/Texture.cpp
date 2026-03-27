#include "..\..\Header\Texture.h"


CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_fMaxFrameCnt(0.f)
{
}

CTexture::CTexture(const CTexture & rhs)
	: CComponent(rhs)
	, m_fMaxFrameCnt(rhs.m_fMaxFrameCnt)
{
	_uint	iSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iSize);

	m_vecTexture = rhs.m_vecTexture;

	for (_uint i = 0; i < iSize; ++i)
		m_vecTexture[i]->AddRef();

}

CTexture::~CTexture()
{
}

HRESULT CTexture::Ready_Texture(TEXTYPE eType, const _tchar * pPath, const _uint & iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9*		pTexture = nullptr;

	m_fMaxFrameCnt = (_float)iCnt;

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR		szFileName[128] = L"";

		wsprintf(szFileName, pPath, i);

		switch (eType)
		{
		case TEX_NORMAL:
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;

		case TEX_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		m_vecTexture.push_back(pTexture);
	}

	return S_OK;
}

void CTexture::Set_Texture(const _uint & iIndex)
{
	if (m_vecTexture.size() < iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

void CTexture::Get_Texture(const _uint& iIndex)
{
	if (m_vecTexture.size() < iIndex)
		return;

	m_pGraphicDev->GetTexture(0, &m_vecTexture[iIndex]);
}

CComponent * CTexture::Clone(void)
{
	return new CTexture(*this);
}

CTexture * CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTYPE eType, const _tchar* pPath, const _uint& iCnt)
{
	CTexture *		pInstance = new CTexture(pGraphicDev);

	

	if (FAILED(pInstance->Ready_Texture(eType, pPath, iCnt)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	
	return pInstance;
}

void CTexture::Free(void)
{
	__super::Free();

	for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
	m_vecTexture.clear();
	m_vecTexture.shrink_to_fit();
}
