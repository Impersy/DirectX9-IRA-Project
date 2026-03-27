#pragma once
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;

END

class CInventory : public Engine::CGameObject
{
protected:
	explicit CInventory(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInventory(const CInventory& rhs);
	virtual ~CInventory();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component(void);
	void	Change_State(void);
	void	Frame_Check_Close(const _float& fTimeDelta);
	void	Frame_Check_Open(const _float& fTimeDelta);

private:
	HRESULT	Create_InventoryUi(void);
	HRESULT Modify_InventoryUi(void);

public:
	static CInventory* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	CRcTex* m_pBufferCom;
	CTransform* m_pTransformCom;
	CTexture* m_pTextureCom;

	_float			m_fFrame;
	_bool			m_bCheck;

	PLAYER_INFO		m_tRecieveInfo_Pre;
	_float			m_fClose_Delay;
	_float			m_fOpen_Delay;


protected:
	virtual void Free(void) override;

};

