
// DriveWatchDlg.cpp : implementation file
//

#include "pch.h"

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

CDriveWatchDlg::CDriveWatchDlg(
	bool bSkipRemovable,
	bool bSkipFixed,
	bool bSkipRemote,
	bool bSkipCdrom,
	bool bSkipRamDisk)
	: ParentClass(IDD_DRIVEWATCH_DIALOG, nullptr),
	bSkipRemovable_(bSkipRemovable),
	bSkipFixed_(bSkipFixed),
	bSkipRemote_(bSkipRemote),
	bSkipCdrom_(bSkipCdrom),
	bSkipRamDisk_(bSkipRamDisk),
	m_strFreeSpace(_T(""))
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
	ON_WM_QUERYENDSESSION()
END_MESSAGE_MAP()


// CDriveWatchDlg message handlers
BOOL CDriveWatchDlg::OnInitDialog()
{
	ParentClass::OnInitDialog();

	SetDuplicateInstanceData(DRIVEWATCH_MUTEX_NAME, (void*)m_hWnd);

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
			pSysMenu->AppendMenu(MF_STRING, IDM_TOPMOST, I18N(L"&Top most"));
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
	vector<wstring> lines;
	vector<wstring> titles;
	for (auto&& volume : volumes)
	{
		if (volume.paths.empty())
			continue;

		bool bSkip = false;
		if (bSkipRemovable_ ||
			bSkipFixed_ ||
			bSkipRemote_ ||
			bSkipCdrom_ ||
			bSkipRamDisk_)
		{
			switch (GetDriveType(volume.paths[0].c_str()))
			{
			case DRIVE_UNKNOWN:
				ASSERT(false);
				break;
			case DRIVE_NO_ROOT_DIR:
				ASSERT(false);
				break;
			case DRIVE_REMOVABLE:
				if (bSkipRemovable_)
					bSkip = true;
				break;
			case DRIVE_FIXED:
				if (bSkipFixed_)
					bSkip = true;
				break;
			case DRIVE_REMOTE:
				if (bSkipRemote_)
					bSkip = true;
				break;
			case DRIVE_CDROM:
				if (bSkipCdrom_)
					bSkip = true;
				bSkip = true;
				break;
			case DRIVE_RAMDISK:
				if (bSkipRamDisk_)
					bSkip = true;
				break;
			default:
				ASSERT(false);
				break;
			}
			if (bSkip)
				continue;
		}

		wstringstream line;
		wstringstream title;

		ULARGE_INTEGER userFreeSpace;
		ULARGE_INTEGER userTotal;
		ULARGE_INTEGER freeSpace;
		line << volume.paths[0].c_str() << L" ";

		if (!GetDiskFreeSpaceEx(volume.paths[0].c_str(),
			&userFreeSpace,
			&userTotal,
			&freeSpace))
		{
			line << L"Failed to get free space";
		}
		else
		{
			auto percent = (100.0 * userFreeSpace.QuadPart / userTotal.QuadPart);
			wstringstream wss;
			wss << FormatSizeof(userFreeSpace.QuadPart) << L" / " <<
				FormatSizeof(userTotal.QuadPart) << L" (" <<
				setprecision(3) <<
				percent << L"% free)";
			line << wss.str();

			wstringstream wssTitle;
			wssTitle << setprecision(3) << percent << L"%";
			title << wssTitle.str();
		}

		lines.push_back(line.str());
		titles.push_back(title.str());
	}
	titles.push_back(AfxGetAppName());
	SetWindowText(stdJoinStrings(titles, L" | ", L"", L"").c_str());

	std::sort(lines.begin(), lines.end());
	m_strFreeSpace = stdTrimEnd(stdJoinStrings(lines,L"\r\n", L"", L"")).c_str();
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
	else if (nID == IDM_TOPMOST)
	{
		SetWindowTopMost(m_hWnd, !IsWindowTopMost(m_hWnd));
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


BOOL CDriveWatchDlg::OnQueryEndSession()
{
	// CDialog::OnQueryEndSession() returns FALSE if the restart manager
	// is not activated
	//if (!CResizableDialog::OnQueryEndSession())
	//	return FALSE;
	return TRUE;
}


