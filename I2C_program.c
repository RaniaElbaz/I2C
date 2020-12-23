/*
 * I2C_program.c
 *
 *  Created on: Dec 20, 2020
 *      Author: Rania Elbaz
 */
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "ERROR_STATUS.h"

#include "RCC_interface.h"

#include "I2C_private.h"
#include "I2C_interface.h"
#include "I2C_config.h"
#include "I2C_register.h"

//Global structures as handles for peripheral devices:
I2C_Init_t i2c;

void I2C1_voidInit(void){
	MRCC_voidEnablePeripheralClock(RCC_APB1,RCC_APB1_I2C1);

	// Setup the I2C BUS properties through the defined "i2c" structure:
	i2c.I2C_ClkSpeed = CLK_SPEED;    //Bus clock speed
	i2c.I2C_Mode = I2C_Mode_I2C;
	i2c.I2C_DutyCycle = I2C_DUTYCYCLE_2;
	// Setup I2C BUS master properties:
	i2c.I2C_OwnAddress1 = 0x15;
	i2c.I2C_Ack = I2C_ACK_ENABLE;
	i2c.I2C_AckAddress = I2C_ACK_ADDRESS_7BIT;

	// Finally initialize the I2C1 unit using the "i2c" structure.
	I2C_voidInit(I2C1, &i2c);

	// Enable the I2C1 module:
	SET_BIT(I2C1_CR1, I2C_CR1_PE);
}

ErrorStatus_t I2C_StartTransmission(I2C_t* I2Cx,u8 Copy_u8TxDirection, u8 Copy_u8SlaveAddress){
	volatile u32 Local_u32Wait;

	// Wait until I2C module is idle:
	Local_u32Wait = I2C_TIMEOUT;
	while(GET_BIT(I2Cx->SR2, I2C_SR2_BUSY) && --Local_u32Wait);
	if(!Local_u32Wait){
		return TIME_OUT;
	}

	// Generate the start condition
	SET_BIT(I2Cx->CR1,I2C_CR1_START);
	Local_u32Wait = I2C_TIMEOUT;
	while(!GET_BIT(I2Cx->CR1, I2C_CR1_START) && --Local_u32Wait);
	if(!Local_u32Wait){
		return TIME_OUT;
	}

	//Master Mode
	Local_u32Wait = I2C_TIMEOUT;
	while(!I2C_CheckEv(I2Cx, I2C_MASTER_MODE) && --Local_u32Wait);
	if(!Local_u32Wait){
		return TIME_OUT;
	}

	//Send the address of the slave to be contacted:
	Local_u32Wait = I2C_TIMEOUT;
	if (Copy_u8TxDirection != I2C_Tx_Direction){
		/*set i2C for receive*/
		while(!I2C_CheckEv(I2Cx, I2C_MASTER_TRANSMITTER) && --Local_u32Wait);
		if(!Local_u32Wait){
			return TIME_OUT;
		}
		/* Set the address bit0 for read */
		SET_BIT(Copy_u8SlaveAddress, I2C_OAR1_ADD0);
	}
	else if (Copy_u8TxDirection != I2C_Rx_Direction){
		/*set I2C for transmit*/
		while(!I2C_CheckEv(I2Cx, I2C_MASTER_RECEIVER) && --Local_u32Wait);
		if(!Local_u32Wait){
			return TIME_OUT;
		}
		/* Reset the address bit0 for write */
		CLR_BIT(Copy_u8SlaveAddress,I2C_OAR1_ADD0);
	}
	else{
		return NOK;
	}

	//Copy address into DR to start transmission
	I2Cx->DR = Copy_u8SlaveAddress;

	CLR_BIT(I2Cx->CR1,I2C_CR1_START);

	return OK;
}

ErrorStatus_t I2C_StopTransmission(I2C_t* I2Cx,u8 Copy_u8TxDirection){
	volatile u32 Local_u32Wait;

	// Generate the stop condition
	SET_BIT(I2Cx->CR1, I2C_CR1_STOP);

	//Wait until transmission is complete:
	Local_u32Wait = I2C_TIMEOUT;
	while(!GET_BIT(I2Cx->CR1, I2C_CR1_STOP) && --Local_u32Wait);
	if(!Local_u32Wait) return TIME_OUT;

	CLR_BIT(I2Cx->CR1, I2C_CR1_STOP);

	return OK;
}

ErrorStatus_t I2C_ReadData(I2C_t* I2Cx, u8* Copy_pu8DataReceived){
	volatile u32 Local_u32Wait;

	// Wait for RxNE flag
	Local_u32Wait = I2C_TIMEOUT;
	while (!GET_BIT(I2Cx->SR1,I2C_SR1_RXNE) && --Local_u32Wait);
	if (!Local_u32Wait) return TIME_OUT;

	*Copy_pu8DataReceived++ = (u8)I2Cx->DR; // Receive byte



	return OK;
}

ErrorStatus_t I2C_WriteData(I2C_t* I2Cx, u8* Copy_u8TxData){
	volatile u32 Local_u32Wait;

	//Wait until transmission is complete:
	Local_u32Wait = I2C_TIMEOUT;
	while(!GET_BIT(I2Cx->SR1, I2C_SR1_TXE) && --Local_u32Wait);
	if(!Local_u32Wait) return TIME_OUT;

	// Write the data on the bus
	I2Cx->DR = Copy_u8TxData;

	//Wait until transmission is complete:
	Local_u32Wait = I2C_TIMEOUT;
	while(!I2C_CheckEv(I2Cx, I2C_MASTER_BYTE_TRANSMITTED) && --Local_u32Wait);
	if(!Local_u32Wait) return TIME_OUT;

	return OK;
}

static void I2C_voidInit(I2C_t* I2Cx, I2C_Init_t* Copy_I2C_InitStruct){

	u16 Local_u16TmpReg = 0, result;
	u32 Local_u32Clk = CLK_FREQ * 1000000;

	/* Get the I2Cx CR2 value */
	Local_u16TmpReg = I2Cx->CR2;
	/* Clear frequency FREQ[5:0] bits */
	Local_u16TmpReg &= I2C_CR2_FREQ_RESET;
	/* Set frequency bits*/
	Local_u16TmpReg |= CLK_FREQ;
	/* Write to I2Cx CR2 */
	I2Cx->CR2 = Local_u16TmpReg;

	/* Disable the selected I2C peripheral to configure TRISE */
	CLR_BIT(I2Cx->CR1,I2C_CR1_PE);
	/* Reset tmpreg value */
	Local_u16TmpReg = 0;

	/* Configure speed in standard mode */
	if (Copy_I2C_InitStruct->I2C_ClkSpeed <= 100000) {
		/* Standard mode speed calculate */
		result = (u16)(Local_u32Clk / (Copy_I2C_InitStruct->I2C_ClkSpeed << 1));
		/* Test if CCR value is under 0x4*/
		if (result < 0x04){
			/* Set minimum allowed value */
			result = 0x04;
		}
		/* Set speed value for standard mode */
		Local_u16TmpReg |= result;
		/* Set Maximum Rise Time for standard mode */
		I2Cx->TRISE = CLK_FREQ + 1;
	}
	else{
		if (Copy_I2C_InitStruct->I2C_DutyCycle == I2C_DUTYCYCLE_2){
			/* Fast mode speed calculate: Tlow/Thigh = 2 */
			result = (u16)(Local_u32Clk / (Copy_I2C_InitStruct->I2C_ClkSpeed * 3));
		}
		else /*I2C_InitStruct->I2C_DutyCycle == I2C_DutyCycle_16_9*/{
			/* Fast mode speed calculate: Tlow/Thigh = 16/9 */
			result = (u16)(Local_u32Clk / (Copy_I2C_InitStruct->I2C_ClkSpeed * 25));
			/* Set DUTY bit */
			result |= I2C_DUTYCYCLE_16_9;
		}

		/* Test if CCR value is under 0x1*/
		if ((result & I2C_CCR_CCR_SET) == 0){
			/* Set minimum allowed value */
			result |= (u16)0x0001;
		}
		/* Set speed value and set F/S bit for fast mode */
		Local_u16TmpReg |= (u16)(result | I2C_CCR_FS_SET);
		/* Set Maximum Rise Time for fast mode */
		I2Cx->TRISE = (u16)(((CLK_FREQ * (u16)300) / (u16)1000) + (u16)1);
	}

	/* Write to I2Cx CCR */
	I2Cx->CCR = Local_u16TmpReg;
	/* Enable the selected I2C peripheral */
	SET_BIT(I2Cx->CR1,I2C_CR1_PE);
	/* Get the I2Cx CR1 value */
	Local_u16TmpReg = I2Cx->CR1;
	/* Clear ACK, SMBTYPE and  SMBUS bits */
	Local_u16TmpReg &= I2C_CR1_CLR_MASK;
	/* Configure I2Cx: mode and acknowledgement */
	/* Set SMBTYPE and SMBUS bits according to I2C_Mode value */
	/* Set ACK bit according to I2C_Ack value */
	Local_u16TmpReg |= (u16)((u32) Copy_I2C_InitStruct->I2C_Mode | Copy_I2C_InitStruct->I2C_Ack);
	/*write to I2Cx CR1*/
	I2Cx->CR1 = Local_u16TmpReg;

	/* Set I2Cx Own Address1 and acknowledged address */
	I2C1_OAR1 = (Copy_I2C_InitStruct->I2C_AckAddress | Copy_I2C_InitStruct->I2C_OwnAddress1);
}

static ErrorStatus_t I2C_CheckEv(I2C_t* I2Cx, u32 Copy_I2C_Event){
	u32 Local_u32LastEv = 0;
	u32 Local_u32Flag1 = 0, Local_u32Flag2 = 0;

	/*Read I2Cx Status Regs*/
	Local_u32Flag1 = I2Cx->SR1;
	Local_u32Flag2 = I2Cx->SR2;
	Local_u32Flag2 = Local_u32Flag2 << 16;

	/*Get the last event*/
	Local_u32LastEv = (Local_u32Flag1 | Local_u32Flag2) & I2C_FLAG_MASK;

	/* Check whether the last event contains the I2C_EVENT */
	if((Local_u32LastEv & Copy_I2C_Event) == Copy_I2C_Event){
		return OK;
	}
	else{
		return NOK;
	}
}
