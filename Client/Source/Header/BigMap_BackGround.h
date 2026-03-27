#pragma once
#include "FrontUi.h"

class CBigMap_BackGround : public CFrontUi
{
private:
	explicit CBigMap_BackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBigMap_BackGround(const CBigMap_BackGround& rhs);
	virtual ~CBigMap_BackGround();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	virtual HRESULT	Add_Component(void) override;
	virtual void	Change_State(void) override;
	virtual void	Frame_Check(const _float& fTimeDelta) override;
	virtual void	Reset_Proj_Matrix(void) override;

	void			Ui_Print(_int _iNum);
	_bool			BigMap_Close(void);

public:
	static CBigMap_BackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

