#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CMiniMap : public Engine::CGameObject
{
protected:
	explicit CMiniMap(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMiniMap(const CMiniMap& rhs);
	virtual ~CMiniMap();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component(void);
	void	Change_State(void);
	void	Frame_Check(const _float& fTimeDelta);

private:
	HRESULT	Create_StatusUi(void);

public:
	static CMiniMap* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;

	_float			m_fFrame;
	_bool			m_bCheck;

	FRONTUI*		m_pINFO;

protected:
	virtual void Free(void) override;

};

