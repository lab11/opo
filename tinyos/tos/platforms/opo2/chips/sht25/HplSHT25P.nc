#include "sht25.h"

module HplSHT25P {
  provides {
    interface HplSHT25;
  }
  uses {
    interface I2CPacket<TI2CBasicAddr>;
    interface Resource as I2CResource;
    interface Timer<TMilli> as WaitTimer;
    interface Leds;
  }
}

implementation {
  uint8_t cmd;
  uint8_t readBuffer[15];
  uint16_t temperature;
  uint16_t rh;

  sht25_state_t sht25_state;

  task void I2C_SHT25_TASK() {

      if (!call I2CResource.isOwner()) {
        call I2CResource.request();
        return;
      }

      switch (sht25_state) {
        case SHT25_MEASURE_TEMPERATURE:
          sht25_state = SHT25_READ_TEMPERATURE;
          cmd = T_MEASURE;
          call I2CPacket.write( (I2C_START | I2C_ACK_END),
                                SHT25_ADDR,
                                1,
                                &cmd);
          break;

        case SHT25_READ_TEMPERATURE:
          sht25_state = SHT25_READ_TEMPERATURE_DONE;
          call I2CPacket.read( (I2C_START | I2C_ACK_END),
                                SHT25_ADDR,
                                3,
                                &readBuffer[0]);
          break;

        case SHT25_READ_TEMPERATURE_DONE:
          sht25_state = SHT25_IDLE;
          call I2CResource.release();
          temperature = readBuffer[0];
          //temperature = temperature << 8;
          temperature += readBuffer[1];
          signal HplSHT25.readTemperatureDone(temperature);
          break;

        case SHT25_MEASURE_HUMIDITY:
          sht25_state = SHT25_READ_HUMIDITY;
          cmd = RH_MEAURE;
          call I2CPacket.write( (I2C_START | I2C_ACK_END),
                                SHT25_ADDR,
                                1,
                                &cmd);
          break;

        case SHT25_READ_HUMIDITY:
          sht25_state = SHT25_READ_HUMIDITY_DONE;
          call I2CPacket.read( (I2C_START | I2C_STOP),
                                SHT25_ADDR,
                                2,
                                &readBuffer[0]);
          break;

        case SHT25_READ_HUMIDITY_DONE:
          sht25_state = SHT25_IDLE;
          call I2CResource.release();
          rh = readBuffer[0];
          rh = rh << 8;
          rh += readBuffer[1];

          signal HplSHT25.readRHDone(rh);
          break;

        case SHT25_SOFT_RESET:
          sht25_state = SHT25_SOFT_RESET_DONE;
          cmd = SOFT_RESET;
          call I2CPacket.write( (I2C_START | I2C_ACK_END),
                                SHT25_ADDR,
                                1,
                                &cmd);
          break;

        case SHT25_SOFT_RESET_DONE:
          call I2CResource.release();
          signal HplSHT25.softResetDone();
          break;
      }
  }

  task void SHT25_WAIT_TASK() {
     switch (sht25_state) {

        case SHT25_READ_TEMPERATURE:
          call WaitTimer.startOneShot(T_MEASURE_TIME);
          break;

        case SHT25_READ_HUMIDITY:
          call WaitTimer.startOneShot(RH_MEASURE_TIME);
          break;

        case SHT25_SOFT_RESET_DONE:
          post I2C_SHT25_TASK();
          break;
      }
  }

  event void I2CResource.granted() {
    post I2C_SHT25_TASK();
  }

  event void WaitTimer.fired() {
    post I2C_SHT25_TASK();
  }

  async event void I2CPacket.readDone(error_t error,
                                      uint16_t addr,
                                      uint8_t length,
                                      uint8_t* data) {
    post I2C_SHT25_TASK();
  }

  async event void I2CPacket.writeDone(error_t error,
                                       uint16_t addr,
                                       uint8_t length,
                                       uint8_t* data) {
    if(sht25_state == SHT25_READ_TEMPERATURE) {
      sht25_state = SHT25_READ_TEMPERATURE_DONE;
      call I2CPacket.read( (I2C_START | I2C_ACK_END),
                            SHT25_ADDR,
                            3,
                            &readBuffer[0]);
    }
    else if(sht25_state == SHT25_READ_HUMIDITY) {
      sht25_state = SHT25_READ_HUMIDITY_DONE;
      call I2CPacket.read( (I2C_START | I2C_ACK_END),
                            SHT25_ADDR,
                            3,
                            &readBuffer[0]);
    }
    else if (sht25_state == SHT25_SOFT_RESET_DONE) {
      signal HplSHT25.softResetDone();
    }
  }

  command void HplSHT25.readTemperature() {
    sht25_state = SHT25_MEASURE_TEMPERATURE;
    post I2C_SHT25_TASK();
  }

  command void HplSHT25.readRH() {
    sht25_state = SHT25_MEASURE_HUMIDITY;
    post I2C_SHT25_TASK();
  }

  command void HplSHT25.softReset() {
    sht25_state = SHT25_SOFT_RESET;
    post I2C_SHT25_TASK();
  }

}