#pragma once
#include "Ghost.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CPlayer;


class CGhostChild : public CGhost
{
	enum { DASH_045, DASH_135, DASH_END };
	enum { ANGLE_000, ANGLE_045, ANGLE_090, ANGLE_135, ANGLE_180, ANGLE_225, ANGLE_270, ANGLE_315, ANGLE_STATE_END };


private:
	explicit CGhostChild(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGhostChild(const CGhostChild& rhs);
	virtual  ~CGhostChild();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;


public:
	static CGhostChild* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
	

private:
	_float     m_Sprite = 0.f;
	_float     m_Alive_Time = 0.f;
	int        m_iAngleState;
	int        m_iAlpha = 200;


public:
	_float     m_Render_Time = 0.f;

};

