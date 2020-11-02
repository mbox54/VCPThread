
// VCPThreadDlg.h : header file
//

#pragma once

///////////////////////////////////////////////////////////
// Include
///////////////////////////////////////////////////////////
#include "KeyEdit.h"
#include "win_comport.h"


///////////////////////////////////////////////////////////
// typedef
///////////////////////////////////////////////////////////
typedef enum
{
	VCPThread_UNKNOWN,
	VCPThread_SUCCESS,
	VCPThread_FAILURE,
} VCPThread_OpStatus_t;


///////////////////////////////////////////////////////////
// globals
///////////////////////////////////////////////////////////
// ** support **
// output
void Trace_Custom(CEdit* pEdit, CString str);
void OutputLog(LPCTSTR szFmt, ...);


///////////////////////////////////////////////////////////
// CVCPThreadDlg dialog
///////////////////////////////////////////////////////////
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
	

	// CDC device OPs
	VCPThread_OpStatus_t ConnectDevice();
	void Thread_ListenComPort(void);

	// control settings
	CString m_strEdit_PortAddr;
	CKeyEdit m_EditCommand;
	CEdit m_EditOutput;

	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonClearlog();
	
	afx_msg void OnBnClickedButtonListerEn();
	afx_msg void OnBnClickedButtonListerDis();
	afx_msg void OnBnClickedButtonRxGetStatus();
	afx_msg void OnBnClickedButtonTxSend();
	afx_msg void OnBnClickedButtonTxGetStatus();
};
