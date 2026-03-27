#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Docheol_Meteor : public CEffect
{
public:
	enum TYPE{INTRO , SLAM};
private:
	explicit CEffect_Docheol_Meteor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Docheol_Meteor(const CEffect_Docheol_Meteor& rhs);
	virtual ~CEffect_Docheol_Meteor();

public:
	virtual HRESULT Ready_GameObject(const _vec3& vDestPos, const _vec3& vStartPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Docheol_Meteor*		Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3& vDestPos , const _vec3& vStartPos);

private:
	virtual void Free(void) override;

	void	Create_DeathEffect();

private:
	_float				m_fAccTime = 0.f;
	TYPE				m_eType;
	_vec3				m_vDir;
	_float				m_fSpeed = 0.f;
	_vec3				m_vDestPos;

};

