#include "RV4162.h"

module HplRV4162P {
  provides {
    interface HplRV4162;
  }
  uses {
    interface I2CPacket<TI2CBasicAddr>;
    interface Resource as I2CResource;
    interface Leds;
  }
}

implementation {
  uint8_t i2c_write_buffer[17];
  uint8_t *i2c_write_buffer_ptr = i2c_write_buffer;
  uint8_t fullTime[20];
  uint8_t full_time_start_addr = 0;

  i2c_state_t i2c_state;

  uint8_t masks[8];

  task void I2C_RV4162_TASK() {

      if (!call I2CResource.isOwner()) {
        call I2CResource.request();
        return;
      }

      switch (i2c_state) {
        case RV_WRITE_FULL_TIME_START_ADDRESS:
          i2c_state = RV_READ_FULL_TIME;
          call I2CPacket.write( (I2C_START | I2C_STOP),
                                RV4162_ADDR,
                                1,
                                &full_time_start_addr);
          break;
        case RV_READ_FULL_TIME:
          i2c_state = RV_FULL_TIME_READ;
          call I2CPacket.read( (I2C_START | I2C_STOP),
                                RV4162_ADDR,
                                8,
                                &fullTime[0]);
          break;

        case RV_FULL_TIME_READ:
          i2c_state = RV_IDLE;
          fullTime[1] = fullTime[1] & 0x7F;
          fullTime[2] = fullTime[2] & 0x7F;
          fullTime[4] = fullTime[4] & 0x07;

          call I2CResource.release();
          signal HplRV4162.readFullTimeDone(SUCCESS, &fullTime[0]);
          break;

        case RV_WRITE_ST_BIT:
          i2c_state = RV_WRITE_ST_BIT_2;
          i2c_write_buffer[0] = 0x01;
          i2c_write_buffer[1] = 0x80;
          call I2CPacket.write( (I2C_START | I2C_STOP),
                                RV4162_ADDR,
                                2,
                                i2c_write_buffer_ptr);
          break;

        case RV_WRITE_ST_BIT_2:
          i2c_state = RV_ST_BIT_WRITTEN;
          i2c_write_buffer[0] = 0x01;
          i2c_write_buffer[1] = 0x00;
          call I2CPacket.write( (I2C_START | I2C_STOP),
                                RV4162_ADDR,
                                2,
                                i2c_write_buffer_ptr);
          break;

        case RV_ST_BIT_WRITTEN:
          i2c_state = RV_IDLE;
          call I2CResource.release();
          signal HplRV4162.writeSTBitDone(SUCCESS);

          break;

        case RV_SET_TIME:
          i2c_state = RV_TIME_SET;
          call I2CPacket.write( (I2C_START | I2C_STOP),
                                RV4162_ADDR,
                                9,
                                i2c_write_buffer);
          break;

        case RV_TIME_SET:
          i2c_state = RV_IDLE;
          call I2CResource.release();
          signal HplRV4162.setTimeDone(SUCCESS);
          break;

        case RV_RESET_TIME:
          i2c_state = RV_TIME_RESET;
          i2c_write_buffer[0] = 0x00;
          i2c_write_buffer[1] = 0x00;
          i2c_write_buffer[2] = 0x00;
          i2c_write_buffer[3] = 0x00;
          i2c_write_buffer[4] = 0x00;
          i2c_write_buffer[5] = 0x01;
          i2c_write_buffer[6] = 0x01;
          i2c_write_buffer[7] = 0x81;
          i2c_write_buffer[8] = 0x00;
          call I2CPacket.write( (I2C_START | I2C_ACK_END),
                                 RV4162_ADDR,
                                 9,
                                 i2c_write_buffer);
          break;

        case RV_TIME_RESET:
          i2c_state = RV_IDLE;
          call I2CResource.release();
          signal HplRV4162.resetTimeDone(SUCCESS);
          break;
      }
  }

  event void I2CResource.granted() {
    post I2C_RV4162_TASK();
  }

  async event void I2CPacket.readDone(error_t error,
                                      uint16_t addr,
                                      uint8_t length,
                                      uint8_t* data) {
    post I2C_RV4162_TASK();
  }

  async event void I2CPacket.writeDone(error_t error,
                                       uint16_t addr,
                                       uint8_t length,
                                       uint8_t* data) {
    post I2C_RV4162_TASK();
  }

  command error_t HplRV4162.readFullTime() {
    i2c_state = RV_WRITE_FULL_TIME_START_ADDRESS;
    post I2C_RV4162_TASK();
    return SUCCESS;
  }

  command error_t HplRV4162.writeSTBit() {
    i2c_state = RV_WRITE_ST_BIT;
    post I2C_RV4162_TASK();
    return SUCCESS;
  }

  command error_t HplRV4162.setTime(uint8_t t[8]) {
    uint8_t i;
    i2c_state = RV_SET_TIME;
    i2c_write_buffer[0] = 0x00;
    masks[0] = 0xFF;
    masks[1] = 0x7F;
    masks[2] = 0x7F;
    masks[3] = 0x7F;
    masks[4] = 0x07;
    masks[5] = 0x3F;
    masks[6] = 0xDF;
    masks[7] = 0xFF;
    for(i=0; i < 8; i++) {
      i2c_write_buffer[i+1] = t[i] & masks[i];
    }

    i2c_write_buffer[5] = i2c_write_buffer[5] | 0x10;

    post I2C_RV4162_TASK();
    return SUCCESS;
  }

  command error_t HplRV4162.resetTime() {
    i2c_state = RV_RESET_TIME;
    post I2C_RV4162_TASK();
    return SUCCESS;
  }

}