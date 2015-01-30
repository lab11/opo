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
//#include "lib/sensors.h"

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

static void disable_all_ioc_override() {
  uint8_t portnum = 0;
  uint8_t pinnum = 0;
  for(portnum = 0; portnum < 4; portnum++) {
      for(pinnum = 0; pinnum < 8; pinnum++) {
          ioc_set_over(portnum, pinnum, IOC_OVERRIDE_DIS);
      }
  }
}
/*---------------------------------------------------------------------------*/
/**
 * \brief Main routine for the cc2538dk platform
 */
int
main(void)
{

  gpio_init();

  // Set up GPIO pins. Gotta stop that current leakage yo
  GPIO_SET_OUTPUT(GPIO_A_BASE, GPIO_A_OUTPUT_MASK);
  GPIO_SET_INPUT(GPIO_A_BASE, GPIO_A_INPUT_MASK);
  GPIO_CLR_PIN(GPIO_A_BASE, GPIO_A_CLR_MASK);
  GPIO_SET_PIN(GPIO_A_BASE, GPIO_A_SET_MASK);

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


  nvic_init();
  ioc_init();
  sys_ctrl_init();
  clock_init();
  lpm_init();
  rtimer_init();

  leds_init();

  process_init();

  watchdog_init();
  spi_init();
  nrf8001_init();
  rf_switch_init();
  cloudcomm_init();

  disable_all_ioc_override();
  ioc_set_over(I2C_SDA_PORT_NUM, I2C_SDA_PIN_NUM, IOC_OVERRIDE_PUE);
  ioc_set_over(I2C_SCL_PORT_NUM, I2C_SCL_PIN_NUM, IOC_OVERRIDE_PUE);

  /*
   * Character I/O Initialization.
   * When the UART receives a character it will call serial_line_input_byte to
   * notify the core. The same applies for the USB driver.
   *
   * If slip-arch is also linked in afterwards (e.g. if we are a border router)
   * it will overwrite one of the two peripheral input callbacks. Characters
   * received over the relevant peripheral will be handled by
   * slip_input_byte instead
   */
#if UART_CONF_ENABLE
  uart_init(0);
  uart_init(1);
  uart_set_input(SERIAL_LINE_CONF_UART, serial_line_input_byte);
#endif

#if USB_SERIAL_CONF_ENABLE
  usb_serial_init();
  usb_serial_set_input(serial_line_input_byte);
#endif

  serial_line_init();

  INTERRUPTS_ENABLE();

  PUTS(CONTIKI_VERSION_STRING);
  PUTS(BOARD_STRING);

  /* Initialise the H/W RNG engine. */
  random_init(0);

  udma_init();

  process_start(&etimer_process, NULL);
  ctimer_init();

  set_rf_params();
  netstack_init();

#if UIP_CONF_IPV6
  memcpy(&uip_lladdr.addr, &linkaddr_node_addr, sizeof(uip_lladdr.addr));
  queuebuf_init();
  process_start(&tcpip_process, NULL);
#endif /* UIP_CONF_IPV6 */

  //process_start(&sensors_process, NULL);

  energest_init();
  ENERGEST_ON(ENERGEST_TYPE_CPU);

  autostart_start(autostart_processes);

#if WATCHDOG_CONF_ENABLE
  watchdog_start();
#endif

  while(1) {
    uint8_t r;
    do {
      /* Reset watchdog and handle polls and events */
      watchdog_periodic();

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
