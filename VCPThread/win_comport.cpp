/**
  ******************************************************************************
  * @file           : dw1000.c
  * @brief          : Implementation file for dw1000
  *                   This file contains DW1000 procs and properties.
  ******************************************************************************
  * @attention
  *
  * ver. 1.0
  * ELCUB
  * 09.10.2020
  *
  ******************************************************************************
  */


////////////////////////////////////////////////////////////	
// Includes
////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "win_comport.h"


//////////////////////////////////////////////////////////// 
// Properties
////////////////////////////////////////////////////////////
// ComPort handler
hSerialCDC* m_hPort;

// ComPort Rx_Buf
struct PortListenProcess_t
{
	BYTE bEnable;
	WinComPort_ListenStates TState;
	BYTE aRxBuf[USBUART_BUFFER_SIZE * 4];
	WORD wRxIndex;

} m_TPortListenProcess;




WinComPort_ReturnCodes_t WINCOMPORT_Init(void)
{
	// init common

	// init PortListenProcess
	m_TPortListenProcess.bEnable = FALSE;

	return WINCOMPORT_OP_SUCCESS;
}

// Open
WinComPort_ReturnCodes_t WINCOMPORT_Open_SyncMode(BYTE dwComNum)
{
	// > Open USBUART Device	
	char szPort[COM_PORT_STRING_LEN];
	sprintf_s(szPort, sizeof(szPort), "\\\\.\\COM%d", dwComNum);
	int iResult = COMPort_Open(m_hPort, dwComNum);

	WinComPort_ReturnCodes_t TResult = WINCOMPORT_UNKNOWN;
	// check OP state
	if (iResult != COM_PORT_OP_SUCCESS)
	{
		// [FAILED]

		switch (iResult)
		{
		case COM_PORT_OP_FAILURE:

			TResult = WINCOMPORT_OP_FAILURE;

			break;


		default:
			break;
		}

		// abort
		return TResult;
	}

	// > Set COM Port Config
	//iResult = COMPort_SetConfig(&m_hPort, UART_3M_BAUDRATE, 8, ONESTOPBIT, ONESTOPBIT, 1, 0);
	iResult = COMPort_SetConfig(m_hPort, 9600, 8, ONESTOPBIT, 0, 0, 0);

	// check OP state
	if (iResult != COM_PORT_OP_SUCCESS)
	{
		// [FAILED]

		switch (iResult)
		{
		case COM_PORT_OP_FAILURE:

			TResult = WINCOMPORT_OP_FAILURE;

			break;


		default:
			break;
		}

		// abort
		return TResult;
	}

	return WINCOMPORT_OP_SUCCESS;
}

// Close
WinComPort_ReturnCodes_t WINCOMPORT_Close(void)
{
	// > Close COM Port
	BYTE ucResult = COMPort_Close(m_hPort);

//	fclose(fs);

	WinComPort_ReturnCodes_t TResult = WINCOMPORT_UNKNOWN;

	switch (ucResult)
	{
	case COM_PORT_OP_SUCCESS:

		// set return code
		TResult = WINCOMPORT_OP_SUCCESS;

		break;


	case COM_PORT_OP_FAILURE:

		// set return code
		TResult = WINCOMPORT_OP_FAILURE;

		break;


	default:
		break;
	}

	return TResult;
}

// TODO
WinComPort_ReturnCodes_t WINCOMPORT_Read_Instantenious(BYTE* aData, WORD wCount)
{
	return WinComPort_ReturnCodes_t();
}


WinComPort_ReturnCodes_t WINCOMPORT_Write_Instantenious(BYTE* aData, WORD wCount)
{
	DWORD dwCountWritten = 0;
	BYTE ucResult = COMPort_Write(m_hPort, aData, wCount, &dwCountWritten);

	// check result
	if (ucResult != COM_PORT_OP_SUCCESS)
	{
		// [FAILURE]

		switch (ucResult)
		{
		case COM_PORT_OP_FAILURE:
			// set return code
			return WINCOMPORT_OP_FAILURE;

			break;

		case COM_PORT_OP_MISMATCH:
			// set return code
			return WINCOMPORT_ERR_WRITTEN_MISMATCH;

			break;


		default:
			break;
		}
	}

	return WINCOMPORT_OP_SUCCESS;
}

// -----
// ** COMPLEX FUNCTIONS ** 
// -----
UINT WINCOMPORT_ListenStart(LPVOID rawInput)
{
	// * init process *
	// set state
	m_TPortListenProcess.bEnable = TRUE;
	m_TPortListenProcess.TState = LISTEN_STATES_LISTENING;

	// reset buffer
	memset(m_TPortListenProcess.aRxBuf, 0x00, sizeof(USBUART_BUFFER_SIZE));

	// > Open File (config)
	// default name
//	char* file_name = "rx_log.txt";

	// try open
//	FILE* fs = fopen(file_name, "w");


	

	// config ComPort Rx mode
	if (!SetCommMask(*m_hPort, EV_RXCHAR))
	{
		// [ Error setting communications event mask ]

//		return COM_PORT_OP_FAILURE;
	}


	// * Receive *
	DWORD dwCommEvent;
	DWORD dwRead;
	char  chRead;

	m_TPortListenProcess.wRxIndex = 0;
	while (m_TPortListenProcess.bEnable)
	{
		// check for Rx
		if (WaitCommEvent(*m_hPort, &dwCommEvent, NULL))
		{
			// [RX OCCURED]

			// set state
			m_TPortListenProcess.TState = LISTEN_STATES_RECEIVING;

			// get all Rx data
			while (ReadFile(*m_hPort, &chRead, 1, &dwRead, NULL))
			{
				// [Read OP success]

				// check for empty buffer
				if (dwRead == 1)
				{
					// [ NOT EMPTY ]

					m_TPortListenProcess.aRxBuf[m_TPortListenProcess.wRxIndex] = chRead;
					m_TPortListenProcess.wRxIndex++;
				}
				else
				{
					// [ EMPTY ]

					// end operation
					break;

				}
			}//while Read buffer
		}
		else
		{
			// [ERROR]

			// NOTE: Errored case
			// abort OP
			m_TPortListenProcess.bEnable = FALSE;

			// set state
			m_TPortListenProcess.TState = LISTEN_STATES_EXIT_ERROR;

			return WINCOMPORT_OP_FAILURE;

			break;
		}

		// restore Listen
		m_TPortListenProcess.TState = LISTEN_STATES_LISTENING;

	}//while (m_TPortListenProcess.bEnable) : Listen

	// NOTE: Deactivated case
	// set state
	m_TPortListenProcess.TState = LISTEN_STATES_DEACTIVATED;

	return WINCOMPORT_OP_SUCCESS;
}

// stop process
void WINCOMPORT_ListenCancel(void)
{
	m_TPortListenProcess.bEnable = TRUE;
}


WinComPort_ListenStates WINCOMPORT_GetListenState(void)
{
	return m_TPortListenProcess.TState;
}


WORD WINCOMPORT_GetRxCount(void)
{
	return m_TPortListenProcess.wRxIndex;
}


void WINCOMPORT_GetRxData(BYTE* aRxData, WORD wCount)
{
	/** SAFEPROTECT:
	 * check valid range 
	 */ 
	if (wCount > m_TPortListenProcess.wRxIndex)
	{
		// [INVALID]

		return;
	}

	// copy
	memcpy(aRxData, m_TPortListenProcess.aRxBuf, wCount);
}



// EOF