
// DriveWatchDlg.cpp : implementation file
//

#include "pch.h"
#include <sstream>
#include <string>
#include "framework.h"
#include "DriveWatch.h"
#include "AboutDlg.h"
#include "DriveWatchDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

CDriveWatchDlg::CDriveWatchDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRIVEWATCH_DIALOG, pParent)
	, m_strFreeSpace(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDriveWatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_FREESPACE, m_strFreeSpace);
}

BEGIN_MESSAGE_MAP(CDriveWatchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDriveWatchDlg message handlers

BOOL CDriveWatchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetTimer(1, 1000, nullptr);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CDriveWatchDlg::OnTimer(UINT_PTR nIDEvent)
{
	ULARGE_INTEGER userFreeSpace;
	ULARGE_INTEGER userTotal;
	ULARGE_INTEGER freeSpace;
	if (!GetDiskFreeSpaceEx(L"C:\\",
		&userFreeSpace,
		&userTotal,
		&freeSpace))
	{
		m_strFreeSpace = L"Failed to get free space";
	}
	else
	{
		wstringstream wss;
		wss << userFreeSpace.QuadPart << L" " <<
			userTotal.QuadPart;
		m_strFreeSpace = wss.str().c_str();
	}
	UpdateData(FALSE);
	return;

	CDialogEx::OnTimer(nIDEvent);
}

void CDriveWatchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDriveWatchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDriveWatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



