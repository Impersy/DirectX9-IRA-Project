#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Docheol_Tile : public CEffect
{
private:
	explicit CEffect_Docheol_Tile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Docheol_Tile(const CEffect_Docheol_Tile& rhs);
	virtual ~CEffect_Docheol_Tile();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vPos , const _vec3& vScale, const _int& iTextureNum , const _float& fAliveTime);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Docheol_Tile*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vPos , const _vec3& vScale , const _int& iTextureNum , const _float& fAliveTime);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;
	_float				m_fAliveTime = 0.f;
	_int				m_iTextureNum = -1;

};

