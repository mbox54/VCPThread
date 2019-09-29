
// VCPThreadDlg.h : header file
//

#pragma once

////////////////////////////////////////////////////////////////////////////////
// Include
////////////////////////////////////////////////////////////////////////////////
#include "KeyEdit.h"
#include "COMPort.h"

////////////////////////////////////////////////////////////////////////////////
// globals
////////////////////////////////////////////////////////////////////////////////
// ** support **
// output
void Trace_Custom(CEdit* pEdit, CString str);
void OutputLog(LPCTSTR szFmt, ...);

// ** ComPort **
// ComPort handler
static HANDLE g_hPort;

// ComPort Rx_Buf
static BYTE gv_RxBuf[USBUART_BUFFER_SIZE * 4];
static WORD g_usRxIndex;

// ComPort open Proc
static BYTE Open_ComPort(BYTE dwComNum);

// ComPort Rx_Listen Proc
static UINT ListenRx_ComPort(LPVOID rawInput);


////////////////////////////////////////////////////////////////////////////////
// CVCPThreadDlg dialog
////////////////////////////////////////////////////////////////////////////////
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
	BYTE ConnectDevice();
	void Thread_ListenComPort(void);

	// control settings
	CString m_strEdit_PortAddr;
	CKeyEdit m_EditCommand;
	CEdit m_EditOutput;

	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonClearlog();
	
};
