#pragma once
#include "Effect.h"

class CEffect_TestEffect : public CEffect
{
private:
	explicit CEffect_TestEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_TestEffect(const CEffect_TestEffect& rhs);
	virtual ~CEffect_TestEffect();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	void	Ui_Print(void);
	void	Reset_Proj_Matrix(void);

private:
	HRESULT			Add_Component(void);
	
public:
	static CEffect_TestEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
	_vec3			m_vPos;
	PLAYER_INFO		pReceiveInfo;

private:
	virtual void Free(void) override;
};

