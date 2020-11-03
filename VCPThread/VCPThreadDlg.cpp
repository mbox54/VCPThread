// VCPThreadDlg.cpp : implementation file
//

///////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "VCPThread.h"
#include "VCPThreadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// !debug
BYTE aData[20] = {0, 1, 2, 16, 255};
char cTest[128];


////////////////////////////////////////////////////////////////////////////////
// Module functions
////////////////////////////////////////////////////////////////////////////////
// support
void Trace_Custom(CEdit* pEdit, CString str)
{
	CString strWndText;
	pEdit->GetWindowText(strWndText);
	strWndText += str;
	pEdit->SetWindowText(strWndText);

	pEdit->SetSel(str.GetLength() - 1, str.GetLength() - 2, FALSE);
	pEdit->LineScroll(-pEdit->GetLineCount());
	pEdit->LineScroll(pEdit->GetLineCount() - 4);
}


void OutputLog(LPCTSTR szFmt, ...)
{
	// get control from memory
	CWnd* pEditLog = AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_EDIT_OUTPUT);
	HWND hWND = pEditLog->GetSafeHwnd();

	// check control appropriate class
	wchar_t str_buf[256];
	GetClassName(hWND, str_buf, 255);
	if (StrCmpIW(str_buf, _T("Edit")) == 0)
	{
		// [VALID]

		// > perform op
		CEdit* pEdit = static_cast<CEdit*>(pEditLog);

		// format the message text
		CString str;
		va_list argptr;
		va_start(argptr, szFmt);
		str.FormatV(szFmt, argptr);
		va_end(argptr);

		str.Replace(_T("\n"), _T("\r\n"));

		// output to control
		Trace_Custom(pEdit, str);
	}
}


////////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
////////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////
// CVCPThreadDlg dialog
////////////////////////////////////////////////////////////////////////////////
CVCPThreadDlg::CVCPThreadDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_VCPTHREAD_DIALOG, pParent)
	, m_strEdit_PortAddr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVCPThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_EditCommand);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_EditOutput);
	DDX_Text(pDX, IDC_EDIT_COMPORT_ADDR, m_strEdit_PortAddr);
}

BEGIN_MESSAGE_MAP(CVCPThreadDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CVCPThreadDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_CLEARLOG, &CVCPThreadDlg::OnBnClickedButtonClearlog)
	ON_BN_CLICKED(IDC_BUTTON_LISTER_EN, &CVCPThreadDlg::OnBnClickedButtonListerEn)
	ON_BN_CLICKED(IDC_BUTTON_LISTER_DIS, &CVCPThreadDlg::OnBnClickedButtonListerDis)
	ON_BN_CLICKED(IDC_BUTTON_RX_GET_STATUS, &CVCPThreadDlg::OnBnClickedButtonRxGetStatus)
	ON_BN_CLICKED(IDC_BUTTON_TX_SEND, &CVCPThreadDlg::OnBnClickedButtonTxSend)
	ON_BN_CLICKED(IDC_BUTTON_TX_GET_STATUS, &CVCPThreadDlg::OnBnClickedButtonTxGetStatus)
	ON_BN_CLICKED(IDC_BUTTON_MODBUS_SEND, &CVCPThreadDlg::OnBnClickedButtonModbusSend)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CVCPThreadDlg message handlers

BOOL CVCPThreadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	// TODO: Add extra initialization here
	InitProg();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVCPThreadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVCPThreadDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVCPThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVCPThreadDlg::Trace(LPCTSTR szFmt, ...)
{
	CString str;

	// Format the message text
	va_list argptr;
	va_start(argptr, szFmt);
	str.FormatV(szFmt, argptr);
	va_end(argptr);

	str.Replace(_T("\n"), _T("\r\n"));

	CString strWndText;
	m_EditOutput.GetWindowText(strWndText);
	strWndText += str;
	m_EditOutput.SetWindowText(strWndText);

	//	m_TraceWnd.SetSel(str.GetLength()-1, str.GetLength()-2, FALSE);
	m_EditOutput.LineScroll(-m_EditOutput.GetLineCount());
	m_EditOutput.LineScroll(m_EditOutput.GetLineCount() - 4);
}


void CVCPThreadDlg::Thread_ListenComPort(void)
{
	// create new thread with no params
	AfxBeginThread(WINCOMPORT_ListenStart, NULL);
}


void CVCPThreadDlg::Thread_TransmitComPort(void)
{
	// create new thread with no params
	AfxBeginThread(WINCOMPORT_Transmit, NULL);
}


void CVCPThreadDlg::Thread_ModbusSend(void)
{

}


void CVCPThreadDlg::PrintResult(WinComPort_ReturnCodes_t TResult)
{
	CString str;
	switch (TResult)
	{
	case WINCOMPORT_UNKNOWN:

		str.Append(_T("WINCOMPORT_UNKNOWN"));

		break;


	case WINCOMPORT_PORT_IS_EMPTY:

		str.Append(_T("WINCOMPORT_PORT_IS_EMPTY"));

		break;


	case WINCOMPORT_BAD_CONNECT:

		str.Append(_T("WINCOMPORT_BAD_CONNECT"));

		break;


	case WINCOMPORT_OP_SUCCESS:

		str.Append(_T("WINCOMPORT_OP_SUCCESS"));

		break;


	case WINCOMPORT_OP_FAILURE:

		str.Append(_T("WINCOMPORT_OP_FAILURE"));

		break;


	case WINCOMPORT_ABORT_PARAM_MISMATCH:

		str.Append(_T("WINCOMPORT_ABORT_PARAM_MISMATCH"));

		break;


	case WINCOMPORT_ERR_WRITTEN_MISMATCH:

		str.Append(_T("WINCOMPORT_ERR_WRITTEN_MISMATCH"));

		break;


	default:

		break;
	}

	str.Append(_T("/n"));

	Trace(str);
}


void CVCPThreadDlg::GetDataFromStrHex(char* strInput, BYTE* aDataOutput)
{
	/** FORMAT:
	 * strInput = "XX XX XX"
	 * aDataOutput = {n, n, n} 
	 * 
	 * ALGORITHM:
	 * if Length >= 2, then copy first 2 symbols as First Hex,
	 * and then if space delimeter exists proceed next symbol.
	 * 
	 * NOTE:
	 * only symbols with 2 digits presents, only space delimeter can be.
	 * 
	 * ALGORITHM2:
	 * in str: "XX XX XX":
	 * first "XX" - unique,
	 * next " XX" - repeated.
	 * So simple while() can be implemented based on this pattern.
	 */ 

	// get length
	WORD wInputLength = strlen(strInput);	

	// check at least 1 value exists
	if (wInputLength < 2)
	{
		// [NO VALUES]

		return;
	}

	char cStrBuf[2];
	WORD wSymbolIndex = 0;

	// get substr_hex_byte
	strncpy(cStrBuf, strInput, 2);

	// convert in value
	aDataOutput[wSymbolIndex] = (BYTE)strtoul(cStrBuf, NULL, 16);
	wSymbolIndex++;

	// decomposing
	while ((wSymbolIndex * 3 < wInputLength) && (strInput[wSymbolIndex * 3 - 1] == ' '))
	{
		// [MODE 0]

		// get substr_hex_byte
		strncpy(cStrBuf, strInput + wSymbolIndex * 3, 2);

		// convert in value
		aDataOutput[wSymbolIndex] = (BYTE)strtoul(cStrBuf, NULL, 16);
		wSymbolIndex++;
	}
}


void CVCPThreadDlg::GetStrHexFromData(BYTE* aDataInput, char* strOutput, WORD wCount)
{
	// check at least 1 value exists
	if (wCount < 1)
	{
		return;
	}

	// copy first symbol: "XX"
	char cStrBuf[4];
	sprintf(cStrBuf, "%02X", aDataInput[0]);
	strncpy(strOutput, cStrBuf, 2);

	// copy next symbols " XX"
	for (WORD wSymbolIndex = 1; wSymbolIndex < wCount; wSymbolIndex++)
	{
		sprintf(cStrBuf, " %02X", aDataInput[wSymbolIndex]);
		strncpy(strOutput + wSymbolIndex * 3 - 1, cStrBuf, 3);
	}
}


void CVCPThreadDlg::ExecuteCommand()
{
	// Update command text
	m_EditCommand.GetWindowText(m_strCommand);

	// output in log
	Trace(_T("%s\n"), m_strCommand);
}


int CVCPThreadDlg::InitProg()
{
	// Printout in the basic command line window.
	Trace(_T("Elcub(c) 2020\n\n"));

	// Output control State
	Trace(_T("Connect to device to start. \n"));


	return 0;
}


VCPThread_OpStatus_t CVCPThreadDlg::ConnectDevice()
{
	UpdateData(TRUE);
	DWORD dwComNum = (DWORD)_tcstoul(m_strEdit_PortAddr, NULL, 10);	

	// > Connect to USBUART Device
	// Input Conf Parameter
	Trace(_T("trying to connect COM%d \n"), dwComNum, 4);

	// > Open USBUART Device	 
	WinComPort_ReturnCodes_t TResult = WINCOMPORT_Open_SyncMode(dwComNum);
	
	if (TResult != WINCOMPORT_OP_SUCCESS)
	{
		Trace(_T("Open failed! \n"));

		return VCPThread_FAILURE;
	}

	// Output control State
	Trace(_T("Open ...[OK] \n"));

	// Start Listen Port
	//PortListen();

	return VCPThread_SUCCESS;
}


void CVCPThreadDlg::StartTimer(void)
{
	m_nTimer = SetTimer(TIMER1, 15, NULL);
}


void CVCPThreadDlg::StopTimer(void)
{
	KillTimer(m_nTimer);
}


void CVCPThreadDlg::OnBnClickedButtonConnect()
{
	// open ComPort
	ConnectDevice();
}


void CVCPThreadDlg::OnBnClickedButtonClearlog()
{
	m_EditOutput.SetWindowText(_T(""));
}




void CVCPThreadDlg::OnBnClickedButtonListerEn()
{
	// Listen ComPort continiously
	Thread_ListenComPort();
}


void CVCPThreadDlg::OnBnClickedButtonListerDis()
{
	WINCOMPORT_ListenCancel();
}


void CVCPThreadDlg::OnBnClickedButtonRxGetStatus()
{
	// TODO: Add your control notification handler code here
}


void CVCPThreadDlg::OnBnClickedButtonTxSend()
{
	m_EditCommand.GetWindowText(m_strCommand);

	char TxBuf[256];

	//sprintf(TxBuf, (LPCSTR)(LPCWSTR)m_strCommand, "%s");
	WinComPort_ReturnCodes_t TResult = WINCOMPORT_Write_Instantenious((BYTE*)(LPCSTR)(LPCWSTR)m_strCommand, m_strCommand.GetLength());

	PrintResult(TResult);
}


void CVCPThreadDlg::OnBnClickedButtonTxGetStatus()
{
	//char* strTest = "00 01 02 03 10 A0 A2 FF";
	
	//GetDataFromStrHex(strTest, aData);

	GetStrHexFromData(aData, cTest, 5);
}


void CVCPThreadDlg::OnBnClickedButtonModbusSend()
{


}


void CVCPThreadDlg::OnTimer(UINT_PTR nIDEvent)
{	
	CDialog::OnTimer(nIDEvent);
}
