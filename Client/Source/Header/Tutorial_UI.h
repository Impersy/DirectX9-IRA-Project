#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CTutorial_UI : public Engine::CGameObject
{
private:
	explicit CTutorial_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTutorial_UI(const CTutorial_UI& rhs);
	virtual ~CTutorial_UI();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

	void			Tutorial_UI_Print();
	void			Reset_Proj_Matrix();

private:
	CRcTex*		m_pBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom;

	_int		m_iFrameNum = 0;

	_float		m_fAccTime = 0.f;


public:
	static CTutorial_UI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

