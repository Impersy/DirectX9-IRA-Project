#pragma once

#define KEY_UP      0x00000001      /* 0000 0001 */
#define KEY_DOWN   0x00000002      /* 0000 0010 */
#define KEY_LEFT   0x00000004      /* 0000 0100 */
#define KEY_RIGHT   0x00000008      /* 0000 1000 */
#define KEY_RETURN   0x00000010      /* 0001 0000 */
#define KEY_SPACE   0x00000020      /* 0010 0000 */
#define KEY_LBUTTON   0x00000040      /* 0100 0000 */
#define KEY_RBUTTON   0x00000080      /* 1000 0000 */
#define KEY_W      0x00000100      /* 0001 0000 0000 */
#define KEY_S      0x00000200      /* 0010 0000 0000 */
#define KEY_A      0x00000400      /* 0100 0000 0000 */
#define KEY_D      0x00000800      /* 1000 0000 0000 */
#define KEY_Q      0x00001000      /* 1000 0000 0000 */
#define KEY_F      0x00002000      /* 1000 0000 0000 */
#define KEY_L      0x00004000      /* 1000 0000 0000 */
#define KEY_P      0x00008000      /* 1000 0000 0000 */
#define KEY_H      0x00010000      /* 1000 0000 0000 */
#define KEY_C      0x00020000      /* 1000 0000 0000 */
#define KEY_V      0x00040000      /* 1000 0000 0000 */
#define KEY_F1     0x00080000      /* 1000 0000 0000 */
#define KEY_F2     0x00100000      /* 1000 0000 0000 */
#define KEY_F3     0x00200000      /* 1000 0000 0000 */
#define KEY_F4     0x00400000      /* 1000 0000 0000 */
#define KEY_F8     0x00800000      /* 1000 0000 0000 */
#define KEY_1      0x01000000      /* 1000 0000 0000 */
#define KEY_9      0x02000000      /* 1000 0000 0000 */
#define KEY_0      0x04000000      /* 1000 0000 0000 */

#define KEY_2      0x08000000      /* 1000 0000 0000 */
#define KEY_3      0x10000000      /* 1000 0000 0000 */
#define KEY_4      0x20000000      /* 1000 0000 0000 */
#define KEY_T      0x40000000      /* 1000 0000 0000 */
#define KEY_E      0x80000000      /* 1000 0000 0000 */


#define VK_MAX 0xff

class CKeyMgr
{

private:
    CKeyMgr();
    ~CKeyMgr();

public:
    void         Update(void);

    bool         Key_Pressing(unsigned int iCurrentKey);
    bool         Key_Down(unsigned int iCurrentKey);      // 한 번만 메세지가 들어오는 함수
    bool         Key_Up(unsigned int iCurrentKey);      // 눌렀다가 뗐을 때 들어오는 함수


public:
    static      CKeyMgr* Get_Instance(void)
    {
        if (nullptr == m_pInstance)
            m_pInstance = new CKeyMgr;

        return m_pInstance;
    }

    static void      Destroy_Instance(void)
    {
        if (nullptr != m_pInstance)
        {
            delete m_pInstance;
            m_pInstance = nullptr;
        }
    }

private:
    static CKeyMgr* m_pInstance;
    bool      m_bKeyState[VK_MAX];

private:
    unsigned int m_iKeyWasDown = 0; // 이전에 down 처리한적 있냐
    unsigned int m_iKeyWasUp = 0; // 이전에 up 처리한적 있냐
    unsigned int m_iKeyState = 0;
};
