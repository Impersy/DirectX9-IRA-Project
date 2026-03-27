#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex : public CVIBuffer
{
private:
	explicit CCubeTex(void);
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex();

public:
	
	HRESULT		Ready_Buffer(void);
	void		Render_Buffer(void);

public:
	virtual CComponent* Clone(void) override;
	static CCubeTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

END