#include "opo3tx.h"
#include "nrf8001.h"
#include "contiki.h"
#include "nrf8001.h"
#include "dev/gpio.h"
#include "dev/ioc.h"
#include "dev/gptimer.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include "simple_network_driver.h"
#include "rf_switch.h"
#include <stdio.h>
#include <stdbool.h>
#include "dev/rfcore-xreg.h"
#include "dev/cctest.h"
#include "sys/rtimer.h"

/*---------------------------------------------------------------------------*/
PROCESS(tx_test, "Opo3Tx");
AUTOSTART_PROCESSES(&tx_test);
/*---------------------s------------------------------------------------------*/

uint8_t open_pipes[64] = {0};
uint8_t test_data[6] = {65,65,65,65,65,65};
nrf8001_event_packet ep = {0};
int msg_count = 0;
uint8_t test = 0;
static struct etimer et;
uint tx_stage = 0;
static struct rtimer rt;
static void stage_1(struct rtimer *t, void *ptr);
static void stage_2(struct rtimer *t, void *ptr);
static void stage_3(struct rtimer *t, void *ptr);

static void stage_1(struct rtimer *t, void *ptr) {
	gpt_disable_event(1, GPTIMER_SUBTIMER_A);
	rtimer_set(&rt, RTIMER_NOW() + (RTIMER_ARCH_SECOND/1000 * 50), 1,
	     	   &stage_2, NULL);
}

static void stage_2(struct rtimer *t, void *ptr) {
	packetbuf_copyfrom((void *) test_data, 6);
	NETSTACK_MAC.send(NULL, NULL);
	gpt_enable_event(1, GPTIMER_SUBTIMER_A);
	rtimer_set(&rt, RTIMER_NOW() + (RTIMER_ARCH_SECOND/1000), 1,
	     	   &stage_3, NULL);
}

static void stage_3(struct rtimer *t, void *ptr) {
	gpt_disable_event(1, GPTIMER_SUBTIMER_A);
	etimer_set(&et, CLOCK_SECOND * 5);
}


PROCESS_THREAD(tx_test, ev, data) {
	PROCESS_BEGIN();

	// Setting all the radio shit up.
	packetbuf_clear();
	packetbuf_copyfrom((void *) test_data, 6);
	cc2538_ant_enable();
	NETSTACK_MAC.on();

	//Setting all the timer shit up
	ungate_gpt(1);
	gpt_set_16_bit_timer(1);
	gpt_set_mode(1, GPTIMER_SUBTIMER_A, GPTIMER_PERIODIC_MODE);
	gpt_set_alternate_mode(1, GPTIMER_SUBTIMER_A, GPTIMER_ALTERNATE_MODE_PWM);
	gpt_set_interval_value(1, GPTIMER_SUBTIMER_A, 0x190);
	gpt_set_match_value(1, GPTIMER_SUBTIMER_A, 0xC8);
	ioc_set_sel(PWM_PORT_NUM, PWM_PIN, IOC_PXX_SEL_GPT1_ICP1);
  	ioc_set_over(PWM_PORT_NUM, PWM_PIN, IOC_OVERRIDE_OE);
  	GPIO_PERIPHERAL_CONTROL(PWM_PORT_BASE, PWM_PIN_MASK);

  	// Outputting SFD signal for debugging
  	GPIO_SET_OUTPUT(GPIO_C_BASE, 0x20);
  	ioc_set_over(GPIO_C_NUM, 5, IOC_OVERRIDE_DIS);
  	REG(RFCORE_XREG_RFC_OBS_CTRL0) = 0x0F;
  	REG(CCTEST_OBSSEL5) = 0x80;

  	// Set up Opo Tx/Rx Control
  	GPIO_SET_OUTPUT(GPIO_C_BASE, 0x01);
  	GPIO_CLR_PIN(GPIO_C_BASE, 0x01);

	etimer_set(&et, CLOCK_SECOND * 5);
	while(1) {
		PROCESS_YIELD();
		if (ev == PROCESS_EVENT_TIMER) {
			gpt_enable_event(1, GPTIMER_SUBTIMER_A);
			rtimer_set(&rt, RTIMER_NOW() + RTIMER_ARCH_SECOND/1000, 1,
     			       &stage_1, NULL);
		}
	}
	PROCESS_END();
}
