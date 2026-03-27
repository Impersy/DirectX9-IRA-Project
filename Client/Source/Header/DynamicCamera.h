#pragma once

#include "Camera.h"

class CDynamicCamera : public CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera();

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
							const _vec3* pAt,
							const _vec3* pUp,
							const _float& fFov,
							const _float& fAspect,
							const _float& fNear,
							const _float& fFar) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(void);
	void		Fix_Mouse(void);

private:
	_bool		m_bFix;
	_bool		m_bClick;

public:
	static CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp,
										const _float& fFov,
										const _float& fAspect,
										const _float& fNear,
										const _float& fFar);

private:
	virtual void Free(void) override;

};

