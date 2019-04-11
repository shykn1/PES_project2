/**
*	@file 			port_setting
*	@brief 		portsetting functions for this project
*	
*
*	@author 		
*	@date 			March 30 2019 
*	@version  	1.0
*/
#include "port_setting.h"
void Init_FLL(void){
	SIM->CLKDIV1 = 0x00030000U;
	MCG->C4 = (MCG->C4 & (~(MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS_MASK)))  | MCG_C4_DMX32(1) |   MCG_C4_DRST_DRS(1);
}

void Init_UART0_CLK_SRC(void){
	SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_UART0SRC_MASK) | SIM_SOPT2_UART0SRC(1));
}

void Init_PortA_clock(void){
	 uint32_t reg_addr = SIM_BASE + 0x1038U;
    *((__IO uint32_t *)reg_addr) |= (1U << 9U);
}
void Init_PortB_clock(void){
	 uint32_t reg_addr = SIM_BASE + 0x1038U;
    *((__IO uint32_t *)reg_addr) |= (1U << 10U);
}
void Init_LED_PIN(void){
	Set_Pin_Alt(PORTB,18,1);
};
void Init_UART_PIN(void){
	Init_PortA_clock();
	Set_Pin_Alt(PORTA,1,2);
	Set_Pin_Alt(PORTA,2,2);
	Init_UART_data_source();
}

void Set_Pin_Alt(PORT_Type *port, uint32_t pin, uint32_t mux)
{
	port->PCR[pin] = (port->PCR[pin] & ~PORT_PCR_MUX_MASK) | ((mux << 8U) & PORT_PCR_MUX_MASK);
}

void Init_UART_data_source(void){
	uint32_t temp;
	/* get the existing value and mask the UART0 TX RX source bits */
	temp=SIM->SOPT5 & (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK));
	/* UART0 transmit data source select: UART0_TX(set to 0) and receive data source select: UART0_RX(set to 0) */
	temp |= SIM_SOPT5_UART0TXSRC(0) | SIM_SOPT5_UART0RXSRC(0) ;
	SIM->SOPT5 = temp;
}

void GPIO_PinInit(GPIO_Type *base, uint32_t pin, gpio_config *config)
{

    if (config->direction == INPUT)
    {
        base->PDDR &= ~(1U << pin);
    }
    else
    {
        if(config->outputlevel==HIGH)
        	base->PSOR |= (1U << pin);
        else
        	base->PCOR |= (1U << pin);
        base->PDDR |= (1U << pin);
    }
}

void GPIO_Toggle(GPIO_Type *base, uint32_t pin){
	base->PTOR |= (1U << pin);
}
