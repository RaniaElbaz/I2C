/*
 * I2C_private.h
 *
 *  Created on: Dec 20, 2020
 *      Author: Rania Elbaz
 */

#ifndef I2C_PRIVATE_H_
#define I2C_PRIVATE_H_

/****************************** I2C registers *********************************/
//I2C1
#define I2C1_BASEADD			*((volatile u32*) 0x40005400)

//I2C2
#define I2C2_BASEADD			*((volatile u32*) 0x40005800)

/*I2C CR1*/
#define I2C_CR1_PE			0
#define I2C_CR1_START		8
#define I2C_CR1_STOP		9
#define I2C_CR1_ACK			10
#define I2C_CR1_POS 		11
/*I2C CCR*/
#define I2C_CCR_DUTY		14
#define I2C_CCR_FS			15
/*I2C OAR1*/
#define I2C_OAR1_ADD0		0
/*I2C SR2*/
#define I2C_SR2_BUSY		1
/*I2C SR1*/
#define I2C_SR1_ADDR		1
#define I2C_SR1_BTF			2
#define I2C_SR1_RXNE		6
#define I2C_SR1_TXE			7
/********************************************************************************/

//I2C type
typedef struct{
	volatile u16 CR1;
	u16  RESERVED0;
	volatile u16 CR2;
	u16  RESERVED1;
	volatile u16 OAR1;
	u16  RESERVED2;
	volatile u16 OAR2;
	u16  RESERVED3;
	volatile u16 DR;
	u16  RESERVED4;
	volatile u16 SR1;
	u16  RESERVED5;
	volatile u16 SR2;
	u16  RESERVED6;
	volatile u16 CCR;
	u16  RESERVED7;
	volatile u16 TRISE;
	u16  RESERVED8;
} I2C_t;
//Initial I2C struct
typedef struct{
	  u32 I2C_ClkSpeed;            /*!< Specifies the clock frequency.
	                                         This parameter must be set to a value lower than 400kHz */
	  u16 I2C_Mode;                /*!< Specifies the I2C mode.
	                                         This parameter can be a value of @ref I2C_mode */
	  u16 I2C_DutyCycle;           /*!< Specifies the I2C fast mode duty cycle.
	                                         This parameter can be a value of @ref I2C_duty_cycle_in_fast_mode */
	  u16 I2C_OwnAddress1;         /*!< Specifies the first device own address.
	                                         This parameter can be a 7-bit or 10-bit address. */
	  u16 I2C_Ack;                 /*!< Enables or disables the acknowledgement.
	                                         This parameter can be a value of @ref I2C_acknowledgement */
	  u16 I2C_AckAddress;		   /*!< Specifies if 7-bit or 10-bit address is acknowledged.
	                                         This parameter can be a value of @ref I2C_acknowledged_address */
}I2C_Init_t;

#define I2C_Mode_I2C					0x0000

#define I2C_ACK_ENABLE					0x4000

#define I2C_ACK_ADDRESS_7BIT			0x4000

#define I2C_CCR_CCR_SET					0x0FFF
#define I2C_CCR_FS_SET					0x8000

#define I2C_DUTYCYCLE_2					0xBFFF
#define I2C_DUTYCYCLE_16_9				0x4000

#define I2C_CR1_CLR_MASK				0xFBF5

#define I2C_CR2_FREQ_RESET				0xFFC0

#define I2C_FLAG_MASK					(u8)0x1F

/* Timeout value */
#define I2C_TIMEOUT             		1000

/*I2C events modes*/
#define I2C_MASTER_MODE					((u32)0x00030001)
#define I2C_MASTER_RECEIVER        		((u32)0x00030002)
#define I2C_MASTER_TRANSMITTER        	((u32)0x00070082)  /* BUSY, MSL, ADDR, TXE and TRA flags */
#define I2C_MASTER_BYTE_TRANSMITTED		((u32)0x00070084)  /* TRA, BUSY, MSL, TXE and BTF flags */
#define I2C_MASTER_BYTE_RECEIVED		((u32)0x00030040)  /* BUSY, MSL and RXNE flags */

/*CLK FREQ OPTIONS*/
#define FREQ_8MHZ						(u16)8

/*************************Private Functions************************************/
/*
 * Description: Initializes I2C1 peripheral
 */
static void I2C_voidInit(I2C_t* I2Cx, I2C_Init_t* Copy_I2C_InitStruct);

/*Check i2c events*/
static ErrorStatus_t I2C_CheckEv(I2C_t* I2Cx, u32 Copy_I2C_Event);

/*Send Slave Address*/
static void I2C_SendSlaveAddress(I2C_t* I2Cx, u8 Copy_u8Address, u8 Copy_u8Direction);

#endif /* I2C_PRIVATE_H_ */
