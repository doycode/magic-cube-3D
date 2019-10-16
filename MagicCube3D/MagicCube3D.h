
// MagicCube3D.h : MagicCube3D Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "OpenGL.h"

// CMagicCube3DApp:
// �йش����ʵ�֣������ MagicCube3D.cpp
//

class CMagicCube3DApp : public CWinAppEx
{
public:
	CMagicCube3DApp();

	COpenGL openGL;

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnIdle(LONG lCount);
};

extern CMagicCube3DApp theApp;
