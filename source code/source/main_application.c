/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>

#include "port_setting.h"
#include "UART.h"
#include "ring.h"
#include "application.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
volatile uint32_t tick;
ring_t* TxRingBuffer;
ring_t* RxRingBuffer;
uint32_t seed=1234;
bool TxRingBufferFullWarning = false;
bool RxRingBufferFullWarning = false;
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
		tick = 0;
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
 * @systick for output
 */
void SysTick_Handler(void)  {                               /* SysTick interrupt Handler.*/
	if(tick <= TIMEOUT_TICK){
		tick++;
	}
}
/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t uart_num;
    uint8_t uplink_buffer[255];

    //Initialize core clock, peripheral clock and the PIN for UART
    Init_UART_PIN();
    Init_FLL();
    Init_UART0_CLK_SRC();
    //////////LED//////////////////
    Init_PortB_clock();
    Init_LED_PIN();
    GPIO_PinInit(GPIOB,18,&config_LED);
    SysTick_Config(48000);
    ///////////////////////////////
    TxRingBuffer=init(1000);
    if(!TxRingBuffer)
    	while(1);

    RxRingBuffer=init(200);
	if(!RxRingBuffer)
		while(1);
    ///////////////////////////////
    UART0_Init(OPENSDA_UART, &config_UART, FLL_OUTPUT_CLK_RATE);
    uart_num=sprintf(uplink_buffer,"application code: tracking the times of occurrence for each characters\r\n");
    UART_BLOCKING_WRITEBLOCK(OPENSDA_UART,uplink_buffer,uart_num);

    UART_Interrput_enable(OPENSDA_UART,RxDataRegFullInterrupt);
    NVIC_EnableIRQ(UART0_IRQn);
    while (1)
    {
    	application();
    	PRG_data(&seed);
    }
}
