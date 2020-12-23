/*
 * I2C_register.h
 *
 *  Created on: Dec 22, 2020
 *      Author: Dalia
 */

#ifndef I2C_REGISTER_H_
#define I2C_REGISTER_H_

/* I2C registers */
//I2C1
#define I2C1_BASE			*((volatile u32*) 0x40005400)
#define I2C1_CR1			*((volatile u32*) 0x40005400)
#define I2C1_CR2			*((volatile u32*) 0x40005404)
#define I2C1_OAR1			*((volatile u32*) 0x40005408)
#define I2C1_OAR2			*((volatile u32*) 0x4000540C)
#define I2C1_DR				*((volatile u32*) 0x40005410)
#define I2C1_SR1			*((volatile u32*) 0x40005414)
#define I2C1_SR2			*((volatile u32*) 0x40005418)
#define I2C1_CCR			*((volatile u32*) 0x4000541C)
#define I2C1_TRISE			*((volatile u32*) 0x40005420)

//I2C2
#define I2C2_BASE			*((volatile u32*) 0x40005800)
#define I2C2_CR1			*((volatile u32*) 0x40005800)
#define I2C2_CR2			*((volatile u32*) 0x40005804)
#define I2C2_OAR1			*((volatile u32*) 0x40005808)
#define I2C2_OAR2			*((volatile u32*) 0x4000580C)
#define I2C2_DR				*((volatile u32*) 0x40005810)
#define I2C2_SR1			*((volatile u32*) 0x40005814)
#define I2C2_SR2			*((volatile u32*) 0x40005818)
#define I2C2_CCR			*((volatile u32*) 0x4000581C)
#define I2C2_TRISE			*((volatile u32*) 0x40005820)

/*I2C CR1*/
#define I2C_CR1_PE			0
#define I2C_CR1_SMBWS		1
#define I2C_CR1_SMBTYPE		3
#define I2C_CR1_ENARP		4
#define I2C_CR1_ENPEC		5
#define I2C_CR1_ENGC	    6
#define I2C_CR1_NOSTRETCH	7
#define I2C_CR1_START		8
#define I2C_CR1_STOP		9
#define I2C_CR1_ACK			10
#define I2C_CR1_POS 		11
#define I2C_CR1_PEC 		12
#define I2C_CR1_ALERT 		13
#define I2C_CR1_SWRST 		15
/*I2C CR2*/
#define I2C_CR2_FREQ0		0
#define I2C_CR2_FREQ1		1
#define I2C_CR2_FREQ2		2
#define I2C_CR2_FREQ3		3
#define I2C_CR2_FREQ4		4
#define I2C_CR2_FREQ5		5
#define I2C_CR2_ITERREN		8
#define I2C_CR2_ITEVTEN		9
#define I2C_CR2_ITBUFEN		10
#define I2C_CR2_DMAEN		11
#define I2C_CR2_LAST		12
/*I2C CCR*/
#define I2C_CCR_DUTY		14
#define I2C_CCR_FS			15
/*I2C OAR1*/
#define I2C_OAR1_ADD0		0
#define I2C_OAR1_ADD1		1
#define I2C_OAR1_ADD2		2
#define I2C_OAR1_ADD3		3
#define I2C_OAR1_ADD4		4
#define I2C_OAR1_ADD5		5
#define I2C_OAR1_ADD6		6
#define I2C_OAR1_ADD7		7
#define I2C_OAR1_ADD8		8
#define I2C_OAR1_ADD9		9
#define I2C_OAR1_ADDMODE	15
/*I2C SR2*/
#define I2C_SR2_BUSY		1
#define I2C_SR1_RXNE		6
#define I2C_SR1_TXE			7

#endif /* I2C_REGISTER_H_ */
