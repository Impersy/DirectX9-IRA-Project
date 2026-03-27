#pragma once
#include "Landscape.h"
#include "Base.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
//class CCollider;

END

class CDynamicObject : public CLandscape
{
protected:
	explicit CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicObject(const CDynamicObject& rhs);
	virtual ~CDynamicObject();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	static DYNAMIC_OBJECT_ID	CompareID(wstring strObjKey);
	DYNAMIC_OBJECT_ID	Get_DynamicObj_ID(void) { return m_eID; }
	void				Set_DynamicObj_ID(DYNAMIC_OBJECT_ID id) { m_eID = id; }
	_int				Get_MaxFrameCnt(void); 
	void				Set_OriginPos(_vec3 vPos) { m_vOriginPos = vPos; }
	 
protected:
	virtual HRESULT	Add_Component(void)override;
	virtual void SetUp_OnTerrain(void) {}
	//virtual void Change_State(void)PURE;					
	//virtual void Frame_Check(const _float& fTimeDelta)PURE;

protected:
	CTexture*		m_pTextureCom[DYNAMIC_OBJ_END];
	DYNAMIC_OBJECT_ID	m_eID;
	_vec3			m_vOriginPos;

protected:
	virtual void Free(void) override;

};

