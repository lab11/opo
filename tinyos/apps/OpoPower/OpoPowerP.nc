module OpoPowerP {
    uses {
        interface Boot;
        interface SplitControl as FlashPower;
        //interface HplMsp430GeneralIO as VPIN;
        interface HplLIS3DHTR;
        interface Timer<TMilli> as PowerTimer;
    }

}

implementation {

    enum {IDLE, RFP, FLASHP, ACCELP} pstate;

    event void Boot.booted() {
        //printf("Booted");
        //printfflush();
        call FlashPower.stop();
    }


    event void FlashPower.startDone(error_t err) {}

    event void FlashPower.stopDone(error_t err) {
        call PowerTimer.startOneShot(60);
    }

    event void HplLIS3DHTR.power_down_done() {}

    event void HplLIS3DHTR.set_default_mode_done() {}

    event void PowerTimer.fired() {
        call HplLIS3DHTR.power_down();
    }

}