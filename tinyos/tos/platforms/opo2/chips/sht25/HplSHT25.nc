interface HplSHT25 {
	command void readTemperature();
	command void readRH();

	event void readTemperatureDone(uint16_t temp);
	event void readRHDone(uint16_t rh);

	command void softReset();
	event void softResetDone();
}