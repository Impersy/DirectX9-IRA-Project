#pragma once
#include "Apostle_Skill_UI.h"

class CStory_Book : public CApostle_Skill_UI
{
private:
	explicit CStory_Book(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStory_Book(const CStory_Book& rhs);
	virtual ~CStory_Book();

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
	static CStory_Book* Create(LPDIRECT3DDEVICE9 pGraphicDev, _float size);

private:
	virtual void Free(void) override;


private:
	_float m_Size = 0.f;
	_float m_InitSize = 0.f;
	_float m_fAccTime = 0.f;
	_float dir = 3.f;
	_float m_Angle = 0.f;
	int    m_iAlpha = 255.f;
	


};

