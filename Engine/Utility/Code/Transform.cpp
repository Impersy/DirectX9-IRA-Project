#include "..\..\Header\Transform.h"


CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_Revolution);
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_vScale(rhs.m_vScale)
	, m_vAngle(rhs.m_vAngle)
	, m_matWorld(rhs.m_matWorld)
	, m_Revolution(rhs.m_Revolution)
{
	for (size_t i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];
}

CTransform::~CTransform()
{
}

HRESULT CTransform::Ready_Transform(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (size_t i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	
	return S_OK;
}

_int Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (size_t i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// 크기 변환
	for (size_t i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	// 회전 변환
	_matrix			matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (size_t i = 0; i < INFO_POS; ++i)
	{
		for (size_t j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (size_t i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	return 0;
}

void Engine::CTransform::LateUpdate_Component()
{

}


void CTransform::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta, MONSTER_NAME m_eName)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	switch (m_eName)
	{
	case NAME_SLIME:
		if (!(((vDir.x > -2.0f) && (vDir.x < 2.0f)) && ((vDir.z > -2.0f) && (vDir.z < 2.0f))))
			m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
		break;

	case NAME_SOUL:
		if (!(((vDir.x > -15.0f) && (vDir.x < 15.0f)) && ((vDir.z > -15.0f) && (vDir.z < 15.0f))))
			m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
		break;
	case SOUL_BULLET:
		if (!(((vDir.x > -1.0f) && (vDir.x < 1.0f)) && ((vDir.z > -1.0f) && (vDir.z < 1.0f))))
			m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
		break;

	default:
		break;
	}
	_matrix		matScale, matRot, matTrans;

	if (m_eName != SOUL_BULLET)
	{
		D3DXMatrixScaling(&matScale, 5.f, 5.f, 1.f);
	}
	if (m_eName == SOUL_BULLET)
	{
		D3DXMatrixScaling(&matScale, 2.f, 3.f, 1.f);
	}
	D3DXMatrixTranslation(&matTrans,
		m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y,
		m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matTrans;

}

void CTransform::Bullet_Move(const _vec3 _Dir, const _float& fSpeed, const _float& fTimeDelta, _int iDirCount = 0)
{
	_vec3		vNull(0, 0, 0);

	if (vNull == m_vBulletTarget && 0 == iDirCount)
	{
		m_vBulletTarget = _Dir;
	}
	if (vNull != m_vBulletTarget && 0 == iDirCount)
	{
		m_vInfo[INFO_POS] += *D3DXVec3Normalize(&m_vBulletTarget, &m_vBulletTarget) * fSpeed * fTimeDelta;
	}

	if (vNull == m_vBulletTarget && 0 != iDirCount)
	{
		_vec3 Temp;
		Temp.x = float(cos(45 * iDirCount));
		Temp.y = 0.f;
		Temp.z = float(sin(45 * iDirCount));

		if (8 == iDirCount)
		{
			Temp.x = 0.f;
			Temp.y = 0.f;
			Temp.z = float(sin(45 * iDirCount));
		}
		if (8 == iDirCount)
		{
			Temp.x = 0.f;
			Temp.y = 0.f;
			Temp.z = float(sin(45 * iDirCount));
		}

		if (7 == iDirCount)
		{
			Temp.x = 0.f;
			Temp.y = 0.f;
			Temp.z = float(sin(180));
		}

		m_vBulletTarget = Temp;
	}
	if (vNull != m_vBulletTarget && 0 != iDirCount)
	{
		m_vInfo[INFO_POS] += *D3DXVec3Normalize(&m_vBulletTarget, &m_vBulletTarget) * fSpeed * fTimeDelta;
	}

	_matrix		matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, 3.f, 3.f, 1.f);

	if (0 == iDirCount)
	{
		D3DXMatrixTranslation(&matTrans,
			m_vInfo[INFO_POS].x,
			m_vInfo[INFO_POS].y,
			m_vInfo[INFO_POS].z);

		m_matWorld = matScale * matTrans;
	}

	if (0 != iDirCount)
	{
		_float Temp_Degree = 0;

		switch (iDirCount)
		{
		case 1:
			Temp_Degree = 135;
			break;
		case 2:
			Temp_Degree = 45;
			break;
		case 3:
			Temp_Degree = 0;
			break;
		case 4:
			Temp_Degree = 315;
			break;
		case 5:
			Temp_Degree = 225;
			break;
		case 6:
			Temp_Degree = 180;
			break;
		case 7:
			Temp_Degree = 270;
			break;
		case 8:
			Temp_Degree = 90;
			break;

		default:
			break;
		}

		_float	fRadian_Bullet = D3DXToRadian(Temp_Degree);

		D3DXMatrixRotationYawPitchRoll(&matRot, fRadian_Bullet, D3DXToRadian(90), 0.f);

		D3DXMatrixTranslation(&matTrans,
			m_vInfo[INFO_POS].x,
			m_vInfo[INFO_POS].y,
			m_vInfo[INFO_POS].z);

		m_matWorld = matScale * matRot * matTrans;
	}
}

void CTransform::Bullet_Move_Boss(const _vec3 _Dir, const _float& fSpeed, const _float& fTimeDelta, _int iDirCount)
{
	_vec3		vNull(0, 0, 0);

	if (vNull == m_vBulletTarget && 0 != iDirCount)
	{
		_vec3 Temp;
		Temp.x = float(cos(45 * iDirCount));
		Temp.y = 0.f;
		Temp.z = float(sin(45 * iDirCount));

		if (8 == iDirCount)
		{
			Temp.x = 0.f;;
			Temp.y = 0.f;
			Temp.z = float(sin(45 * iDirCount));
		}
		if (8 == iDirCount)
		{
			Temp.x = 0.f;;
			Temp.y = 0.f;
			Temp.z = float(sin(45 * iDirCount));
		}

		if (7 == iDirCount)
		{
			Temp.x = 0.f;;
			Temp.y = 0.f;
			Temp.z = float(sin(180));
		}

		m_vBulletTarget = Temp;
	}
	if (vNull != m_vBulletTarget && 0 != iDirCount)
	{
		m_vInfo[INFO_POS] += *D3DXVec3Normalize(&m_vBulletTarget, &m_vBulletTarget) * fSpeed * fTimeDelta;
	}

	_matrix		matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 1.f);

	if (0 == iDirCount)
	{
		D3DXMatrixTranslation(&matTrans,
			m_vInfo[INFO_POS].x,
			m_vInfo[INFO_POS].y + 3.f,
			m_vInfo[INFO_POS].z);

		m_matWorld = matScale * matTrans;
	}

	if (0 != iDirCount)
	{
		_float Temp_Degree = 0;

		switch (iDirCount)
		{
		case 1:
			Temp_Degree = 135;
			break;
		case 2:
			Temp_Degree = 45;
			break;
		case 3:
			Temp_Degree = 0;
			break;
		case 4:
			Temp_Degree = 315;
			break;
		case 5:
			Temp_Degree = 225;
			break;
		case 6:
			Temp_Degree = 180;
			break;
		case 7:
			Temp_Degree = 270;
			break;
		case 8:
			Temp_Degree = 90;
			break;

		default:
			break;
		}

		_float	fRadian_Bullet = D3DXToRadian(Temp_Degree);
		//D3DXMatrixRotationY(&matRot, fRadian_Bullet);
		//D3DXMatrixRotationYawPitchRoll(&matRot, fRadian_Bullet, 0.f, 0.f);

		D3DXMatrixTranslation(&matTrans,
			m_vInfo[INFO_POS].x,
			m_vInfo[INFO_POS].y + 3.f,
			m_vInfo[INFO_POS].z);

		m_matWorld = matScale * matTrans;
	}
}

_vec3 CTransform::Patrol_Map(const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3	vTemp;
	_vec3	vNull(0, 0, 0);

	if (vNull == m_vPatrolTarget)
	{
		vTemp.x = (_float(rand() % 200) + 257.f);
		vTemp.y = m_vInfo[INFO_POS].y;
		vTemp.z = (_float(rand() % 200) + 257.f);

		_vec3		vDir_Temp = vTemp - m_vInfo[INFO_POS];

		if (((3 <= vDir_Temp.x) || (-3 >= vDir_Temp.x)) || ((3 <= vDir_Temp.z) || (-3 >= vDir_Temp.z)))
		{
			m_vPatrolTarget = vTemp;
		}
	}

	if (vNull != m_vPatrolTarget)
	{
		_vec3		vDir = m_vPatrolTarget - m_vInfo[INFO_POS];

		if (((0.5f >= vDir.x) && (-0.5f <= vDir.x)) && ((0.5f >= vDir.z) && (-0.5f <= vDir.z)))
		{
			m_vPatrolTarget = vNull;
		}
		else
		{
			m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;

			_matrix		matScale, matRot, matTrans;

			D3DXMatrixScaling(&matScale, 5.f, 5.f, 1.f);
			D3DXMatrixTranslation(&matTrans,
				m_vInfo[INFO_POS].x,
				m_vInfo[INFO_POS].y,
				m_vInfo[INFO_POS].z);

			m_matWorld = matScale * matTrans;

			return vDir;
		}
	}

	return vNull;
}

const _matrix* CTransform::Compute_LookAtTarget(const _vec3* pTargetPos)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	/*
		_vec3		vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir);

		_vec3		vUp;

		_float		fDot = D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]));

		_matrix		matRot;

		D3DXMatrixRotationAxis(&matRot, &vAxis, acosf(fDot));

		return &matRot;*/


	_matrix		matRot;
	_vec3		vAxis, vUp;

	return D3DXMatrixRotationAxis(&matRot,
		D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir),
		acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
			D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}

void CTransform::Ui_Status_Print(_int _iHpNumber, _int _iUiNumber)
{
	_matrix		matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	switch (_iUiNumber)
	{
	case 1:
		m_matWorld._11 = 0.55f;
		m_matWorld._22 = 0.55f;
		m_matWorld._41 = -(18.6f - ((_iHpNumber * 1.2f) - 1.2f));
		m_matWorld._42 = 10.3f - ((_iUiNumber - 1) * 1.2);
		break;
	case 2:
		m_matWorld._11 = 0.6f;
		m_matWorld._22 = 0.6f;
		m_matWorld._41 = -(18.6f - ((_iHpNumber * 1.2f) - 1.2f));
		m_matWorld._42 = 10.3f - ((_iUiNumber - 1) * 1.2);
		break;
	case 3:
		m_matWorld._11 = 0.65f;
		m_matWorld._22 = 0.65f;
		m_matWorld._41 = -18.9f;
		m_matWorld._42 = 10.25f - ((_iUiNumber - 1) * 1.2);
		break;
	case 4:
		m_matWorld._11 = 0.65f;
		m_matWorld._22 = 0.65f;
		m_matWorld._41 = -18.9f;
		m_matWorld._42 = 10.25f - ((_iUiNumber - 1) * 1.2);
		break;
	case 5:
		m_matWorld._11 = 0.65f;
		m_matWorld._22 = 0.65f;
		m_matWorld._41 = -18.9f;
		m_matWorld._42 = 10.25f - ((_iUiNumber - 1) * 1.2);
		break;
	case 6:
		m_matWorld._22 = 2.6f;

		if (3 > _iHpNumber)
		{
			m_matWorld._11 = 3.3f;
			m_matWorld._41 = -16.2f;
		}
		if (3 <= _iHpNumber)
		{	
			m_matWorld._11 = 2.6f;
			m_matWorld._41 = -16.84f;
		}
		m_matWorld._42 = 10.2f - ((_iHpNumber * 1.2f) - 1.2f);
		break;

	default:
		break;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

}

void CTransform::Ui_Apostle_Print(_int _iApNumber, _int _iUiNumber)
{
	_matrix		matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	switch (_iUiNumber)
	{
	case 1:	// Frame
		m_matWorld._11 = 6.1f;
		m_matWorld._22 = 2.5f;
		m_matWorld._41 = -13.4f;
		m_matWorld._42 = -8.2f;
		break;
	case 2: // Pattern
		m_matWorld._11 = 1.7f;
		m_matWorld._22 = 1.7f;
		m_matWorld._41 = -17.2f;
		m_matWorld._42 = -8.0f;
		break;
	case 3: // Frame_Key_Q
		m_matWorld._11 = 0.4f;
		m_matWorld._22 = 0.4f;
		m_matWorld._41 = -17.2f;
		m_matWorld._42 = -6.0f;
		break;
	case 4: // Count
		m_matWorld._11 = 0.7f;
		m_matWorld._22 = 0.7f;
		m_matWorld._41 = -14.5f + ((_iApNumber * 1.55f) - 1.55f);
		m_matWorld._42 = -9.2f;
		break;
	case 5:	// Frame_Back
		m_matWorld._11 = 6.1f;
		m_matWorld._22 = 2.5f;
		m_matWorld._41 = -13.4f;
		m_matWorld._42 = -8.2f;
		break;
	case 6:	//Glow
		m_matWorld._11 = 1.7f;
		m_matWorld._22 = 1.7f;
		m_matWorld._41 = -17.2f;
		m_matWorld._42 = -8.0f;
		break;

	default:
		break;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CTransform::Ui_QuickSlot_Print(_int _iUiNumber)
{
	_matrix		matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	switch (_iUiNumber)
	{
	case 1:
		m_matWorld._11 = 2.4f;
		m_matWorld._22 = 2.4f;
		m_matWorld._41 = 16.9f;
		m_matWorld._42 = -7.7f;
		break;
	case 2:
		m_matWorld._11 = 1.7f;
		m_matWorld._22 = 1.7f;
		m_matWorld._41 = 17.3f;
		m_matWorld._42 = -7.4f;
		break;
	case 3:
		m_matWorld._11 = 0.6f;
		m_matWorld._22 = 0.6f;
		m_matWorld._41 = 17.3f;
		m_matWorld._42 = -9.63f;
		break;
	case 4:
		m_matWorld._41 = -15.7f;
		m_matWorld._42 = 7.3f;
		break;

	default:
		break;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CTransform::Ui_MiniMap_Print(_int _iUiNumber, SCENE_TYPE _eRoom_Name, _int _iRoomNumber, _vec3* _pPlayerPos)
{
	_matrix		matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	switch (_iUiNumber)
	{
	case 1:
		m_matWorld._11 = 2.f;
		m_matWorld._22 = 2.f;
		m_matWorld._41 = 17.2f;
		m_matWorld._42 = 8.1f;
		break;
	case 2:
		m_matWorld._11 = 2.8f;
		m_matWorld._22 = 2.8f;
		m_matWorld._41 = 17.3f;
		m_matWorld._42 = 10.7f;
		break;
	case 3:
		m_matWorld._11 = 0.3f;
		m_matWorld._22 = 0.3f;

		if (_eRoom_Name == SCENE_BOSS2)
		{
			m_matWorld._41 = 16.7f + _float(_pPlayerPos->x / VTXCNTX);		// 17.2 기준 (0.5 / 16.7 / 17.7)	// 반경 1로 잡고(1600 / 40 * 1)(900 / 22.5 * 1)
			m_matWorld._42 = 7.7f + _float(_pPlayerPos->z / VTXCNTZ);		// 8.2 기준 (0.5 / 7.7 / 8.7)		(기준 값이 )
		}
		else
		{
			m_matWorld._41 = 16.7f + _float((_pPlayerPos->x - 257.f) / VTXCNTX);		// 17.2 기준 (0.5 / 16.7 / 17.7)	// 반경 1로 잡고(1600 / 40 * 1)(900 / 22.5 * 1)
			m_matWorld._42 = 7.7f + _float((_pPlayerPos->z - 257.f) / VTXCNTZ);		// 8.2 기준 (0.5 / 7.7 / 8.7)		(기준 값이 )
		}

		break;

	case 4:
		m_matWorld._11 = 0.6f;
		m_matWorld._22 = 0.6f;
		
		if (1 == _iRoomNumber)
		{
			m_matWorld._41 = 15.9f;
			m_matWorld._42 = 9.4f;
		}
		else if (2 == _iRoomNumber) // 중앙 상단 2번
		{
			m_matWorld._41 = 17.2f;
			m_matWorld._42 = 9.4f;
		}
		else if (3 == _iRoomNumber) // 중앙 상단 3번
		{
			m_matWorld._41 = 18.5f;
			m_matWorld._42 = 9.4f;
		}
		else if (4 == _iRoomNumber) // 지금 제일좌측
		{
			m_matWorld._41 = 15.9f;
			m_matWorld._42 = 8.1f;
		}
		else if (5 == _iRoomNumber)
		{
			m_matWorld._41 = 17.2f;	// 중앙 상단(5번)
			m_matWorld._42 = 8.1f;
		}
		else if (6 == _iRoomNumber) // 우측
		{
			m_matWorld._41 = 18.5f;
			m_matWorld._42 = 8.1f;
		}
		else if (7 == _iRoomNumber) //중앙 하단 8번
		{
			m_matWorld._41 = 15.9f;
			m_matWorld._42 = 6.8f;
		}
		else if (8 == _iRoomNumber) //중앙 하단 8번
		{
			m_matWorld._41 = 17.2f;
			m_matWorld._42 = 6.8f;
		}
		else if (9 == _iRoomNumber) //중앙 하단 9번
		{
			m_matWorld._41 = 18.5f;
			m_matWorld._42 = 6.8f;
		}
		break;

	default:
		break;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CTransform::Ui_BossBar_Print(_int _iUiNumber, BOSS_NAME _Name, _int _iHp)
{
	_matrix		matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 40.f, 22.5f, 0.f, 1.f);

	switch (_iUiNumber)
	{
	case 1:
		m_matWorld._11 = 6.f;
		m_matWorld._22 = 0.22f;
		m_matWorld._41 = 1.2f;
		m_matWorld._42 = 9.1f;
		break;
	case 2:
		m_matWorld._11 = 5.9f;
		m_matWorld._22 = 0.14f;
		m_matWorld._41 = 1.2f;
		m_matWorld._42 = 9.1f;
		break;
	case 3:
		m_matWorld._11 = 1.6f;
		m_matWorld._22 = 1.6f;
		m_matWorld._41 = -5.9f;
		m_matWorld._42 = 9.4f;
		break;
	case 4:
		break;

	default:
		break;
	}

	if (_iUiNumber == 4 && (_Name == BOSS_DOEWOLE))
	{
		_float fTempHp;
		fTempHp = _iHp / (DOEWOLE_MAXHP/100);
		float HP_percentage = fTempHp * 0.01f; // 현재 HP의 비율 (0.0f ~ 1.0f)

		float HP_bar_width = 5.9f; // HP바 전체 너비
		float HP_bar_left_boundary = 1.2f; // HP바 중심
		float HP_bar_current_width = HP_percentage * HP_bar_width; // 현재 HP에 맞는 HP바 너비

		float HP_bar_left_offset = ((HP_bar_width - HP_bar_current_width)); // HP바의 왼쪽 경계를 이동

		m_matWorld._11 = HP_bar_current_width; // HP바의 너비를 조절
		m_matWorld._22 = 0.14f;
		m_matWorld._41 = HP_bar_left_boundary - HP_bar_left_offset;
		m_matWorld._42 = 9.1f;
	}
	if (_iUiNumber == 4 && (_Name == BOSS_DOCHEOL))
	{
		_float fTempHp;
		fTempHp = _iHp / (DOCHEOL_MAXHP / 100);
		float HP_percentage = fTempHp * 0.01f; // 현재 HP의 비율 (0.0f ~ 1.0f)

		float HP_bar_width = 5.9f; // HP바 전체 너비
		float HP_bar_left_boundary = 1.2f; // HP바 중심
		float HP_bar_current_width = HP_percentage * HP_bar_width; // 현재 HP에 맞는 HP바 너비

		float HP_bar_left_offset = ((HP_bar_width - HP_bar_current_width)); // HP바의 왼쪽 경계를 이동

		m_matWorld._11 = HP_bar_current_width; // HP바의 너비를 조절
		m_matWorld._22 = 0.14f;
		m_matWorld._41 = HP_bar_left_boundary - HP_bar_left_offset;
		m_matWorld._42 = 9.1f;
	}

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

}

void CTransform::ColorBlend_Hit(_int iFrame)
{
	DWORD AlphaValue;
	AlphaValue = D3DCOLOR_ARGB(200, 255, 255, 255);

	if (3 == iFrame)
	{
		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(200, 255, 5, 5));
		m_pGraphicDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
		m_pGraphicDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	}
	if (3 != iFrame)
	{
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDDIFFUSEALPHA);
	}
}

void CTransform::Apostle_Skill_Time_Circle(_float size , _float angle)
{

	_matrix		matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);


	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);


	m_matWorld._11 = size;
	m_matWorld._22 = size;
	m_matWorld._41 = 0.f;
	m_matWorld._42 = 0.f;


	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CTransform::Apostle_Skill_Time_illust(_float size_x,_float size_y , _vec3 pos)
{
	_matrix		matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);


	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);


	m_matWorld._11 = size_x;
	m_matWorld._22 = size_y;
	m_matWorld._41 = pos.x;
	m_matWorld._42 = pos.y;


	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

}

void CTransform::Arrow_Move(float yaw, float pich, float roll)
{
	D3DXMATRIX		matScale, matRot, matTrans;


	D3DXMatrixRotationYawPitchRoll(&matRot, yaw, pich, roll);
	D3DXMatrixTranslation(&matTrans,
		m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y,
		m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;


}

void CTransform::Reverse_Scale_x(void)
{
	_matrix		matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, -5.f, 5.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y,
		m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matTrans;
}

void CTransform::Scale_x(void)
{
	_matrix		matScale, matRot, matTrans;

	D3DXMatrixScaling(&matScale, 5.f, 5.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		m_vInfo[INFO_POS].x,
		m_vInfo[INFO_POS].y,
		m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matTrans;
}

void CTransform::UpdatePos_OnWorld(void)
{
	D3DXMatrixIdentity(&m_matWorld);
	memcpy(&m_matWorld.m[INFO_POS][0], &m_vInfo[INFO_POS], sizeof(_vec3));
}

void Engine::CTransform::Free(void)
{
	__super::Free();
}

CTransform * CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform *	pInstance = new CTransform(pGraphicDev);

	if (FAILED(pInstance->Ready_Transform()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	
	return pInstance;
}

Engine::CComponent* Engine::CTransform::Clone(void)
{
	return new CTransform(*this);
}
