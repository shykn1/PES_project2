/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#define DEMO_LPSCI UART0
#define DEMO_LPSCI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_LPSCI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
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
