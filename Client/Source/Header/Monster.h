#pragma once
#include "GameObject.h"
#include "Base.h"
#include "Fire_Brave.h"
#include "SoundMgr.h"


BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;

END

class CMonster_Shadow;

class CMonster : public Engine::CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	//_float	Get_Hp(void) { return m_fHp; }
	_bool  Get_Dead(void) {
		return m_bDead;
	}


public:
	CTransform* Get_Transform(void) { return m_pTransformCom; }
	MONSTER_STATE	Get_State(void) { return m_eState; }
	_vec3*	 Get_Dir(void) { return &m_vDir; }
	void	Set_Dir(_vec3 vDir) { m_vDir = vDir; }
	void	Set_SoundChannel(_byte byChannel) { m_bySoundChannel = byChannel; }
	CSoundMgr::CHANNELID Get_SoundChannel(void);

	CTransform* Get_TransCom(void) {
		return m_pTransformCom;
	}
	void	Set_Shadow(CMonster_Shadow* pShadow) { m_pShadow = pShadow; }

protected:
	// 준석 수정 (23.03.02)
	virtual HRESULT	Add_Component(void)PURE;				
	virtual void SetUp_OnTerrain(void)PURE;					
	virtual void Change_State(void)PURE;					
	virtual void Frame_Check(const _float& fTimeDelta)PURE;

	void			GetDamage_Update(const _float& fTimeDelta);



protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;
	CTexture*		m_pTextureCom;
	CTexture*		m_pTextureCom_2;

	CCollider*		m_pColliderCom;
	CCalculator*	m_pCalculatorCom;
	
	_float			m_fSpeed;
	_float			m_fFrame;
	_bool			m_bCheck;
	_int			m_iMonsterHp;
	_int			m_HitCount;

	MONSTER_STATE	m_eState;
	MONSTER_NAME	m_eName;

	_vec3			m_vDir;

	_int			m_iMonsterMaxHP = 0;
	_int			m_iMonsterCurHP = 0;

	_byte			m_bySoundChannel = 1;

	_float			m_fDamageTimeDelta = 0.f;

	CMonster_Shadow* m_pShadow = nullptr;

public:
	_bool			m_bHit = false;
	list<int>		m_Damage_List;

protected:
	virtual void Free(void) override;

};

