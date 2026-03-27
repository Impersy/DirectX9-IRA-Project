#pragma once
#include "FrontUi.h"

class CMiniMap_Gage : public CFrontUi
{
private:
	explicit CMiniMap_Gage(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMiniMap_Gage(const CMiniMap_Gage& rhs);
	virtual ~CMiniMap_Gage();

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

public:
	static CMiniMap_Gage* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	virtual void Free(void) override;
};

