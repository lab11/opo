module OpoUltrasonicTransmitterP {
	uses {
		interface Boot;
		interface Leds;
		interface Timer<TMilli> as TxTimer;
		interface HplMsp430GeneralIO as TxRxSwitch;
	}

}
implementation {

	bool txStarted = FALSE;

	event void Boot.booted() {

	}

	event void TxTimer.fired() {

	}



  /*============================= Helper =======================================
    Helper functions, primarily for ultrasonic stuff.
  ============================================================================*/

  /*
   * TACCR0 = Compare Register, what the timer counts to before rolling over
   * TACCR1 = Compare register, triggers output changes.
   * Zero these out to disable the ultrasonic PWM
  */
  inline void startTransducer() {
    atomic {
      TACCR0 = 0x000C;
      TACCR1 = 0x0005;
    }
  }

  inline void stopTransducer() {
    atomic {
      TACCR0 = 0x0000;
      TACCR1 = 0x0000;
    }
  }

  /*
   * Sets up Timer A for PWM.
   * Specifically, this sets up TACCTL1, or Timer A Compare Output 1.
  */
  inline void setupUltrasonicPWM() {
    atomic {
      // Sets Port16 to be a Timer A module and an output.
      P1SEL = P1SEL | BIT6;
      P1DIR = P1DIR | BIT6;

      // Timer A PWM signal controlled by TACCR0 and TACCR1
      // See Start and Stop Transducer for more.
      // TACTl = Timer A Control Register
      // TASSEL_2 = Timer A Clock Source select 2 - SMCLK
      // ID_1 = Timer A Input (Clock) Divider - 2
      // MC_1 = Mode Control - Up Mode
      // TAIE = Interrupt Enabled
      TACTL = 0x0000;
      TACTL = TASSEL_2 | ID_1 | MC_1;

      // Timer A Compare register settings
      // CM0 = Capture Mode 0 - Compare instead of Capture
      // CCIS_0 = Capture/compare input select done from CCI1A
      // OUTMOD_3 = Outmode 3, PWM Set on TACCR1, Reset on TACCR0
      // CCIE = Interrupt Enabled
      TACCTL1 = 0x0000;
      TACCTL1 = CM0 | CCIS_0 | OUTMOD_3 | CCIE;
    }
  }

  inline void setupTACTL() {
    atomic {
      TACTL = 0x0000;
      TACTL = TASSEL_2 | ID_1 | MC_1;
    }
  }

}