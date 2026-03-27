#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CBigMap : public Engine::CGameObject
{
protected:
	explicit CBigMap(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBigMap(const CBigMap& rhs);
	virtual ~CBigMap();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component(void);
	void	Change_State(void);
	void	Frame_Check(const _float& fTimeDelta);


private:

	HRESULT Create_BigMapUi(void);
	void Frame_Check_Close(const _float& fTimeDelta);
	void Frame_Check_Open(const _float& fTimeDelta);


public:
	static CBigMap* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;

	_float			m_fFrame;
	_bool			m_bCheck;

	_float			m_fClose_Delay;
	_float			m_fOpen_Delay;


protected:
	virtual void Free(void) override;

};

