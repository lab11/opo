interface HplRV4162 {

    command error_t readFullTime();
    command error_t setTime(uint8_t t[8]);

    event void readFullTimeDone(error_t err, uint8_t* fullTime);
    event void setTimeDone(error_t err);
}