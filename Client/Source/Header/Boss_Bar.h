#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CBoss_Bar : public Engine::CGameObject
{
protected:
	explicit CBoss_Bar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBoss_Bar(const CBoss_Bar& rhs);
	virtual ~CBoss_Bar();

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
	static CBoss_Bar* Create(LPDIRECT3DDEVICE9 pGraphicDev, BOSS_NAME _Name);

protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;

	_float			m_fFrame;
	_bool			m_bCheck;

	FRONTUI*		m_pINFO;

	BOSS_NAME m_eBoss_Name = BOSS_END;

protected:
	virtual void Free(void) override;

};

