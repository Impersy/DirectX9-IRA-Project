#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CTitle_Font : public Engine::CGameObject
{
private:
	explicit CTitle_Font(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTitle_Font(const CTitle_Font& rhs);
	virtual ~CTitle_Font();

public:
	
	

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

	_float              m_iAlpha = 0;
	_float              m_Dir = 1;

	

public:
	static CTitle_Font*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
	
};

