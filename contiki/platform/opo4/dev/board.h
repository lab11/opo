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
 * Defines related to the Opo4 Platform
 *
 * This file provides connectivity information on LEDs, UL frontends, etc
 *
 * This file can be used as the basis to configure other platforms using the
 * cc2538 SoC.
 * @{
 *
 * \file
 * Header file with definitions related to I/O connections
 *
 * \note   Do not include this file directly. It gets included by contiki-conf
 *         after all relevant directives have been set.
 */
#ifndef BOARD_H_
#define BOARD_H_

#include "dev/gpio.h"
#include "dev/nvic.h"
#include "dev/gptimer.h"
#include "dev/ioc.h"
/*---------------------------------------------------------------------------*/
/** \name LED configuration
 *
 *
 * @{
 */
/*---------------------------------------------------------------------------*/
/* Some files include leds.h before us, so we need to get rid of defaults in
 * leds.h before we provide correct definitions */
#undef LEDS_RED
#undef LEDS_GREEN
#undef LEDS_BLUE
#undef LEDS_CONF_ALL

 /* No actual leds on board Using

 */

#define LEDS_RED    0x01 /* PB0 */
#define LEDS_GREEN  0x10 /* PC4 */
#define LEDS_BLUE   0x40 /* PC6 */

#define LEDS_CONF_ALL 0x51

/* Notify various examples that we have LEDs */
#define PLATFORM_HAS_LEDS        1
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
/** \name Pin configs
 *
 * Opo4 pin configurations
 * @{
 */

// TX_RX sel pin info
#define OPO_TX_RX_SEL_PORT_BASE    GPIO_A_BASE
#define OPO_TX_RX_SEL_PIN_MASK	   0x08

// Integrator pin info
#define OPO_INT_PORT_BASE	GPIO_C_BASE
#define OPO_INT_PORT_NUM	GPIO_C_NUM
#define OPO_INT_PIN_MASK	0x01
#define OPO_INT_PIN_NUM		0
#define OPO_GPT_INT_PIN		16

// COMP1 pin info
#define OPO_COMP1_PORT_BASE	GPIO_C_BASE
#define OPO_COMP1_PORT_NUM	GPIO_C_NUM
#define OPO_COMP1_PIN_MASK	0x02
#define OPO_COMP1_PIN_NUM	1
#define OPO_GPT_COMP1_PIN	17

// COMP2 pin info
#define OPO_COMP2_PORT_BASE	GPIO_A_BASE
#define OPO_COMP2_PORT_NUM	GPIO_A_NUM
#define OPO_COMP2_PIN_MASK	0x80
#define OPO_COMP2_PIN_NUM	7
#define OPO_GPT_COMP2_PIN	7

// TX PWM Pins
#define OPO_PWM_PORT_BASE	GPIO_A_BASE
#define OPO_PWM_PORT_NUM	GPIO_A_NUM
#define OPO_PWM_PIN_MASK	0x10
#define OPO_PWM_PIN_NUM	    4

#define GPIO_A_OUTPUT_MASK 0x7a
#define GPIO_A_INPUT_MASK  0x85
#define GPIO_A_CLR_MASK    0xff
#define GPIO_A_SET_MASK    0x00

#define GPIO_B_OUTPUT_MASK 0x3b
#define GPIO_B_INPUT_MASK  0xc4
#define GPIO_B_CLR_MASK	   0xec
#define GPIO_B_SET_MASK    0x13

#define GPIO_C_OUTPUT_MASK 0xfc
#define GPIO_C_INPUT_MASK  0x03
#define GPIO_C_CLR_MASK    0xa7
#define GPIO_C_SET_MASK    0x58

#define GPIO_D_OUTPUT_MASK 0xFF
#define GPIO_D_INPUT_MASK  0x00
#define GPIO_D_CLR_MASK    0x27
#define GPIO_D_SET_MASK    0xD8

#define OPO_INT_NVIC       NVIC_INT_GPIO_PORT_C
#define OPO_COMP1_NVIC     NVIC_INT_GPIO_PORT_C

/** @} */
/*---------------------------------------------------------------------------*/
/** \name Pin configs
 *
 * Opo4 tx and rx timer config
 * @{
 */
#define OPO_PWM_GPTIMER			GPTIMER_1
#define OPO_PWM_GPSUBTIMER		GPTIMER_SUBTIMER_A

#define OPO_COMP1_GPTIMER		GPTIMER_2
#define OPO_COMP1_GPSUBTIMER	GPTIMER_SUBTIMER_A
#define OPO_COMP1_GPT_OCP		IOC_GPT2OCP1
#define OPO_COMP1_GPT_BASE		GPTIMER_2_BASE
#define OPO_COMP1_GPT_TV		GPTIMER_TAV
#define OPO_COMP1_OFINT			GPTIMER_ICR_TATOCINT // overflow interrupt
#define OPO_COMP1_CAPINT		GPTIMER_ICR_CAECINT  // capture interrupt
#define OPO_COMP1_GPT_NVIC		NVIC_INT_GPTIMER_2A

#define OPO_COMP2_GPTIMER		GPTIMER_2
#define OPO_COMP2_GPSUBTIMER	GPTIMER_SUBTIMER_B
#define OPO_COMP2_GPT_OCP		IOC_GPT2OCP2
#define OPO_COMP2_GPT_BASE		GPTIMER_2_BASE
#define OPO_COMP2_GPT_TV		GPTIMER_TBV

#define OPO_INT_GPTIMER			GPTIMER_1
#define OPO_INT_GPSUBTIMER		GPTIMER_SUBTIMER_B
#define OPO_INT_GPT_OCP			IOC_GPT1OCP2

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
#define ADC_ALS_PWR_PIN          5 /**< ALS power GPIO control pin */
#define ADC_ALS_OUT_PIN          6 /**< ALS output ADC input pin on port A */
/** @} */
/** @} */
/*---------------------------------------------------------------------------*/
/**
 * \name nrf8001 configuration
 *
 * These values configure which CC2538 pins are used to communicate with the
 * nrf8001.
 * @{
 */

#define RADIO_SELECT_PORT			GPIO_D_NUM
#define RADIO_SELECT_PORT_BASE  	GPIO_D_BASE
#define RADIO_SELECT_PIN			2
#define RADIO_SELECT_PIN_MASK   	0x04
#define RADIO_SELECT_VECTOR			NVIC_INT_GPIO_PORT_D

#define NRF8001_RDYN_PORT       	GPIO_B_NUM
#define NRF8001_RDYN_PORT_BASE		GPIO_B_BASE
#define NRF8001_RDYN_PIN        	7
#define NRF8001_RDYN_PIN_MASK		0x80
#define NRF8001_RDYN_VECTOR     	NVIC_INT_GPIO_PORT_B

#define NRF8001_REQN_PORT 			GPIO_B_NUM
#define NRF8001_REQN_PORT_BASE		GPIO_B_BASE
#define NRF8001_REQN_PIN			4
#define NRF8001_REQN_PIN_MASK		0x10
#define NRF8001_REQN_VECTOR 		NVIC_INT_GPIO_PORT_B

#define NRF8001_ACTIVE_PORT		 	GPIO_B_NUM
#define NRF8001_ACTIVE_PORT_BASE 	GPIO_B_BASE
#define NRF8001_ACTIVE_PIN		 	2
#define NRF8001_ACTIVE_PIN_MASK	 	0x04
#define NRF8001_ACTIVE_VECTOR	 	NVIC_INT_GPIO_PORT_B

#define NRF8001_RESET_PORT			GPIO_D_NUM
#define NRF8001_RESET_PORT_BASE		GPIO_D_BASE
#define NRF8001_RESET_PIN			6
#define NRF8001_RESET_PIN_MASK		0x40
#define NRF8001_RESET_VECTOR		NVIC_INT_GPIO_PORT_D


/** @} */

/*---------------------------------------------------------------------------*/
/**
 * \name Flash Storage configuration
 *
 * These values configure which CC2538 pins are used to communicate with the
 * SST25VF flash chip.
 * @{
 */

#define SST25VF_CS_PORT_NUM					GPIO_B_NUM
#define SST25VF_CS_PIN_NUM					1

#define SST25VF_FLASH_POWER_PORT_BASE		GPIO_C_BASE
#define SST25VF_FLASH_POWER_PIN_MASK		0x08

#define SST25VF_FLASH_RESET_PORT_BASE		GPIO_D_BASE
#define SST25VF_FLASH_RESET_PIN_MASK		0x02

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

#define I2C_SDA_PORT_NUM   GPIO_D_NUM
#define I2C_SDA_PIN_NUM	   4

#define I2C_SCL_PORT_NUM   GPIO_D_NUM
#define I2C_SCL_PIN_NUM	   3
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
