#include "contiki.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "dev/leds.h"
#include "nrf8001.h"
#include "spi-arch.h"
#include "spi.h"
#include "dev/gptimer.h"
#include "dev/sys-ctrl.h"
#include "dev/nvic.h"
#include "ioc.h"
#include "dev/ssi.h"
#include "lpm.h"

/*---------------------------------------------------------------------------*/
PROCESS(spi_test, "spiTest");
AUTOSTART_PROCESSES(&spi_test);
/*---------------------------------------------------------------------------*/

uint32_t test_tamr = 0;
uint32_t test_cfg = 0;

void test_func(uint8_t timer, uint8_t subtimer, uint8_t i, uint32_t gpt_time) {
	leds_on(LEDS_YELLOW);
	printf("Test func\n");
	printf("gpt %lu\n", gpt_time);
	printf("-----------------\n");
	nvic_interrupt_enable(NVIC_INT_GPTIMER_1A);
}

void info_func(uint8_t port, uint8_t pin) {
	uint32_t mtamr = REG(GPT_1_BASE | GPTIMER_TAMR);
	uint32_t mctl = REG(GPT_1_BASE | GPTIMER_CTL);
	uint32_t mcfg = REG(GPT_1_BASE | GPTIMER_CFG);
	uint32_t mgptimerimr = REG(GPT_1_BASE | GPTIMER_IMR);
	uint32_t mgptimerris = REG(GPT_1_BASE | GPTIMER_RIS);
	uint32_t mgptimermis = REG(GPT_1_BASE | GPTIMER_MIS);
	uint32_t mgptimericr = REG(GPT_1_BASE | GPTIMER_ICR);
	uint32_t rcgcgpt = REG(SYS_CTRL_RCGCGPT);
	uint32_t scgcgpt = REG(SYS_CTRL_SCGCGPT);
	uint32_t dcgcgpt = REG(SYS_CTRL_DCGCGPT);
	uint32_t srgpt = REG(SYS_CTRL_SRGPT);
	uint32_t gpt1ocp1 = REG(IOC_GPT1OCP1);
	uint32_t gpioafsel = REG(GPIO_C_BASE | GPIO_AFSEL);
	uint32_t ioc_sel = REG(IOC_PC4_SEL);

	printf("TAMR: %lu\n", mtamr);
	printf("CTL: %lu\n", mctl);
	printf("CFG: %lu\n", mcfg);
	printf("IMR: %lu\n", mgptimerimr);
	printf("RIS: %lu\n", mgptimerris);
	printf("MIS: %lu\n", mgptimermis);
	printf("ICR: %lu\n", mgptimericr);
	printf("rcgcgpt: %lu\n", rcgcgpt);
	printf("scgcgpt: %lu\n", scgcgpt);
	printf("dcgcgpt: %lu\n", dcgcgpt);
	printf("srgpt: %lu\n", srgpt);
	printf("gpt1ocp1: %lu\n", gpt1ocp1);
	printf("gpioafsel: %lu\n", gpioafsel);
	printf("iocsel: %lu\n", ioc_sel);
	printf("---------------------------------\n");

	//leds_on(LEDS_RED);

}

PROCESS_THREAD(spi_test, ev, data) {
	uint8_t stat = 0;
	PROCESS_BEGIN();
	//leds_on(LEDS_YELLOW);

	stat += ungate_gpt(GPTIMER_1);
	stat += gpt_set_16_bit_timer(GPTIMER_1);

	stat += gpt_set_mode(GPTIMER_1, GPTIMER_SUBTIMER_A, GPTIMER_CAPTURE_MODE);
	stat += gpt_set_capture_mode(GPTIMER_1, GPTIMER_SUBTIMER_A, GPTIMER_CAPTURE_MODE_EDGE_TIME);
	stat += gpt_set_alternate_mode(GPTIMER_1, GPTIMER_SUBTIMER_A, GPTIMER_ALTERNATE_MODE_CAPTURE);
	stat += gpt_set_count_dir(GPTIMER_1, GPTIMER_SUBTIMER_A, GPTIMER_COUNT_DIR_UP);
	stat += gpt_set_event_mode(GPTIMER_1, GPTIMER_SUBTIMER_A, GPTIMER_EVENT_MODE_POSITIVE_EDGE);

	stat += gpt_register_callback(&test_func, GPTIMER_1, GPTIMER_SUBTIMER_A, GPTIMER_CAPTURE_EVENT);
	stat += gpt_enable_event(GPTIMER_1, GPTIMER_SUBTIMER_A);
	stat += gpt_enable_interrupt(GPTIMER_1, GPTIMER_SUBTIMER_A, GPTIMER_CAPTURE_EVENT_INT);

	REG(IOC_GPT1OCP1) = 20;
/*
	GPIO_SET_INPUT(GPIO_C_BASE, 0x10);
	GPIO_DETECT_EDGE(GPIO_C_BASE, 0x10);
	GPIO_TRIGGER_SINGLE_EDGE(GPIO_C_BASE, 0x10);
	GPIO_DETECT_RISING(GPIO_C_BASE, 0x10);
	GPIO_ENABLE_INTERRUPT(GPIO_C_BASE, 0x10);
*/
	ioc_set_over(GPIO_C_NUM, 4, IOC_OVERRIDE_PDE);
	//ioc_set_sel(GPIO_C_NUM, 4, IOC_PXX_SEL_GPT1_ICP1);
	nvic_interrupt_enable(NVIC_INT_GPTIMER_1A);
	GPIO_PERIPHERAL_CONTROL(GPIO_C_BASE, 0x10); // RF1.6

	printf("stat: %u\n", stat);


	//Helper pin config
	GPIO_SOFTWARE_CONTROL(GPIO_C_BASE, 0x20); // RF 1.8
	GPIO_SET_INPUT(GPIO_C_BASE, 0x20);
	GPIO_DETECT_EDGE(GPIO_C_BASE, 0x20);
	GPIO_TRIGGER_SINGLE_EDGE(GPIO_C_BASE, 0x20);
	GPIO_DETECT_FALLING(GPIO_C_BASE, 0x20);
	GPIO_ENABLE_INTERRUPT(GPIO_C_BASE, 0x20);

	ioc_set_over(GPIO_C_NUM, 5, IOC_OVERRIDE_PUE);
	gpio_register_callback(&info_func, GPIO_C_NUM, 5);
	nvic_interrupt_enable(NVIC_INT_GPIO_PORT_C);

	lpm_set_max_pm(LPM_PM0);

/*
  //REG(SYS_CTRL_RCGCGPT) |= SYS_CTRL_RCGCGPT_GPT1;
  //REG(SYS_CTRL_DCGCGPT) |= SYS_CTRL_DCGCGPT_GPT1;

  //Initialize GPT1
  //REG(GPT_1_BASE | GPTIMER_CTL) = 0; // Make sure GPT1 is turned off
  //REG(GPT_1_BASE | GPTIMER_CFG) = 0x04; // 16 bit timer

  REG(GPT_1_BASE | GPTIMER_TBMR) = 0;
  REG(GPT_1_BASE | GPTIMER_TBMR) |= GPTIMER_TBMR_TBAMS; // 1=PWM.0=Capture
  REG(GPT_1_BASE | GPTIMER_TBMR) |= GPTIMER_TBMR_TBMR_PERIODIC;
  REG(GPT_1_BASE | GPTIMER_TBMR) |= GPTIMER_TBMR_TBPWMIE;

  // Set up and turn on PWM
  REG(GPT_1_BASE | GPTIMER_TBILR) = 0x190;
  REG(GPT_1_BASE | GPTIMER_TBMATCHR) = 0xC8;
  REG(GPT_1_BASE | GPTIMER_CTL) |= GPTIMER_CTL_TBEN;

  // Reroute GPT_1 PWM output to PB1
  ioc_set_sel(GPIO_B_NUM, 3, IOC_PXX_SEL_GPT1_ICP2);
  ioc_set_over(GPIO_B_NUM, 3, IOC_OVERRIDE_OE);
  GPIO_PERIPHERAL_CONTROL(GPIO_B_BASE, 0x08);
*/
	//leds_on(LEDS_RED);
	PROCESS_END();
}
