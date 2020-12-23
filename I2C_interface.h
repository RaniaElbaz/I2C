/*
 * I2C_interface.h
 *
 *  Created on: Dec 20, 2020
 *      Author: Rania Elbaz
 */

#ifndef I2C_INTERFACE_H_
#define I2C_INTERFACE_H_

/*I2C handles*/
#define I2C1	((I2C_t *) I2C1_BASE)

/* Transmission Direction*/
#define I2C_Tx_Direction	1
#define I2C_Rx_Direction	0

/*initializes I2C1 only
 * precondition: rcc and gpio initialized*/
void I2C1_voidInit(void);

/*
 * Description: Send Start Condition
 */
ErrorStatus_t I2C_StartTransmission(I2C_t* I2Cx, u8 transmissionDirection,  u8 slaveAddress);

/*
 * Description: Send Stop Condition
 */
void I2C_StopTransmission(I2C_t* I2Cx, u8 transmissionDirection,  u8 slaveAddress);

/*
 * Description:  Reads single byte from device
 * args:    I2Cx: Pointer to I2Cx peripheral to be used in communication
 * outputs: read data from i2c
 */
ErrorStatus_t I2C_ReadData(I2C_t* I2Cx, u8* Copy_pu8DataReceived);

/*
 * Description:  Writes single byte to device
 * Inputs: 1- I2Cx: Pointer to I2Cx peripheral to be used in communication
 * 		   2- data: Data to be written to device
 */
ErrorStatus_t I2C_WriteData(I2C_t* I2Cx, u8 Copy_u8TxData);

/*
 * Description:  Reads multiple bytes from device
 * args:   1- I2Cx: Pointer to I2Cx peripheral to be used in communication
 *		   2- device_address: 7-bit, left aligned device address used for communication
 *		   3- register_address: Register address from where read operation will start
 * 		   4- data: Pointer to variable where data will be stored from read operation
 *		   5- count: Number of elements to read from device
 *outputs: i2c error state: TM_I2C_Result_t
 */
ErrorStatus_t I2C_ReadMulti(I2C_t* I2Cx, u8 device_address, u8 register_address, u8* data, u16 count);

/*
 * Description:  Checks if device is connected to I2C port and ready to use
 * inputs: 1- I2Cx: Pointer to I2Cx peripheral to be used in communication
 * 		   2- device_address: 7-bit, left aligned device address used for communication
 * outputs: i2c error state: TM_I2C_Result_t
 */
ErrorStatus_t I2C_IsConnected(I2C_t* I2Cx, u8 address);

#endif /* I2C_INTERFACE_H_ */
