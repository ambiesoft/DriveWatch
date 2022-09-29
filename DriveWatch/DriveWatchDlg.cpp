
// DriveWatchDlg.cpp : implementation file
//

#include "pch.h"
#include <sstream>
#include <string>
#include <iomanip>
#include "../../lsMisc/GetVolumeInfo.h"
#include "../../lsMisc/stdosd/stdosd.h"

#include "framework.h"
#include "DriveWatch.h"
#include "AboutDlg.h"
#include "DriveWatchDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

CDriveWatchDlg::CDriveWatchDlg(CWnd* pParent /*=nullptr*/)
	: ParentClass(IDD_DRIVEWATCH_DIALOG, pParent)
	, m_strFreeSpace(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDriveWatchDlg::DoDataExchange(CDataExchange* pDX)
{
	ParentClass::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_FREESPACE, m_strFreeSpace);
}

BEGIN_MESSAGE_MAP(CDriveWatchDlg, ParentClass)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDriveWatchDlg message handlers




BOOL CDriveWatchDlg::OnInitDialog()
{
	ParentClass::OnInitDialog();

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

	AddAnchor(IDC_STATIC_FREESPACE, TOP_LEFT, BOTTOM_RIGHT);

	LoadWindowLocation(*this, theApp.GetIniPath().c_str());

	SetTimer(1, 10000, nullptr);
	OnTimer(1);
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CDriveWatchDlg::OnTimer(UINT_PTR nIDEvent)
{
	std::vector<VolumeInfo> volumes;
	GetVolumeInfo(&volumes);
	wstringstream allText;
	wstringstream allTitle;
	for (auto&& volume : volumes)
	{
		if (volume.paths.empty())
			continue;
		ULARGE_INTEGER userFreeSpace;
		ULARGE_INTEGER userTotal;
		ULARGE_INTEGER freeSpace;
		allText << volume.paths[0].c_str() << L" ";

		if (!GetDiskFreeSpaceEx(volume.paths[0].c_str(),
			&userFreeSpace,
			&userTotal,
			&freeSpace))
		{
			allText << L"Failed to get free space";
		}
		else
		{
			auto percent = (100.0 * userFreeSpace.QuadPart / userTotal.QuadPart);
			wstringstream wss;
			wss << FormatSizeof(userFreeSpace.QuadPart) << L" / " <<
				FormatSizeof(userTotal.QuadPart) << L" (" <<
				setprecision(3) <<
				percent << L"% free)";
			allText << wss.str();

			wstringstream wssTitle;
			wssTitle << setprecision(3) << percent << L"%" <<
				L" | " << AfxGetAppName();
			allTitle << wssTitle.str();
		}
		allText << L"\r\n";
	}
	SetWindowText(allTitle.str().c_str());
	m_strFreeSpace = stdTrimEnd(allText.str()).c_str();
	UpdateData(FALSE);
	return;

	ParentClass::OnTimer(nIDEvent);
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
		ParentClass::OnSysCommand(nID, lParam);
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
		ParentClass::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDriveWatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDriveWatchDlg::OnDestroy()
{
	if (!SaveWindowLocation(*this, theApp.GetIniPath().c_str()))
	{
		AfxMessageBox(I18N(L"Failed to save to ini file"));
	}
	ParentClass::OnDestroy();
}
