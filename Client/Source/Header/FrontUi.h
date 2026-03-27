#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CFrontUi : public Engine::CGameObject
{	
	enum FRONT_STATUS { FRONT_STA_BACK, FRONT_STA_HP, FRONT_STA_ENERGE, FRONT_STA_KEY, FRONT_STA_MONEY, FRONT_STA_STONE, FRONT_STA_END }; //6개
	enum FRONT_APOSTLE { FRONT_APO_BACK, FRONT_APO_CATEGORY, FRONT_APO_COUNT, FRONT_APO_END}; //3개
	enum FRONT_MINIMAP { FRONT_MIN_BACK, FRONT_MIN_ROOM, FRONT_MIN_ICON, FRONT_MIN_CHAOS, FRONT_MIN_END};	//4개
	enum FRONT_QUICK { FRONT_QUICK_BACK, FRONT_QUICK_WEAPONIMG, FRONT_QUICK_ARROWCOUNT, FRONT_QUICK_END };	//3개

protected:
	explicit CFrontUi(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFrontUi(const CFrontUi& rhs);
	virtual ~CFrontUi();

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
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;

	_float			m_fFrame;
	_bool			m_bCheck;
	UI_CATEGORY		m_eCategory;

	FRONTUI			m_tINFO = { 0, 0, 0 };



protected:
	virtual void Free(void) override;

};

