#include "stdafx.h"
#include "..\Header\MyMouse.h"
#include "Export_Function.h"

CMyMouse::CMyMouse(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTransformCom(nullptr), m_pTextureCom(nullptr)
{

}

CMyMouse::CMyMouse(const CMyMouse& rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTransformCom(rhs.m_pTransformCom), m_pTextureCom(rhs.m_pTextureCom)
{

}

CMyMouse::~CMyMouse()
{
	Free();
}

HRESULT CMyMouse::Ready_GameObject(void)
{	
	Add_Component();
	Update_Mouse_Pos();

	return S_OK;
}

_int CMyMouse::Update_GameObject(const _float& fTimeDelta)
{	
	Update_Mouse_Pos();

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_BLACK, this);

	return OBJ_NOEVENT;
}

void CMyMouse::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMyMouse::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)0);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	Ui_Print();

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//TCHAR Buffer_X[256];
	//_stprintf_s(Buffer_X, _T("%d"), (_int)(m_vPrintMouse.x));
	//TCHAR Buffer_Y[256];
	//_stprintf_s(Buffer_Y, _T("%d"), (_int)(m_vPrintMouse.y));

	//std::wstring message;
	//message += L"X : ";
	//message += Buffer_X;
	//message += L" Y : ";
	//message += Buffer_Y;
	//Engine::Render_Font(L"Font_Title_3", message.c_str(), &_vec2(m_vMyMouse.x, m_vMyMouse.y), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

void CMyMouse::Update_Mouse_Pos(void)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);	// 전체 스크린 영역에서 클라이언트 좌표로 변환해주는 함수

	m_vMyMouse.x = (float)ptMouse.x;
	m_vMyMouse.y = (float)ptMouse.y;

	m_vPrintMouse.x = m_vMyMouse.x - 800.f;
	m_vPrintMouse.y = 450 - m_vMyMouse.y;
	ShowCursor(false);	// 마우스 커서를 보여줄지 말지 결정하는 함수
}

void CMyMouse::Ui_Print(void)
{
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 0.f, 1.f);

	matWorld._11 = 36.f;
	matWorld._22 = 36.f;
	matWorld._41 = m_vPrintMouse.x;
	matWorld._42 = m_vPrintMouse.y;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CMyMouse::Reset_Proj_Matrix()
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

HRESULT CMyMouse::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Mouse_Cursor_Img"));
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Mouse_Cursor_Img", pComponent });

	return S_OK;
}

CMyMouse* CMyMouse::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMyMouse* pInstance = new CMyMouse(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMyMouse::Free(void)
{
	__super::Free();
}
