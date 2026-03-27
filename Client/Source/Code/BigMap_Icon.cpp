#include "stdafx.h"
#include "..\Header\BigMap_Icon.h"
#include "Export_Function.h"

#include "Player.h"

CBigMap_Icon::CBigMap_Icon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFrontUi(pGraphicDev)
{
}

CBigMap_Icon::CBigMap_Icon(const CBigMap_Icon& rhs)
	: CFrontUi(rhs)
{
}

CBigMap_Icon::~CBigMap_Icon()
{
	Free();
}

HRESULT CBigMap_Icon::Ready_GameObject(void)
{
	Add_Component();
	m_eCategory = UI_FRONT;

	return S_OK;
}

_int CBigMap_Icon::Update_GameObject(const _float& fTimeDelta)
{	
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	_bool bInvenClose = pPlayer->Get_bBigMapUi_On();

	if (bInvenClose == false)
		return OBJ_DEAD;

	if (m_iIcon_Number == 0) // 아이콘이 플레이어일 때
	{	
		_vec3 vPlayerPos;
		CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
		pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);
		m_vPlayerPos = vPlayerPos;

		m_ePlayer_Loc = g_eCurScene;
		m_fFrame = 0;
	}
	else
	{
		Frame_Check(fTimeDelta);
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBigMap_Icon::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBigMap_Icon::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrixPointer());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	_matrix matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	if (m_iIcon_Number == 0)
		Player_Loc_Check(m_ePlayer_Loc , &m_vPlayerPos);

	if (m_iIcon_Number > 0)
		Ui_Print(m_iIcon_Number);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matCamWorld);
	Reset_Proj_Matrix();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBigMap_Icon::Add_Component(void)
{
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_ProtoComponent(L"Proto_RcTex"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_ProtoComponent(L"Proto_Transform"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_ProtoComponent(L"Proto_Texture_Ui_BigMap_Min_Icon"));
	 
	m_uMapComponent[ID_STATIC].insert({ L"Proto_Texture_Ui_BigMap_Min_Icon", pComponent });

	return S_OK;
}

void CBigMap_Icon::Change_State(void)
{

}

void CBigMap_Icon::Frame_Check(const _float& fTimeDelta)
{	
	m_fFrame = (_float)m_iIcon_Number;
}

void CBigMap_Icon::Ui_Print(_int _iNum)
{

	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 12.f;
	matWorld._22 = 12.f;

	switch (_iNum)
	{
	case 1:	// 사도공간
		matWorld._41 = -300.f;
		matWorld._42 = 90.f;
		break;
	case 2:	// 상점 (상인)
		matWorld._41 = -75.f;
		matWorld._42 = 15.f;
		break;
	case 3:	// 상점 (대장장이)
		matWorld._41 = -125.f;
		matWorld._42 = 15.f;
		break;
	case 4:	// 던전로비(베이스)
		matWorld._41 = 0.f;
		matWorld._42 = -10.f;
		break;
	case 5:	// 노말맵
		matWorld._41 = 100.f;
		matWorld._42 = -10.f;
		break;
	case 6:	// 항아리맵
		matWorld._41 = 100.f;
		matWorld._42 = -110.f;
		break;
	case 7:	// 리젠
		matWorld._41 = 200.f;
		matWorld._42 = -110.f;
		break;
	case 8:	// 보스1
		matWorld._41 = 0.f;
		matWorld._42 = 90.f;
		break;
	case 9:	// 보스2
		matWorld._41 = 0.f;
		matWorld._42 = 190.f;
		break;
	case 10:	// 상점 (포탈)
		matWorld._41 = -100.f;
		matWorld._42 = -15.f;
		break;
	case 11:	// 튜토리얼맵
		matWorld._41 = -300.f;
		matWorld._42 = 190.f;
		break;

	default:
		break;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

_bool CBigMap_Icon::BigMap_Close(void)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	if (!(pPlayer->Get_bBigMapUi_On()))
	{
		return true;
	}

	return true;
}

void CBigMap_Icon::Player_Loc_Check(SCENE_TYPE _Scene, _vec3* _pPlayerPos)
{	
	_matrix		matWorld, matScale, matRot, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixOrthoLH(&matProj, 1600.f, 900.f, 0.f, 1.f);

	matWorld._11 = 12.f;
	matWorld._22 = 12.f;

	switch (_Scene)
	{
	case SCENE_TUTORIAL:
		matWorld._41 = -345.f + _float((_pPlayerPos->x - 257.f) / 2.85);
		matWorld._42 = 145.f  + _float((_pPlayerPos->z - 257.f) / 2.85);
		break;
	case SCENE_MAINLOBBY:
		matWorld._41 = -345.f + _float((_pPlayerPos->x - 257.f) / 2.85);
		matWorld._42 = 45.f   + _float((_pPlayerPos->z - 257.f) / 2.85);
		break;
	case SCENE_DUNGEONLOBBY:
		matWorld._41 = -45.f + _float((_pPlayerPos->x - 257.f) / 2.85);
		matWorld._42 = -55.f + _float((_pPlayerPos->z - 257.f) / 2.85);
		break;
	case SCENE_REGEN:
		matWorld._41 = 245.f + _float((_pPlayerPos->x - 257.f) / 2.85);
		matWorld._42 = -55.f + _float((_pPlayerPos->z - 257.f) / 2.85);
		break;
	case SCENE_SHOP:
		matWorld._41 = -145.f + _float((_pPlayerPos->x - 257.f) / 2.85);
		matWorld._42 = -55.f  + _float((_pPlayerPos->z - 257.f) / 2.85);
		break;
	case SCENE_BOSS1:
		matWorld._41 = -45.f + _float((_pPlayerPos->x - 257.f) / 2.85);
		matWorld._42 = 45.f  + _float((_pPlayerPos->z - 257.f) / 2.85);
		break;
	case SCENE_BOSS2:
		matWorld._41 = -45.f + _float((_pPlayerPos->x - 257.f) / 2.85);
		matWorld._42 = 145.f + _float((_pPlayerPos->z - 257.f) / 2.85);
		break;
	case SCENE_POT:
		matWorld._41 =   55.f + _float((_pPlayerPos->x - 257.f) / 2.85);
		matWorld._42 = -155.f + _float((_pPlayerPos->z - 257.f) / 2.85);
		break;
	case SCENE_NORMAL:
		matWorld._41 = 55.f  + _float((_pPlayerPos->x - 257.f) / 2.85);
		matWorld._42 = -55.f + _float((_pPlayerPos->z - 257.f) / 2.85);
		break;
	case SCENE_END:
		break;
	default:
		break;
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CBigMap_Icon::Reset_Proj_Matrix(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
}

CBigMap_Icon* CBigMap_Icon::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _IconNumber)
{
	CBigMap_Icon* pInstance = new CBigMap_Icon(pGraphicDev);

	pInstance->m_iIcon_Number = _IconNumber;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBigMap_Icon::Free(void)
{
	__super::Free();
}
