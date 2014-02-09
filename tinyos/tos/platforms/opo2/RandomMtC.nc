configuration RandomMtC {
	provides interface RandomMt;
}
implementation {
	components LedsC, RandomMtP;
	RandomMtP.Leds -> LedsC.Leds;

	RandomMt = RandomMtP;
}