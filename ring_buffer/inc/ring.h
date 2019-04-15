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

#include <stdio.h>
#include <stdlib.h>
typedef struct
{
	char *Buffer;
	int Length;
	int Ini;
	int Outi;
	unsigned char Full;
} ring_t;

ring_t *init( int length );
int resize( ring_t *ring, int length );
int insert( ring_t *ring, char data );
int extract( ring_t *ring, char *data );
int entries( ring_t *ring );

#endif
