configuration RV4162C {
    provides {
      interface HplRV4162;
    } 
}

implementation {

    components HplRV4162P; 
    HplRV4162 = HplRV4162P.HplRV4162;
    
    components new Msp430I2CC() as I2CA;
    HplRV4162P.I2CPacket -> I2CA.I2CBasicAddr;
    HplRV4162P.I2CResource -> I2CA.Resource; 
}