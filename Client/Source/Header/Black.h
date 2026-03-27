#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CBlack : public Engine::CGameObject
{
private:
	explicit CBlack(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBlack(const CBlack& rhs);
	virtual ~CBlack();

public:
	const _float&		Get_Alpha() { return m_fAlpha; }
	void				Set_FadeIn() { m_bFadeIn = true; }
	void				Set_FadeOut() { m_bFadeOut = true; }

	void				Set_Alpha(float fAlpha);

	void				Scene_Change(SCENE_TYPE _e, const _vec3& vNextPos)
	{
		m_bChangeScene[_e] = true;
		g_vNextPos = vNextPos;
	}

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

	void			Black_Print();
	void			Reset_Proj_Matrix();

private:
	CRcTex*		m_pBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom;

	_float		m_fAlpha = 255.f;
	_float		m_fAlphaStep = 10.f;

	_bool		m_bFadeIn = true;
	_bool		m_bFadeOut = false;

	_bool		m_bChangeScene[SCENE_END] = {};

public:
	static CBlack*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

