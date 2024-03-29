
// DriveWatchDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "DriveWatch.h"
#include "AboutDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Ambiesoft;

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
, m_strAppVersion(_T(""))
{
	m_strAppVersion.Format(L"%s v%s",
		AfxGetAppName(),
		GetVersionStringFromResource(nullptr, 3).c_str());
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_APPVERSION, m_strAppVersion);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

