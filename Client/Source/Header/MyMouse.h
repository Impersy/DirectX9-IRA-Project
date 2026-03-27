#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;

END

class CMyMouse : public Engine::CGameObject
{
private:
	explicit CMyMouse(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMyMouse(const CMyMouse& rhs);
	virtual ~CMyMouse();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT	Add_Component(void);
	void	Update_Mouse_Pos(void);
	void	Ui_Print(void);
	void	Reset_Proj_Matrix();

public:
	static CMyMouse* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	virtual void Free(void) override;

private:
	CRcTex*		m_pBufferCom;
	CTransform* m_pTransformCom;
	CTexture*	m_pTextureCom;

	_float		m_fFrame = 0.f;
	_vec3		m_vMyMouse = { 0.f, 0.f, 0.f };
	_vec3		m_vPrintMouse = { 0.f, 0.f, 0.f };


};

