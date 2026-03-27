#pragma once
#include "FrontUi.h"

class CMiniMap_Character : public CFrontUi
{
private:
	explicit CMiniMap_Character(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMiniMap_Character(const CMiniMap_Character& rhs);
	virtual ~CMiniMap_Character();

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

	void			Text_Print(void);

public:
	static CMiniMap_Character* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	_vec3 m_vPlayerPos;

private:
	virtual void Free(void) override;
};

