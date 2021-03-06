/*
Basic interface for the opo mobile networking primitive
Author: William Huang
*/

interface Opo {
	command error_t transmit(message_t* packet, size_t psize);
	event void transmit_done();
    event void transmit_failed();

	command error_t enable_receive();
	command error_t disable_receive();
    event void enable_receive_failed();

	event void receive(uint16_t t_rf,
		               uint16_t t_ultrasonic,
		               message_t* msg);
	event void receive_failed(uint8_t rx_status);

    command error_t setup_pins();
    command bool is_receiving();
}