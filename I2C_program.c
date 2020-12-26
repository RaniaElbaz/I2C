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

I2C_Init_t i2c;

void I2C1_voidInit(u32 Copy_u32ClkSpeed){
	MRCC_voidEnablePeripheralClock(RCC_APB1,RCC_APB1_I2C1);

	// Setup the I2C BUS properties through the defined "i2c" structure:
	i2c.I2C_ClkSpeed = Copy_u32ClkSpeed;    //Bus clock speed
	i2c.I2C_Mode = I2C_Mode_I2C;
	i2c.I2C_DutyCycle = I2C_DUTYCYCLE_2;
	// Setup I2C BUS master properties:
	i2c.I2C_OwnAddress1 = 0x00;
	i2c.I2C_Ack = I2C_ACK_ENABLE;
	i2c.I2C_AckAddress = I2C_ACK_ADDRESS_7BIT;

	// Finally initialize the I2C1 unit using the "i2c" structure.
	I2C_voidInit(I2C1, &i2c);
}

ErrorStatus_t I2C_StartTransmission(I2C_t* I2Cx,u8 Copy_u8Direction, u8 Copy_u8SlaveAddress){
	volatile u32 Local_u32Wait;

	// Wait until I2C module is idle:
	Local_u32Wait = I2C_TIMEOUT;
	while(GET_BIT(I2Cx->SR2, I2C_SR2_BUSY) && --Local_u32Wait);
	if(!Local_u32Wait){
		return TIME_OUT;
	}

	// Generate the start condition
	SET_BIT(I2Cx->CR1,I2C_CR1_START);

	//Master Mode
	Local_u32Wait = I2C_TIMEOUT;
	while((I2C_CheckEv(I2Cx, I2C_MASTER_MODE) == NOK) && --Local_u32Wait);
	if(!Local_u32Wait){
		return TIME_OUT;
	}

	//Send the address of the slave to be contacted:
	I2C_SendSlaveAddress(I2Cx, Copy_u8SlaveAddress, Copy_u8Direction);

	return OK;
}

ErrorStatus_t I2C_ReadData(I2C_t* I2Cx, u8* Copy_pu8DataReceived, u16 Copy_u16Size){
	volatile u32 Local_u32Wait;

	if(Copy_u16Size <= 0U){
		return NOK;
	}
	else if(Copy_u16Size == 1U){
		/* Disable Acknowledge */
		CLR_BIT(I2Cx->CR1, I2C_CR1_ACK);
		/*Clear ADDR*/
		CLR_BIT(I2Cx->SR1, I2C_SR1_ADDR);
		// Generate the stop condition
		CLR_BIT(I2Cx->CR1, I2C_CR1_START);
		SET_BIT(I2Cx->CR1, I2C_CR1_STOP);
	}
	else if(Copy_u16Size == 2U){
		/*Enable Pos */
		SET_BIT(I2Cx->CR1, I2C_CR1_POS);
		/*Enable Acknowledge */
		SET_BIT(I2Cx->CR1, I2C_CR1_ACK);
		/*wait for ADDR to be set*/
		Local_u32Wait = I2C_TIMEOUT;
		while(!GET_BIT(I2Cx->SR1, I2C_SR1_ADDR) && --Local_u32Wait);
		if(!Local_u32Wait)	return TIME_OUT;
		/*Clear ADDR*/
		CLR_BIT(I2Cx->SR1, I2C_SR1_ADDR);
		/* Disable Acknowledge */
		CLR_BIT(I2Cx->CR1, I2C_CR1_ACK);
	}
	else{
		/* Enable Acknowledge */
		SET_BIT(I2Cx->CR1, I2C_CR1_ACK);
		/*Clear ADDR*/
		CLR_BIT(I2Cx->SR1, I2C_SR1_ADDR);
	}

	while(Copy_u16Size > 0U){
		if(Copy_u16Size <= 3U){
			if(Copy_u16Size == 1U){
				//Wait until RXNE flag is set
				Local_u32Wait = I2C_TIMEOUT;
				while (!GET_BIT(I2Cx->SR1,I2C_SR1_RXNE) && --Local_u32Wait);
				if (!Local_u32Wait) return TIME_OUT;

				(*Copy_pu8DataReceived++) = (u8)I2Cx->DR; // Receive byte
				--Copy_u16Size;
			}
			else if(Copy_u16Size == 2U){
				//Wait until BTF flag is set
				Local_u32Wait = I2C_TIMEOUT;
				while (!GET_BIT(I2Cx->SR1,I2C_SR1_BTF) && --Local_u32Wait);
				if (!Local_u32Wait) return TIME_OUT;
				// Generate the stop condition
				SET_BIT(I2Cx->CR1, I2C_CR1_STOP);

				(*Copy_pu8DataReceived++) = (u8)I2Cx->DR; // Receive byte
				--Copy_u16Size;
				(*Copy_pu8DataReceived++) = (u8)I2Cx->DR; // Receive byte
				--Copy_u16Size;
			}
			else{
				//Wait until BTF flag is set
				Local_u32Wait = I2C_TIMEOUT;
				while (!GET_BIT(I2Cx->SR1,I2C_SR1_BTF) && --Local_u32Wait);
				if (!Local_u32Wait) return TIME_OUT;
				/* Disable Acknowledge */
				CLR_BIT(I2Cx->CR1, I2C_CR1_ACK);

				(*Copy_pu8DataReceived++) = (u8)I2Cx->DR; // Receive byte
				--Copy_u16Size;

				//Wait until BTF flag is set
				Local_u32Wait = I2C_TIMEOUT;
				while (!GET_BIT(I2Cx->SR1,I2C_SR1_BTF) && --Local_u32Wait);
				if (!Local_u32Wait) return TIME_OUT;
				// Generate the stop condition
				SET_BIT(I2Cx->CR1, I2C_CR1_STOP);

				(*Copy_pu8DataReceived++) = (u8)I2Cx->DR; // Receive byte
				--Copy_u16Size;
				(*Copy_pu8DataReceived++) = (u8)I2Cx->DR; // Receive byte
				--Copy_u16Size;
			}
		}
		else{
			//Wait until RXNE flag is set
			Local_u32Wait = I2C_TIMEOUT;
			while (!GET_BIT(I2Cx->SR1,I2C_SR1_RXNE) && --Local_u32Wait);
			if (!Local_u32Wait) return TIME_OUT;

			(*Copy_pu8DataReceived++) = (u8)I2Cx->DR; // Receive byte
			--Copy_u16Size;
		}
	}

	return OK;
}

ErrorStatus_t I2C_WriteData(I2C_t* I2Cx, u8* Copy_u8TransmittedData, u16 Copy_u16Size){
	volatile u32 Local_u32Wait;

	/*Clear ADDR*/
	CLR_BIT(I2Cx->SR1, I2C_SR1_ADDR);

	while(Copy_u16Size > 0U){
		//Wait until transmission is complete:
		Local_u32Wait = I2C_TIMEOUT;
		while(!GET_BIT(I2Cx->SR1, I2C_SR1_TXE) && --Local_u32Wait);
		if(!Local_u32Wait) return TIME_OUT;

		// Write the data on the bus
		I2Cx->DR = (*Copy_u8TransmittedData++);
		--Copy_u16Size;

		if(GET_BIT(I2Cx->SR1, I2C_SR1_BTF) && (Copy_u16Size != 0U)){
			// Write the data on the bus
			I2Cx->DR = (*Copy_u8TransmittedData++);
			--Copy_u16Size;
		}

		//wait till BTF is set
		Local_u32Wait = I2C_TIMEOUT;
		while(!GET_BIT(I2Cx->SR1, I2C_SR1_BTF) && --Local_u32Wait);
		if(!Local_u32Wait) return TIME_OUT;
	}

	// Generate the stop condition
	CLR_BIT(I2Cx->CR1, I2C_CR1_START);
	SET_BIT(I2Cx->CR1, I2C_CR1_STOP);

	return OK;
}

static void I2C_voidInit(I2C_t* I2Cx, I2C_Init_t* Copy_I2C_InitStruct){
	u16 Local_u16TmpReg = 0, result;
	u32 Local_u32Clk = CLK_FREQ * 1000000;
	/***************************CR2 config*********************************/
	/* Get the I2Cx CR2 value */
	Local_u16TmpReg = I2Cx->CR2;
	/* Clear frequency FREQ[5:0] bits */
	Local_u16TmpReg &= I2C_CR2_FREQ_RESET;
	/* Set frequency bits*/
	Local_u16TmpReg |= CLK_FREQ;
	/* Write to I2Cx CR2 */
	I2Cx->CR2 = Local_u16TmpReg;

	/***************************CCR config*********************************/
	/* Disable the selected I2C peripheral to configure TRISE */
	CLR_BIT(I2Cx->CR1,I2C_CR1_PE);
	/* Reset tmpreg value */
	Local_u16TmpReg = 0;
	/* Configure speed in standard mode */
	if (Copy_I2C_InitStruct->I2C_ClkSpeed <= 100000){
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
	else{ /*(Copy_I2C_InitStruct->I2C_ClkSpeed <= 400000)*/
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

	/***************************CR1 config*********************************/
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

	/***************************OAR1 config*********************************/
	/* Set I2Cx Own Address1 and acknowledged address */
	I2Cx->OAR1 = (Copy_I2C_InitStruct->I2C_AckAddress | Copy_I2C_InitStruct->I2C_OwnAddress1);

	// Enable the I2C1 module:
	SET_BIT(I2Cx->CR1, I2C_CR1_PE);

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

static void I2C_SendSlaveAddress(I2C_t* I2Cx, u8 Copy_u8Address, u8 Copy_u8Direction){
	if (Copy_u8Direction != I2C_Tx_Direction){
		/* Set the address bit0 for read */
		SET_BIT(Copy_u8Address, I2C_OAR1_ADD0);
	}
	else if(Copy_u8Direction != I2C_Rx_Direction){
		/* Reset the address bit0 for write */
		CLR_BIT(Copy_u8Address,I2C_OAR1_ADD0);
	}

	//Copy address into DR to start transmission
	I2Cx->DR = Copy_u8Address;
}
