/**
 * \addtogroup platform
 * @{
 *
 * \defgroup atum The opo4 sensor mote module.
 * @{
 *
 * \file
 *   Main module for the atum platform
 */
/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include "dev/leds.h"
#include "dev/sys-ctrl.h"
#include "dev/scb.h"
#include "dev/nvic.h"
#include "dev/uart.h"
#include "dev/watchdog.h"
#include "dev/ioc.h"
#include "dev/serial-line.h"
#include "dev/slip.h"
#include "dev/cc2538-rf.h"
#include "dev/udma.h"
#include "usb/usb-serial.h"
#include "lib/random.h"
#include "net/netstack.h"
#include "net/queuebuf.h"
#include "net/ip/tcpip.h"
#include "net/ip/uip.h"
#include "net/mac/frame802154.h"
#include "cpu.h"
#include "reg.h"
#include "ieee-addr.h"
#include "lpm.h"
#include "spi-arch.h"
#include "spi.h"
#include "nrf8001.h"
#include "rf_switch.h"
#include "simple_sfd_handler.h"
#include "cloudcomm.h"
#include "sst25vf.h"
#include "simplestore.h"
#include "opo.h"
#include "rv4162.h"
#include "vtimer.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
/*---------------------------------------------------------------------------*/
#if STARTUP_CONF_VERBOSE
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#if UART_CONF_ENABLE
#define PUTS(s) puts(s)
#else
#define PUTS(s)
#endif
/*---------------------------------------------------------------------------*/
static void
set_rf_params(void)
{
    uint16_t short_addr;
    uint8_t ext_addr[8];

    ieee_addr_cpy_to(ext_addr, 8);

    short_addr = ext_addr[7];
    short_addr |= ext_addr[6] << 8;

    /* Populate linkaddr_node_addr. Maintain endianness */
    memcpy(&linkaddr_node_addr, &ext_addr[8 - LINKADDR_SIZE], LINKADDR_SIZE);

    NETSTACK_RADIO.set_value(RADIO_PARAM_PAN_ID, IEEE802154_PANID);
    NETSTACK_RADIO.set_value(RADIO_PARAM_16BIT_ADDR, short_addr);
    NETSTACK_RADIO.set_value(RADIO_PARAM_CHANNEL, CC2538_RF_CHANNEL);
    NETSTACK_RADIO.set_object(RADIO_PARAM_64BIT_ADDR, ext_addr, 8);
}

/* Disables all pull up resistors EXCEPT the FTDI related ones */
static void disable_all_ioc_override() {
  uint8_t portnum = 0;
  uint8_t pinnum = 0;
  for(portnum = 1; portnum < 4; portnum++) {
      for(pinnum = 0; pinnum < 8; pinnum++) {
          ioc_set_over(portnum, pinnum, IOC_OVERRIDE_DIS);
      }
  }
  ioc_set_over(GPIO_A_NUM, 0, IOC_OVERRIDE_PDE);
  ioc_set_over(GPIO_A_NUM, 1, IOC_OVERRIDE_PDE);
  ioc_set_over(GPIO_A_NUM, 2, IOC_OVERRIDE_PUE);
  for(pinnum = 3; pinnum < 8; pinnum++) {ioc_set_over(GPIO_A_NUM, pinnum, IOC_OVERRIDE_DIS);}
}
/*---------------------------------------------------------------------------*/
/**
 * \brief Main routine for the opo 4 platform
 */
int main(void) {
  uint8_t i = 0;
  gpio_init();

  // Set up GPIO pins. Gotta stop that current leakage yo
  GPIO_SET_OUTPUT(GPIO_A_BASE, GPIO_A_OUTPUT_MASK);
  GPIO_SET_INPUT(GPIO_A_BASE, GPIO_A_INPUT_MASK);
  GPIO_CLR_PIN(GPIO_A_BASE, GPIO_A_CLR_MASK);
  GPIO_SET_PIN(GPIO_A_BASE, GPIO_A_SET_MASK);
  GPIO_SOFTWARE_CONTROL(GPIO_A_BASE, 0xFB);

  GPIO_SET_OUTPUT(GPIO_B_BASE, GPIO_B_OUTPUT_MASK);
  GPIO_SET_INPUT(GPIO_B_BASE, GPIO_B_INPUT_MASK);
  GPIO_CLR_PIN(GPIO_B_BASE, GPIO_B_CLR_MASK);
  GPIO_SET_PIN(GPIO_B_BASE, GPIO_B_SET_MASK);

  GPIO_SET_OUTPUT(GPIO_C_BASE, GPIO_C_OUTPUT_MASK);
  GPIO_SET_INPUT(GPIO_C_BASE, GPIO_C_INPUT_MASK);
  GPIO_CLR_PIN(GPIO_C_BASE, GPIO_C_CLR_MASK);
  GPIO_SET_PIN(GPIO_C_BASE, GPIO_C_SET_MASK);

  GPIO_SET_OUTPUT(GPIO_D_BASE, GPIO_D_OUTPUT_MASK);
  GPIO_SET_INPUT(GPIO_D_BASE, GPIO_D_INPUT_MASK);
  GPIO_CLR_PIN(GPIO_D_BASE, GPIO_D_CLR_MASK);
  GPIO_SET_PIN(GPIO_D_BASE, GPIO_D_SET_MASK);

  leds_init();

  clock_init();
  nvic_init();

  ioc_init();
  disable_all_ioc_override();
  ioc_set_over(I2C_SDA_PORT_NUM, I2C_SDA_PIN_NUM, IOC_OVERRIDE_PUE);
  ioc_set_over(I2C_SCL_PORT_NUM, I2C_SCL_PIN_NUM, IOC_OVERRIDE_PUE);

  sys_ctrl_init();

  lpm_init();
  vtimer_init();
  process_init();

  watchdog_init();
  spi_init();

  rf_switch_init();

  //sst25vf_init();

  INTERRUPTS_ENABLE();

  random_init(0); // we use a hardware rng, so seed doesn't matter

  udma_init();

  set_rf_params();
  netstack_init();

  //nrf8001_init();
  //cloudcomm_init();

  opo_init();
  NETSTACK_MAC.off(0);
  autostart_start(autostart_processes);

  while(1) {
    uint8_t r;
    do {
      r = process_run();
    } while(r > 0);

    /* We have serviced all pending events. Enter a Low-Power mode. */
    lpm_enter();
  }
}
/*---------------------------------------------------------------------------*/

/**
 * @}
 * @}
 */
