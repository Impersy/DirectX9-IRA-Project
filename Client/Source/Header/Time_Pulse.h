#pragma once
#include "Effect.h"
#include "Base.h"

class CTime_Pulse : public CEffect
{

private:
	explicit CTime_Pulse(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTime_Pulse(const CTime_Pulse& rhs);
	virtual ~CTime_Pulse();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	void    Update_State();


private:
	HRESULT			Add_Component(void);

public:
	static CTime_Pulse*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	_float				m_fAccTime = 0.f;

	_matrix m_MatWorld;

	_vec3   m_vPos;

	_vec3  m_Size = { 3.5f * PUBLIC_SCALE,3.5f * PUBLIC_SCALE,1.f };

	_vec3  m_OffSet = { 0.4f * PUBLIC_SCALE , 0.1f * PUBLIC_SCALE ,-2.0f };

	_float m_Dis = 6.f;


	_vec3   m_Bow_Pos;
	_float  m_Bow_Angle = 0.f;
	_float  m_Pre_Bow_Angle = 0.f;

	int m_iAlpha = 255;
	


};

