#pragma once
#include "FrontUi.h"

class CQuickSlot_WeaponImg : public CFrontUi
{
private:
	explicit CQuickSlot_WeaponImg(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CQuickSlot_WeaponImg(const CQuickSlot_WeaponImg& rhs);
	virtual ~CQuickSlot_WeaponImg();

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
	static CQuickSlot_WeaponImg* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber);

private:
	virtual void Free(void) override;

private:
	BOW_TYPE	m_eRevieve_Bow_Type = BOW_END;
};

