/**
*	@file 			application.h
*	@brief 		application functions for this project
*	
*
*	@author 		
*	@date 			March 30 2019 
*	@version  	1.0
*/
#ifndef  __APPLICATION__
#define __APPLICATION__
#include "common.h"
#include "ring.h"
#include "UART.h"
typedef enum{
	UPDATE_REPORT = 0U,
	WAIT_FOR_PREVIOUS_TX = 1U,
	GENERATE_REPORT = 2U,
}state;


void application(void);
void report_generation(ring_t* ring, uint32_t* report_cnt);
uint32_t ring_block_insert(ring_t* ring, uint8_t* data, uint32_t length);
void PRG_data(uint32_t* data);

#endif
