#pragma once

#include "Scene.h"
#include "Base.h"
#include "Terrain.h"

class CStage_Regen : public Engine::CScene
{
private:
	explicit CStage_Regen(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage_Regen();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Ready_Scene(void) override;
	virtual			_int		Update_Scene(const _float& fTimeDelta);
	virtual			void		LateUpdate_Scene(void);
	virtual			void		Render_Scene(void);

public:
	void			StartWave(void) { m_iCurWave = 1; }

private:
	HRESULT			Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT			Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT			Ready_LightInfo();
	HRESULT			Load_Terrain_Info(const _tchar* pPath);
	HRESULT			Load_Object_Info(const _tchar* pPath);
	HRESULT			Load_Dynamic_Object_Info(const _tchar* pPath);

public:

public:
	static	CStage_Regen*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
	void				StartWave1(const _float& fTimeDelta);
	void				StartWave2(const _float& fTimeDelta);
	void				StartWave3(const _float& fTimeDelta);
	void				StartWave4(const _float& fTimeDelta);
	void				StartWave5(const _float& fTimeDelta);
	void				StartTimeWave1(const _float& fTimeDelta);
	void				StartTimeWave2(const _float& fTimeDelta);
	void				Restrict_Monster_Activity_Scope(void);
	_bool				Is_In_Area(const _vec3& vPos);

	void				Check_PlayerPos();


	void			Door_Init();

	void			Door_Activate_Check();

private:
	_bool				m_bTerrainInit = false;
	_float				m_fAccumulatedTime = 0.f;
	_float				m_fAccumulatedTimeMonsterCnt = 0.f;
	_int				m_iMonsterCnt = 0;
	_int				m_iCurWave = 0;

	_float				m_fTimeWaveTime = 0.f;

	_bool				m_bMonsterSummoned[10] = {};
	_bool				m_bTimeWave1 = false;
	_bool				m_bTimeWave2 = false;
};

