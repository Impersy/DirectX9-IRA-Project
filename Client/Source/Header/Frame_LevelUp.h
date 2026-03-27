#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CFrame_LevelUp : public Engine::CGameObject
{
protected:
	explicit CFrame_LevelUp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFrame_LevelUp(const CFrame_LevelUp& rhs);
	virtual ~CFrame_LevelUp();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component(void);
	void	Change_State(void);
	void	Frame_Check(const _float& fTimeDelta);

	HRESULT Create_Frame_LevelUpUi(void);

public:
	static CFrame_LevelUp* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;

	_float			m_fFrame;
	_bool			m_bCheck;

protected:
	virtual void Free(void) override;

};

