#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(void);
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex();

public:
	const _vec3*	Get_VtxPos(void) { return m_pPos; }
	_ulong			Get_VtxCntX(void) const { return m_dwCntX; }
	_ulong			Get_VtxCntZ(void) const { return m_dwCntZ; }
	_ulong			Get_VtxItv(void) const { return m_dwVtxItv; }

	HRESULT		Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv, _vec3* pPos
		,_float fDetailLevel = 1, _int iHeightMapLevel = 20.f, LPCWSTR pfilePath = nullptr);
	void		Render_Buffer(void);

	//void		Update_Buffer_Data(void);
	void		Set_VtxCnt(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv = 1) {
					m_dwCntX = dwCntX; m_dwCntZ = dwCntZ; m_dwVtxItv = dwVtxItv;
				}
	void		Set_DetailLevel(float fLevel) { m_fDetailLevel = fLevel; }
	void		Set_HeightMapLevel(int iLevel) { m_iHeightMapLevel = iLevel; }
	void		Set_HeightMapFilePath(const _tchar* pPath);
	void		Set_VtxPos(_vec3* pVtxPos, _ulong dwVtxCnt);

private:
	HANDLE				m_hFile;
	BITMAPINFOHEADER	m_ih;
	BITMAPFILEHEADER	m_fh;
	_vec3*				m_pPos;

	_ulong				m_dwCntX;
	_ulong				m_dwCntZ;
	_ulong				m_dwVtxItv;
	_float				m_fDetailLevel;
	_int				m_iHeightMapLevel;
	_tchar*				m_pHeightMapFilePath;

public:
	virtual CComponent* Clone(void) override;
	static CTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv, _vec3* pPos
		, _float fDetailLevel = 1, _int iHeightMapLevel = 20.f, LPCWSTR pfilePath = nullptr);

private:
	virtual void Free(void) override;
};

END