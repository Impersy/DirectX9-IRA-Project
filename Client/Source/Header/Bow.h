#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;


END

class CBow : public Engine::CGameObject
{
	
	

protected:
	explicit CBow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBow(const CBow& rhs);
	virtual ~CBow();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	

protected:

	virtual HRESULT	Add_Component(void)PURE;				


protected:
	CRcTex*			m_pBufferCom;

	CTransform*		m_pTransformCom;

	CTexture* m_pTextureCom[BOW_END] = {};
	
	_float			m_fSpeed;

	_float          m_Bow_Angle = 0.f;
	_float          m_Pre_Bow_Angle = 0.f;

	_vec3           m_Bow_Pos;

protected:
	virtual void Free(void) override;

};

