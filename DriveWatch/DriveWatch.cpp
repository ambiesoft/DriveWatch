
// DriveWatch.cpp : Defines the class behaviors for the application.
//

#include "pch.h"

#include "../../lsMisc/CommandLineParser.h"

#include "framework.h"
#include "DriveWatch.h"
#include "DriveWatchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

// CDriveWatchApp

BEGIN_MESSAGE_MAP(CDriveWatchApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()



// CDriveWatchApp construction

CDriveWatchApp::CDriveWatchApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDriveWatchApp object

CDriveWatchApp theApp;


// CDriveWatchApp initialization

BOOL CDriveWatchApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// keep m_pszProfileName active
	free((void*)m_pszProfileName);
	m_pszProfileName = _tcsdup(GetIniPath().c_str());

	{
		CCommandLineParser parser(
			I18N(L"Monitor free drive space"),
			AfxGetAppName());
		parser.setStrict();

		bool bHelp = false;
		std::wstring strLaunchIf;

		parser.AddOptionRange({ L"-h",L"/h",L"/?",L"--help" },
			ArgCount::ArgCount_Zero,
			&bHelp,
			ArgEncodingFlags_Default,
			I18N(L"Shows help"));

		parser.AddOption(L"--launch-if",
			ArgCount::ArgCount_One,
			&strLaunchIf,
			ArgEncodingFlags_Default,
			I18N(L"Start application if the free space is in specified condition met. ex) --launch-if <10G"));

		try
		{
			parser.Parse();
		}
		catch (CCommandLineParserException& ex)
		{
			AfxMessageBox(ex.wwhat().c_str());
			return FALSE;
		}

		if (bHelp)
		{
			MessageBox(nullptr,
				parser.getHelpMessage().c_str(),
				AfxGetAppName(),
				MB_ICONINFORMATION);
			return FALSE;
		}

		if (!strLaunchIf.empty())
		{
			enum class LAUNCH_IF_INEQUALITY {
				UNINITIALIZED,
				LESS,
				GREATER,
			} iniequality = LAUNCH_IF_INEQUALITY::UNINITIALIZED;

			// find first letter
			switch (strLaunchIf[0])
			{
			case L'<':
				iniequality = LAUNCH_IF_INEQUALITY::LESS;
				break;
			case L'>':
				iniequality = LAUNCH_IF_INEQUALITY::GREATER;
				break;
			default:
				AfxMessageBox(I18N(L"First letter of the argument of '--launch-if' must be '<' or '>'."));
				return FALSE;
			}

			std::wstring arg = strLaunchIf.substr(1);
			if (arg.empty())
			{
				AfxMessageBox(I18N(L"The argument of '--launch-if' is empty."));
				return FALSE;
			}
			int nSign;
			int64_t threadhold;
			stdGetUnittedSize(arg, &nSign, &threadhold);// stdFromString<int64_t>(arg.c_str());
			if (threadhold < 0)
			{
				AfxMessageBox(I18N(L"The argument of '--launch-if' is minus."));
				return FALSE;
			}

			auto fnDecideLaunch = [&]() {
				do {
					std::vector<VolumeInfo> volumes;
					GetVolumeInfo(&volumes);
					for (auto&& volume : volumes)
					{
						if (volume.paths.empty())
							continue;
						ULARGE_INTEGER userFreeSpace;
						ULARGE_INTEGER userTotal;
						ULARGE_INTEGER freeSpace;

						if (GetDiskFreeSpaceEx(volume.paths[0].c_str(),
							&userFreeSpace,
							&userTotal,
							&freeSpace))
						{
							switch (iniequality)
							{
							case LAUNCH_IF_INEQUALITY::LESS:
								if (freeSpace.QuadPart < (ULONGLONG)threadhold)
								{
									// Go launch
									return true;
								}
								continue;

							case LAUNCH_IF_INEQUALITY::GREATER:
								if (freeSpace.QuadPart > (ULONGLONG)threadhold)
								{
									// Go launch
									return true;
								}
								continue;
							default:
								ASSERT(false);
							}
						}
					}
					return false;
				} while (false);
				};
			
			if (!fnDecideLaunch())
				return FALSE;
		}
	}

	CDriveWatchDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

std::wstring CDriveWatchApp::GetIniPath() const
{
	return stdCombinePath(
		stdGetParentDirectory(stdGetModuleFileName()),
		stdGetFileNameWithoutExtension(stdGetModuleFileName()) + L".ini");
}

