/*========================================================================
** ring.h
** Circular buffer
** ECEN5813
**========================================================================*/
/**
*	@file 			ring.h
*	@brief 		ring buffer header file
*						function prototypes and struct definition of the ring buffer
*	@author 			Linfeng Li;
*	@date 			Mar 18 2019 
*	@version  	1.0
*/
#ifndef __RING_H__
#define __RING_H__

#define NULL_RING_PTR (-1)
#define RING_EMPTY (-2)
#define RING_FULL (-2)

#include "common.h"

typedef struct
{
	uint8_t *Buffer;
	volatile int32_t Length;
	volatile int32_t Ini;
	volatile int32_t Outi;
	volatile uint8_t Full;
} ring_t;

ring_t *init( int32_t length );
int32_t resize( ring_t *ring, int32_t length );
int32_t insert( ring_t *ring, uint8_t data );
int32_t extract( ring_t *ring, uint8_t *data );
int32_t entries( ring_t *ring );

#endif
