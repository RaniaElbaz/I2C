/*
 * I2C_config.h
 *
 *  Created on: Dec 20, 2020
 *      Author: Rania Elbaz
 */

#ifndef I2C_CONFIG_H_
#define I2C_CONFIG_H_

/*
 * choose clk speed:
 * must be >= 100k
 * and < 400k
 */
#define CLK_SPEED	100000

/*choose clk freq:
 * 1- FREQ_50MHZ
 * 2- FREQ_8MHZ
 * 3- FREQ_2MHZ
 */
#define CLK_FREQ	FREQ_8MHZ

#endif /* I2C_CONFIG_H_ */
