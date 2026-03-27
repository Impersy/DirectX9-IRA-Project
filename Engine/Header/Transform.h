#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	const _matrix*		Get_WorldMatrixPointer() { return &m_matWorld; }
	void				Get_Info(INFO eType, _vec3* pInfo)
	{
		memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
	}
	void				Move_Pos(const _vec3* const pDir)
	{
		m_vInfo[INFO_POS] += *pDir;
	}
	void				Move_Pos(const _vec3* pPos, const _float& fSpeed, const _float& fTimeDelta)
	{
		_vec3	vDir = *pPos - m_vInfo[INFO_POS];
		m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fTimeDelta * fSpeed;
	} 

	void	Rotation(ROTATION eRot, const _float& fAngle)
	{
		*(((_float*)&m_vAngle) + eRot) += fAngle;
	}
	void				Set_Pos(const 	_float& fX, const _float& fY, const _float& fZ)
	{
		m_vInfo[INFO_POS].x = fX;
		m_vInfo[INFO_POS].y = fY;
		m_vInfo[INFO_POS].z = fZ;
	}
	void				Set_Scale(const _float& fX, const _float& fY, const _float& fZ) { m_vScale.x = fX; m_vScale.y = fY; m_vScale.z = fZ; }
	void				Set_Angle(const _float& fX, const _float& fY, const _float& fZ) { m_vAngle.x = fX; m_vAngle.y = fY; m_vAngle.z = fZ; }
	void				Set_Right_Reverse()
	{
		m_vInfo[INFO_RIGHT] *= -1;
		
	}

	void                Set_Scale_Ratio(_vec3 scale) 
	{
		m_vScale.x /= fabs(m_vScale.x);
		m_vScale.x *= scale.x;

		m_vScale.y /= fabs(m_vScale.y);
		m_vScale.y *= scale.y;
	}

	void                Set_Scale_X_Ratio(_float x)
	{
		m_vScale.x *= x;
	}

	void  Set_Revolution(const _matrix& revol) {
		m_Revolution = revol;
	}


	void					Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta, const MONSTER_NAME m_eName);
	void					Bullet_Move(const _vec3 _Dir, const _float& fSpeed, const _float& fTimeDelta, _int iDirCount);
	void					Bullet_Move_Boss(const _vec3 _Dir, const _float& fSpeed, const _float& fTimeDelta, _int iDirCount);
	_vec3					Patrol_Map(const _float& fSpeed, const _float& fTimeDelta);
	void					Reverse_Scale_x(void);
	void					Scale_x(void);		// 원 추가
	void					UpdatePos_OnWorld(void);
	const	_matrix*		Compute_LookAtTarget(const _vec3* pTargetPos);

	void					Ui_Status_Print(_int _iHpNumber, _int _iUiNumber);
	void					Ui_Apostle_Print(_int _iApNumber, _int _iUiNumber);
	void					Ui_QuickSlot_Print(_int _iUiNumber);
	void					Ui_MiniMap_Print(_int _iUiNumber, SCENE_TYPE _eRoom_Name, _int _iRoomNumber, _vec3* _pPlayerPos = nullptr);
	void					Ui_BossBar_Print(_int _iUiNumber, BOSS_NAME _Name, _int _fHp = 0 );

	void					ColorBlend_Hit(_int iFrame);



	// 사도 스킬 UI Effect
	void                    Apostle_Skill_Time_Circle(_float size,_float angle);
	void                    Apostle_Skill_Time_illust(_float size_x, _float size_y ,_vec3 pos);

	void                    Arrow_Move(float yaw , float pich , float roll);

public:
	HRESULT		Ready_Transform(void);
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;

public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;
	_matrix         m_Revolution;

	
public:
	static CTransform*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void) override;

private:
	_vec3		m_vPatrolTarget;
	_vec3		m_vBulletTarget = {0, 0, 0};

private:

	virtual void Free(void) override;

};

END
