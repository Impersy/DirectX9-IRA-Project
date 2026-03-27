#pragma once
#include "Landscape.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCollider;

END

class CDocheol_SpecialDoor : public CLandscape
{
public:
	enum COLOR{RED = 0 , BLUE , GREEN , WHITE};

private:
	explicit CDocheol_SpecialDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDocheol_SpecialDoor(const CDocheol_SpecialDoor& rhs);
	virtual ~CDocheol_SpecialDoor();

public:
	void			Set_BulletColor(int iColor) { m_iBulletColor = iColor; }

public:
	_bool		m_bHit = false;

	virtual HRESULT Ready_GameObject(const _vec3& vPos, const _vec3& vScale , const _float& vfAngleX, const _float& vfAngleZ , COLOR eColor);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT		Add_Component(void) override;
	virtual void		SetUp_OnTerrain(void){};

	void				BulletColor_Check();

	void				Create_DeathEffect();

public:
	static CDocheol_SpecialDoor* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vPos, const _vec3& vScale, const _float& vfAngleX, const _float& vfAngleZ , COLOR eColor);

private:
	virtual void Free(void) override;

private:
	CTexture*	m_pTextureCom;
	CCollider*	m_pColliderCom;
	COLOR		m_eColor;

	_int		m_iBulletColor = -1;
};

