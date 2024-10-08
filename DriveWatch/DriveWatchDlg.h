
// DriveWatchDlg.h : header file
//

#pragma once


// CDriveWatchDlg dialog
class CDriveWatchDlg : public CResizableDialog
{
	using ParentClass = CResizableDialog;
	bool bSkipRemovable_;
	bool bSkipFixed_;
	bool bSkipRemote_;
	bool bSkipCdrom_;
	bool bSkipRamDisk_;
	// Construction
public:
	CDriveWatchDlg(
		bool bSkipRemovable,
		bool bSkipFixed,
		bool bSkipRemote,
		bool bSkipCdrom,
		bool bSkipRamDisk);

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRIVEWATCH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strFreeSpace;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnQueryEndSession();
};
