#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "ComponentMgr.h"
#include "Renderer.h"
#include "LightMgr.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"
#include "SphereTex.h"

#include "Transform.h"
#include "Texture.h"
#include "Camera.h"
#include "Calculator.h"
#include "Pipeline.h"
#include "Collider.h"

BEGIN(Engine)

// Management
inline	HRESULT			Create_Management(CManagement** ppManagement);

inline CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline CGameObject*		Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
inline CLayer*			Get_Layer(const _tchar* pLayerTag);
inline CScene*			Get_Scene(void);
inline HRESULT			Set_Scene(CScene* pScene);
inline _int				Update_Management(const _float& fTimeDelta);
inline void				LateUpdate_Management();
inline void				Render_Management(LPDIRECT3DDEVICE9 pGraphicDev);
inline void				Delete_Scene();

// ComponentMgr
inline HRESULT			Ready_ProtoComponent(const _tchar* pProtoTag, CComponent* pComponent);
inline CComponent*		Clone_ProtoComponent(const _tchar* pProtoTag);
inline void				Delete_ProtoComponent();

// Renderer
inline void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void		Clear_RenderGroup(void);

// LightMgr
inline 	HRESULT			Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);


inline void				Release_Utility(void);

#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
