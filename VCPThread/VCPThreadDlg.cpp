
// VCPThreadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCPThread.h"
#include "VCPThreadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

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


// CVCPThreadDlg dialog



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
	Trace(_T("Cypress Basic UART Communication Test\n\n"));

	// Output control State
	Trace(_T("Enter Master COM Port (1-99): \n"));


	return 0;
}

int CVCPThreadDlg::ConnectDevice()
{
	// Define Local Variables.
	HANDLE hMaster = INVALID_HANDLE_VALUE;

	UpdateData(TRUE);
	DWORD dwComNum = (DWORD)_tcstoul(m_strEdit_PortAddr, NULL, 10);	

	// > Connect to USBUART Device
	// Input Conf Parameter
	Trace(_T("trying to connect COM%d \n"), dwComNum, 4);

	// > Open USBUART Device	
	char szPort[COM_PORT_STRING_LEN];
	sprintf_s(szPort, sizeof(szPort), "\\\\.\\COM%d", dwComNum);
	int iResult = COMPort_Open(&m_hPort, dwComNum, 0);

	if (iResult != COM_PORT_OP_SUCCESS)
	{
		Trace(_T("Open failed! \n"));

		return iResult;
	}

	// Output control State
	Trace(_T("Open ...[OK] \n"));

	// > Set COM Port Config
	//iResult = COMPort_SetConfig(&m_hPort, UART_3M_BAUDRATE, 8, ONESTOPBIT, ONESTOPBIT, 1, 0);
	iResult = COMPort_SetConfig(&m_hPort, 9600, 8, ONESTOPBIT, 0, 0, 0);

	if (iResult != COM_PORT_OP_SUCCESS)
	{
		return iResult;
	}

	// Start Listen Port
	PortListen();

	return 0;
}

int CVCPThreadDlg::PortListen()
{
	/////////////////////////////////////////////////////////
	// Test, we short the Rx with Tx.
	/////////////////////////////////////////////////////////
	DCB dcb;

	UCHAR wrBuffer[USBUART_BUFFER_SIZE];
	UCHAR rdBuffer[USBUART_BUFFER_SIZE];
	DWORD dwWritten = 0;

	memset(wrBuffer, 0, sizeof(wrBuffer));
	memset(rdBuffer, 0, sizeof(rdBuffer));

	for (int nCount = 0; nCount < sizeof(wrBuffer); nCount++)
	{
		wrBuffer[nCount] = nCount;
	}


	//   // Let us perform a simple write of 64 bytes of data.
	//DWORD dwNumBytesWritten;
	//iResult = COMPort_Write(&hPort, wrBuffer, &dwNumBytesWritten);

	//if (iResult != COM_PORT_OP_SUCCESS)
	//{
	//	return iResult;
	//}


	//BOOL bWriteStatus;

	//   // Let us read the data that we wrote.
	//   DWORD dwRead = 0;
	//   DWORD dwBytesRead = 0;
	//   DWORD dwSizeLeftToRead = sizeof(rdBuffer);
	//   BOOL bReadStatus = FALSE;
	//   
	//DWORD dwNumBytesRead;
	//iResult = COMPort_Read(&hPort, rdBuffer, &dwNumBytesRead);

	//if (iResult != COM_PORT_OP_SUCCESS)
	//{
	//	return iResult;
	//}

	//   
	//   // Perform a comparison operation to make sure that we have data integrity.
	//   if (memcmp(wrBuffer, rdBuffer, dwRead) != 0 )
	//   {
	//       _tprintf(L"\n\n\nREAD and WRITE data comparison failed.......\n\n");
	//       CloseHandle(hPort);
	//       return -3;
	//   }

	//   _tprintf(L"\n\n\nSuccessfully Completed Flow Control Enabled UART operation .......\n\n");



	if (!SetCommMask(m_hPort, EV_RXCHAR))
	{
		// [ Error setting communications event mask ]

		return -1;
	}


	// Test RX Listen example


	// Set Values to Transfer - Get
	for (BYTE kk = 0; kk < 8; kk++)
	{
		wrBuffer[kk] = 0x10 + kk;

	}

	// ** TRANSFER
	DWORD dwNumBytesWritten;
	BYTE iResult = 0;
	iResult = COMPort_Write8(&m_hPort, wrBuffer, &dwNumBytesWritten);

	if (iResult != COM_PORT_OP_SUCCESS)
	{
		return iResult;
	}

	// ** RECEIVE
	DWORD dwCommEvent;
	DWORD dwRead;
	char  chRead;

	WORD usReceiveCounter = 0;

	while(1)
	{

		if (WaitCommEvent(m_hPort, &dwCommEvent, NULL))
		{
			// [ Read OP defined ]

			BYTE ucByteIndex = 0;

			BYTE bAct = 1;
			while (bAct)
			{
				if (ReadFile(m_hPort, &chRead, 1, &dwRead, NULL))
				{
					// [ Read OP success ]

					if (dwRead)
					{
						// [ NOT EMPTY ]

						rdBuffer[ucByteIndex] = chRead;
						ucByteIndex++;

						//Trace(_T("."));
					}
					else
					{
						// [ EMPTY ]

						// end operation
						bAct = 0;
					}
				}
				else
				{
					// [ An error occurred in the ReadFile call ]

					break;
				}

			}//while (bAct) 

			 // end string
			rdBuffer[ucByteIndex] = '\0';

			// output
			Trace(_T("Receive %03d: %s \n"), usReceiveCounter, CString(rdBuffer));

			usReceiveCounter++;

		}
		else
		{
			// [ Error in WaitCommEvent ]

			break;
		}

	}//for (; ; )


	 // > Close COM Port
	iResult = COMPort_Close(&m_hPort);

	Trace(_T("\n\n\n End of program, Port closed.  \n\n"));


	if (iResult != COM_PORT_OP_SUCCESS)
	{
		return iResult;
	}


	return 0;
}


void CVCPThreadDlg::OnBnClickedButtonConnect()
{
	ConnectDevice();
}


void CVCPThreadDlg::OnBnClickedButtonClearlog()
{
	m_EditOutput.SetWindowText(_T(""));
}
