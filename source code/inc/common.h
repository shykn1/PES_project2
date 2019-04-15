/**
*	@file 			common.h
*	@brief 		Some common functions for this project
*	
*
*	@author 		
*	@date 			March 30 2019 
*	@version  	1.0
*/
#ifndef  __COMMON__
#define __COMMON__
#include "MKL25Z4.h"
#include "MKL25Z4_features.h"
#include "stdbool.h"
#include "stdint.h"
#include <stdio.h>
#include <stdlib.h>
#define TIMEOUT_TICK (50)
#define MAX_REPORT_CHAR (1000)
#define OPENSDA_UART UART0
#define MOD (2147483648U)
#define MULT (1103515245U)
#define INC (12345U)
#endif
