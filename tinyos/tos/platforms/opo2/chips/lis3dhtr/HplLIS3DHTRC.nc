configuration HplLIS3DHTRC {
    provides {
      interface HplLIS3DHTR;
    }
}

implementation {

    components HplLIS3DHTRP;
    HplLIS3DHTR = HplLIS3DHTRP.HplLIS3DHTR;

    components new Msp430I2CC() as I2CA;
    HplLIS3DHTRP.I2CPacket -> I2CA.I2CBasicAddr;
    HplLIS3DHTRP.I2CResource -> I2CA.Resource;
}