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
    UART0_Init(OPENSDA_UART, &config, FLL_OUTPUT_CLK_RATE);
    uart_num=sprintf(uplink_buffer,"Blocking part\r\n");
    UART_BLOCKING_WRITEBLOCK(OPENSDA_UART,uplink_buffer,uart_num);

	uint8_t ch_rec;
    while (1)
    {
    	UART_BLOCKING_RX_GETCHAR(OPENSDA_UART, &ch_rec);
		UART_BLOCKING_PUTCHAR(OPENSDA_UART,ch_rec);
    }
}
