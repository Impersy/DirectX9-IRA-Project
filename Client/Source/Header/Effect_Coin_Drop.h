#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Coin_Drop : public CEffect
{

private:
	explicit CEffect_Coin_Drop(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Coin_Drop(const CEffect_Coin_Drop& rhs);
	virtual ~CEffect_Coin_Drop();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);
	
public:
	static CEffect_Coin_Drop* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* _pPos);

private:
	_vec3	m_vPos;
	_vec3*	m_pReceive_CoinPos;

private:
	virtual void Free(void) override;
};

