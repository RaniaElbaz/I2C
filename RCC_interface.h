/*********************************************************************************/
/* Author    : abdallah reda                                                     */
/* Version   : V01                                                               */
/* Date      : 19 / 12 / 2020                                                    */
/*********************************************************************************/
#ifndef RCC_INTERFACE_H
#define RCC_INTERFACE_H

#define RCC_AHB       0 
#define RCC_APB1      1
#define RCC_APB2      2

/*Peripherals*/
#define RCC_APB1_I2C1	21


void MRCC_voidInitSysClock(void);
void MRCC_voidEnablePeripheralClock(u8 Copy_u8BusId, u8 Copy_u8PerId);
void MRCC_voidDisablePeripheralClock(u8 Copy_u8BusId, u8 Copy_u8PerId);



#endif
