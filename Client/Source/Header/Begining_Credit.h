#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CBegining_Credit : public Engine::CGameObject
{

	enum ROGO_TYPE { ROGO_JUSIN, ROGO_SUNA, ROGO_TEAM, ROGO_END };


private:
	explicit CBegining_Credit(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBegining_Credit(const CBegining_Credit& rhs);
	virtual ~CBegining_Credit();

public:



public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

	void			Black_Print();
	void			Reset_Proj_Matrix();

private:
	CRcTex* m_pBufferCom;
	CTransform* m_pTransformCom;
	CTexture* m_pTextureCom[ROGO_END] = {};

	_float              m_iAlpha = 0;
	_float              m_Dir = 1;
	_float              m_fAccTime = 0.f;
	_float              m_fStartTime = 0.f;


	bool               m_Is_Alpha_Max = false;
	bool               m_Is_Start = false;
	
	_float              m_fNextTime = 0.f;

	ROGO_TYPE           m_Type = ROGO_JUSIN;
	int                 m_Type_Int = 0;

public:
	static CBegining_Credit* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

