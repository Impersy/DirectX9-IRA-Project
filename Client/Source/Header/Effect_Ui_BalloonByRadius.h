#pragma once
#include "Effect.h"

class CEffect_Ui_BalloonByRadius : public CEffect
{
private:
	explicit CEffect_Ui_BalloonByRadius(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Ui_BalloonByRadius(const CEffect_Ui_BalloonByRadius& rhs);
	virtual ~CEffect_Ui_BalloonByRadius();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	void	SetUp_OnTerrain(void);
	void	TargetCheck_BalloonNum(void);

private:
	HRESULT			Add_Component(void);
	
public:
	static CEffect_Ui_BalloonByRadius* Create(LPDIRECT3DDEVICE9 pGraphicDev, INTERACT_TARGET _Target, _vec3 vPos);

private:
	_vec3			m_vPos;
	INTERACT_TARGET m_eInter_Target = TARGET_END;
	_vec3			m_vMasterPos = { 0.f, 0.f, 0.f };

	_int			m_BalloonNum = 0;

private:
	virtual void Free(void) override;
};

