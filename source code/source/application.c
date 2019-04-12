/**
*	@file 			application.c
*	@brief 		application codes for this project
*	
*
*	@author 		
*	@date 			March 30 2019 
*	@version  	1.0
*/

#include "application.h"

static bool first_itr_update_report = false;
static bool empty_before_first_itr_update_report = false;
static bool wait_for_new_char = false;
extern ring_t* TxRingBuffer;
extern ring_t* RxRingBuffer;
extern bool TxRingBufferFullWarning;
extern volatile bool RxRingBufferFullWarning;
extern volatile uint32_t tick;
extern uint32_t seed;
uint32_t report_cnt[256];
static char* sp_char_lookuptable[]={"NUL","SOH","STX","ETX","EOT","ENQ","ACK","BEL","BS","TAB",
		"LF","VT","FF","CR","SO","SI","DLE","DC1","DC2","DC3","DC4",
		"NAK","SYN","ETB","CAN","EM ","SUB","ESC","FS","GS","RS","US","SPACE","DEL"};

void application(void){
	static state current_state = UPDATE_REPORT;
	static uint8_t character;
	static uint32_t updated_cnt = 0;
	static int32_t status;
	switch(current_state){
		case UPDATE_REPORT:
			if(!first_itr_update_report){
				first_itr_update_report=1;
				empty_before_first_itr_update_report = (entries(RxRingBuffer)==0)? true : false;
				if(empty_before_first_itr_update_report){
					wait_for_new_char = true;
					break;
				}
			}
			status = extract(RxRingBuffer,&character);
			if(status != RING_EMPTY){
				wait_for_new_char = false;
				report_cnt[(uint32_t)character]++;
				updated_cnt++;
			}
			if( ((tick >TIMEOUT_TICK) &&  !wait_for_new_char) || (updated_cnt>=MAX_REPORT_CHAR) )//||  ((status ==RING_EMPTY) && !wait_for_new_char)  )
				current_state = WAIT_FOR_PREVIOUS_TX;
			break;
		case WAIT_FOR_PREVIOUS_TX:
			first_itr_update_report = false;
			empty_before_first_itr_update_report = false;
			wait_for_new_char = false;
			updated_cnt = 0;
			if(!UART_NONBLOCKING_TX_BUSY(OPENSDA_UART))
				current_state = GENERATE_REPORT;
			break;
		case GENERATE_REPORT:
			report_generation(TxRingBuffer,report_cnt);
			UART_Interrput_enable(OPENSDA_UART, TxDataRegEmptyInterrupt);
			current_state = UPDATE_REPORT;
			break;
	}
}

void report_generation(ring_t* ring, uint32_t* report_cnt){
	int32_t num;
	char temp_buffer[20];
	char *special_char;
	//report start
	num = sprintf(temp_buffer,"\n\nreport start\n");
	ring_block_insert(ring,(uint8_t*)temp_buffer,num);
	//output the random number
	num = sprintf(temp_buffer,"random number is:");
	ring_block_insert(ring,(uint8_t*)temp_buffer,num);
	num = sprintf(temp_buffer,"%d\n",seed);
	ring_block_insert(ring,(uint8_t*)temp_buffer,num);
	if(TxRingBufferFullWarning){
		TxRingBufferFullWarning=false;
		num = sprintf(temp_buffer,"TxRingBufferFullWarning\n");
		ring_block_insert(ring,(uint8_t*)temp_buffer,num);
	}
	if(RxRingBufferFullWarning){
		RxRingBufferFullWarning=false;
		num = sprintf(temp_buffer,"RxRingBufferFullWarning\n");
		ring_block_insert(ring,(uint8_t*)temp_buffer,num);
	}

	for(int i=0;i<256;i++){
		if(report_cnt[i]==0)
			continue;
		if((i>=0) && (i<=32)){
			special_char = sp_char_lookuptable[i];
			num = sprintf(temp_buffer,"%s-%d\n",special_char,report_cnt[i]);
		}
		else if(i==127){
			special_char = sp_char_lookuptable[33];
			num = sprintf(temp_buffer,"%s-%d\n",special_char,report_cnt[i]);
		}
		else if(i<127){
			num = sprintf(temp_buffer,"%c-%d\n",i,report_cnt[i]);
		}
		else{
			num = sprintf(temp_buffer,"VAL:%d-%d\n",i,report_cnt[i]);
		}
		if(ring_block_insert(ring,(uint8_t*)temp_buffer,num) == RING_FULL){
			TxRingBufferFullWarning=true;
			return;
		}
	}
	num = sprintf(temp_buffer,"report over\n");
	ring_block_insert(ring,(uint8_t*)temp_buffer,num);

}




uint32_t ring_block_insert(ring_t* ring, uint8_t* data, uint32_t length){
	uint32_t status;
	if(data==NULL)
		return NULL_RING_PTR;
	for(int i=0;i<length;i++){
		status=insert(ring,data[i]);
		if(status !=0){
			return status;
		}
	}
	return status;
}

void PRG_data(uint32_t* data){
	if(data==NULL)
		return;
	uint64_t temp =(uint64_t)*data;
	temp=(temp*(uint64_t)MULT+(uint64_t)INC)%((uint64_t)MOD);
	*data = (uint32_t)temp;
}
