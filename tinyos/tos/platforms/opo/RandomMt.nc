interface RandomMt {
	command void seed(uint32_t);
	async command uint32_t rand32();
	async command uint16_t rand16();
}