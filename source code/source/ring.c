/*========================================================================
** Circular buffer
** ECEN5813
**========================================================================*/
/**
*	@file 			ring.c
*	@brief 		ring buffer functions include:initialization with certain length, insert, extract, and return the entries in the buffer
*	
*	@author 			Linfeng Li;
*	@date 			Mar 18 2019 
*	@version  	1.0
*/

#include "ring.h"




/**
*	@brief 		ring buffer functions :initialization with certain length
*	
*	@param		int32_t length			capacity of the buffer in terms of bytes
*	
*	@return   	pointer to the ring_t if initialized succesfully;
* 						NULL if initialized not succesfuly
*/
ring_t *init( int32_t length ){
	ring_t* ring = malloc(sizeof(ring_t));
	if(ring == NULL)
		return 0;
	ring->Buffer= malloc(length);
	if(ring->Buffer == NULL){
		free(ring);
		return 0;
	}
	ring->Length =length;
	ring->Ini=0;
	ring->Outi=0;
	ring->Full=0;
	return ring;
};
/**
*	@brief 		ring buffer functions :resize an exisiting buffer with certain length
*	
*	@param		int32_t length			capacity of the buffer in terms of bytes
*	
*	@return   	0 if resizee succesfully;
* 						-1 if input with NULL pointer to ring_t 
*						-2 if new buffer allocation failed, the old buffer would remain  intact
*/
int32_t resize(ring_t *ring, int32_t length ){
	if(ring == NULL)
		return -1;
	uint8_t* temp = NULL;
	uint8_t* ptrPreRing;
	int32_t entries_pre = entries(ring);
	temp = malloc(length);
	if(temp == NULL){
		return -2;
	}
	if(entries_pre>=length){
		ptrPreRing=(uint8_t*)(ring->Buffer+ring->Ini);
		if(ring->Full){
			ptrPreRing++;
			if(ptrPreRing == (ring->Buffer+ring->Length))
				ptrPreRing = ring->Buffer;
		}
		for(int32_t i=0;i<length;i++){
			ptrPreRing = (ptrPreRing == ring->Buffer) ? (ring->Buffer + ring->Length-1) : (ptrPreRing-1);
			*(temp+length-1-i) = *ptrPreRing;
		}
			ring->Length =length;
			free(ring->Buffer);
			ring->Buffer = temp;
			ring->Ini=length-1;
			ring->Outi=0;
			ring->Full=1;
	}
	else{
		ptrPreRing=(uint8_t*)(ring->Buffer+ring->Outi);
		for(int32_t i=0;i<entries_pre;i++){
			*(temp+i) = *ptrPreRing;
			ptrPreRing++;
			ptrPreRing = (ptrPreRing == (ring->Buffer+ring->Length) ) ? (ring->Buffer) : ptrPreRing;
		}
			ring->Length =length;
			free(ring->Buffer);
			ring->Buffer = temp;
			ring->Ini=entries_pre;
			ring->Outi=0;
			ring->Full=0;
	}
	return 0;
};

/**
*	@brief 		ring buffer functions :insert one element into the buffer
*	
*	@param		ring_t* ring		pointer to the ring_t which represent the buffer to write 
*	@param		uint8_t data		data to write
*	
*	@return   	0 if write succesfully;
* 						-1 if input with NULL pointer to ring_t 
*						-2 if buffer is already full
*/

int32_t insert( ring_t *ring, uint8_t data ){
	if(ring == NULL)
		return -1;
	int32_t Ini_next = ring->Ini +1;
	Ini_next = (Ini_next >= ring->Length)? 0: Ini_next;
	if(ring->Full)
		return -2;
	else if(Ini_next == ring->Outi){
		ring->Buffer[ring->Ini] = data;
		ring->Full = 1;
		return 0;
	}
	else{
		ring->Buffer[ring->Ini] = data;
		ring->Ini = Ini_next;
		return 0;
	}
	
}

/**
*	@brief 		ring buffer functions :extract one element from the buffer
*	
*	@param		ring_t * ring		pointer to the ring_t which represent the buffer to read 
*	@param		uint8_t * data		pointer to the variable which hold the extraced data if read succesfully
*	
*	@return   	0 if read succesfully;
* 						-1 if input with NULL pointer to ring_t  or NULL pointer to uint8_t
*						-2 if buffer is already empty 
*/
int32_t extract( ring_t *ring, uint8_t *data ){
	if(ring == NULL)
		return -1;
	if(data == NULL)
		return -1;
	if(ring->Outi != ring->Ini){
		*data = ring->Buffer[ring->Outi++];
		ring->Outi = (ring->Outi>=ring->Length)? 0: ring->Outi;
		if(ring->Full)
			ring->Ini++;
		ring->Ini = (ring->Ini>=ring->Length)? 0: ring->Ini;	
		ring->Full = 0;
		return 0;
	}
	else
		return -2;
}
/**
*	@brief 		ring buffer functions :return the  elements in the buffer that have been wrote but yet to read 
*	
*	@param		ring_t * ring		pointer to the ring_t which represent the buffer to test 
*	
*	@return   	number of entries if test succesfully;
* 						-1 if input with NULL pointer to ring_t;
*/
int32_t entries( ring_t *ring ){
	if(ring == NULL)
		return -1;
	if(ring->Ini<ring->Outi)
		return ring->Ini+ring->Length-ring->Outi+ring->Full;
	else
		return ring->Ini-ring->Outi+ring->Full;
}

/* int32_t main(void){
	ring_t* ring;
	ring=init(5);
	uint8_t data;
	insert(ring,123);
	insert(ring,56);
	for(int32_t i=0;i<5;i++){
		if(insert(ring,(uint8_t)i)==-2)
			printf("full\n");
		else
			printf("Not full\n");
		printf("entries: %d\n",entries(ring));
		printf("Ini: %d\n\n",ring->Ini);
		for(int32_t j=0;j<5;j++)
			printf("buffer%d: %d  ",j,ring->Buffer[j]);
		printf("\n\n");
	}
	printf("\n\n");
	printf("\n\n");
	extract(ring,&data);
	extract(ring,&data);
	insert(ring,17);
	insert(ring,18);
	insert(ring,19);
	insert(ring,19);
	for(int32_t i=0;i<5;i++){
		printf("Outi: %d\n",ring->Outi);
		if(extract(ring,&data)==-2)
			printf("empty\n");
		else
			printf("Not empty\n");
		printf("entries: %d, extracted: %d\n",entries(ring),data);
		printf("Ini: %d\n",ring->Ini);
		printf("Outi: %d\n\n",ring->Outi);
		for(int32_t j=0;j<5;j++)
			printf("buffer%d: %d  ",j,ring->Buffer[j]);
		printf("\n\n");
	}	
	return 0;
	
} */
