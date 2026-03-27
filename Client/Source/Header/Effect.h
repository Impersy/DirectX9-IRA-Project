#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCollider;

END

class CEffect : public Engine::CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect();

public:
	virtual HRESULT Ready_GameObject(void){return S_OK;}
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;
	CTexture*		m_pTextureCom_Sub;

	_float			m_fFrame = 0.f;
	_float			m_fMaxFrame = 0.f;

protected:
	virtual void Free(void) override;

};

