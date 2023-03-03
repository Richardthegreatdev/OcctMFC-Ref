
// OCCViewer.h : main header file for the OCCViewer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COCCViewerApp:
// See OCCViewer.cpp for the implementation of this class
//

class COCCViewerApp : public CWinAppEx
{
public:
	COCCViewerApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	int SelectTemplate();
	afx_msg void OnFileOpen();
};

extern COCCViewerApp theApp;
