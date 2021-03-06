// COMPort.cpp
// Based on Cypress USB-Serial Windows realisation (uart.cpp)
//
// NOTE:
//
// Half-Duplex realisation, Synchronous, non-Overlapped
//

///////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "comport_lib.h"


///////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////
// check ComPort state
BYTE COMPort_CheckStatus(hSerialCDC* hPort)
{
	// check ComPort pointer
	if (hPort == NULL)
	{
		return COM_PORT_IS_EMPTY;
	}
	else
	{
		// check ComPort connection
		DCB dcb;
		if (GetCommState(*hPort, &dcb) == FALSE)
		{
			// [ERROR: PORT OP FAILURE]

			return COM_PORT_BAD_CONNECT;
		}
		else
		{
			return COM_PORT_OP_SUCCESS;
		}
	}
}


BYTE COMPort_Open(hSerialCDC * hPort, DWORD dwComNum)
{
	// > Open PORT
	char szPort[COM_PORT_STRING_LEN];
	char * szPort_fileStr = "\\\\.\\COM";

	// > Form COM Port filename
	// String Part
	for (UINT k = 0; k < 7; k++)
	{
		szPort[k] = szPort_fileStr[k];
	}

	// Digit Part
	char chBuf[7];
	_itoa_s(dwComNum, chBuf, 7, 10);
	
	UCHAR ucDigitLen = 0;
	while (chBuf[ucDigitLen] != '\0')
	{
		ucDigitLen++;
	}
	
	for (UINT k = 0; k < ucDigitLen; k++)
	{
		szPort[7 + k] = chBuf[k];
	}

	// String Ender
	szPort[7 + ucDigitLen] = '\0';

	// > Define Communication Regime
	// This creates a synchronous handle. So that only a read or write can be performed
	// for this at a particular point of time.
	// For performing Read and Write operation together, please refer to OVERLAPPED COM
	// PORT USAGE example.
	*hPort = CreateFileA(szPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL);
	if (*hPort == INVALID_HANDLE_VALUE)
	{
		return COM_PORT_BAD_CONNECT;
	}

	// > Set the Read and Write API time outs.
	// These Values will be over ridden during read/write API timeout.
	COMMTIMEOUTS objTimeout;
	GetCommTimeouts(*hPort, &objTimeout);
	objTimeout.WriteTotalTimeoutConstant = DEFAULT_WRITE_TIMEOUT;
	objTimeout.ReadTotalTimeoutConstant = DEFAULT_READ_TIMEOUT;

	// Set up the time out value for ReadFile and WriteFile API.
	SetCommTimeouts(*hPort, &objTimeout);

	return COM_PORT_OP_SUCCESS;
}


BYTE COMPort_SetConfig(hSerialCDC * hPort, DWORD dwBaudRate, UCHAR ucByteSize, UCHAR ucStopBits, UCHAR ucParity, BOOL bCTS_flow_ctrl, BOOL bDSR_flow_ctrl)
{
	// NOTE:
	// NAMES:
	// Data Terminal Ready (DTR) - Data Set Ready (DSR)
	// Clear To Send (CTS) - Request To Send (RTS)
	//
	// RESTRICTIONS:
	// 8 or 7 bits are supported.
	// Device supports one or two stop bits.
	// Device supports no parity, odd, even, mark and space.
	//
	// COMMENT:
	// Flow control happen based on option used. 
	// CTS flow control is popularly used.
	// - CTS Flow control.Please interconnect RTS to CTS of the same device.
	// - DSR Flow Control.Please interconnect DTR to DSR of the same device.
	// Between UART, both UART is expected to understand Hardware Flow control.


	// TODO:
	// check COM_Handler for NULL


	DCB dcb;

	// > Get COM Port curr Config
	if (GetCommState(*hPort, &dcb) == FALSE)
	{
		// [ERROR: PORT OP FAILURE]

		return COM_PORT_OP_FAILURE;
	}

	// > Update Config Values
	dcb.BaudRate = dwBaudRate;
	dcb.ByteSize = ucByteSize;				// 8 or 7 bits are supported.
	dcb.StopBits = ucStopBits;				// Device supports one or two stop bits.
	dcb.Parity = ucParity;					// device supports no parity, odd, even, mark and space.

	dcb.fOutxCtsFlow = bCTS_flow_ctrl;		// This will enable RTS-CTS flow control.
	dcb.fOutxDsrFlow = bDSR_flow_ctrl;		// This will disable DTR-DCR Flow control. 

	// > Set COM Port Config
	if (SetCommState(*hPort, &dcb) == FALSE)
	{
		// [ERROR: PORT OP FAILURE]

		return COM_PORT_OP_FAILURE;
	}

	return COM_PORT_OP_SUCCESS;
}


BYTE COMPort_Read(hSerialCDC * hPort, UCHAR * v_ReadBuffer, DWORD dwCount, DWORD* dwNumBytesRead)
{
	// check ComPort_Handler
	BYTE ucComPortStatus = COMPort_CheckStatus(hPort);
	if (ucComPortStatus != COM_PORT_OP_SUCCESS)
	{
		return ucComPortStatus;
	}

	BYTE errNumber = 0;
	BOOL bReadStatus = ReadFile(*hPort, v_ReadBuffer, dwCount, dwNumBytesRead, NULL);

	// > Check Valid PROC
	if (bReadStatus == FALSE)
	{
		// [ERROR: PORT OP FAILURE]

		*dwNumBytesRead = GetLastError();

		errNumber = COM_PORT_OP_FAILURE;
	}
	else if (dwCount != *dwNumBytesRead)
	{
		// [ERROR: TRANSFER MISMATCH]

		errNumber = COM_PORT_OP_MISMATCH;
	}

	// safe check
	if (errNumber != COM_PORT_OP_SUCCESS)
	{
		// [ERROR CASE]

		CloseHandle(*hPort);

		return errNumber;
	}

	return COM_PORT_OP_SUCCESS;
}


BYTE COMPort_Write(hSerialCDC * hPort, UCHAR * v_WriteBuffer, WORD wCount, DWORD * dwNumBytesWritten)
{
	// check ComPort_Handler
	BYTE ucComPortStatus = COMPort_CheckStatus(hPort);
	if (ucComPortStatus != COM_PORT_OP_SUCCESS)
	{
		return ucComPortStatus;
	}

	*dwNumBytesWritten = 0;
	BOOL bWriteStatus = WriteFile(*hPort, v_WriteBuffer, wCount, dwNumBytesWritten, NULL);

	// > Check Valid PROC
	BYTE errNumber = 0;
	if (bWriteStatus == FALSE)
	{
		// [ERROR: PORT OP FAILURE]

		*dwNumBytesWritten = GetLastError();

		errNumber = COM_PORT_OP_FAILURE;
	}
	else
	{
		if (*dwNumBytesWritten != wCount)
		{
			// [ERROR: TRANSFER MISMATCH]

			errNumber = COM_PORT_OP_MISMATCH;
		}
	}

	if (errNumber != 0)
	{
		// [ERROR CASE]

		CloseHandle(*hPort);

		return errNumber;
	}


	return COM_PORT_OP_SUCCESS;
}


BYTE COMPort_Close(hSerialCDC * hPort)
{		
	// check COM_Handler for NULL
	if (*hPort == NULL)
	{
		// [NULL]

		return COM_PORT_IS_EMPTY;
	}

	// > PROC Close
	if (CloseHandle(*hPort) == FALSE)
	{
		// [ERROR: PORT OP FAILURE]

		return COM_PORT_OP_FAILURE;
	}

	// reset pointer
	*hPort = NULL;

	return COM_PORT_OP_SUCCESS;
}


