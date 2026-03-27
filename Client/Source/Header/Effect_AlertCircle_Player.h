#pragma once
#include "Effect.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;

END

class CEffect_AlertCircle_Player : public CEffect
{
private:
	explicit CEffect_AlertCircle_Player(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_AlertCircle_Player(const CEffect_AlertCircle_Player& rhs);
	virtual ~CEffect_AlertCircle_Player();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos, const _vec3& vScale, const _float& fAliveTime, const _float& fTraceTime, _bool bSpreadMode);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_AlertCircle_Player* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vScale, const _float& fAliveTime, const _float& fTraceTime,_bool bSpreadMode);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_float				m_fAliveTime = 0.f;
	_float				m_fTraceTime = 0.f;
	_vec3				m_vMaxScale;

	_bool				m_bSpreadMode = false;

	int                 m_iAlpha = 255;
	int                 m_R = 255;
	int                 m_G = 255;
	int                 m_B = 255;

	bool                m_Thunder = false;
	bool                m_bRender = false;
	_float              m_Thunder_Time = 0.f;


	

};

