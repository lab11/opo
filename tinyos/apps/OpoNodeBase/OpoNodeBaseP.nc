#include "OpoNodeBase.h"

module OpoNodeBaseP {
	uses {
		interface UartStream;
		interface Boot;
		interface Leds;
		interface Timer<TMilli> as SendTimer;
		interface StdControl as UartControl;
		interface SplitControl as RfControl;
		interface Receive as RxReceive;
	}
}
implementation {
	event void Boot.booted(){
		call Leds.led0On();
		call UartControl.start();
		call RfControl.start();
	}

	async event void UartStream.sendDone(uint8_t *buf, uint16_t len, error_t error) {
		call Leds.led0Toggle();
		call SendTimer.startOneShot(2000);
	}

	async event void UartStream.receivedByte(uint8_t byte) {}
	async event void UartStream.receiveDone(uint8_t *buf, uint16_t len, error_t error) {}

	event message_t* RxReceive.receive(message_t *msg, void *payload, uint8_t len) {
		uint8_t *p;
	    opo_rx_base_msg_t *data = (opo_rx_base_msg_t *) payload;
	    UartStream.
  	}

  	event void RfControl.startDone(error_t err) {}
  	event void RfControl.stopDone(error_t err) {}

}