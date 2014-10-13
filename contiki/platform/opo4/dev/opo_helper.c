
#include "gptimer.h"

void opo_init() {
	//Setting up GPT_1
	ungate_gpt(GPTIMER_1);
	gpt_set_16_bit_timer(GPTIMER_1);
}