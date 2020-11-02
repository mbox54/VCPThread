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
  * 2018
  * /VPK
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
	LISTEN_STATES_DEACTIVATED
} WinComPort_ListenStates;


///////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////
// COM-port init
WinComPort_ReturnCodes_t WINCOMPORT_Init(void);

// COM-port open
WinComPort_ReturnCodes_t WINCOMPORT_Open_SyncMode(BYTE dwComNum);

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
WORD WINCOMPORT_GetRxCount(void);
void WINCOMPORT_GetRxData(BYTE * aRxData, WORD wCount);



#endif	// __WIN_COMPORT_H

// EOF
