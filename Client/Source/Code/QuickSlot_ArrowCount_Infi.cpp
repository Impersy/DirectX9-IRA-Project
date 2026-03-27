#include "stdafx.h"
#include "..\Header\QuickSlot_ArrowCount_Infi.h"
#include "Export_Function.h"

CQuickSlot_ArrowCount_Infi::CQuickSlot_ArrowCount_Infi(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
	//m_tINFO[0]->iMax = 5;
}

CQuickSlot_ArrowCount_Infi::CQuickSlot_ArrowCount_Infi(const CQuickSlot_ArrowCount_Infi& rhs)
	: CFrontUi(rhs)
{
}

CQuickSlot_ArrowCount_Infi::~CQuickSlot_ArrowCount_Infi()
{
}

HRESULT CQuickSlot_ArrowCount_Infi::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;
	return S_OK;
}

_int CQuickSlot_ArrowCount_Infi::Update_GameObject(const _float& fTimeDelta)
{
	Frame_Check(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CQuickSlot_ArrowCount_Infi::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CQuickSlot_ArrowCount_Infi::Render_GameObject()
{
	if (!g_UI_Render)
		return;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	m_pTransformCom->Ui_QuickSlot_Print(3); //서순 주의

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//CDevice::Get_Instance()->Get_Sprite()->Draw(pTextInfo->pTexture,
	//	nullptr, // 출력할 이미지 영역에 대한 RECT 주소
	//	&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // 출력할 이미지의 중심 축에 대한 Vec3 포인터(null인 경우 0,0이 중심 좌표)
	//	nullptr, // 이미지 출력 위치 vec3 포인터
	//	D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 원본 이미지와 섞을 색상 값, 0xffffffff를 넣어주면 원본 색상값을 유지
}

HRESULT CQuickSlot_ArrowCount_Infi::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_QuickSlot_Infi"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_QuickSlot_Infi", pComponent });

	return S_OK;
}

void CQuickSlot_ArrowCount_Infi::Change_State(void)
{

}

void CQuickSlot_ArrowCount_Infi::Frame_Check(const _float& fTimeDelta)
{
	m_fFrame += 8.f * fTimeDelta;

	if (8.f < m_fFrame)
	{
		m_fFrame = 0.f;
		m_bCheck = false;
	}
}

void CQuickSlot_ArrowCount_Infi::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CQuickSlot_ArrowCount_Infi* CQuickSlot_ArrowCount_Infi::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iNumber)
{
	CQuickSlot_ArrowCount_Infi* pInstance = new CQuickSlot_ArrowCount_Infi(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	pInstance->m_tINFO.iNum = _iNumber;

	return pInstance;
}

void CQuickSlot_ArrowCount_Infi::Free(void)
{
	__super::Free();
}
