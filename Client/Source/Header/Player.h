#pragma once
#include "GameObject.h"
#include "Base.h"
#include "InventoryMgr.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CCalculator;
class CCollider;
class CSphereTex;

END

class CPlayer : public Engine::CGameObject
{
	enum {PLAYER_HEAD,PLAYER_FACE,PLAYER_END};

	enum {STAND,RUN,MOVE_ATTACK,STAND_ATTACK,DEATH,DASH,TELEPORT,LANDING,STATE_END};

	enum { STAND_000, STAND_045, STAND_090, STAND_135, STAND_180, STAND_END };
	enum { RUN_000, RUN_045, RUN_090, RUN_135, RUN_180, RUN_END };
	enum { MOVE_ATTACK_045,MOVE_ATTACK_135, MOVE_ATTACK_END };
	enum { STAND_ATTACK_045,STAND_ATTACK_135,STAND_ATTACK_END };
	enum { DASH_045, DASH_135, DASH_END };
	enum { ANGLE_000, ANGLE_045, ANGLE_090, ANGLE_135, ANGLE_180, ANGLE_225, ANGLE_270, ANGLE_315, ANGLE_STATE_END };
	enum { ATTACK_ANGLE_045, ATTACK_ANGLE_135, ATTACK_ANGLE_225, ATTACK_ANGLE_315, ATTACK_ANGLE_STATE_END };
	

protected:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;


public:
	_float Get_Dash_Frame(void) {
		return m_fDashFrame;
	}
	int    Get_Anlge_State(void) {
		return m_iAngleState;
	}

	_float Get_Run_Frame(void) {
		return m_fRunFrame;
	}

	const PLAYER_INFO&  Get_Player_Info(void) {
		return m_tInfo;
	}

	void Set_Player_Info(const PLAYER_INFO& tInfo) {
		m_tInfo = tInfo;
	}

	void Teleport(void);
		
	void Landing(_vec3 pos);




	// 준석 추가 (인벤토리, 상호작용 용도)
	void			Set_bItemUi_On(_bool _OnOff) { m_bItemUi_On = _OnOff; } 
	_bool			Get_bItemUi_On(void) { return m_bItemUi_On; }
	void			Set_bBigMapUi_On(_bool _OnOff) { m_bBigMapUi_On = _OnOff; }
	_bool			Get_bBigMapUi_On(void) { return m_bBigMapUi_On; }
	void			Set_bDialog_On(_bool _OnOff) { m_bDialog_On = _OnOff; } 
	_bool			Get_bDialog_On(void) { return m_bDialog_On; }
	void			Set_bLevelUpUi_On(_bool _OnOff) { m_bLevelUpUi_On = _OnOff; }
	_bool			Get_bLevelUpUi_On(void) { return m_bLevelUpUi_On; }
	CInventoryMgr*	Get_InvenInfo(void) { return m_pInventory; }
	PLAYER_LVSK		Get_tPlayerLvSk(void) { return m_tPlayerLvSk; }
	void			Set_tPlayerLvSk(_int _Number);
	_int			Get_Apostle_Count(void) { return m_Apostle_Count; }

	void			Save_Player_InvenInfo(void);
	void			Load_Player_InvenInfo(void);
	void			Set_Key(void) { m_tInfo.Key += 1; }
		

	void			Set_Player_Dialog_Target(INTERACT_TARGET _SetTarget) { m_bPlayer_Dialog_Target = _SetTarget; }
	INTERACT_TARGET Get_Player_Dialog_Target(void) { return m_bPlayer_Dialog_Target; }

	void			Set_Player_Money(MONEY_VARIATION _Money_Vari, _int _MoneyValue);	// 머니 변화량만큼 플레이어 인포 수정 및 텍스트 출력
	void			Set_Player_Money_Print(void);	// 머니 변화량만큼 플레이어 STATUS 텍스트 출력

	void			Set_Player_Gem(GEM_VARIATION _Gem_Vari, _int _GemValue);	// 젬 변화량만큼 플레이어 인포 수정 및 텍스트 출력
	void			Set_Player_Gem_Print(void);	// 젬 변화량만큼 플레이어 STATUS 텍스트 출력

	BOW_TYPE		Get_Player_BowType(void) { return m_Bow_Type; }
	APOSTLE_TYPE	Get_Player_ApostleType(void) { return m_Apostle_Type; }

private:
	HRESULT			Add_Component(void);

	void			Key_Input(const _float& fTimeDelta);

	void			SetUp_OnTerrain(void);

	_vec3			Picking_OnTerrain(void);

	void            Update_State();

	_vec3           Get_World_Mouse();

	void            Dash(const _float& fTimeDelta);

	void            Fire_Arrow(void);;

	void            Fire_Charge_Arrow(void);

	void            Effect_Charge_Arrow(void);

	void            Effect_Foot_Step_Smoke(void);
	void            Effect_Foot_Step_Water(void);

	void            Frame_Manage(const _float& fTimeDelta);

	void            Hp_Down(void)
	{
		m_tInfo.Nowhp--;

		if (m_tInfo.Nowhp < 0) {
			m_tInfo.Nowhp = 0;
		}

		m_bHit = false;
	}

	void            Apostle_Skill_Effect_Time(void);
	void            Apostle_Skill_Effect_Courage(void);
	void            Apostle_Skill_Effect_Night(void);


	void            Apostle_Manage(void);
	void			Player_Level_Up(void);


protected:
	CRcTex*			m_pBufferCom;
	CTransform*		m_pTransformCom;

	CTexture*       m_pStandTextureCom[STAND_END] = {};
	CTexture*       m_pRunTextureCom[RUN_END] = {};

	CTexture*       m_pMoveAttackTextureCom[MOVE_ATTACK_END] = {};
	CTexture*       m_pStandAttackTextureCom[STAND_ATTACK_END] = {};

	CTexture*       m_pDashTextureCom[DASH_END] = {};

	CTexture*		m_pDeathTextureCom;

	CTexture*		m_pTeleportTextureCom;
	CTexture*		m_pLandingTextureCom;

	CCollider*		m_pColliderCom;
	CCalculator*	m_pCalculatorCom;

	_bool			m_bItemUi_On;
	_bool			m_bBigMapUi_On = false;
	_bool			m_bLevelUpUi_On = false;
	_bool			m_bDialog_On;
	INTERACT_TARGET m_bPlayer_Dialog_Target = TARGET_END;

	_float			m_fMoney_Vari_Frame = 0.f;
	_float			m_fMoney_Vari_Frame_Max = 40.f;

	_float			m_fGem_Vari_Frame = 0.f;
	_float			m_fGem_Vari_Frame_Max = 40.f;

	_bool			m_fMoney_Vari = false;
	_bool			m_fGem_Vari = false;

	MONEY_VARIATION m_tRecieve_Vari = MONEY_END;
	GEM_VARIATION	m_tRecieve_Vari_GEM = GEM_END;
	_int			m_iRecieve_Money_Value = 0;
	_int			m_iRecieve_Gem_Value = 0;


	PLAYER_LVSK		m_tPlayerLvSk = { 1, 0, 0, 1, 0 }; // 레벨, 공격력, 스피드, 머니, 젬 추가 

private:	
	_float			m_fDashSpeed = 1.2f * PUBLIC_SCALE;

	_float			m_fStandFrame = 0.f;
	_float			m_fRunFrame = 0.f;
	_float			m_fAttackFrame = 0.f;
	_float			m_fDashFrame = 0.f;
	_float			m_fDeathFrame = 0.f;
	_float			m_fTeleportFrame = 0.f;
	_float			m_fLandingFrame = 0.f;
	


	_int            m_iState = STAND;
	_int            m_iPreState = STAND;

	_int            m_iAngleState = ANGLE_000;
	_int            m_iPreAngleState = ANGLE_000;

	_int            m_iAttackAngleState = ATTACK_ANGLE_045;
	_int            m_iPreAttackAngleState = ATTACK_ANGLE_045;

	
	bool            m_bAttackReverse = false;

	_float          m_fAngle = 0.f;

	_float          m_Dash_Time = 0.f;
	bool            m_Is_Dash = false;
	bool            m_Is_Jump = false;
	bool            m_Is_Run = false;
	float           jump = 0.f;
	float           jumpDir = 1.f;

	bool            m_Is_Fire_Arrow = false;
	_float          m_Fire_Frame = 3.f;
	_float          m_Fire_Init = 4.f;

	bool            m_Is_Effect_Charge_Arrow = false;
	int             m_Charge_Effect_Cnt = 0;
	bool            m_Is_Charge_Arrow = false;
	_float			m_fChargeFrame = 0.f;


	CSphereTex*		m_pSphereBufferCom;

	_vec3           m_Mouse_Dir;
	

	// Foot
	_float          m_FootFrame = 0.f;
	bool            m_FootRev = false;

	
public:
	_float          m_Fire_Speed = 4.f;
	_float			m_fSpeed = 25.f * PUBLIC_SCALE;

	bool            m_bReverse = false;


	bool            m_bHit = false;
	bool            m_bImmuned = false;
	bool            m_Is_Render = true;
	float           m_Critical_Rate = 38.f;


	// 타임
	_float          m_Time_Stop_Frame = 0.f;
	bool            m_Is_Time_Stop = false;

    // 루
	_float          m_Memory_Frame = 0.f;
	bool            m_Is_Memory = false;
	// 용기
	_float          m_Courage_Frame = 0.f;
	bool            m_Is_Courage = false;
	// 밤
	_float          m_Night_Frame = 0.f;
	bool            m_Is_Night= false;

	// 활 정보
	BOW_TYPE        m_Bow_Type = BOW_SYLPH;
	ARROW_TYPE      m_Arrow_Type = ARROW_TYPE_SYLPH;

	CInventoryMgr* m_pInventory;

	// 사도스킬
	APOSTLE_TYPE    m_Apostle_Type;
	int             m_Apostle_Type_Int = 0;
	int             m_Apostle_Proof;
	int				m_Apostle_Count = 3;


	_float          m_Scene_Time = 0.f;
	bool            m_Scene_Landing = false;
private:
	// UI 정보
	PLAYER_INFO     m_tInfo;

	// Hit 관련
	bool            m_HitBlend = false;
	bool            m_HitBlendRender = false;
	float           m_HitBlendFrame = 0.f;
	float           m_HitMaxFrame = 2.f;
	int             m_HitBlendCnt = 0;
	int             m_HitBlendMaxCnt = 7;

	// Teleport - Landing
	bool            m_Is_Teleport = false;
	bool            m_Is_Landing = false;
	_float          m_fLanding_Time = 0.f;
	_float          m_fLanding_Cnt = 0.f;
	

	DWORD           AlphaValue = 50;
	DWORD           R = 255;
	DWORD           G = 255;
	DWORD           B = 255;

	// 타임 활 스킬 
	_float          m_Alert_Time = 0.f;
	bool            m_Is_Alert = false;


	bool            m_Is_Charge_Sound = false;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void) override;

};

