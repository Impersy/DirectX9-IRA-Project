#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCol : public CVIBuffer
{
private:
	explicit CRcCol(void);
	explicit CRcCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcCol(const CRcCol& rhs);
	virtual ~CRcCol();

public:
	
	HRESULT		Ready_Buffer(void);
	void		Render_Buffer(void);

public:
	virtual CComponent* Clone(void) override;
	static CRcCol*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

END