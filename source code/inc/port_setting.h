/**
*	@file 			common.h
*	@brief 		Some common functions for this project
*	
*
*	@author 		
*	@date 			March 30 2019 
*	@version  	1.0
*/
#ifndef  __PORT_SETTING__
#define __PORT_SETTING__
#include "common.h"
typedef enum{
	INPUT = 0U,
	OUTPUT = 1U,

}gpio_diretion;

typedef enum{
	HIGH = 0U,
	LOW = 1U,

}gpio_outputlevel;

typedef struct
{
	/* Output level setup, not used if set as input*/
	gpio_outputlevel outputlevel;
	/*input or output */
	gpio_diretion direction;
} gpio_config;


void Init_FLL(void);
void Init_UART_PIN(void);
void Init_LED_PIN(void);
void Init_UART0_CLK_SRC(void);
void Init_PortA_clock(void);
void Init_PortB_clock(void);
void Init_UART_data_source(void);
void Set_Pin_Alt(PORT_Type *base, uint32_t pin, uint32_t mux);
void GPIO_PinInit(GPIO_Type *base, uint32_t pin, gpio_config *config);
void GPIO_Toggle(GPIO_Type *base, uint32_t pin);

#endif
