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
#include "ring.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPSCI UART0
#define DEMO_LPSCI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_LPSCI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define FLL_OUTPUT_CLK_RATE (47972352)
#define DELAY_CNT (100000)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t txbuff[] = "LPSCI polling example\r\nSend back received data\r\n";
uint8_t rxbuff[20] = {0};
extern uint32_t SystemCoreClock;
static UART_config config_UART ={
		.Baudrate = 115200,
		.ParityMode = ParityDisabled,
		.Stopbit = StopBitOne,
		.Txenable = 1,
		.Rxenable = 1
};
static gpio_config config_LED ={
		.outputlevel = LOW,
		.direction = OUTPUT
};
ring_t* TxRingBuffer;
ring_t* RxRingBuffer;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*
 * @UART0_ISR
 */
void UART0_IRQHandler(void)
{
	uint8_t character;
	int32_t status;
	if((DEMO_LPSCI->S1 & UART0_S1_RDRF_MASK) != false){
		UART_RX_GETCHAR(DEMO_LPSCI,&character);
		insert(RxRingBuffer,character);
	}


	if((DEMO_LPSCI->S1 & UART0_S1_TDRE_MASK) != false && ((DEMO_LPSCI->C2 & UART0_C2_TCIE_MASK)!=false)){
		status = extract(TxRingBuffer,&character);
		if(status == RING_EMPTY){
			UART_Interrput_disable(DEMO_LPSCI, TxDataRegEmptyInterrupt);
		}
		else if(status != NULL_RING_PTR){
			UART_PUTCHAR(DEMO_LPSCI,character);
		}
		else{
			UART_Interrput_disable(DEMO_LPSCI, TxDataRegEmptyInterrupt);
		}
	}
}
/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t uart_num;
    uint32_t toggle_cnt=0;
    uint8_t uplink_buffer[255];
    //lpsci_config_t config;
    Init_UART_PIN();
    Init_FLL();
    Init_UART0_CLK_SRC();
    //////////LED//////////////////
    Init_PortB_clock();
    Init_LED_PIN();
    GPIO_PinInit(GPIOB,18,&config_LED);
    ///////////////////////////////
    TxRingBuffer=init(100);
    if(!TxRingBuffer)
    	while(1);
    RxRingBuffer = TxRingBuffer;
    ///////////////////////////////
    UART0_Init(DEMO_LPSCI, &config_UART, FLL_OUTPUT_CLK_RATE);
    uart_num=sprintf(uplink_buffer,"non-Blocking part\r\n");
    UART_BLOCKING_WRITEBLOCK(DEMO_LPSCI,uplink_buffer,uart_num);
    UART_Interrput_enable(DEMO_LPSCI, RxDataRegFullInterrupt);

    NVIC_EnableIRQ(UART0_IRQn);
    while (1)
    {
    	if(   (!UART_NONBLOCKING_TX_BUSY(DEMO_LPSCI)) && (  (entries(TxRingBuffer) != 0) )   ){
    		UART_Interrput_enable(DEMO_LPSCI, TxDataRegEmptyInterrupt);
    	}
    	toggle_cnt++;
    	if(toggle_cnt==DELAY_CNT){
    		toggle_cnt=0;
    		GPIO_Toggle(GPIOB,18);
    	}
    }
}
