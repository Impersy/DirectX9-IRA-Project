#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CCubeTex;
class CTransform;
class CTexture;

END

class CSkyBox : public Engine::CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

private:
	CCubeTex*	m_pBufferCom;
	CTransform*	m_pTransformCom;
	CTexture*	m_pTextureCom;

public:
	static CSkyBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

