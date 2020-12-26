/*
 * I2C_interface.h
 *
 *  Created on: Dec 20, 2020
 *      Author: Rania Elbaz
 */

#ifndef I2C_INTERFACE_H_
#define I2C_INTERFACE_H_

/*I2C handles I2Cx*/
#define I2C1	((I2C_t *) I2C1_BASEADD)

/*I2C Clock Values*/
#define I2C_CLOCK_STANDARD         (u32)100000  /* I2C Standard speed */
#define I2C_CLOCK_FAST_MODE        (u32)400000  /* I2C Fast mode speed */
#define I2C_CLOCK_FAST_MODE_PLUS   (u32)1000000 /* I2C Fast mode plus speed */
#define I2C_CLOCK_HIGH_SPEED       (u32)3400000 /* I2C High speed */

/* Transmission Direction*/
#define I2C_Tx_Direction	1 //transmit
#define I2C_Rx_Direction	0 //receive

/*initializes I2C1 only
 * precondition: rcc and gpio initialized
 */
void I2C1_voidInit(u32 Copy_u32ClkSpeed);

/*
 * Description: Send Start Condition
 */
ErrorStatus_t I2C_StartTransmission(I2C_t* I2Cx, u8 transmissionDirection,  u8 slaveAddress);

/*
 * Description:  Reads single byte from device
 * args:    I2Cx: Pointer to I2Cx peripheral to be used in communication
 * outputs: read data from i2c
 */
ErrorStatus_t I2C_ReadData(I2C_t* I2Cx, u8* Copy_pu8DataReceived, u16 Copy_u16Size);

/*
 * Description:  Writes single byte to device
 * Inputs: 1- I2Cx: Pointer to I2Cx peripheral to be used in communication
 * 		   2- data: Data to be written to device
 */
ErrorStatus_t I2C_WriteData(I2C_t* I2Cx, u8 Copy_u8TxData);

#endif /* I2C_INTERFACE_H_ */
