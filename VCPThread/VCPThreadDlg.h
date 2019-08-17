
// VCPThreadDlg.h : header file
//

#pragma once

#include "KeyEdit.h"
#include "COMPort.h"

// CVCPThreadDlg dialog
class CVCPThreadDlg : public CDialog
{
// Construction
public:
	CVCPThreadDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VCPTHREAD_DIALOG };
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
	CString m_strCommand;

	void Trace(LPCTSTR szFmt, ...);

	void ExecuteCommand();

	int InitProg();

	// CDC
	HANDLE m_hPort;
	int ConnectDevice();
	int PortListen();


//	CString m_strEditOutput;
//	CString m_strEditCommand;
	afx_msg void OnBnClickedButtonConnect();

	CKeyEdit m_EditCommand;
	CEdit m_EditOutput;
	afx_msg void OnBnClickedButtonClearlog();
	CString m_strEdit_PortAddr;
};
