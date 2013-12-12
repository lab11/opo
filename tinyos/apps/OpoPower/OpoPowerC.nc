configuration OpoPowerC {}

implementation {
    components MainC, OpoPowerP;
    OpoPowerP.Boot -> MainC.Boot;

    components At45dbPowerC;
    OpoPowerP.FlashPower -> At45dbPowerC.SplitControl;

    components HplLIS3DHTRC;
    OpoPowerP.HplLIS3DHTR -> HplLIS3DHTRC.HplLIS3DHTR;

    components new TimerMilliC() as PowerTimer;
    OpoPowerP.PowerTimer -> PowerTimer;

    //components HplMsp430GeneralIOC;
    //OpoPowerP.VPIN -> HplMsp430GeneralIOC.Port45;
}