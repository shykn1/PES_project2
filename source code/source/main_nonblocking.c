
 /**
*	@file 			main_nonblocking.c
*	@brief 			main function in the nonblocking implement
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
#include "ring.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FLL_OUTPUT_CLK_RATE (47972352)
#define DELAY_CNT (100000)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
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
	if((OPENSDA_UART->S1 & UART0_S1_RDRF_MASK) != false){
		UART_RX_GETCHAR(OPENSDA_UART,&character);
		insert(RxRingBuffer,character);
	}


	if((OPENSDA_UART->S1 & UART0_S1_TDRE_MASK) != false && ((OPENSDA_UART->C2 & UART0_C2_TCIE_MASK)!=false)){
		status = extract(TxRingBuffer,&character);
		if(status == RING_EMPTY){
			UART_Interrput_disable(OPENSDA_UART, TxDataRegEmptyInterrupt);
		}
		else if(status != NULL_RING_PTR){
			UART_PUTCHAR(OPENSDA_UART,character);
		}
		else{
			UART_Interrput_disable(OPENSDA_UART, TxDataRegEmptyInterrupt);
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
    UART0_Init(OPENSDA_UART, &config_UART, FLL_OUTPUT_CLK_RATE);
    uart_num=sprintf(uplink_buffer,"non-Blocking part\r\n");
    UART_BLOCKING_WRITEBLOCK(OPENSDA_UART,uplink_buffer,uart_num);
    UART_Interrput_enable(OPENSDA_UART, RxDataRegFullInterrupt);

    NVIC_EnableIRQ(UART0_IRQn);
    while (1)
    {
    	if(   (!UART_NONBLOCKING_TX_BUSY(OPENSDA_UART)) && (  (entries(TxRingBuffer) != 0) )   ){
    		UART_Interrput_enable(OPENSDA_UART, TxDataRegEmptyInterrupt);
    	}
    	toggle_cnt++;
    	if(toggle_cnt==DELAY_CNT){
    		toggle_cnt=0;
    		GPIO_Toggle(GPIOB,18);
    	}
    }
}
