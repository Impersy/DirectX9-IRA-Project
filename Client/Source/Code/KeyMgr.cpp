#include "stdafx.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
    ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

CKeyMgr::~CKeyMgr()
{
}


void CKeyMgr::Update(void)
{
    m_iKeyState = 0;

    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        m_iKeyState |= KEY_UP;
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        m_iKeyState |= KEY_DOWN;
    }

    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        m_iKeyState |= KEY_LEFT;
    }

    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        m_iKeyState |= KEY_RIGHT;
    }

    if (GetAsyncKeyState(VK_RETURN) & 0x8000)
    {
        m_iKeyState |= KEY_RETURN;
    }

    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        m_iKeyState |= KEY_SPACE;
    }

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
    {
        m_iKeyState |= KEY_LBUTTON;
    }

    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
    {
        m_iKeyState |= KEY_RBUTTON;
    }

    if (GetAsyncKeyState('W') & 0x8000)
    {
        m_iKeyState |= KEY_W;
    }

    if (GetAsyncKeyState('S') & 0x8000)
    {
        m_iKeyState |= KEY_S;
    }

    if (GetAsyncKeyState('A') & 0x8000)
    {
        m_iKeyState |= KEY_A;
    }

    if (GetAsyncKeyState('D') & 0x8000)
    {
        m_iKeyState |= KEY_D;
    }

    if (GetAsyncKeyState('Q') & 0x8000)
    {
        m_iKeyState |= KEY_Q;
    }

    if (GetAsyncKeyState('F') & 0x8000)
    {
        m_iKeyState |= KEY_F;
    }

	if (GetAsyncKeyState('L') & 0x8000)
	{
		m_iKeyState |= KEY_L;
	}

    if (GetAsyncKeyState('P') & 0x8000)
    {
        m_iKeyState |= KEY_P;
    }

    if (GetAsyncKeyState('H') & 0x8000)
    {
        m_iKeyState |= KEY_H;
    }

    if (GetAsyncKeyState('C') & 0x8000)
    {
        m_iKeyState |= KEY_C;
    }

    if (GetAsyncKeyState('V') & 0x8000)
    {
        m_iKeyState |= KEY_V;
    }

    if (GetAsyncKeyState(VK_F1) & 0x8000)
    {
        m_iKeyState |= KEY_F1;
    }

    if (GetAsyncKeyState(VK_F2) & 0x8000)
    {
        m_iKeyState |= KEY_F2;
    }

    if (GetAsyncKeyState(VK_F3) & 0x8000)
    {
        m_iKeyState |= KEY_F3;
    }

    if (GetAsyncKeyState(VK_F4) & 0x8000)
    {
        m_iKeyState |= KEY_F4;
    }

    if (GetAsyncKeyState(VK_F8) & 0x8000)
    {
        m_iKeyState |= KEY_F8;
    }

    if (GetAsyncKeyState('1') & 0x8000)
    {
        m_iKeyState |= KEY_1;
    }

    if (GetAsyncKeyState('9') & 0x8000)
    {
        m_iKeyState |= KEY_9;
    }

    if (GetAsyncKeyState('0') & 0x8000)
    {
        m_iKeyState |= KEY_0;
    }

    if (GetAsyncKeyState('2') & 0x8000)
    {
        m_iKeyState |= KEY_2;
    }

    if (GetAsyncKeyState('3') & 0x8000)
    {
        m_iKeyState |= KEY_3;
    }

    if (GetAsyncKeyState('4') & 0x8000)
    {
        m_iKeyState |= KEY_4;
    }

    if (GetAsyncKeyState('T') & 0x8000)
    {
        m_iKeyState |= KEY_T;
    }

    if (GetAsyncKeyState('E') & 0x8000)
    {
        m_iKeyState |= KEY_E;
    }


}

bool CKeyMgr::Key_Pressing(unsigned int iCurrentKey)
{
    if (m_iKeyState & iCurrentKey)
        return true;

    return false;
}

bool CKeyMgr::Key_Down(unsigned int iCurrentKey)
{
    // 이전에 누른적 없고 현재 누른 경우 true
    if (!(iCurrentKey & m_iKeyWasDown) && (iCurrentKey & m_iKeyState))
    {
        m_iKeyWasDown |= iCurrentKey;
        return true;
    }

    // 이전에 누른적 있고 현재 누르지 않은 경우 false
    if ((iCurrentKey & m_iKeyWasDown) && !(iCurrentKey & m_iKeyState))
    {
        // iCurrentKey               => 0001
        // m_iKeyWasDown            => 0101
        // m_iKeyWasDown ^= iCurrentKey => 0100
        m_iKeyWasDown ^= iCurrentKey;
        return false;
    }

    return false;
}

bool CKeyMgr::Key_Up(unsigned int iCurrentKey)
{
    // 이전에 누른적 있고 현재 누르지 않은 경우 true
    if ((iCurrentKey & m_iKeyWasUp) && !(iCurrentKey & m_iKeyState))
    {
        m_iKeyWasUp ^= iCurrentKey;
        return true;
    }

    // 이전에 누른적 없고 현재 누른 경우 false
    if (!(iCurrentKey & m_iKeyWasUp) && (iCurrentKey & m_iKeyState))
    {
        m_iKeyWasUp |= iCurrentKey;
        return false;
    }

    return false;
}