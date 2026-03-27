#pragma once
#include "Bow.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;


END

class CSylphBow : public CBow
{

	
private:
	explicit CSylphBow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSylphBow(const CSylphBow& rhs);
	virtual ~CSylphBow();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void    Update_Bow_State();

	void    Set_Render(bool render) {
		m_bRender = render;
	}

private:
	virtual HRESULT	Add_Component(void) override;					
	

public:
	static CSylphBow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float Angle);


	void Sylph_Frame(const _float& fTimeDelta);
	void Fire_Frame(const _float& fTimeDelta);
	void Night_Frame(const _float& fTimeDelta);
	void Time_Frame(const _float& fTimeDelta);



private:
	virtual void Free(void) override;

private:	
	bool   m_bRender = false;
	bool   m_bRealRender = false;
	float  m_Time = 3.f;

	float  m_fFrame = 0.f;
	float  m_fFireFrame = 0.f;
	float  m_fNightFrame = 0.f;
	float  m_fTimeFrame = 0.f;

	int    m_iAlpha = 0;

public:
	_matrix m_MatWorld;
	_vec3   m_vPos;
	bool   m_bCharge;


	_vec3  m_Size = { -2.6f * PUBLIC_SCALE,2.6f * PUBLIC_SCALE,1.f };

	_vec3  m_OffSet = { -0.2f * PUBLIC_SCALE ,2.6f * PUBLIC_SCALE ,0.f };

	_float m_Dis = 3.f;

	_float m_Y_Set = 1.3f;

	BOW_TYPE m_Bow_Type = BOW_SYLPH;



};

