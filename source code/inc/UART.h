/**
*	@file 			UART.h
*	@brief 		UART functions for this project
*	
*
*	@author 		
*	@date 			March 30 2019 
*	@version  	1.0
*/
#ifndef __UART__
#define __UART__
#include "common.h"

typedef enum
{
    ParityDisabled = 0x0U, /*!< Parity disabled */
    ParityEven = 0x2U,     /*!< Parity set to even, PE:1 PT:0 */
    ParityOdd = 0x3U,      /*!< Parity set to odd,  PE:1 PT:1 */
} parity_mode;

typedef enum
{
    StopBitOne = 0x0U,     /*!< Parity disabled */
	StopBitTwo = 0x1U,     /*!< Parity enabled, type even, bit setting: PE|PT = 10 */

} stopbit_mode;
typedef struct
{
    uint32_t Baudrate;          /*!< baudrate setting  */
    parity_mode ParityMode;     /*!< Parity mode setting */
    stopbit_mode Stopbit;      	/*!< 1 stop bit (default) or 2 stop bits  */
    uint8_t Txenable; 				/*!< Enable TX */
    uint8_t Rxenable; 				/*!< Enable RX */
}UART_config;

typedef enum
{
	TxDataRegEmptyInterrupt = UART0_C2_TCIE_MASK,
	RxDataRegFullInterrupt =  UART0_C2_RIE_MASK,
} Interrput_Mask;

uint8_t UART0_Init(UART0_Type *base, UART_config *config, uint32_t src_clk_rate);
void UART_BLOCKING_RX_GETCHAR(UART0_Type *base, uint8_t* data);
void UART_RX_GETCHAR(UART0_Type *base, uint8_t* data);
void UART_TX_RD(UART0_Type *base);
int32_t UART_NONBLOCKING_TX_BUSY(UART0_Type *base);
void UART_PUTCHAR(UART0_Type *base, uint8_t data);
void UART_BLOCKING_PUTCHAR(UART0_Type *base, uint8_t data);
void UART_BLOCKING_WRITEBLOCK(UART0_Type *base, uint8_t* data, uint32_t length);
void UART_Interrput_enable(UART0_Type *base, Interrput_Mask type);
void UART_Interrput_disable(UART0_Type *base, Interrput_Mask type);
#endif
