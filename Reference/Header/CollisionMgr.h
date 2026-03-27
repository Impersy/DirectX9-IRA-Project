#pragma once

#include "Base.h"
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr :public CBase
{
private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	static void	Collision_Sphere(unordered_map<const _tchar*, CGameObject*> _Dest, unordered_map<const _tchar*, CGameObject*> _Src);
	static bool Check_Sphere(CGameObject* pDest, CGameObject* pSrc);


};

END