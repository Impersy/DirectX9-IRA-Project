HRESULT			Create_Management(CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();
	if (nullptr == pManagement) return E_FAIL;

	*ppManagement = pManagement;

	return S_OK;
}

CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

CGameObject*		Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	return CManagement::GetInstance()->Get_GameObject(pLayerTag, pObjTag);
}

CLayer* Get_Layer(const _tchar* pLayerTag)
{
	return CManagement::GetInstance()->Get_Layer(pLayerTag);
}

CScene* Get_Scene(void)
{
	return CManagement::GetInstance()->Get_Scene();
}

HRESULT			Set_Scene(CScene* pScene)
{
	return	CManagement::GetInstance()->Set_Scene(pScene);
}

_int				Update_Management(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Management(fTimeDelta);
}
void				LateUpdate_Management()
{
	CManagement::GetInstance()->LateUpdate_Management();
}
void				Render_Management(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement::GetInstance()->Render_Management(pGraphicDev);
}

void				Delete_Scene()
{
	CManagement::GetInstance()->Delete_Scene();
}

HRESULT			Ready_ProtoComponent(const _tchar* pProtoTag, CComponent* pComponent)
{
	return CComponentMgr::GetInstance()->Ready_ProtoComponent(pProtoTag, pComponent);
}
CComponent*		Clone_ProtoComponent(const _tchar* pProtoTag)
{
	return CComponentMgr::GetInstance()->Clone_ProtoComponent(pProtoTag);
}

void Delete_ProtoComponent()
{
	CComponentMgr::GetInstance()->Delete_ProtoComponent();
}

void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eID, pGameObject);
}
void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void		Clear_RenderGroup(void)
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}
HRESULT			Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

void				Release_Utility(void)
{
	CLightMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CComponentMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}