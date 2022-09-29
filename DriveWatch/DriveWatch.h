
// DriveWatch.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CDriveWatchApp:
// See DriveWatch.cpp for the implementation of this class
//

class CDriveWatchApp : public CWinApp
{
public:
	CDriveWatchApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	std::wstring GetIniPath() const;

	DECLARE_MESSAGE_MAP()
};

extern CDriveWatchApp theApp;
