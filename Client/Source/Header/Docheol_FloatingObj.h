#pragma once
#include "Landscape.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CDocheol_FloatingObj : public CLandscape
{
private:
	explicit CDocheol_FloatingObj(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDocheol_FloatingObj(const CDocheol_FloatingObj& rhs);
	virtual ~CDocheol_FloatingObj();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos, const _vec3& vScale, const _vec3& vRot , const _int& iTextureNum);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT		Add_Component(void) override;
	virtual void SetUp_OnTerrain(void){};

private:
	CTexture*			m_pTextureCom;
	int					m_iTextureNum = -1;

	_vec3				m_vDir;
	_float				m_fSpeed;

	_float				m_fAccTime = 0.f;


public:
	static CDocheol_FloatingObj* Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vScale , const _vec3& vRot , const _int& iTextureNum);

private:
	virtual void Free(void) override;

};

