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

void Init_FLL(void);
void Init_UART_PIN(void);
void Init_UART0_CLK_SRC(void);
void Init_PortA_clock(void);
void Init_UART_data_source(void);
void Set_Pin_Alt(PORT_Type *base, uint32_t pin, uint32_t mux);

#endif
