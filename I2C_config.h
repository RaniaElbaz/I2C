/*
 * I2C_config.h
 *
 *  Created on: Dec 20, 2020
 *      Author: Rania Elbaz
 */

#ifndef I2C_CONFIG_H_
#define I2C_CONFIG_H_


/*I2C Clock Values:
1- I2C_CLOCK_STANDARD          I2C Standard speed
2- I2C_CLOCK_FAST_MODE         I2C Fast mode speed
3- I2C_CLOCK_FAST_MODE_PLUS    I2C Fast mode plus speed
4- I2C_CLOCK_HIGH_SPEED        I2C High speed
 */
#define CLK_SPEED	I2C_CLOCK_FAST_MODE

/*choose clk freq:
 * 1- FREQ_50MHZ
 * 2- FREQ_8MHZ
 * 3- FREQ_2MHZ
 */
#define CLK_FREQ	FREQ_8MHZ

#endif /* I2C_CONFIG_H_ */
