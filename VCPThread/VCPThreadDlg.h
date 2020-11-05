
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


#define TIMER1					1


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
	BYTE m_bModbusActive;

	HICON m_hIcon;
	UINT_PTR m_nTimer;

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
	VCPThread_OpStatus_t ConnectDevice(void);
	void Thread_ListenComPort(void);
	void Thread_TransmitComPort(void);
	void Thread_ModbusSend_simple(void);
	void Thread_ModbusSend_complex(void);
	
	// support
	void PrintResult(WinComPort_ReturnCodes_t TResult);
	void GetDataFromStrHex(char* strInput, BYTE* aDataOutput, WORD* wCountRead);
	void GetStrHexFromData(BYTE* aDataInput, char* strOutput, WORD wCount);

	// control settings
	CString m_strEdit_PortAddr;
	CKeyEdit m_EditCommand;
	CEdit m_EditOutput;

	void StartTimer(void);
	void StopTimer(void);

	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonClearlog();
	
	afx_msg void OnBnClickedButtonListerEn();
	afx_msg void OnBnClickedButtonListerDis();
	afx_msg void OnBnClickedButtonRxGetStatus();
	afx_msg void OnBnClickedButtonTxSend();
	afx_msg void OnBnClickedButtonTxGetStatus();
	afx_msg void OnBnClickedButtonModbusSend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strEdit_PortBaudrate;
	CString m_strEdit_ModbusTimeout;
	afx_msg void OnBnClickedButtonConnect2();
	afx_msg void OnBnClickedButtonModbusSend2();
};

// EOF