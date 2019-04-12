 /**
*	@file 			main_nonblocking.c
*	@brief 			main function in the blocking implement
*	
*	@author 			Linfeng Li;
*	@date 			Apr 11 2019 
*	@version  	1.0
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>

#include "port_setting.h"
#include "UART.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FLL_OUTPUT_CLK_RATE (47972352)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t txbuff[] = "LPSCI polling example\r\nSend back received data\r\n";
uint8_t rxbuff[20] = {0};
extern uint32_t SystemCoreClock;
static UART_config config ={
		.Baudrate = 115200,
		.ParityMode = ParityDisabled,
		.Stopbit = StopBitOne,
		.Txenable = 1,
		.Rxenable = 1
};
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t ch;
    uint32_t uart_num;
    uint8_t uplink_buffer[255];
    //lpsci_config_t config;
    Init_UART_PIN();
    Init_FLL();
    Init_UART0_CLK_SRC();
    UART0_Init(DEMO_LPSCI, &config, FLL_OUTPUT_CLK_RATE);
    uart_num=sprintf(uplink_buffer,"Blocking part\r\n");
    UART_BLOCKING_WRITEBLOCK(DEMO_LPSCI,uplink_buffer,uart_num);

	uint8_t ch_rec;
    while (1)
    {
    	UART_BLOCKING_RX_GETCHAR(DEMO_LPSCI, &ch_rec);
		UART_BLOCKING_PUTCHAR(DEMO_LPSCI,ch_rec);
    }
}
