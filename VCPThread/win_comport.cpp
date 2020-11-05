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
hSerialCDC m_hPort;

// ComPort Rx_Buf
struct PortListenProcess_t
{
	BYTE bEnable;
	WinComPort_ListenStates TState;
	BYTE aBuf[USBUART_BUFFER_SIZE * 4];
	WORD wRxIndex;

	BYTE bTimeoutStopEnable;
	WORD wTimeoutStopValue;
	WORD wTimeoutValue;

} m_TPortListenProcess;


struct PortTransmitOp_t
{
	WinComPort_TransmitStates TState;
	BYTE aBuf[USBUART_BUFFER_SIZE * 4];
	WORD wCount;

} m_TPortTransmitProcess;


struct PortModbusTransaction_t
{
	WinComPort_ModbusProcess TState;
	UINT64 u64OperationTime;

} m_TPortModbusTransaction;



//////////////////////////////////////////////////////////// 
// Functions
////////////////////////////////////////////////////////////
WinComPort_ReturnCodes_t WINCOMPORT_Init(void)
{
	// init common
	m_hPort = NULL;

	// init PortListenProcess
	m_TPortListenProcess.bEnable = FALSE;

	return WINCOMPORT_OP_SUCCESS;
}

// Open
WinComPort_ReturnCodes_t WINCOMPORT_Open_SyncMode(BYTE dwComNum, DWORD dwBaudrate)
{
	// > Open USBUART Device	
	char szPort[COM_PORT_STRING_LEN];
	sprintf_s(szPort, sizeof(szPort), "\\\\.\\COM%d", dwComNum);
	int iResult = COMPort_Open(&m_hPort, dwComNum);

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
	iResult = COMPort_SetConfig(&m_hPort, dwBaudrate, 8, ONESTOPBIT, 0, 0, 0);

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
	BYTE ucResult = COMPort_Close(&m_hPort);

//	fclose(fs);

	WinComPort_ReturnCodes_t TResult = WINCOMPORT_UNKNOWN;

	switch (ucResult)
	{
	case COM_PORT_OP_SUCCESS:
		// set return code
		TResult = WINCOMPORT_OP_SUCCESS;

		break;


	case COM_PORT_IS_EMPTY:
		// set return code
		TResult = WINCOMPORT_PORT_IS_EMPTY;

		break;


	case COM_PORT_OP_FAILURE:
		// set return code
		TResult = WINCOMPORT_OP_FAILURE;

		break;


	default:
		// err
		TResult = WINCOMPORT_UNKNOWN;

		break;
	}

	return TResult;
}


WinComPort_ReturnCodes_t WINCOMPORT_Read_Instantenious(BYTE* aData, WORD wCount)
{
	m_TPortListenProcess.TState = LISTEN_STATES_RECEIVING;

	DWORD dwNumBytesRead = 0;
	BYTE ucResult = COMPort_Read(&m_hPort, aData, wCount, &dwNumBytesRead);

	// check result
	if (ucResult != COM_PORT_OP_SUCCESS)
	{
		// [FAILURE]

		// set state
		m_TPortTransmitProcess.TState = TRANSMIT_STATES_EXIT_ERROR;

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

	// set state
	m_TPortTransmitProcess.TState = TRANSMIT_STATES_COMPLETED;

	return WINCOMPORT_OP_SUCCESS;
}


WinComPort_ReturnCodes_t WINCOMPORT_Write_Instantenious(BYTE* aData, WORD wCount)
{
	m_TPortTransmitProcess.TState = TRANSMIT_STATES_TRANSMISSION;

	DWORD dwCountWritten = 0;
	BYTE ucResult = COMPort_Write(&m_hPort, aData, wCount, &dwCountWritten);

	// check result
	if (ucResult != COM_PORT_OP_SUCCESS)
	{
		// [FAILURE]

		// set state
		m_TPortTransmitProcess.TState = TRANSMIT_STATES_EXIT_ERROR;

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

	// set state
	m_TPortTransmitProcess.TState = TRANSMIT_STATES_COMPLETED;

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
	memset(m_TPortListenProcess.aBuf, 0x00, sizeof(USBUART_BUFFER_SIZE));

	// > Open File (config)
	// default name
//	char* file_name = "rx_log.txt";

	// try open
//	FILE* fs = fopen(file_name, "w");

	// config ComPort Rx mode
	// setting communications event mask
	if (!SetCommMask(m_hPort, EV_RXCHAR))
	{
		// [ERROR]

		return WINCOMPORT_OP_FAILURE;
	}

	// * Receive *
	DWORD dwCommEvent;
	DWORD dwRead;
	char  chRead;

	UINT64 u64Time = GetTickCount64();
	m_TPortListenProcess.wRxIndex = 0;
	while (m_TPortListenProcess.bEnable)
	{
		// check timeout
		if (m_TPortListenProcess.bTimeoutStopEnable)
		{
			/** NOTES:
			 * GetTickCount64() counted from App start, always incrementing.
			 */ 
			if (GetTickCount64() - u64Time > m_TPortListenProcess.wTimeoutStopValue)
			{
				// [TIMEOUT]

				// abort OP
				m_TPortListenProcess.bEnable = FALSE;

				// set state
				m_TPortListenProcess.TState = LISTEN_STATES_COMPLETE;

				// set op period
				m_TPortListenProcess.wTimeoutValue = GetTickCount64() - u64Time;

				return WINCOMPORT_OP_SUCCESS;
			}
		}
		
		// check for Rx
		if (WaitCommEvent(m_hPort, &dwCommEvent, NULL))
		{
			// [RX OCCURED]

			// reset time
			u64Time = GetTickCount64();

			// set state
			m_TPortListenProcess.TState = LISTEN_STATES_RECEIVING;

			// get all Rx data
			while (ReadFile(m_hPort, &chRead, 1, &dwRead, NULL))
			{
				// [Read OP success]

				// check for empty buffer
				if (dwRead == 1)
				{
					// [ NOT EMPTY ]

					m_TPortListenProcess.aBuf[m_TPortListenProcess.wRxIndex] = chRead;
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

			// set op period
			m_TPortListenProcess.wTimeoutValue = GetTickCount64() - u64Time;

			return WINCOMPORT_OP_FAILURE;

			break;
		}

		// restore Listen
		m_TPortListenProcess.TState = LISTEN_STATES_LISTENING;

	}//while (m_TPortListenProcess.bEnable) : Listen

	// NOTE: Deactivated case
	// set state
	m_TPortListenProcess.TState = LISTEN_STATES_COMPLETE;

	// set op period
	m_TPortListenProcess.wTimeoutValue = GetTickCount64() - u64Time;

	return WINCOMPORT_OP_SUCCESS;
}


UINT WINCOMPORT_Transmit(LPVOID rawInput)
{
	// set state
	m_TPortTransmitProcess.TState = TRANSMIT_STATES_INIT;

	// reset struc
	memset (m_TPortTransmitProcess.aBuf, 0x00, USBUART_BUFFER_SIZE);

	// proc
	WinComPort_ReturnCodes_t TResult = WINCOMPORT_Write_Instantenious(m_TPortTransmitProcess.aBuf, m_TPortTransmitProcess.wCount);

	return 0;
}


void WINCOMPORT_SetPacketData(BYTE* aData, WORD wCount)
{
	// reset buffer
	memset(m_TPortTransmitProcess.aBuf, 0x00, USBUART_BUFFER_SIZE);

	// set values
	memcpy(m_TPortTransmitProcess.aBuf, aData, wCount);
	m_TPortTransmitProcess.wCount = wCount;
}


WORD WINCOMPORT_GetTxCount(void)
{
	return m_TPortTransmitProcess.wCount;
}


UINT WINCOMPORT_ModbusTransact_simple(LPVOID rawInput)
{
	// set state
	m_TPortModbusTransaction.TState = MODBUS_INIT;

	// process
	/** NOTES:
	 * 1) send request packet,
	 * 2) get response packet.
	 */
	
	// set state
	m_TPortModbusTransaction.TState = MODBUS_TRANSMISSION;

	WinComPort_ReturnCodes_t TResult = WINCOMPORT_Write_Instantenious(m_TPortTransmitProcess.aBuf, m_TPortTransmitProcess.wCount);	
	if (TResult != WINCOMPORT_OP_SUCCESS)
	{
		// [ERROR]

		// set state
		m_TPortModbusTransaction.TState = MODBUS_TIMEOUT;

		return 0;
	}

	// pause for MCU proc request
	Sleep(20);

	// set state
	m_TPortModbusTransaction.TState = MODBUS_LISTENING;

	TResult = WINCOMPORT_Read_Instantenious(m_TPortListenProcess.aBuf, m_TPortListenProcess.wRxIndex);

	if (TResult != WINCOMPORT_OP_SUCCESS)
	{
		// [ERROR]

		// set state
		m_TPortModbusTransaction.TState = MODBUS_TIMEOUT;

		return 0;
	}

	// set state
	m_TPortModbusTransaction.TState = MODBUS_COMPLETE;
}


UINT WINCOMPORT_ModbusTransact_complex(LPVOID rawInput)
{
	// init process
	m_TPortModbusTransaction.TState = MODBUS_INIT;
	m_TPortModbusTransaction.u64OperationTime = GetTickCount64();

	// set next state
	m_TPortModbusTransaction.TState = MODBUS_TRANSMISSION;

	// process
	/** NOTES:
	 * 1) send request packet,
	 * 2) get response packet.
	 */
	while (TRUE)
	{
		switch (m_TPortModbusTransaction.TState)
		{
		case MODBUS_TRANSMISSION:
			// send request
		{
			WinComPort_ReturnCodes_t TResult = WINCOMPORT_Write_Instantenious(m_TPortTransmitProcess.aBuf, m_TPortTransmitProcess.wCount);

			// set next state
			m_TPortModbusTransaction.TState = MODBUS_LISTENING;
		}

		break;


		case MODBUS_LISTENING:
		{
			// get responce
			UINT uiResult = WINCOMPORT_ListenStart(0);

			// set next state
			if (m_TPortListenProcess.TState == LISTEN_STATES_COMPLETE)
			{
				m_TPortModbusTransaction.TState = MODBUS_COMPLETE;
			}
			else
			{
				m_TPortModbusTransaction.TState = MODBUS_TIMEOUT;
			}
		}

		break;


		case MODBUS_COMPLETE:
		case MODBUS_TIMEOUT:
		{
			// [EXIT]

			return 0;
		}


		default:
			break;
		}
	}

	return 0;
}


WinComPort_ModbusProcess WINCOMPORT_GetModbusState(void)
{
	return m_TPortModbusTransaction.TState;
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


void WINCOMPORT_SetRxCountLength(WORD wCount)
{
	m_TPortListenProcess.wRxIndex = wCount;
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
	memcpy(aRxData, m_TPortListenProcess.aBuf, wCount);
}



// EOF