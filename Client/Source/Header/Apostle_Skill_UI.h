#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CApostle_Skill_UI : public Engine::CGameObject
{


protected:
	explicit CApostle_Skill_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CApostle_Skill_UI(const CApostle_Skill_UI& rhs);
	virtual ~CApostle_Skill_UI();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	virtual HRESULT	Add_Component(void)PURE;
	virtual void Change_State(void)PURE;
	virtual void Frame_Check(const _float& fTimeDelta)PURE;
	virtual void Reset_Proj_Matrix(void) PURE;

protected:
	CRcTex* m_pBufferCom;
	CTransform* m_pTransformCom;
	CTexture* m_pTextureCom;

	_float			m_fFrame;
	_bool			m_bCheck;
	

	FRONTUI			m_tINFO = { 0, 0, 0 };



protected:
	virtual void Free(void) override;

};

