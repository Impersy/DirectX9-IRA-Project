#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSphereTex : public CVIBuffer
{
private:
	explicit CSphereTex(void);
	explicit CSphereTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphereTex(const CSphereTex& rhs);
	virtual ~CSphereTex();

public:
	
	HRESULT		Ready_Buffer(void);
	void		Render_Buffer(void);

public:
	virtual CComponent* Clone(void) override;
	static CSphereTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

END