/**
  ******************************************************************************
  * @file           : win_comport.h
  * @brief          : Header for win_comport.c file.
  *                   This file contains functions for extended work with 
						COM-port in Windows using threads.
  *
  * core service module type
  ******************************************************************************
  * @attention
  *
  * based on:
  * 1) Cypress USB-Serial Windows realisation (uart.cpp) example
  * Half-Duplex realisation, Synchronous, non-Overlapped
  * 200x
  * 2) VPK ComPort application, that inherit p.1.
  * INFO:
  * - sync mode
  * - interrupted Read procedure with stand-along thread
  * Virtual COM-Port listen and command (160719 rev.1)  /VPK 
  * 
  * 
  * ver. 1.0 - init version
  * INFO:
  * - win_comport module performed with WinApp abstraction from ComPort file
  * primitives.
  * - comprehensive Read_thread flow control API performed
  * ELCUB
  * 02.11.2020
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion ----------------*/
#ifndef __WIN_COMPORT_H
#define __WIN_COMPORT_H


///////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////
#include "comport_lib.h"


///////////////////////////////////////////////////////////
// typedef
///////////////////////////////////////////////////////////
typedef enum
{
	WINCOMPORT_UNKNOWN,
	WINCOMPORT_PORT_IS_EMPTY,
	WINCOMPORT_BAD_CONNECT,
	WINCOMPORT_OP_SUCCESS,
	WINCOMPORT_OP_FAILURE,
	WINCOMPORT_ABORT_PARAM_MISMATCH,
	WINCOMPORT_ERR_WRITTEN_MISMATCH

} WinComPort_ReturnCodes_t;


typedef enum
{
	LISTEN_STATES_UNKNOWN,
	LISTEN_STATES_LISTENING,
	LISTEN_STATES_RECEIVING,
	LISTEN_STATES_EXIT_ERROR,
	LISTEN_STATES_COMPLETE

} WinComPort_ListenStates;


typedef enum
{
	TRANSMIT_STATES_UNKNOWN,
	TRANSMIT_STATES_INIT,
	TRANSMIT_STATES_TRANSMISSION,
	TRANSMIT_STATES_COMPLETED,
	TRANSMIT_STATES_EXIT_ERROR,
	
} WinComPort_TransmitStates;


typedef enum
{
	MODBUS_INIT,
	MODBUS_TRANSMISSION,
	MODBUS_LISTENING,
	MODBUS_COMPLETE,
	MODBUS_TIMEOUT
} WinComPort_ModbusProcess;


////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////
// COM-port init
WinComPort_ReturnCodes_t WINCOMPORT_Init(void);

// COM-port open
WinComPort_ReturnCodes_t WINCOMPORT_Open_SyncMode(BYTE dwComNum, DWORD dwBaudrate);

// COM-port close
WinComPort_ReturnCodes_t WINCOMPORT_Close(void);

// Read instant
WinComPort_ReturnCodes_t WINCOMPORT_Read_Instantenious(BYTE * aData, WORD wCount);

// Write instant
WinComPort_ReturnCodes_t WINCOMPORT_Write_Instantenious(BYTE* aData, WORD wCount);

// ** COMPLEX FUNCTIONS ** 
// * ComPort Rx_Listen: Thread header * /UINT Funct(LPVOID par);
UINT WINCOMPORT_ListenStart(LPVOID rawInput);

// ComPort Rx_Listen: service routines
void WINCOMPORT_ListenCancel(void);
WinComPort_ListenStates WINCOMPORT_GetListenState(void);
void WINCOMPORT_SetRxCountLength(WORD wCount);
WORD WINCOMPORT_GetRxCount(void);
void WINCOMPORT_GetRxData(BYTE * aRxData, WORD wCount);

// * Tx OP: Thread header * /UINT Funct(LPVOID par);
UINT WINCOMPORT_Transmit(LPVOID rawInput);
void WINCOMPORT_SetPacketData(BYTE* aData, WORD wCount);
WORD WINCOMPORT_GetTxCount(void);

// *Modbus transaction * /UINT Funct(LPVOID par);
// *Modbus transaction * /UINT Funct(LPVOID par);
UINT WINCOMPORT_ModbusTransact_simple(LPVOID rawInput);
UINT WINCOMPORT_ModbusTransact_complex(LPVOID rawInput);
WinComPort_ModbusProcess WINCOMPORT_GetModbusState(void);

#endif	// __WIN_COMPORT_H

// EOF
