#pragma once
#include "DynamicObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;

END

class CDebris : public CDynamicObject
{
private:
	explicit CDebris(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDebris(const CDebris& rhs);
	virtual ~CDebris();

public:
	virtual HRESULT Ready_GameObject(void) { return S_OK; };
	virtual HRESULT Ready_GameObject(_byte byDrawId, const _vec3 vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void)override;
	virtual void SetUp_OnTerrain(void)override;
	void	Move(const _float& fTimeDelta);

private:
	CTexture*	m_pTextureCom[DEBRIS_END];
	_int		m_iTouchDownCnt = 0;

public:
	static CDebris* Create(LPDIRECT3DDEVICE9 pGraphicDev, _byte byDrawId, const _vec3 vPos);

private:
	virtual void Free(void) override;

};

