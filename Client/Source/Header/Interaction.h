#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CInteraction : public Engine::CGameObject
{
protected:
	explicit CInteraction(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInteraction(const CInteraction& rhs);
	virtual ~CInteraction();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void) PURE;
	virtual void	Change_State(void) PURE;
	virtual void	Frame_Check(const _float& fTimeDelta) PURE;
	void			Reset_Proj_Matrix(void);

protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;

	_float			m_fFrame;
	_float			m_fFrame_Max = 0.f;
	_bool			m_bCheck;

	_vec3			m_vMasterPos;

	INTERACT_TYPE	m_eInter_Type;
	INTERACT_TARGET m_eInter_Target;

protected:
	virtual void Free(void) override;

};

