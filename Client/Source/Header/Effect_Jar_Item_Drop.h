#pragma once
#include "Effect.h"
#include "Base.h"

class CEffect_Jar_Item_Drop : public CEffect
{
private:
	explicit CEffect_Jar_Item_Drop(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Jar_Item_Drop(const CEffect_Jar_Item_Drop& rhs);
	virtual ~CEffect_Jar_Item_Drop();

public:
	virtual HRESULT Ready_GameObject(CGameObject* pOwner);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(void);

public:
	static CEffect_Jar_Item_Drop*	Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner);

private:
	virtual void Free(void) override;
	void	Create_Debris(void);

private:
	CGameObject*		m_pOwner;
	_float				m_fAccumulatedTime = 0.f;

};

