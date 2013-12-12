module MotePlatformC @safe() {
  provides interface Init;
  uses interface Init as SubInit;
}
implementation {

  command error_t Init.init() {
    // reset all of the ports to be input and using i/o functionality
    atomic {
        P1SEL = 0;
        P1DIR = 0xe4;
        P1OUT = 0x00;
        P1IE = 0;

        P2SEL = 0;
        P2DIR = 0xbb;
        P2OUT = 0xb0;
        P2IE = 0;

        P3SEL = 0;
        P3DIR = 0xff;
        P3OUT = 0x0a;

        P4SEL = 0;
        P4DIR = 0xf5;
        P4OUT = 0xd5;

        P5SEL = 0;
        P5DIR = 0xff;
        P5OUT = 0x08;

        P6SEL = 0;
        P6DIR = 0xfe;
        P6OUT = 0x00;

	// the commands above take care of the pin directions
	// there is no longer a need for explicit set pin
	// directions using the TOSH_SET/CLR macros

    }//atomic

    return call SubInit.init();
  }

  default command error_t SubInit.init() { return SUCCESS; }
}
