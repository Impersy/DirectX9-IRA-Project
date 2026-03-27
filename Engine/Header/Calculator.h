#pragma once
#include "Component.h"

BEGIN(Engine)

class CTerrainTex;
class CTransform;
class ENGINE_DLL CCalculator :	public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCalculator();

public:
	HRESULT		Ready_Calculator(void);
	_float		Compute_HeightOnTerrain(_vec3* pPos, const _vec3* pTerrainVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv =1);

	_vec3		Picking_OnTerrain(HWND hWnd, CTerrainTex* pTerrainBufferCom, CTransform* pTerrainTransCom);


public:
	static CCalculator*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);

private:
	virtual void Free(void);
};

END