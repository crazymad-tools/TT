
// ChatRoom.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CChatRoomApp: 
// �йش����ʵ�֣������ ChatRoom.cpp
//

class CChatRoomApp : public CWinApp
{
public:
	CChatRoomApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CChatRoomApp theApp;