/**
*	@file 			UART.c
*	@brief 		UART functions for this project
*	
*
*	@author 		Linfeng Li
*	@date 			March 30 2019 
*	@version  	1.0
*/

#include "UART.h"
/**
*	@brief 		Put UART received data to buffer (blocking)
*	
*	@param		UART0_Type *base	UART source	
*				uint8_t* data		received data buffer 
*	
*/
void UART_BLOCKING_RX_GETCHAR(UART0_Type *base, uint8_t* data){
	while ((base->S1 & UART0_S1_RDRF_MASK) == 0);
	*(data) = base->D;
}
/**
*	@brief 		Put UART received data to buffer (nonblocking)
*	
*	@param		UART0_Type *base	UART source	
*				uint8_t* data		received data buffer 
*	
*/
void UART_RX_GETCHAR(UART0_Type *base, uint8_t* data){
	*(data) = base->D;
}
/**
*	@brief 		Block the program until all UART transfered data are read (blocking)
*	
*	@param		UART0_Type *base	UART source	
*	
*/
void UART_TX_RD(UART0_Type *base){
	while (!(base->S1 & UART0_S1_TDRE_MASK));
}
/**
*	@brief 		Check the UART transfered procedure complete (nonblocking)
*	
*	@param		UART0_Type *base	UART source	
*	
*	@return		0	process complete
*				1	process incomplete
*/
int32_t UART_NONBLOCKING_TX_BUSY(UART0_Type *base){
	return (((base->C2 & UART0_C2_TCIE_MASK)!=false)?1:0);
}
/**
*	@brief 		Put single character into UART buffer (nonblocking)
*	
*	@param		UART0_Type *base	UART source	
*				uint8_t* data		data need to be put to UART buffer 
*	
*/
void UART_PUTCHAR(UART0_Type *base, uint8_t data){
	 base->D = data;
}
/**
*	@brief 		Put single character into UART buffer after UART transfered procedure complete (blocking)
*	
*	@param		UART0_Type *base	UART source	
*				uint8_t* data		data need to be put to UART buffer 
*	
*/
void UART_BLOCKING_PUTCHAR(UART0_Type *base, uint8_t data){
	UART_TX_RD(base);
	UART_PUTCHAR(base,data);
}
/**
*	@brief 		Put certain length characters into UART buffer after UART transfered procedure complete (blocking)
*	
*	@param		UART0_Type *base	UART source	
*				uint8_t* data		data need to be put to UART buffer 
*				uint32_t length		length of data			
*/
void UART_BLOCKING_WRITEBLOCK(UART0_Type *base, uint8_t* data, uint32_t length){
	while(length!=0){
		length--;
		UART_BLOCKING_PUTCHAR(base, *data);
		data++;
	}
}
/**
*	@brief 		Enable the UART interrupt 
*	
*	@param		UART0_Type *base	UART source	
*				Interrput_Mask type	Type of interrupt		
*/
void UART_Interrput_enable(UART0_Type *base, Interrput_Mask type){
	base->C2 |= (uint8_t)type;
}
/**
*	@brief 		Disable the UART interrupt 
*	
*	@param		UART0_Type *base		UART source	
*				Interrput_Mask type		Type of interrupt		
*/
void UART_Interrput_disable(UART0_Type *base, Interrput_Mask type){
	base->C2 &= ~((uint8_t)type);
}
/**
*	@brief 		Initialize the UART  
*	
*	@param		UART0_Type *base		UART source	
*				UART_config *config		Configure of UART
*				uint32_t src_clk_rate	Clock frequency
*	@return		0	procedure complete
*				-1	ERROR:Unacceptable baud rate difference of more than 3%
*						
*/
uint8_t UART0_Init(UART0_Type *base, UART_config *config, uint32_t src_clk_rate){


	uint8_t pre_val;
    uint16_t sbr = 0;
    uint16_t sbrTemp;
    uint32_t osr = 0;
    uint32_t osrTemp;
    uint32_t tempDiff, calculatedBaud, baudDiff;

    /* This LPSCI instantiation uses a slightly different baud rate calculation
     * The idea is to use the best OSR (over-sampling rate) possible
     * Note, OSR is typically hard-set to 16 in other LPSCI instantiations
     * loop to find the best OSR value possible, one that generates minimum baudDiff
     * iterate through the rest of the supported values of OSR */

    baudDiff = config->Baudrate;
    for (osrTemp = 4; osrTemp <= 32; osrTemp++)
    {
        /* calculate the temporary sbr value   */
        sbrTemp = (src_clk_rate / (config->Baudrate * osrTemp));
        /* set sbrTemp to 1 if the sourceClockInHz can not satisfy the desired baud rate */
        if (sbrTemp == 0)
        {
            sbrTemp = 1;
        }
        /* Calculate the baud rate based on the temporary OSR and SBR values */
        calculatedBaud = (src_clk_rate / (osrTemp * sbrTemp));

        tempDiff = calculatedBaud - config->Baudrate;

        /* Select the better value between srb and (sbr + 1) */
        if (tempDiff > (config->Baudrate - (src_clk_rate / (osrTemp * (sbrTemp + 1)))))
        {
            tempDiff = config->Baudrate - (src_clk_rate / (osrTemp * (sbrTemp + 1)));
            sbrTemp++;
        }

        if (tempDiff <= baudDiff)
        {
            baudDiff = tempDiff;
            osr = osrTemp; /* update and store the best OSR value calculated*/
            sbr = sbrTemp; /* update store the best SBR value calculated*/
        }
    }

    /* next, check to see if actual baud rate is within 3% of desired baud rate
     * based on the best calculate OSR value */
    if (baudDiff > ((config->Baudrate / 100) * 3))
    {
        /* Unacceptable baud rate difference of more than 3%*/
        return (-1);
    }


    /* enable the UART0 clock by disabling the corresponding clock gate */
    SIM->SCGC4= (SIM->SCGC4 & ~SIM_SCGC4_UART0_MASK) | SIM_SCGC4_UART0(true);


    /* Disable TX RX before setting. */
    base->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);

    /* Acceptable baud rate */
    /* Check if OSR is between 4x and 7x oversampling*/
    /* If so, then "BOTHEDGE" sampling must be turned on*/
    if ((osr > 3) && (osr < 8))
    {
        base->C5 |= UART0_C5_BOTHEDGE_MASK;
    }

    /* program the osr value (bit value is one less than actual value)*/
    base->C4 = ((base->C4 & ~UART0_C4_OSR_MASK) | (osr - 1));

    /* program the sbr (divider) value obtained above*/
    base->BDH = ((base->C4 & ~UART0_BDH_SBR_MASK) | (uint8_t)(sbr >> 8));
    base->BDL = (uint8_t)sbr;

    /* set parity mode */
    pre_val = base->C1 & ~(UART0_C1_PE_MASK | UART0_C1_PT_MASK | UART0_C1_M_MASK);

    if (config->ParityMode != ParityDisabled )
    {
        pre_val |= (uint8_t)config->ParityMode | UART0_C1_M_MASK;
    }

    base->C1 = pre_val;

    /* set stop bit per char */
    base->BDH &= ~UART0_BDH_SBNS_MASK;
    base->BDH |= UART0_BDH_SBNS((uint8_t)config->Stopbit);


    /* Enable TX/RX base on configure structure. */
    pre_val = base->C2;

    if (config->Txenable)
    {
        pre_val |= UART0_C2_TE_MASK;
    }

    if (config->Rxenable)
    {
        pre_val |= UART0_C2_RE_MASK;
    }

    base->C2 = pre_val;

    return true;
}

