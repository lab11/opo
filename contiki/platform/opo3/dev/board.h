/*
 * Copyright (c) 2012, Texas Instruments Incorporated - http://www.ti.com/
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** \addtogroup cc2538
 * @{
 *
 * \defgroup cc2538-smartrf SmartRF06EB Peripherals
 *
 * Defines related to the SmartRF06EB
 *
 * This file provides connectivity information on LEDs, Buttons, UART and
 * other SmartRF peripherals
 *
 * Notably, PC0 is used to drive LED1 as well as the USB D+ pullup. Therefore
 * when USB is enabled, LED1 can not be driven by firmware.
 *
 * This file can be used as the basis to configure other platforms using the
 * cc2538 SoC.
 * @{
 *
 * \file
 * Header file with definitions related to the I/O connections on the TI
 * SmartRF06EB
 *
 * \note   Do not include this file directly. It gets included by contiki-conf
 *         after all relevant directives have been set.
 */
#ifndef BOARD_H_
#define BOARD_H_

#include "dev/gpio.h"
#include "dev/nvic.h"
/*---------------------------------------------------------------------------*/
/** \name LED configuration
 *
 *
 * @{
 */
/*---------------------------------------------------------------------------*/
/* Some files include leds.h before us, so we need to get rid of defaults in
 * leds.h before we provide correct definitions */
#undef LEDS_GREEN
#undef LEDS_YELLOW
#undef LEDS_RED
#undef LEDS_CONF_ALL

 /* No actual leds on board Using

 */

#define LEDS_YELLOW    128 /**< LED2 (Yellow) -> PC1 */
#define LEDS_GREEN     64 /**< LED3 (Green)  -> PC2 */
#define LEDS_ORANGE    32 /**< LED4 (Orange) -> PC3 */

#define LEDS_CONF_ALL 224

/* Notify various examples that we have LEDs */
#define PLATFORM_HAS_LEDS        0
/** @} */
/*---------------------------------------------------------------------------*/
/** \name UART configuration
 *
 * UART 0 configuration. This is how the bsl programs the board
 * @{
 */
#define UART0_RX_PORT            GPIO_A_NUM
#define UART0_RX_PIN             0

#define UART0_TX_PORT            GPIO_A_NUM
#define UART0_TX_PIN             1


/** @} */
/*---------------------------------------------------------------------------*/
/**
 * \name ADC configuration
 *
 * These values configure which CC2538 pins and ADC channels to use for the ADC
 * inputs.
 *
 * ADC inputs can only be on port A.
 * @{
 */
#define ADC_ALS_PWR_PORT         GPIO_A_NUM /**< ALS power GPIO control port */
#define ADC_ALS_PWR_PIN          7 /**< ALS power GPIO control pin */
#define ADC_ALS_OUT_PIN          6 /**< ALS output ADC input pin on port A */

/** @} */
/*---------------------------------------------------------------------------*/
/** \name Pin configs
 *
 * Opo4 pin configurations
 * @{
 */
// Software control pin masks
#define OPO_SFC_A_MASK	0xF8
#define OPO_SFC_B_MASK	0x97
#define OPO_SFC_C_MASK	0xF2
#define OPO_SFC_D_MASK	0xFF

// TX_RX sel pin info
#define OPO_TX_RX_SEL_PORT_BASE    GPIO_C_BASE
#define OPO_TX_RX_SEL_PIN_MASK	   0x02

// Integrator pin info
#define OPO_INT_PORT_BASE	GPIO_C_BASE
#define OPO_INT_PORT_NUM	GPIO_C_NUM
#define OPO_INT_PIN_MASK	0x80
#define OPO_INT_PIN_NUM		7
#define OPO_GPT_INT_PIN		23

// COMP1 pin info
#define OPO_COMP1_PORT_BASE	GPIO_C_BASE
#define OPO_COMP1_PORT_NUM	GPIO_C_NUM
#define OPO_COMP1_PIN_MASK	0x40
#define OPO_COMP1_PIN_NUM	6
#define OPO_GPT_COMP1_PIN	22

// COMP2 pin info
#define OPO_COMP2_PORT_BASE	GPIO_C_BASE
#define OPO_COMP2_PORT_NUM	GPIO_C_NUM
#define OPO_COMP2_PIN_MASK	0x20
#define OPO_COMP2_PIN_NUM	5
#define OPO_GPT_COMP2_PIN	21

// TX PWM Pins
#define OPO_PWM_PORT_BASE	GPIO_C_BASE
#define OPO_PWM_PORT_NUM	GPIO_C_NUM
#define OPO_PWM_PIN_MASK	0x01
#define OPO_PWM_PIN_NUM	    0

#define GPIO_A_OUTPUT_MASK 0xF8
#define GPIO_A_INPUT_MASK  0x00
#define GPIO_A_CLR_MASK    0xF8
#define GPIO_A_SET_MASK    0x00

#define GPIO_B_OUTPUT_MASK 0x13
#define GPIO_B_INPUT_MASK  0x84
#define GPIO_B_CLR_MASK	   0x00
#define GPIO_B_SET_MASK    0x13

#define GPIO_C_OUTPUT_MASK 0x03
#define GPIO_C_INPUT_MASK  0xe0
#define GPIO_C_CLR_MASK    0x03
#define GPIO_C_SET_MASK    0x00

#define GPIO_D_OUTPUT_MASK 0xff
#define GPIO_D_INPUT_MASK  0x00
#define GPIO_D_CLR_MASK    0xff
#define GPIO_D_SET_MASK    0x00

#define OPO_INT_NVIC       NVIC_INT_GPIO_PORT_C

//I2C Pins
#define I2C_SDA_PORT_NUM   GPIO_C_NUM
#define I2C_SDA_PIN_NUM	   3

#define I2C_SCL_PORT_NUM   GPIO_C_NUM
#define I2C_SCL_PIN_NUM	   2

// flash
#define SST25VF_CS_PORT_NUM					GPIO_B_NUM
#define SST25VF_CS_PIN_NUM					1
#define SST25VF_FLASH_POWER_PORT_BASE		GPIO_B_BASE
#define SST25VF_FLASH_POWER_PIN_MASK		0x01

/** @} */
/*---------------------------------------------------------------------------*/
/**
 * \name nrf8001 configuration
 *
 * These values configure which CC2538 pins are used to communicate with the
 * nrf8001.
 * @{
 */

#define RADIO_SELECT_PORT_BASE  GPIO_D_BASE
#define RADIO_SELECT_PIN_MASK   0x04
#define RADIO_SELECT_PORT		GPIO_D_NUM
#define RADIO_SELECT_PIN		2
#define RADIO_SELECT_VECTOR		NVIC_INT_GPIO_PORT_D

#define NRF8001_RDYN_PORT       GPIO_B_NUM
#define NRF8001_RDYN_PORT_BASE	GPIO_B_BASE
#define NRF8001_RDYN_PIN        7
#define NRF8001_RDYN_PIN_MASK	0x80
#define NRF8001_RDYN_VECTOR     NVIC_INT_GPIO_PORT_B

#define NRF8001_REQN_PORT 		GPIO_B_NUM
#define NRF8001_REQN_PORT_BASE	GPIO_B_BASE
#define NRF8001_REQN_PIN		4
#define NRF8001_REQN_PIN_MASK	0x10
#define NRF8001_REQN_VECTOR 	NVIC_INT_GPIO_PORT_B

#define NRF8001_ACTIVE_PORT		 GPIO_B_NUM
#define NRF8001_ACTIVE_PORT_BASE GPIO_B_BASE
#define NRF8001_ACTIVE_PIN		 2
#define NRF8001_ACTIVE_PIN_MASK	 0x04
#define NRF8001_ACTIVE_VECTOR	 NVIC_INT_GPIO_PORT_B
/** @} */

/*---------------------------------------------------------------------------*/
/**
 * \name SPI configuration
 *
 * These values configure which CC2538 pins to use for the SPI lines.
 * @{
 */
#define SPI_CLK_PORT             GPIO_B_NUM
#define SPI_CLK_PIN              3
#define SPI_MOSI_PORT            GPIO_B_NUM
#define SPI_MOSI_PIN             5
#define SPI_MISO_PORT            GPIO_B_NUM
#define SPI_MISO_PIN             6
/** @} */
/*---------------------------------------------------------------------------*/
/**
 * \name Device string used on startup
 * @{
 */
#define BOARD_STRING "Opo3"
/** @} */

#endif /* BOARD_H_ */

/**
 * @}
 * @}
 */
