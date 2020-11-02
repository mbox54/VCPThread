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
  * based on
  * VPK ComPort application
  *
  *
  * ver. 1.0 - init version
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
	WINCOMPORT_OP_SUCCESS,
	WINCOMPORT_IS_EMPTY,
	WINCOMPORT_BAD_CONNECT,
	WINCOMPORT_OP_FAILURE,
	WINCOMPORT_OP_MISMATCH
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
WinComPort_ReturnCodes_t WINCOMPORT_Init(BYTE dwComNum);

// COM-port open
WinComPort_ReturnCodes_t WINCOMPORT_Open(BYTE dwComNum);

// COM-port close
WinComPort_ReturnCodes_t WINCOMPORT_Close(BYTE dwComNum);

// * ComPort Rx_Listen: Thread header * /UINT Funct(LPVOID par);
UINT WINCOMPORT_ListenStart(LPVOID rawInput);

// ComPort Rx_Listen: service routines
void WINCOMPORT_ListenStop(void);

WinComPort_ListenStates WINCOMPORT_GetListenState(void);
WORD WINCOMPORT_GetRxCount(void);
void WINCOMPORT_GetRxData(BYTE * aRxData, WORD wCount);



#endif	// __WIN_COMPORT_H

// EOF
