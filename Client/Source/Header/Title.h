#pragma once

#include "Scene.h"
#include "Base.h"


class CTitle : public Engine::CScene
{
private:
	explicit CTitle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTitle();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene(void) override;
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(void);
	virtual			void		Render_Scene(void);

private:
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);

public:
	static	CTitle*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
	
};

