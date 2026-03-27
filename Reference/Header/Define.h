#pragma once

#define		WINCX		1600
#define		WINCY		900


#define		PUBLIC_SCALE  1.5f

#define		BOSS_1_TELEPORT  { 109.f + 257.f, 5.f, 43.f + 257.f }

#define		DUNGEONLOBBY_TELEPORT  { 384.f, 5.f, 384.f }

#define		TUTORIAL_TELEPORT  { 227.f, 5.f * PUBLIC_SCALE, 310.f }

#define		MAINLOBBY_TELEPORT  { 383.f,5.f * PUBLIC_SCALE,300.f }

#define     TIME_STOP_VOID \
            CLayer* pGameLogicLayer_Time = Engine::Get_Layer(L"Layer_GameLogic"); \
            if (pGameLogicLayer_Time->m_fTimeStop == true) \
             return;

#define     TIME_STOP_S_OK \
            CLayer* pGameLogicLayer_Time = Engine::Get_Layer(L"Layer_GameLogic"); \
            if (pGameLogicLayer_Time->m_fTimeStop == true) \
             return S_OK;

#define     CAMERA_ZOOM_IN \
CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera")); \
pCam->m_bZoom_In = true;

#define     CAMERA_ZOOM_IN_RETURN \
CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera")); \
pCam->m_bZoom_In = false; \
pCam->m_bZoom_In_Return = true;

#define     CAMERA_CHARGE_ZOOM_IN \
CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera")); \
pCam->m_bChargeZoom_In = true;

#define     CAMERA_CHARGE_ZOOM_IN_RETURN \
CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera")); \
pCam->m_bChargeZoom_In = false; \
pCam->m_bChargeZoom_In_Return = true;

#define     CAMERA_ZOOM_OUT \
CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera")); \
pCam->m_bZoom_Out = true;

#define     CAMERA_ZOOM_OUT_RETURN \
CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera")); \
pCam->m_bZoom_Out = false; \
pCam->m_bZoom_Out_Return = true;

#define     CAMERA_CHARGE_ZOOM_OUT \
CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera")); \
pCam->m_bChargeZoom_Out = true;

#define     CAMERA_CHARGE_ZOOM_OUT_RETURN \
CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera")); \
pCam->m_bChargeZoom_Out = false; \
pCam->m_bChargeZoom_Out_Return = true; 

#define     CAMERA_LEAN_X \
CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera")); \
pCam->m_bLean_X = true; \
pCam->m_bLean = true;

#define     CAMERA_LEAN_X_RETURN \
CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera")); \
pCam->m_bLean_X_Return = true; \
pCam->m_bLean = false;

#define     CAMERA_SHAKE(SHAKE_TIME) \
CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera")); \
pCam->m_bShaking = true; \
pCam-> m_Shake_AccTime = SHAKE_TIME;

#define     CAMERA_TARGET_CHANGE(WAIT_TIME) \
CStaticCamera* pCam = dynamic_cast<CStaticCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"StaticCamera"));\
pCam->m_bChange_Target = true; \
pCam->m_Cut_Scene_Wait_Time = WAIT_TIME;


#define		VTXCNTX		257
#define		VTXCNTZ		257
#define		VTXITV		1

#define		MIN_STR			64
#define		MAX_STR			256

#define		DOEWOLE_MAXHP 800000
#define		DOCHEOL_MAXHP 1300000

#define		OBJ_NOEVENT	 0
#define		OBJ_DEAD	 1 

#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}

#define			USING(NAMESPACE)	using namespace NAMESPACE;
//

#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif

#define NULL_CHECK( _ptr)	\
	{if( _ptr == 0){/*__asm*/ { int 3 };return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
	{if( _ptr == 0){/*__asm*/ { int 3 };return _return;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);/*__asm*/ { int 3 };}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);/*__asm*/ { int 3 };return _return;}}


#define  FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); /*__asm*/ { int 3 }; return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); /*__asm*/ { int 3 }; return _return;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); /*__asm*/ { int 3 };return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); /*__asm*/ { int 3 };return _return;}



#define NO_COPY(CLASSNAME)							\
		private:											\
		CLASSNAME(const CLASSNAME&) = delete;					\
		CLASSNAME& operator = (const CLASSNAME&) = delete;		

#define DECLARE_SINGLETON(CLASSNAME)				\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME*	m_pInstance;				\
		public:											\
		static CLASSNAME*	GetInstance( void );		\
		static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
		CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
		CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
			if(NULL == m_pInstance) {					\
				m_pInstance = new CLASSNAME;			\
			}											\
			return m_pInstance;							\
		}												\
		void CLASSNAME::DestroyInstance( void ) {		\
			if(NULL != m_pInstance)	{					\
				delete m_pInstance;						\
				m_pInstance = NULL;						\
			}											\
		}

